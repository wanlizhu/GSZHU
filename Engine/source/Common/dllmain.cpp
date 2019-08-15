#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <string>
#include "glog/logging.h"
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD   reason,
                      LPVOID  reserved)
{
    switch (reason) {
        case DLL_PROCESS_ATTACH:
        {
#ifdef _DEBUG
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
            _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
            char fileName[MAX_PATH] = { 0 };
            GetModuleFileNameA(hModule, fileName, MAX_PATH);
            google::InitGoogleLogging(fileName);
            break;
        }
        case DLL_PROCESS_DETACH:
            google::ShutdownGoogleLogging();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        default:
            break;
    }
    return TRUE;
}