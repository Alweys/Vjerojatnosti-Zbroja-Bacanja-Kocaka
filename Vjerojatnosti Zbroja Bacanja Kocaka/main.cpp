#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <implot.h>
#include <implot_internal.h>
#include <d3d9.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#pragma comment (lib, "d3d9.lib" )


static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ApplyDefaultStyle()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->FrameRounding = 6;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(1.0f, 0.24f, 0.24f, 0.0f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.24f, 0.24f, 0.50f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.24f, 0.24f, 0.70f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 0.24f, 0.24f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.0f, 0.24f, 0.24f, 0.50f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.0f, 0.24f, 0.24f, 0.70f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.0f, 0.24f, 0.24f, 0.90f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.24f, 0.24f, 0.50f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.24f, 0.24f, 0.90f);
    colors[ImGuiCol_Button] = ImVec4(1.0f, 0.24f, 0.24f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(1.0f, 0.24f, 0.24f, 0.50f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.24f, 0.24f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(1.0f, 0.24f, 0.24f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 0.24f, 0.24f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 0.24f, 0.24f, 0.40f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 0.24f, 0.24f, 0.80f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

    colors[ImGuiCol_Tab] = ImVec4(1.0f, 0.24f, 0.24f, 0.40f);
    colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(1.0f, 0.24f, 0.24f, 0.20f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.0f, 0.24f, 0.24f, 0.30f);
}

void calculateProbabilities(std::vector<float>& probabilities, int currentDice, int currentSum) {
    if (currentDice == 0) {
        probabilities[currentSum]++;
    }
    else {
        for (int i = 1; i <= 6; i++) {
            calculateProbabilities(probabilities, currentDice - 1, currentSum + i);
        }
    }
};

void diceProbabilities(int diceCount, std::vector<float>& probabilities) {
    int totalOutcomes = pow(6, diceCount);
    int maxPossibleSum = diceCount * 6;

    probabilities.resize(maxPossibleSum + 1, 0.0);

    calculateProbabilities(probabilities, diceCount, 0);

    for (int i = 0; i <= maxPossibleSum; i++) {
        probabilities[i] /= totalOutcomes;
    }
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

std::vector<int> simulateDiceThrows(int numDice, int numThrows) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 6);

    std::vector<int> sums(6 * numDice + 1, 0);

    for (int i = 0; i < numThrows; ++i) {
        int sum = 0;
        for (int j = 0; j < numDice; ++j) {
            int result = dist(gen);
            sum += result;
        }
        sums[sum]++;
    }

    return sums;
}

int main(int, char**)
{

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("Vjerojatnosti Zbroja Bacanja Kocaka"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Vjerojatnosti Zbroja Bacanja Kocaka"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 900, NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Roboto-Medium.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    ApplyDefaultStyle();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y ));
        ImGui::Begin("Vjerojatnosti Zbroja Bacanja Kocaka", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

        static int kocke = 0, brojBacanja = 100, prevKocke = -1;
        static bool prikaziVjerojatnosti = false;
        static std::vector<int> rezultati;
        static std::vector<float> VjerojatnostRezultata;
        static std::vector<float> Vjerojatnosti;
        ImGui::Text("Broj kocaka"); ImGui::SameLine(); ImGui::SetNextItemWidth(50.0f); ImGui::Combo("   ", &kocke, " 2\0 3\0 4\0 5\0 6\0 7\0 8\0 9\0 10\0\0", 9);
        ImGui::Checkbox("Prikazi Vjerojatnosti", &prikaziVjerojatnosti);

        if (kocke != prevKocke)
        {
            diceProbabilities(kocke + 2, Vjerojatnosti);
            rezultati.clear();
            prevKocke = kocke;
        }

        ImGui::SliderInt("Broj bacanja", &brojBacanja, 1, 10000);
        if (ImGui::Button("Simuliraj bacanje kocaka"))
        {
            rezultati = simulateDiceThrows(kocke + 2, brojBacanja);
        }

        ImGui::BeginChild("GrafChild", ImVec2(ImGui::GetIO().DisplaySize.x / 1.03f, ImGui::GetIO().DisplaySize.y / 1.7f), true);
        {
            static float data[100];
            static float data2[100];
            static float positions[100];
            int count = (kocke + 2) * 6;
            for (int i = 0; i <= count; i++)
            {
                positions[i] = i;
                data[i] = Vjerojatnosti[i];
                if (!rezultati.empty())
                    data2[i] = rezultati[i] / (brojBacanja * 1.0f);
            }

            if (ImPlot::BeginPlot("Vjerojatnosti Zbroja Bacanja Kocaka")) {
                ImPlot::SetupAxes("Zbroj kocaka", "Vjerojatnost", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

                if(!rezultati.empty())
                    ImPlot::PlotBars("Statistika bacanja", data2, rezultati.size(), 0.7);

                ImPlot::PlotStems("Vjerojatnost", data, count + 1);
                ImPlotContext& gp = *GImPlot;
                ImPlotPlot& plot = *gp.CurrentPlot;
                ImPlotAxis& x_axis = plot.XAxis(ImAxis_Y1);
                if(prikaziVjerojatnosti)
                    for (int i = 0; i <= count; i++)
                    {
                        std::string text = std::to_string(data[i]);
                        ImPlot::PlotText(text.c_str(), i, Vjerojatnosti[i] + (fabsf(x_axis.Range.Max - x_axis.Range.Min) / 8.0f) / 2.0f);
                    }

                ImPlot::PlotLine("Vjerojatnost Crte", positions, data, count+1);

                ImPlot::EndPlot();
            }
        }ImGui::EndChild();
       
        ImGui::End();
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);


        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImPlot::DestroyContext();
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
