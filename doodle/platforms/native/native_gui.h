#ifndef __NATIVE_GUI_H__
#define __NATIVE_GUI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <GLFW/glfw3.h>

// cimgui forward declarations
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui/cimgui.h>

// For Win32 handle, if needed
#ifdef _WIN32
#include <windows.h>
#else
typedef void* HWND; // Stub if not on Windows
#endif

#include <doodle/core/modules/doodle_renderer.h>

// Forward declare D3D resources to avoid forcing all includes here
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

/**
 * @brief Singleton that encapsulates all state for ImGui + Direct3D + GLFW.
 */
typedef struct native_gui_state {
    // Window handle
    GLFWwindow*                    window;

    // D3D resources
    struct ID3D11Device*           d3d_device;
    struct ID3D11DeviceContext*    d3d_context;
    struct IDXGISwapChain*         d3d_swap_chain;
    struct ID3D11RenderTargetView* main_render_target_view;

    // ImGui context and a color for clearing, for demonstration
    ImGuiContext*                  imgui_context;
    ImVec4                         clear_color;
    
} native_gui_state_t;

/**
 * @brief Initialize the singleton GUI system (GLFW, D3D, ImGui).
 *
 * @param title   Window title.
 * @param width   Window width.
 * @param height  Window height.
 * @return true   If initialization succeeds.
 * @return false  Otherwise.
 */
bool native_gui_initialize(const char* title, int width, int height);

/**
 * @brief Shut down the singleton GUI system, cleaning up all resources.
 */
void native_gui_shutdown(void);

void native_gui_display_texture(doodle_texture_t *texture);

/**
 * @brief Call once per frame after building your ImGui UI to render and present.
 */
void native_gui_render_frame(void);

/**
 * @brief GLFW window size callback that handles resizing the swap chain, etc.
 */
void native_gui_window_size_callback(GLFWwindow* window, int width, int height);

/**
 * @brief Accessor for the singleton's state pointer.
 *        Use carefully; typically, you won’t need to call this unless you want
 *        direct access to D3D device/context or the window pointer.
 *
 * @return Pointer to the singleton's native_gui_state_t.
 */
native_gui_state_t* native_gui_get_state(void);

#ifdef __cplusplus
}
#endif

#endif // __NATIVE_GUI_H__
