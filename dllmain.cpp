#include <Windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"

#include "statistics.h"
#include "menu.h"
#include "mem.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    uintptr_t baseAddress = (uintptr_t)GetModuleInfo(L"Door in the Woods.exe").lpBaseOfDll;

    //Find addresses
    //HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)startOverlay, 0, 0, 0);
    startOverlay();
    //initGameStatistics(true);



    while (!GetAsyncKeyState(VK_END) || 1)
    {
        Sleep(1000);
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    //CloseHandle(handle);
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

