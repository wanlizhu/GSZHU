#include "Utils/Platform/MonitorInfo.h"
#include "Framework.h"

#include <cstdio>
#include <SetupApi.h>
#include <cfgmgr32.h>
#include <cmath>
#include "Utils/String.h"

#pragma comment(lib, "setupapi.lib")

// With some inspiration from: 
//     http://ofekshilon.com/2011/11/13/reading-monitor-physical-dimensions-or-getting-the-edid-the-right-way/
//     http://ofekshilon.com/2014/06/19/reading-specific-monitor-dimensions/

#define NAME_SIZE 128

namespace GS
{
	namespace local
	{
		static const GUID GUID_CLASS_MONITOR = { 0x4d36e96e, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 };
		static std::vector<GS::MonitorInfo::Desc> _sMinitorDescList;

		bool GetMonitorSizeFromEDID(const HKEY hDevRegKey, short& WidthMm, short& HeightMm)
		{
			DWORD dwType, AcutalValueNameLength = NAME_SIZE;
			TCHAR valueName[NAME_SIZE];

			BYTE EDIDdata[1024];
			DWORD edidsize = sizeof(EDIDdata);

			for (LONG i = 0, retValue = ERROR_SUCCESS; retValue != ERROR_NO_MORE_ITEMS; ++i)
			{
				retValue = RegEnumValueA(hDevRegKey, i, &valueName[0],
										&AcutalValueNameLength, NULL, &dwType,
										EDIDdata, // buffer
										&edidsize); // buffer size

				if (retValue != ERROR_SUCCESS || 0 != strcmp(valueName, "EDID"))
					continue;

				WidthMm = ((EDIDdata[68] & 0xF0) << 4) + EDIDdata[66];
				HeightMm = ((EDIDdata[68] & 0x0F) << 8) + EDIDdata[67];

				return true; // valid EDID found
			}

			return false; // EDID not found
		}

		bool GetSizeForDevID(const std::string& TargetDevID, short& WidthMm, short& HeightMm)
		{
			HDEVINFO devInfo = SetupDiGetClassDevsEx(
				&GUID_CLASS_MONITOR, //class GUID
				NULL, //enumerator
				NULL, //HWND
				DIGCF_PRESENT, // Flags //DIGCF_ALLCLASSES|
				NULL, // device info, create a new one.
				NULL, // machine name, local machine
				NULL);// reserved

			if (NULL == devInfo)
				return false;

			bool bRes = false;

			for (ULONG i = 0; ERROR_NO_MORE_ITEMS != GetLastError(); ++i)
			{
				SP_DEVINFO_DATA devInfoData;
				memset(&devInfoData, 0, sizeof(devInfoData));
				devInfoData.cbSize = sizeof(devInfoData);

				if (SetupDiEnumDeviceInfo(devInfo, i, &devInfoData))
				{
					TCHAR Instance[MAX_DEVICE_ID_LEN];
					SetupDiGetDeviceInstanceId(devInfo, &devInfoData, Instance, MAX_DEVICE_ID_LEN, NULL);

					std::string sInstance(Instance);

					if (sInstance.find(TargetDevID) == std::string::npos) {
						continue;
					}

					HKEY hDevRegKey = SetupDiOpenDevRegKey(devInfo, &devInfoData,
														   DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);

					if (!hDevRegKey || (hDevRegKey == INVALID_HANDLE_VALUE))
						continue;

					bRes = GetMonitorSizeFromEDID(hDevRegKey, WidthMm, HeightMm);

					RegCloseKey(hDevRegKey);
				}
			}
			SetupDiDestroyDeviceInfoList(devInfo);
			return bRes;
		}

		BOOL DisplayDeviceFromHMonitor(HMONITOR hMonitor, DISPLAY_DEVICE& ddMonOut)
		{
			MONITORINFOEX mi;
			mi.cbSize = sizeof(MONITORINFOEX);
			GetMonitorInfo(hMonitor, &mi);

			DISPLAY_DEVICE dd;
			dd.cb = sizeof(dd);
			DWORD devIdx = 0; // device index

			bool bFoundDevice = false;
			while (EnumDisplayDevicesA(0, devIdx, &dd, 0))
			{
				devIdx++;
				if (0 != strcmp(dd.DeviceName, mi.szDevice))
					continue;

				DISPLAY_DEVICE ddMon;
				ZeroMemory(&ddMon, sizeof(ddMon));
				ddMon.cb = sizeof(ddMon);
				DWORD MonIdx = 0;

				while (EnumDisplayDevices(dd.DeviceName, MonIdx, &ddMon, 0))
				{
					MonIdx++;

					ddMonOut = ddMon;
					return TRUE;

					ZeroMemory(&ddMon, sizeof(ddMon));
					ddMon.cb = sizeof(ddMon);
				}

				ZeroMemory(&dd, sizeof(dd));
				dd.cb = sizeof(dd);
			}

			return FALSE;
		}

		BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
									  HDC      hdcMonitor,
									  LPRECT   lprcMonitor,
									  LPARAM   dwData)
		{
			MONITORINFO info;
			info.cbSize = sizeof(info);
			if (GetMonitorInfo(hMonitor, &info))
			{
				DISPLAY_DEVICE dev;
				DisplayDeviceFromHMonitor(hMonitor, dev);

				std::string DeviceID(dev.DeviceID);
				DeviceID = DeviceID.substr(8, DeviceID.find('\\', 9) - 8);

				short WidthMm, HeightMm;

				GetSizeForDevID(DeviceID, WidthMm, HeightMm);

				float wInch = float(WidthMm) / 25.4f;
				float hInch = float(HeightMm) / 25.4f;
				float diag = sqrt(wInch * wInch + hInch * hInch);

				MonitorInfo::Desc desc;
				desc.Identifier = DeviceID;
				desc.Resolution = std::array<float, 2>{
						(float)fabs(info.rcMonitor.left - info.rcMonitor.right),
						(float)fabs(info.rcMonitor.top - info.rcMonitor.bottom)
				};

				//printf("%fx%f mm\n", WidthMm, HeightMm );
				desc.PhysicalSize = std::array<float, 2>{ wInch, hInch };
				std::array<float, 2> vPpi;
				vPpi[0] = desc.Resolution[0] / desc.PhysicalSize[0];
				vPpi[1] = desc.Resolution[1] / desc.PhysicalSize[1];
				desc.PPI = (vPpi[0] + vPpi[1]) * 0.5f;
				desc.IsPrimary = (info.dwFlags & MONITORINFOF_PRIMARY);

				_sMinitorDescList.push_back(desc);
			}
			return TRUE;  // continue enumerating
		}
	} // namespace local

	std::vector<MonitorInfo::Desc> MonitorInfo::GetMinitorDescList()
	{
		local::_sMinitorDescList.clear();
		::EnumDisplayMonitors(NULL, NULL, local::MonitorEnumProc, 0);
		return local::_sMinitorDescList;
	}

	void MonitorInfo::PrintMonitorInfo()
	{
		std::vector<MonitorInfo::Desc> descList = GetMinitorDescList();

		for (auto& desc : descList)
		{
			printf("%s%s: %0.0f x %0.0f pix, %0.1f x %0.1f in, %0.2f ppi\n",
				   desc.Identifier.c_str(),
				   desc.IsPrimary ? " (Primary) " : " ",
				   desc.Resolution[0], desc.Resolution[1],
				   desc.PhysicalSize[0], desc.PhysicalSize[1],
				   desc.PPI);
		}
	}
}