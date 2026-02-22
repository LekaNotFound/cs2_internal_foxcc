#pragma once

#include <winternl.h>

typedef NTSTATUS(NTAPI* tNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG BufferSize, PULONG NumberOfBytesRead);
typedef SIZE_T(WINAPI* tVirtualQuery)(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);
typedef SIZE_T(WINAPI* tVirtualQueryEx)(HANDLE hProcess, LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);

tVirtualQuery oVirtualQuery = nullptr;
tVirtualQueryEx oVirtualQueryEx = nullptr;
tNtReadVirtualMemory oNtReadVirtualMemory = nullptr;

NTSTATUS NTAPI hkNtReadVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG BufferSize, PULONG NumberOfBytesRead)
{
    SPOOF_FUNC;
    return STATUS_ACCESS_VIOLATION;
}

SIZE_T WINAPI hkVirtualQuery(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength) {
    SPOOF_FUNC;
    SIZE_T res = oVirtualQuery(lpAddress, lpBuffer, dwLength);
    if (res == 0) return 0;
    lpBuffer->AllocationProtect = PAGE_NOACCESS;
    lpBuffer->Protect = PAGE_NOACCESS;
    lpBuffer->Type = MEM_MAPPED;
    return res;
}

SIZE_T WINAPI hkVirtualQueryEx(HANDLE hProcess, LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength) {
    SPOOF_FUNC;
    SIZE_T res = oVirtualQueryEx(hProcess, lpAddress, lpBuffer, dwLength);
    if (res == 0) return 0;
    lpBuffer->AllocationProtect = PAGE_NOACCESS;
    lpBuffer->Protect = PAGE_NOACCESS;
    lpBuffer->Type = MEM_MAPPED;
    return res;
}

void EnableHooks() {
    SPOOF_FUNC;
    LPVOID pTarget = (LPVOID)SPOOF_CALL(GetProcAddress)(SPOOF_CALL(GetModuleHandleA)("ntdll.dll"), "NtReadVirtualMemory");
    if (pTarget) {
        if (MH_CreateHook(pTarget, &hkNtReadVirtualMemory, (LPVOID*)&oNtReadVirtualMemory) == MH_OK) {
            MH_EnableHook(pTarget);
        }
    }
    HMODULE hKernel = SPOOF_CALL(GetModuleHandleA)("kernel32.dll");
    if (hKernel) {
        LPVOID pVQuery = (LPVOID)SPOOF_CALL(GetProcAddress)(hKernel, "VirtualQuery");
        LPVOID pVQueryEx = (LPVOID)SPOOF_CALL(GetProcAddress)(hKernel, "VirtualQueryEx");

        if (pVQuery) {
            if (MH_CreateHook(pVQuery, &hkVirtualQuery, (LPVOID*)&oVirtualQuery) == MH_OK) {
                MH_EnableHook(pVQuery);
            }
        }

        if (pVQueryEx) {
            if (MH_CreateHook(pVQueryEx, &hkVirtualQueryEx, (LPVOID*)&oVirtualQueryEx) == MH_OK) {
                MH_EnableHook(pVQueryEx);
            }
        }
    }
}