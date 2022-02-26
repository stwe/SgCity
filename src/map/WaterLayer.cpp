#include "WaterLayer.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::WaterLayer::WaterLayer(const int t_tileCount)
    : Layer(t_tileCount)
{
    Log::SG_LOG_DEBUG("[WaterLayer::WaterLayer()] Create WaterLayer.");

    Init();
}

sg::map::WaterLayer::~WaterLayer() noexcept
{
    Log::SG_LOG_DEBUG("[WaterLayer::~WaterLayer()] Destruct WaterLayer.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::WaterLayer::Render(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera) const
{
    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();

    auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram("E:/Dev/SgCity/resources/shader/water") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", t_window.GetProjectionMatrix());

    vao->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::WaterLayer::Init()
{
    Log::SG_LOG_DEBUG("[WaterLayer::Init()] Initialize the WaterLayer.");

    position = glm::vec3(0.0f, -0.8f, 0.0f);

    modelMatrix = ogl::math::Transform::CreateModelMatrix(
        position,
        glm::vec3(0.0f),
        glm::vec3(static_cast<float>(tileCount))
    );

    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateStaticWaterVbo();

    Log::SG_LOG_DEBUG("[WaterLayer::Init()] The WaterLayer was successfully initialized.");
}
