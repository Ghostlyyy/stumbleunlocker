#include <Windows.h>
#include "pch.h"
#include <Minhook.h>
#include <cstdio>

uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");
uintptr_t HasSkinOffset = 0x59DC00;


void CreateConsole()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
}

void init()
{
    MH_Initialize();
    CreateConsole();

}

bool(__fastcall* HasSkin_o)(DWORD*, const char*, DWORD*);
bool __stdcall HasSkin_h(DWORD* __this, const char* skinID, DWORD* method)
{
    printf("HasSkin called with skinID: %s", skinID + 0xA);

    return true;
}

void main()
{
    init();
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HasSkinOffset), &HasSkin_h, (LPVOID*)&HasSkin_o);
    MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasSkinOffset));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

