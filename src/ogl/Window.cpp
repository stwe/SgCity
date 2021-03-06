// This file is part of the SgCity project.
//
// Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "Game.h"
#include "OpenGL.h"
#include "Log.h"
#include "SgException.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "event/EventManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::Window::Window()
{
    Log::SG_LOG_DEBUG("[Window::Window()] Create Window.");

    Init();
}

sg::ogl::Window::~Window() noexcept
{
    Log::SG_LOG_DEBUG("[Window::~Window()] Destruct Window.");

    CleanUp();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const std::string& sg::ogl::Window::GetTitle() const
{
    return m_title;
}

int sg::ogl::Window::GetWidth() const
{
    return m_width;
}

int sg::ogl::Window::GetHeight() const
{
    return m_height;
}

const glm::mat4& sg::ogl::Window::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

const glm::mat4& sg::ogl::Window::GetOrthographicProjectionMatrix() const
{
    return m_orthographicProjectionMatrix;
}

GLFWwindow* sg::ogl::Window::GetWindowHandle() const
{
    return m_windowHandle;
}

//-------------------------------------------------
// Rendering
//-------------------------------------------------

void sg::ogl::Window::SwapBuffersAndCallEvents() const
{
    glfwSwapBuffers(m_windowHandle);
    glfwPollEvents();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

bool sg::ogl::Window::WindowShouldClose() const
{
    return glfwWindowShouldClose(m_windowHandle);
}

void sg::ogl::Window::Close() const
{
    glfwSetWindowShouldClose(m_windowHandle, true);
}

//-------------------------------------------------
// Input Polling
//-------------------------------------------------

bool sg::ogl::Window::IsKeyPressed(const int t_keyCode) const
{
    return glfwGetKey(m_windowHandle, t_keyCode) == GLFW_PRESS ||
        glfwGetKey(m_windowHandle, t_keyCode) == GLFW_REPEAT;
}

bool sg::ogl::Window::IsMouseButtonPressed(const int t_button) const
{
    return glfwGetMouseButton(m_windowHandle, t_button) == GLFW_PRESS;
}

glm::vec2 sg::ogl::Window::GetMousePosition() const
{
    double x, y;
    glfwGetCursorPos(m_windowHandle, &x, &y);
    return { static_cast<float>(x), static_cast<float>(y) };
}

//-------------------------------------------------
// ImGui
//-------------------------------------------------

void sg::ogl::Window::ImGuiBegin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void sg::ogl::Window::ImGuiEnd()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::Window::Init()
{
    Log::SG_LOG_DEBUG("[Window::Init()] Initializing window.");

    LoadConfig();
    InitWindow();
    InitProjectionMatrix();
    InitImGui();
    InitInputCallbacks();

    Log::SG_LOG_DEBUG("[Window::Init()] The window was successfully initialized.");
}

void sg::ogl::Window::LoadConfig()
{
    Log::SG_LOG_DEBUG("[Window::LoadConfig()] Load values from config file.");

    m_title = Game::INI.Get<std::string>("window", "title");

    Log::SG_LOG_INFO("Game window title: {}", m_title);

    m_width = Game::INI.Get<int>("window", "width");
    m_height = Game::INI.Get<int>("window", "height");

    m_width = std::max(m_width, MIN_WIDTH);
    m_height = std::max(m_height, MIN_HEIGHT);

    Log::SG_LOG_INFO("Game window width: {}", m_width);
    Log::SG_LOG_INFO("Game window height: {}", m_height);

    fovDeg = Game::INI.Get<float>("frustum", "fov_deg");
    nearPlane = Game::INI.Get<float>("frustum", "near");
    farPlane = Game::INI.Get<float>("frustum", "far");
}

void sg::ogl::Window::InitWindow()
{
    Log::SG_LOG_DEBUG("[Window::InitWindow()] Initializing GLFW.");

    // Setup an error callback.
    glfwSetErrorCallback(
        [](const int t_error, const char* t_description)
        {
            Log::SG_LOG_ERROR("GLFW Error ({}) {}", t_error, t_description);
        }
    );

    // Initialize GLFW.
    if (!glfwInit())
    {
        throw SG_EXCEPTION("[Window::InitWindow()] Unable to initialize GLFW.");
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    m_windowHandle = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_windowHandle)
    {
        glfwTerminate();
        throw SG_EXCEPTION("[Window::InitWindow()] Failed to create the GLFW window.");
    }

    // Get the resolution of the primary monitor.
    const auto* primaryMonitor{ glfwGetVideoMode(glfwGetPrimaryMonitor()) };
    if (!primaryMonitor)
    {
        throw SG_EXCEPTION("[Window::InitWindow()] Unable to get the primary monitor.");
    }

    // Center our window.
    glfwSetWindowPos(GetWindowHandle(), (primaryMonitor->width - m_width) / 2, (primaryMonitor->height - m_height) / 2);

    // Make the OpenGL context current.
    glfwMakeContextCurrent(GetWindowHandle());

    // Update viewport.
    glViewport(0, 0, m_width, m_height);

    // Initialize GLEW.
    Log::SG_LOG_DEBUG("[Window::InitWindow()] Initializing GLEW.");
    const auto err{ glewInit() };
    if (err != GLEW_OK)
    {
        throw SG_EXCEPTION("[Window::InitWindow()] Unable to initialize GLEW." + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    // Print out some information about the graphics drivers.
    Log::SG_LOG_INFO("OpenGL version: {}", glGetString(GL_VERSION));
    Log::SG_LOG_INFO("GLSL version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
    Log::SG_LOG_INFO("Vendor: {}", glGetString(GL_VENDOR));
    Log::SG_LOG_INFO("Renderer: {}", glGetString(GL_RENDERER));

    // Make the window visible.
    glfwShowWindow(GetWindowHandle());

    // enable depth && stencil buffer
    OpenGL::EnableDepthAndStencilTesting();
}

void sg::ogl::Window::InitProjectionMatrix()
{
    Log::SG_LOG_DEBUG("[Window::InitProjectionMatrix()] Initializing projection.");

    UpdateProjectionMatrix();
    UpdateOrthographicProjectionMatrix();
}

void sg::ogl::Window::InitImGui() const
{
    Log::SG_LOG_DEBUG("[Window::InitImGui()] Initializing ImGui.");

    // setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // setup renderer bindings without callbacks
    // the callbacks from ImGui will later be integrated into our own
    ImGui_ImplGlfw_InitForOpenGL(m_windowHandle, false);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // setup style
    ImGui::StyleColorsDark();
}

void sg::ogl::Window::InitInputCallbacks() const
{
    Log::SG_LOG_DEBUG("[Window::InitInputCallbacks()] Initializing input callbacks.");

    event::EventManager::SetKeyboardGlfwCallbacks(m_windowHandle);
    event::EventManager::SetMouseGlfwCallbacks(m_windowHandle);
}

//-------------------------------------------------
// Projection matrix
//-------------------------------------------------

void sg::ogl::Window::UpdateProjectionMatrix()
{
    m_projectionMatrix = glm::perspectiveFov
        (
            glm::radians(fovDeg),
            static_cast<float>(m_width),
            static_cast<float>(m_height),
            nearPlane,
            farPlane
        );
}

void sg::ogl::Window::UpdateOrthographicProjectionMatrix()
{
    m_orthographicProjectionMatrix = glm::ortho
        (
            0.0f,
            static_cast<float>(m_width),
            static_cast<float>(m_height),
            0.0f,
            -1.0f,
            1.0f
        );
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::Window::CleanUp() const
{
    Log::SG_LOG_DEBUG("[Window::CleanUp()] CleanUp window.");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_windowHandle);
    glfwTerminate();
}
