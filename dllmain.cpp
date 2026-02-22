#include "pch.h"

namespace Hooks {
    void Initialize();
    void Shutdown();
    HANDLE StartBypassThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter);
}

HMODULE g_hModule = nullptr;

DWORD WINAPI MainThread(LPVOID lpParam) {
    SPOOF_FUNC;
	
    // Wait for fully load
    while (!SPOOF_CALL(GetModuleHandleA)("client.dll") || !SPOOF_CALL(GetModuleHandleA)("engine2.dll")) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Initialize hooks
    Hooks::Initialize();
    
    while (true) {
        if (SPOOF_CALL(GetAsyncKeyState)(VK_END) & 1) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    Hooks::Shutdown();
    
    SPOOF_CALL(FreeLibraryAndExitThread)(g_hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    SPOOF_FUNC;
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        SPOOF_CALL(DisableThreadLibraryCalls)(hModule);
        g_hModule = hModule;
        
        HANDLE hThread = Hooks::StartBypassThread(MainThread, nullptr);
        if (hThread) {
            SPOOF_CALL(CloseHandle)(hThread);
        }
    }
    return TRUE;
}
