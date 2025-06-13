// Copyright (c) 2025 Case Technologies
#pragma once
#include "Az/Az.hpp" // Путь к Azurite
#include "UI/Az/az_impl_dx11.hpp"
#include "UI/Az/az_impl_win32.hpp"

#include <Aid/TypeToType/TypeToType.hpp>
#include <Graphics/Window/Control/Control.hpp>
#include <Graphics/Window/Window.hpp>
#include <d3d11.h>
#include <dwmapi.h>
#include <iostream>
#include <string>
#include <tchar.h>

using namespace CE_Kernel::Aid::TypeToType;
using namespace CE_Kernel::Graphics::Window;

namespace Editor
{
    class SandBox
    {
    public:
        SandBox();
        ~SandBox();
        void Start(const std::string& path_to_begin_level_a);

    private:
        // DirectX 11 members
        ID3D11Device* g_pd3dDevice = nullptr;
        ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
        IDXGISwapChain* g_pSwapChain = nullptr;
        bool g_SwapChainOccluded = false;
        UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
        ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
        WNDPROC original_wnd_proc_ = nullptr;

        // Флаг для контроля выхода из главного цикла
        bool should_quit_ = false;

        // Helper functions
        bool CreateDeviceD3D(HWND hWnd);
        void CleanupDeviceD3D();
        void CreateRenderTarget();
        void CleanupRenderTarget();
        void RunMainLoop(Window& window);
        void RenderAzuriteGUI();

        // Статический обработчик сообщений Windows
        static LRESULT CALLBACK CustomWndProc(HWND hWnd,
                                              UINT msg,
                                              WPARAM wParam,
                                              LPARAM lParam);
        static SandBox* s_instance;
    };
} // namespace Editor