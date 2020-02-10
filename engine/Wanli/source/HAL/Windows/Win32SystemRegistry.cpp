#include "HAL/SystemRegistry.h"
#include "Utilities/String.h"
#include <Windows.h>
#include "Utilities/Log.h"

namespace Wanli
{
    HKEY GetHKey(const String& key)
    {
        if (StartsWith(key, "HKEY_CLASSES_ROOT"))
        {
            return HKEY_CLASSES_ROOT;
        }

        if (StartsWith(key, "HKEY_CURRENT_USER"))
        {
            return HKEY_CURRENT_USER;
        }

        if (StartsWith(key, "HKEY_LOCAL_MACHINE"))
        {
            return HKEY_LOCAL_MACHINE;
        }

        if (StartsWith(key, "HKEY_USERS"))
        {
            return HKEY_USERS;
        }

        if (StartsWith(key, "HKEY_CURRENT_CONFIG"))
        {
            return HKEY_CURRENT_CONFIG;
        }

        if (StartsWith(key, "HKEY_DYN_DATA"))
        {
            return HKEY_DYN_DATA;
        }

        if (StartsWith(key, "HKEY_CURRENT_USER_LOCAL_SETTINGS"))
        {
            return HKEY_CURRENT_USER_LOCAL_SETTINGS;
        }

        return NULL;
    }

    DWORD DetectValueType(const RegValue& value)
    {
        if (std::holds_alternative<Uint>(value))
        {
            return REG_DWORD;
        }

        if (std::holds_alternative<Uint64>(value))
        {
            return REG_QWORD;
        }

        if (std::holds_alternative<String>(value))
        {
            return REG_SZ;
        }

        if (std::holds_alternative<ByteArray>(value))
        {
            return REG_BINARY;
        }

        return 0;
    }

    const void* GetValueData(const RegValue& value)
    {
        if (std::holds_alternative<Uint>(value))
        {
            return &std::get<Uint>(value);
        }

        if (std::holds_alternative<Uint64>(value))
        {
            return &std::get<Uint64>(value);
        }

        if (std::holds_alternative<String>(value))
        {
            return std::get<String>(value).data();
        }

        if (std::holds_alternative<ByteArray>(value))
        {
            return std::get<ByteArray>(value).data();
        }

        return 0;
    }

    DWORD GetValueDataSize(const RegValue& value)
    {
        if (std::holds_alternative<Uint>(value))
        {
            return sizeof(Uint);
        }

        if (std::holds_alternative<Uint64>(value))
        {
            return sizeof(Uint64);
        }

        if (std::holds_alternative<String>(value))
        {
            return (DWORD)std::get<String>(value).size();
        }

        if (std::holds_alternative<ByteArray>(value))
        {
            return (DWORD)std::get<ByteArray>(value).size();
        }

        return 0;
    }

    std::optional<RegValue> SystemRegistry::GetValue(const String& key, ERegValueType* type)
    {
        HKEY hKey = GetHKey(key);
        if (hKey == NULL)
        {
            return std::nullopt;
        }

        auto firstSep = key.find_first_of("/");
        auto lastSep = key.find_last_of("/");
        String subKey = key.substr(firstSep + 1, lastSep - firstSep - 1);
        String name = key.substr(lastSep + 1);

        if (RegOpenKeyExA(hKey, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
        {
            return std::nullopt;
        }

        DWORD size = 0;
        DWORD valueType = 0;
        if (RegQueryValueExA(hKey, name.c_str(), 0, &valueType, NULL, &size) != ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return std::nullopt;
        }

        if (valueType == REG_DWORD)
        {
            Uint value = 0;
            if (RegQueryValueExA(hKey, name.c_str(), 0, &valueType, (LPBYTE)&value, &size) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                return std::nullopt;
            }
            RegCloseKey(hKey);
            return value;
        }
        else if (valueType == REG_QWORD)
        {
            Uint64 value = 0;
            if (RegQueryValueExA(hKey, name.c_str(), 0, &valueType, (LPBYTE)&value, &size) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                return std::nullopt;
            }
            RegCloseKey(hKey);
            return value;
        }
        else if (valueType == REG_SZ)
        {
            String value(size, '\0');;
            if (RegQueryValueExA(hKey, name.c_str(), 0, &valueType, (LPBYTE)(value.data()), &size) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                return std::nullopt;
            }
            RegCloseKey(hKey);
            return value;
        }
        else
        {
            ByteArray value(size, 0);
            if (RegQueryValueExA(hKey, name.c_str(), 0, &valueType, (LPBYTE)(value.data()), &size) != ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                return std::nullopt;
            }
            RegCloseKey(hKey);
            return value;
        }
    }

    void SystemRegistry::SetValue(const String& key, const RegValue& value)
    {
        HKEY hKey = GetHKey(key);
        if (hKey == NULL)
        {
            return;
        }

        auto firstSep = key.find_first_of("/");
        auto lastSep = key.find_last_of("/");
        String subKey = key.substr(firstSep + 1, lastSep - firstSep - 1);
        String name = key.substr(lastSep + 1);
        DWORD valueType = DetectValueType(value);
        const void* valueData = GetValueData(value);
        DWORD valueDataSize = GetValueDataSize(value);

        if (RegCreateKeyExA(hKey, subKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
        {
            if (RegSetValueExA(hKey, name.c_str(), 0, valueType, (const BYTE*)valueData, valueDataSize) != ERROR_SUCCESS)
            {
                LOG_WARNING("Failed to write system register.\n");
            }
        }
        RegCloseKey(hKey);
    }
}