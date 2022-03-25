#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"

#include "mem.h"
#include "menu.h"
#include "statistics.h"

bool bShowMenu;
HWND tWnd;
RECT tSize;
int Width;
int Height;
GLFWwindow* window;
LPCWSTR tWindowName = L"Door in the Woods";
int item_current_idx = 0;

char waypoint_name[50] = "";
std::vector<std::string> waypoints;

void SetWindowToTarget()
{
    while (true)
    {
        //Returns a handle to the target window
        tWnd = FindWindow(0, tWindowName);
        if (tWnd)
        {
            //Gets the dimensions of the window and stores it in tSize
            GetWindowRect(tWnd, &tSize);
            Width = tSize.right - tSize.left;
            Height = tSize.bottom - tSize.top;
            //Gets the styles of the target window (how it looks)
            DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
            //If the window is not borderless
            if (dwStyle & WS_BORDER)
            {
                tSize.top += 23;
                Height -= 23;
            }
            //Sets the coords of our overlay as the coords of the target window
            //MoveWindow(hWnd, tSize.left, tSize.top, Width, Height, true);
            glfwSetWindowPos(window, tSize.left, tSize.top);
            glfwSetWindowSize(window, Width, Height);
        }
        else
        {

        }
    }
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool windowInFocus()
{
    HWND window1 = FindWindowA(NULL, "DITWClient");
    HWND window2 = FindWindowA(NULL, "Door in the Woods");
    return window1 == GetForegroundWindow() || window2 == GetForegroundWindow();
}

int startOverlay()
{ 
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    //glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, "DITWClient", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwSetWindowAttrib(window, GLFW_DECORATED, false);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //IM_ASSERT(font != NULL);

    bool bShowMenu = true;

    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            bShowMenu = !bShowMenu;
        }

        //std::cout << ImGui::IsMouseHoveringAnyWindow() << std::endl;

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (bShowMenu && windowInFocus())
        {
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);
            {

                {
                    ImGuiWindowFlags window_flags = 0;
                    window_flags |= ImGuiWindowFlags_NoTitleBar;
                    ImGui::Begin("DitW Cheat", NULL, window_flags);

                    if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_None))
                    {
                        //Stats tab
                        if (ImGui::BeginTabItem("Stats"))
                        {
                            //Experience
                            ImGui::SliderInt("##expSlide", &menuStats.experience, 0, 500);
                            ImGui::SameLine();
                            ImGui::Text("Experience");
                            //Strength
                            ImGui::SliderInt("##strengthSlide", &menuStats.strength, 0, 9999);
                            ImGui::SameLine();
                            ImGui::Text("Strength");
                            //Agility
                            ImGui::SliderInt("##agilitySlide", &menuStats.agility, 0, 9999);
                            ImGui::SameLine();
                            ImGui::Text("Agility");
                            //Endurance
                            ImGui::SliderInt("##enduranceSlide", &menuStats.endurance, 0, 9999);
                            ImGui::SameLine();
                            ImGui::Text("Endurance");
                            //Willpower
                            ImGui::SliderInt("##willpowerSlide", &menuStats.willpower, 0, 9999);
                            ImGui::SameLine();
                            ImGui::Text("Willpower");

                            if (ImGui::Button("Max")) { maxAllStats(); };
                            ImGui::SameLine();
                            if (ImGui::Button("Update")) { UpdateValues(); };
                            ImGui::EndTabItem();
                        }
                        //Waypoints tab
                        if (ImGui::BeginTabItem("Waypoints"))
                        {
                            ImGui::InputTextWithHint("##waypointInput", "waypoint name", waypoint_name, IM_ARRAYSIZE(waypoint_name));
                            if (ImGui::BeginListBox("##waypointsList"))
                            {
                                for (int n = 0; n < waypoints.size(); n++)
                                {
                                    const bool is_selected = (item_current_idx == n);
                                    if (ImGui::Selectable(waypoints[n].c_str(), is_selected))
                                        item_current_idx = n;

                                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                                    if (is_selected)
                                        ImGui::SetItemDefaultFocus();
                                }
                                ImGui::EndListBox();
                            }
                            if (ImGui::Button("Save")) { saveWaypoint(); };
                            ImGui::SameLine();
                            //Make sure for the real thing that waypoint.size() != 0 for this
                            if (ImGui::Button("Teleport")) { Teleport(item_current_idx); }
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Misc"))
                        {
                            if (ImGui::Button("Refill stamina")) 
                            {
                                if (initGameStatistics(false))
                                {
                                    *gameStats.stamina = 999999;
                                }
                            }
                            if (ImGui::Button("Refill sanity"))
                            {
                                if (initGameStatistics(false))
                                {
                                    if (*gameStats.sanity > 1) { *gameStats.sanity = 999999; }
                                }
                            }
                            if (ImGui::Button("Refill health"))
                            {
                                if (initGameStatistics(false))
                                {
                                    if (*gameStats.health > 1) { *gameStats.health = 999999; }
                                }
                            }
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }

                    ImGui::End();
                }
            }
        }
        else
        {
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true);
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void saveWaypoint()
{
    if (waypoint_name[0] != '\0' && initGameStatistics(false))
    {
        waypoints.push_back(waypoint_name);
        gameStats.posxList.push_back(*gameStats.posx);
        gameStats.posyList.push_back(*gameStats.posy);
        waypoint_name[0] = '\0';
    }
}