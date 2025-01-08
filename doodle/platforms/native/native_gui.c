#include <doodle/platforms/native/native_gui.h>

// Include the Direct3D and related headers as needed.
#include <GLFW/glfw3native.h>
#include <d3d11.h>
#include <stdio.h>
#include <string.h>

// (Optional) For ZeroMemory, etc. if you prefer
#ifdef _WIN32
#include <windows.h>
#endif

//--------------------------------------------------------------------------------------
// Globals / Singleton
//--------------------------------------------------------------------------------------

// Our one and only native_gui_state_t.
static native_gui_state_t g_native_gui = {0};
static bool g_is_initialized = false;

//--------------------------------------------------------------------------------------
// Forward function declarations internal to this file
//--------------------------------------------------------------------------------------

static bool create_device_d3d(native_gui_state_t* state, HWND hwnd);
static void cleanup_device_d3d(native_gui_state_t* state);
static void create_render_target(native_gui_state_t* state);
static void cleanup_render_target(native_gui_state_t* state);

//--------------------------------------------------------------------------------------
// Public API (singleton accessors)
//--------------------------------------------------------------------------------------

native_gui_state_t* native_gui_get_state(void) {
    // You could assert here if !g_is_initialized or return NULL if uninitialized.
    return &g_native_gui;
}

bool native_gui_initialize(const char* title, int width, int height) {
    if (g_is_initialized) {
        // Already initialized, do nothing or return false
        fprintf(stderr, "native_gui_initialize called more than once.\n");
        return true;
    }

    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        return false;
    }

    // Configure GLFW to create a window suitable for no default OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    g_native_gui.window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!g_native_gui.window) {
        fprintf(stderr, "Failed to create GLFW window.\n");
        glfwTerminate();
        return false;
    }

#ifdef _WIN32
    HWND hwnd = glfwGetWin32Window(g_native_gui.window);
    if (!hwnd) {
        fprintf(stderr, "Failed to get Win32 window from GLFW.\n");
        glfwDestroyWindow(g_native_gui.window);
        glfwTerminate();
        return false;
    }

    // Create the D3D device, swap chain, etc.
    if (!create_device_d3d(&g_native_gui, hwnd)) {
        fprintf(stderr, "Failed to create D3D device.\n");
        glfwDestroyWindow(g_native_gui.window);
        glfwTerminate();
        return false;
    }
#endif

    // Set GLFW callback for window resizing
    glfwSetWindowSizeCallback(g_native_gui.window, native_gui_window_size_callback);

    // Create the ImGui context
    g_native_gui.imgui_context = igCreateContext(NULL);
    ImGuiIO* io = igGetIO();
    (void)io;  // not used in this example directly

    // Optional: enable keyboard navigation, docking, viewports, etc.
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
#ifdef IMGUI_HAS_DOCK
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif

    // Initialize ImGui backends
    ImGui_ImplGlfw_InitForOther(g_native_gui.window, true);
    ImGui_ImplDX11_Init(g_native_gui.d3d_device, g_native_gui.d3d_context);

    // Set a default clear color
    g_native_gui.clear_color = (ImVec4){0.45f, 0.55f, 0.60f, 1.0f};

    g_is_initialized = true;
    return true;
}

void native_gui_shutdown(void) {
    if (!g_is_initialized)
        return;  // Already shut down or not initialized

    // ImGui shutdown
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(g_native_gui.imgui_context);
    g_native_gui.imgui_context = NULL;

    // Clean up D3D
    cleanup_device_d3d(&g_native_gui);

    // Destroy GLFW window
    if (g_native_gui.window) {
        glfwDestroyWindow(g_native_gui.window);
        g_native_gui.window = NULL;
    }

    // Terminate GLFW
    glfwTerminate();

    g_is_initialized = false;
}

void native_gui_new_frame(void) {
    if (!g_is_initialized) return;

    // Poll events from GLFW
    glfwPollEvents();

    // Start a new ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();
}

void native_gui_render_frame(void) {
    if (!g_is_initialized) return;

    // End the ImGui frame, gather draw data
    igRender();

    // Clear the screen using our clear color
    float clear_color[4] = {
        g_native_gui.clear_color.x * g_native_gui.clear_color.w,
        g_native_gui.clear_color.y * g_native_gui.clear_color.w,
        g_native_gui.clear_color.z * g_native_gui.clear_color.w,
        g_native_gui.clear_color.w};

    ID3D11DeviceContext_OMSetRenderTargets(
        g_native_gui.d3d_context,
        1,
        &g_native_gui.main_render_target_view,
        NULL);
    ID3D11DeviceContext_ClearRenderTargetView(
        g_native_gui.d3d_context,
        g_native_gui.main_render_target_view,
        clear_color);

    // Render the ImGui draw data
    ImGui_ImplDX11_RenderDrawData(igGetDrawData());

    // (Optional) For multi-viewport support:
#ifdef IMGUI_HAS_DOCK
    ImGuiIO* io = igGetIO();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        igUpdatePlatformWindows();
        igRenderPlatformWindowsDefault(NULL, NULL);
    }
#endif

    // Present with vsync
    IDXGISwapChain_Present(g_native_gui.d3d_swap_chain, 1, 0);
}

//--------------------------------------------------------------------------------------
// GLFW callback for window resizing
//--------------------------------------------------------------------------------------
void native_gui_window_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;  // Not used here directly, but in case you want to do something else
    cleanup_render_target(&g_native_gui);
    if (g_native_gui.d3d_swap_chain) {
        IDXGISwapChain_ResizeBuffers(
            g_native_gui.d3d_swap_chain,
            0,
            width,
            height,
            DXGI_FORMAT_UNKNOWN,
            0);
    }
    create_render_target(&g_native_gui);
}

//--------------------------------------------------------------------------------------
// Internal helper functions for D3D initialization and teardown
//--------------------------------------------------------------------------------------
static bool create_device_d3d(native_gui_state_t* state, HWND hwnd) {
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
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    // createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;  // Enable debug if desired

    D3D_FEATURE_LEVEL featureLevel;
    D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,                      // Adapter
        D3D_DRIVER_TYPE_HARDWARE,  // Driver
        NULL,                      // Software rasterizer
        createDeviceFlags,
        featureLevelArray, 2,
        D3D11_SDK_VERSION,
        &sd,
        &state->d3d_swap_chain,
        &state->d3d_device,
        &featureLevel,
        &state->d3d_context);
    if (FAILED(hr)) {
        fprintf(stderr, "D3D11CreateDeviceAndSwapChain() failed.\n");
        return false;
    }

    create_render_target(state);
    return true;
}

static void cleanup_device_d3d(native_gui_state_t* state) {
    cleanup_render_target(state);
    if (state->d3d_swap_chain) {
        IDXGISwapChain_Release(state->d3d_swap_chain);
        state->d3d_swap_chain = NULL;
    }
    if (state->d3d_context) {
        ID3D11DeviceContext_Release(state->d3d_context);
        state->d3d_context = NULL;
    }
    if (state->d3d_device) {
        ID3D11Device_Release(state->d3d_device);
        state->d3d_device = NULL;
    }
}

static void create_render_target(native_gui_state_t* state) {
    ID3D11Resource* pBackBuffer = NULL;
    IDXGISwapChain_GetBuffer(
        state->d3d_swap_chain,
        0,
        &IID_ID3D11Texture2D,
        (void**)&pBackBuffer);

    if (pBackBuffer) {
        ID3D11Device_CreateRenderTargetView(
            state->d3d_device,
            pBackBuffer,
            NULL,
            &state->main_render_target_view);
        ID3D11Texture2D_Release(pBackBuffer);
    }
}

static void cleanup_render_target(native_gui_state_t* state) {
    if (state->main_render_target_view) {
        ID3D11RenderTargetView_Release(state->main_render_target_view);
        state->main_render_target_view = NULL;
    }
}