#include <sstream>
#include "Application.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/Texture.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/renderer/SpriteRenderer.h"
#include "ogl/math/Transform.h"
#include "ogl/input/MouseInput.h"
#include "ogl/input/PickingTexture.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::Application::Application()
{
    Log::SG_LOG_DEBUG("[Application::Application()] Create Application.");
}

sg::Application::~Application() noexcept
{
    Log::SG_LOG_DEBUG("[Application::~Application()] Destruct Application.");

    CleanUp();
}

//-------------------------------------------------
// Run
//-------------------------------------------------

void sg::Application::Run()
{
    Log::SG_LOG_DEBUG("[Application::Run()] Starts running application.");

    Init();
    GameLoop();

    Log::SG_LOG_DEBUG("[Application::Run()] Goodbye ...");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::Application::Init()
{
    Log::SG_LOG_DEBUG("[Application::Init()] Initializing application.");

    m_window.Init();

    ogl::input::MouseInput::Init(m_window);

    //m_redGrid = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/red.png");
    //m_redGrid->Load();

    //m_full = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/full.png");
    //m_full->Load();

    m_spriteRenderer = std::make_unique<ogl::renderer::SpriteRenderer>();

    m_vao = std::make_unique<ogl::buffer::Vao>();
    m_vao->Add2DQuadVbo();

    m_pickingShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/picking");
    m_pickingShaderProgram->Load();

    m_pickingTexture = std::make_unique<ogl::input::PickingTexture>();
    m_pickingTexture->Init(m_window.GetWidth(), m_window.GetHeight());

    Log::SG_LOG_DEBUG("[Application::Init()] The application was successfully initialized.");
}

void sg::Application::Input()
{
    ogl::input::MouseInput::GetInstance().Input();

    m_window.CloseIfEscKeyPressed();

    if (ogl::input::MouseInput::GetInstance().IsRightButtonPressed())
    {
        m_camera.ProcessMouse(ogl::input::MouseInput::GetInstance().GetDisplVec());

        //Log::SG_LOG_DEBUG("Right Mouse Button pressed.");
    }

    if (ogl::input::MouseInput::GetInstance().IsLeftButtonPressed())
    {
        //Log::SG_LOG_DEBUG("Left Mouse Button pressed.");
        auto id{ m_pickingTexture->ReadId(
            ogl::input::MouseInput::GetInstance().GetX(),
            ogl::input::MouseInput::GetInstance().GetY())
        };
        Log::SG_LOG_DEBUG("Id {}.", id);
    }
}

void sg::Application::Update()
{
    if (m_window.IsKeyPressed(GLFW_KEY_W))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::FORWARD);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_S))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::BACKWARD);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_A))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::LEFT);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_D))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::RIGHT);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_O))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::UP);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_U))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::DOWN);
    }
}

void sg::Application::Render()
{
    // Render to Fbo for picking
    if (m_renderToFbo)
    {
        ogl::OpenGL::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        m_pickingShaderProgram->Bind();
        m_pickingTexture->EnableWriting();

        ogl::OpenGL::Clear();

        auto i{ 0 };
        for (auto z{ 0 }; z < 4; ++z)
        {
            for (auto x{ 0 }; x < 4; ++x)
            {
                auto modelMatrix{ogl::math::Transform::CreateModelMatrix(
                    glm::vec3(x - 2, 0.0f, z - 2),
                    glm::vec3(0.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f)
                )
                };

                m_vao->Bind();

                m_pickingShaderProgram->SetUniform("model", modelMatrix);
                m_pickingShaderProgram->SetUniform("view", m_camera.GetViewMatrix());
                m_pickingShaderProgram->SetUniform("projection", m_window.GetProjectionMatrix());

                // convert "i", the integer mesh Id, into an RGB color
                int r = (i & 0x000000FF) >> 0;
                int g = (i & 0x0000FF00) >> 8;
                int b = (i & 0x00FF0000) >> 16;
                glm::vec4 color{r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
                Log::SG_LOG_DEBUG("i:  {}, r: {},  g: {},  b: {}", i, r / 255.0f, g / 255.0f, b / 255.0f);

                m_pickingShaderProgram->SetUniform("pickingColor", color);

                m_vao->DrawPrimitives();
                m_vao->Unbind();

                i++;
            }
        }

        m_pickingTexture->DisableWriting();
        m_pickingShaderProgram->Unbind();

        m_renderToFbo = false;
    }

    // Render scene
    StartFrame();

    for (auto z{ 0 }; z < 4; ++z)
    {
        for (auto x{ 0 }; x < 4; ++x)
        {
            auto modelMatrix{ogl::math::Transform::CreateModelMatrix(
                    glm::vec3(x - 2, 0.0f, z - 2),
                    glm::vec3(0.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f)
            )
            };

            m_spriteRenderer->Render(
                    modelMatrix,
                    m_camera.GetViewMatrix(),
                    m_window.GetProjectionMatrix()
            );
        }
    }

    EndFrame();
}

//-------------------------------------------------
// Game loop
//-------------------------------------------------

void sg::Application::GameLoop()
{
    Log::SG_LOG_DEBUG("[Application::GameLoop()] Starting the game loop.");

    auto lastTime{ glfwGetTime() };
    auto resetTimer{ lastTime };

    auto dt{ 0.0 };
    auto fps{ 0 };
    auto updates{ 0 };

    while(!m_window.WindowShouldClose())
    {
        auto now{ glfwGetTime() };
        dt += (now - lastTime) / FRAME_TIME;
        lastTime = now;

        Input();

        while (dt >= 1.0)
        {
            Update();
            updates++;
            dt--;
        }

        Render();
        fps++;

        if (glfwGetTime() - resetTimer > 1.0)
        {
            resetTimer++;

            std::stringstream ss;
#ifdef SG_CITY_DEBUG_BUILD
            ss << m_window.GetTitle() << " [DEBUG BUILD] " << "   |   Fps: " << fps << "   |   Updates: " << updates;
#else
            ss << m_window.GetTitle() << " [RELEASE BUILD] " << "   |   Fps: " << fps << "   |   Updates: " << updates;
#endif
            glfwSetWindowTitle(m_window.GetWindowHandle(), ss.str().c_str());

            updates = 0;
            fps = 0;
        }
    }
}

//-------------------------------------------------
// Frame
//-------------------------------------------------

void sg::Application::StartFrame()
{
    ogl::OpenGL::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ogl::OpenGL::Clear();
    ogl::OpenGL::EnableWireframeMode();
}

void sg::Application::EndFrame()
{
    m_window.Update();
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::Application::CleanUp()
{

}
