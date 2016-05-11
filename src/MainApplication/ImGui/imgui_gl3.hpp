// ImGui GLFW binding with OpenGL3 + shaders
// In this binding, ImTextureID is used to store an OpenGL 'GLuint' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

/// @note If you use this binding you'll need to call 4 functions: init(), newFrame(), ImGui::Render() and shutdown()

#include <Engine/Renderer/OpenGL/OpenGL.hpp>

namespace ImGuiGL3 {

    struct GLFWwindow;

    IMGUI_API bool init();
    IMGUI_API void shutdown();
    IMGUI_API void newFrame(int w, int h);

    // Use if you want to reset your rendering device without losing ImGui state.
    IMGUI_API void invalidateDeviceObjects();
    IMGUI_API bool createDeviceObjects();

    // GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
    // Provided here if you want to chain callbacks.
    // You can also handle inputs yourself and use those as a reference.
    //IMGUI_API void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    //IMGUI_API void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    //IMGUI_API void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    //IMGUI_API void charCallback(GLFWwindow* window, unsigned int c);

}