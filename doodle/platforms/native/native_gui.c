#include <doodle/platforms/native/native_gui.h>

// Include the Direct3D and related headers as needed.
#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <GLFW/glfw3.h>
#include <d3d11.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <windows.h>
#endif

#include <d3d11.h>
#include <stdio.h>
#include <string.h>

// (Optional) For ZeroMemory, etc. if you prefer
#ifdef _WIN32
#include <windows.h>
#endif

#include <doodle/platforms/native/native_debug.h>

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

void native_gui_display_texture(doodle_texture_t *texture)
{
    NATIVE_LOG("Displaying texture\n");

#ifdef _WIN32
    // If we don't have a valid device/context, do nothing
    if (!g_native_gui.d3d_device || !g_native_gui.d3d_context || !texture || !texture->pixels)
        return;

    // 1. Create a D3D11 texture description
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(texDesc));
    texDesc.Width              = texture->width;
    texDesc.Height             = texture->height;
    texDesc.MipLevels          = 1;
    texDesc.ArraySize          = 1;
    texDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM; // assumes doodle_texture is RGBA8
    texDesc.SampleDesc.Count   = 1;
    texDesc.Usage              = D3D11_USAGE_DYNAMIC; // so we can write to it each time
    texDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;

    // 2. Create the ID3D11Texture2D
    ID3D11Texture2D* pTexture2D = NULL;
    HRESULT hr = ID3D11Device_CreateTexture2D(g_native_gui.d3d_device, &texDesc, NULL, &pTexture2D);
    if (FAILED(hr) || !pTexture2D)
    {
        NATIVE_LOG("Failed to create ID3D11Texture2D from doodle_texture.\n");
        return;
    }

    // 3. Map the texture and copy the CPU pixels
    D3D11_MAPPED_SUBRESOURCE mapped;
    hr = ID3D11DeviceContext_Map(g_native_gui.d3d_context, (ID3D11Resource*)pTexture2D, 0,
                                 D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (SUCCEEDED(hr))
    {
        // Copy row by row into the mapped subresource
        // Each pixel is 4 bytes (RGBA), each row in GPU texture is mapped.RowPitch
        uint8_t* dstData = (uint8_t*)mapped.pData;
        uint8_t* srcData = (uint8_t*)texture->pixels; // doodle_texture->pixels is an array of (r,g,b,a)

        for (uint32_t y = 0; y < texture->height; ++y)
        {
            memcpy(dstData + y * mapped.RowPitch,
                   srcData + y * (texture->width * 4),
                   texture->width * 4);
        }

        ID3D11DeviceContext_Unmap(g_native_gui.d3d_context, (ID3D11Resource*)pTexture2D, 0);
    }
    else
    {
        NATIVE_LOG("Failed to map ID3D11Texture2D.\n");
        ID3D11Texture2D_Release(pTexture2D);
        return;
    }

    // 4. Create a shader resource view for ImGui to use
    ID3D11ShaderResourceView* pSRV = NULL;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format                    = texDesc.Format;
    srvDesc.ViewDimension            = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels       = 1;

    hr = ID3D11Device_CreateShaderResourceView(
         g_native_gui.d3d_device, (ID3D11Resource*)pTexture2D, &srvDesc, &pSRV);

    if (FAILED(hr) || !pSRV)
    {
        NATIVE_LOG("Failed to create SRV.\n");
        ID3D11Texture2D_Release(pTexture2D);
        return;
    }

    // 5. Use ImGui to display the texture. 
    //    In cimgui, igImage expects an ImTextureID, 
    //    which we pass the SRV pointer as (void*) or (ImTextureID).
    ImVec2 imageSize = (ImVec2){(float)texture->width, (float)texture->height};

    // Typically you wrap this in an ImGui window or just directly call igImage.
    // For example:
    // igBegin("Texture Window", NULL, 0);
    igImage((ImTextureID)pSRV, imageSize, (ImVec2){0, 0}, (ImVec2){1, 1},
            (ImVec4){1, 1, 1, 1}, (ImVec4){0, 0, 0, 0});
    // igEnd();

    // 6. Release the SRV and texture (since we re-create them each call)
    ID3D11ShaderResourceView_Release(pSRV);
    ID3D11Texture2D_Release(pTexture2D);

#endif // _WIN32
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