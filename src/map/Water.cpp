#include "Water.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Water::Water(const int t_tileCount)
    : m_tileCount{ t_tileCount }
{
    Log::SG_LOG_DEBUG("[Water::Water()] Create Water.");

    Init();
}

sg::map::Water::~Water() noexcept
{
    Log::SG_LOG_DEBUG("[Water::~Water()] Destruct Water.");

    CleanUp();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::Water::Render(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera) const
{
    ogl::OpenGL::EnableFaceCulling();

    m_waterVao->Bind();
    m_waterShaderProgram->Bind();

    auto modelMatrix{ ogl::math::Transform::CreateModelMatrix(
        WATER_POSITION,
        glm::vec3(0.0f),
        glm::vec3(static_cast<float>(m_tileCount))
    ) };

    m_waterShaderProgram->SetUniform("model", modelMatrix);
    m_waterShaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    m_waterShaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());

    m_waterVao->DrawPrimitives();

    m_waterShaderProgram->Unbind();
    m_waterVao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Water::Init()
{
    m_waterShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/water");
    m_waterShaderProgram->Load();

    m_waterVao = std::make_unique<ogl::buffer::Vao>();
    m_waterVao->CreateStaticWaterVbo();
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::map::Water::CleanUp()
{
    Log::SG_LOG_DEBUG("[Water::CleanUp()] Clean up Water.");

    // ...
}
