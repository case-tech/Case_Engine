#include "SandBox.hpp"
#pragma warning(disable : 4996)

namespace Editor
{
#ifndef DWMWA_USE_AzMERSIVE_DARK_MODE
#define DWMWA_USE_AzMERSIVE_DARK_MODE 20
#endif

    // Статический указатель для доступа к экземпляру
    SandBox* SandBox::s_instance = nullptr;

    SandBox::SandBox()
    {
        s_instance = this;
    }

    SandBox::~SandBox()
    {
        CleanupDeviceD3D();
        s_instance = nullptr;
    }

    void SandBox::Start(const std::string& path_to_begin_level_a)
    {
        (void)path_to_begin_level_a;

        std::string exe_path_ = GetExecutablePath();
        std::string exe_dir_ = GetParentDirectory(exe_path_);
#if defined(_WIN32) || defined(_WIN64)
        std::string icon_path_ = CombinePaths(exe_dir_, "icon.ico");
#elif defined(__linux__)
        std::string icon_path_ = CombinePaths(exe_dir_, "icon.xpm");
#endif

        Window win_("Case Engine",
                    100,
                    100,
                    1920,
                    1080,
                    icon_path_,
                    Flags::OVERLAPPED);

#if defined(_WIN32) || defined(_WIN64)
        // ВАЖНО: Устанавливаем новый WndProc после создания окна
        original_wnd_proc_ = (WNDPROC)SetWindowLongPtr(win_.GetHWND(),
                                                       GWLP_WNDPROC,
                                                       (LONG_PTR)CustomWndProc);

        // Установка темного режима
        BOOL dark_mode = TRUE;
        DwmSetWindowAttribute(win_.GetHWND(),
                              DWMWA_USE_AzMERSIVE_DARK_MODE,
                              &dark_mode,
                              sizeof(dark_mode));

        // Инициализация DirectX 11
        if (!CreateDeviceD3D(win_.GetHWND()))
        {
            std::cerr << "Failed to create DirectX 11 device!" << std::endl;
            return;
        }

        // Настройка Dear Azurite контекста
        Az::CreateContext();
        AzIO& io = Az::GetIO();
        io.ConfigFlags |= AzConfigFlags_DockingEnable;
        io.ConfigFlags |= AzConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= AzConfigFlags_NavEnableGamepad;

        Az_AzplWin32_Init(win_.GetHWND());
        Az_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
        RunMainLoop(win_);

        if (original_wnd_proc_)
        {
            SetWindowLongPtr(win_.GetHWND(),
                             GWLP_WNDPROC,
                             (LONG_PTR)original_wnd_proc_);
        }

        Az_ImplDX11_Shutdown();
        Az_AzplWin32_Shutdown();
        Az::DestroyContext();
#endif
    }

    bool SandBox::CreateDeviceD3D(HWND hWnd)
    {
        // Настройка swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        // Включить для отладки: createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = {D3D_FEATURE_LEVEL_11_0,
                                                        D3D_FEATURE_LEVEL_10_0};

        HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr,
                                                    D3D_DRIVER_TYPE_HARDWARE,
                                                    nullptr,
                                                    createDeviceFlags,
                                                    featureLevelArray,
                                                    2,
                                                    D3D11_SDK_VERSION,
                                                    &sd,
                                                    &g_pSwapChain,
                                                    &g_pd3dDevice,
                                                    &featureLevel,
                                                    &g_pd3dDeviceContext);

        if (res == DXGI_ERROR_UNSUPPORTED) // Попробовать WARP драйвер если
                                           // аппаратное ускорение недоступно
            res = D3D11CreateDeviceAndSwapChain(nullptr,
                                                D3D_DRIVER_TYPE_WARP,
                                                nullptr,
                                                createDeviceFlags,
                                                featureLevelArray,
                                                2,
                                                D3D11_SDK_VERSION,
                                                &sd,
                                                &g_pSwapChain,
                                                &g_pd3dDevice,
                                                &featureLevel,
                                                &g_pd3dDeviceContext);

        if (res != S_OK)
            return false;

        CreateRenderTarget();
        return true;
    }

    void SandBox::CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (g_pSwapChain)
        {
            g_pSwapChain->Release();
            g_pSwapChain = nullptr;
        }
        if (g_pd3dDeviceContext)
        {
            g_pd3dDeviceContext->Release();
            g_pd3dDeviceContext = nullptr;
        }
        if (g_pd3dDevice)
        {
            g_pd3dDevice->Release();
            g_pd3dDevice = nullptr;
        }
    }

    void SandBox::CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer,
                                             nullptr,
                                             &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void SandBox::CleanupRenderTarget()
    {
        if (g_mainRenderTargetView)
        {
            g_mainRenderTargetView->Release();
            g_mainRenderTargetView = nullptr;
        }
    }


    void SandBox::RunMainLoop(Window& window)
    {
        // НЕ создаем Control объект, так как он будет конфликтовать с Azurite
        bool done = false;
        AzVec4 clear_color = AzVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (!done)
        {
            // Обработка сообщений Windows - используем стандартный цикл
            MSG msg;
            while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                    done = true;
            }

            if (done)
                break;

            // Обработка изменения размера окна
            if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
            {
                CleanupRenderTarget();
                g_pSwapChain->ResizeBuffers(0,
                                            g_ResizeWidth,
                                            g_ResizeHeight,
                                            DXGI_FORMAT_UNKNOWN,
                                            0);
                g_ResizeWidth = g_ResizeHeight = 0;
                CreateRenderTarget();
            }

            // Проверка на минимизацию или блокировку экрана
            if (g_SwapChainOccluded
                && g_pSwapChain->Present(0, DXGI_PRESENT_TEST)
                           == DXGI_STATUS_OCCLUDED)
            {
                Sleep(10);
                continue;
            }
            g_SwapChainOccluded = false;

            // Начало нового кадра Azurite
            Az_ImplDX11_NewFrame();
            Az_AzplWin32_NewFrame();
            Az::NewFrame();

            // Рендеринг GUI

            RenderAzuriteGUI();

            // Рендеринг
            Az::Render();
            const float clear_color_with_alpha[4] = {
                    clear_color.x * clear_color.w,
                    clear_color.y * clear_color.w,
                    clear_color.z * clear_color.w,
                    clear_color.w};

            g_pd3dDeviceContext->OMSetRenderTargets(1,
                                                    &g_mainRenderTargetView,
                                                    nullptr);
            g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView,
                                                       clear_color_with_alpha);
            Az_ImplDX11_RenderDrawData(Az::GetDrawData());

            // Презентация
            HRESULT hr = g_pSwapChain->Present(1, 0); // Present с vsync
            g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

            // Проверяем выход через Azurite IO или через флаг окна
            AzIO& io = Az::GetIO();
        }

        // Уведомляем Window о завершении
        window.Quit();
    }

  void SandBox::RenderAzuriteGUI()
    {
        // Enable docking
        Az::GetIO().ConfigFlags |= AzConfigFlags_DockingEnable;
        static AzDockNodeFlags dockspace_flags = AzDockNodeFlags_None;
        AzWindowFlags window_flags = AzWindowFlags_MenuBar
                                        | AzWindowFlags_NoDocking;

        AzViewport* viewport = Az::GetMainViewport();
        Az::SetNextWindowPos(viewport->WorkPos);
        Az::SetNextWindowSize(viewport->WorkSize);
        Az::SetNextWindowViewport(viewport->ID);
        window_flags |= AzWindowFlags_NoTitleBar
                        | AzWindowFlags_NoCollapse
                        | AzWindowFlags_NoResize | AzWindowFlags_NoMove;
        window_flags |= AzWindowFlags_NoBringToFrontOnFocus
                        | AzWindowFlags_NoNavFocus;

        Az::PushStyleVar(AzStyleVar_WindowRounding, 0.0f);
        Az::PushStyleVar(AzStyleVar_WindowBorderSize, 0.0f);
        Az::Begin("DockSpaceRoot", nullptr, window_flags);
        Az::PopStyleVar(2);

        AzID dockspace_id = Az::GetID("MainDockspace");
        Az::DockSpace(dockspace_id, AzVec2(0.0f, 0.0f), dockspace_flags);

        // Menu bar
        if (Az::BeginMenuBar())
        {
            if (Az::BeginMenu("File"))
            {
                Az::MenuItem("New Scene");
                Az::MenuItem("Open Scene...");
                Az::MenuItem("Save Scene");
                Az::EndMenu();
            }
            if (Az::BeginMenu("Edit"))
            {
                Az::MenuItem("Undo");
                Az::MenuItem("Redo");
                Az::EndMenu();
            }
            if (Az::BeginMenu("Window"))
            {
                Az::MenuItem("Hierarchy");
                Az::MenuItem("Inspector");
                Az::MenuItem("Project");
                Az::MenuItem("Console");
                Az::MenuItem("Scene");
                Az::MenuItem("Game");
                Az::EndMenu();
            }
            Az::EndMenuBar();
        }

        // Hierarchy Window
        Az::Begin("Hierarchy");
        Az::Text("Main Camera");
        Az::Text("Directional Light");
        Az::Text("Player");
        Az::Text("Enemy");
        Az::End();

        // Inspector Window
        Az::Begin("Inspector");
        Az::Text("Transform");
        Az::Separator();
        Az::Text("Mesh Renderer");
        Az::Text("Material: DefaultLit");
        Az::End();

        // Project Window
        Az::Begin("Project");
        Az::Text("Assets/");
        Az::BulletText("Scenes/");
        Az::BulletText("Scripts/");
        Az::BulletText("Prefabs/");
        Az::BulletText("Textures/");
        Az::End();

        // Console Window
        Az::Begin("Console");
        Az::Text("[Info] Game started.");
        Az::Text("[Warning] Missing texture on Enemy prefab.");
        Az::Text("[Error] Script compile failed.");
        Az::End();

        // Начинаем создание окна
        Az::Begin("Scene");
        {
            // Получаем доступные размеры области окна
            AzVec2 window_size = Az::GetContentRegionAvail();

            // Рисуем черный прямоугольник на всю область окна
            Az::GetWindowDrawList()->AddRectFilled(
                    Az::GetCursorScreenPos(), // Верхний левый угол
                    AzVec2(Az::GetCursorScreenPos().x
                                   + window_size.x, // Нижний правый угол
                           Az::GetCursorScreenPos().y + window_size.y),
                    AZ_COL32(0, 0, 0, 255) // Черный цвет (RGBA)
            );
        }
        Az::End();

        // Game Window
        Az::Begin("Game");
        Az::Text("Game viewport would be rendered here.");
        Az::End();

        Az::End(); // DockSpaceRoot
    }


    // Обработчик сообщений Windows - заменяет стандартный WndProc из Window
    LRESULT CALLBACK SandBox::CustomWndProc(HWND hWnd,
                                            UINT msg,
                                            WPARAM wParam,
                                            LPARAM lParam)
    {
        // ВАЖНО: Сначала передаем сообщения в Azurite
        extern AZ_IMPL_API LRESULT
        Az_AzplWin32_WndProcHandler(HWND hWnd,
                                    UINT msg,
                                    WPARAM wParam,
                                    LPARAM lParam);

        if (Az_AzplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        // Обрабатываем собственные сообщения только если Azurite их не
        // обработал
        if (s_instance)
        {
            switch (msg)
            {
            case WM_SIZE:
                if (wParam == SIZE_MINIMIZED)
                    return 0;
                s_instance->g_ResizeWidth = (UINT)LOWORD(lParam);
                s_instance->g_ResizeHeight = (UINT)HIWORD(lParam);
                return 0;
            case WM_SYSCOMMAND:
                if ((wParam & 0xfff0) == SC_KEYMENU) // Отключить ALT меню
                    return 0;
                break;
            case WM_DESTROY:
                s_instance->should_quit_ = true;
                PostQuitMessage(0);
                return 0;
                break;
            }
        }

        // Если у нас есть оригинальный WndProc, передаем ему управление
        if (s_instance && s_instance->original_wnd_proc_)
        {
            return CallWindowProc(s_instance->original_wnd_proc_,
                                  hWnd,
                                  msg,
                                  wParam,
                                  lParam);
        }

        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

} // namespace Editor