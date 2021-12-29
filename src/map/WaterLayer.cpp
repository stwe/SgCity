#include "WaterLayer.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::WaterLayer::WaterLayer(int t_tileCount)
{
    Log::SG_LOG_DEBUG("[WaterLayer::WaterLayer()] Create WaterLayer.");

    tileCount = t_tileCount;
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
    shaderProgram->Bind();

    shaderProgram->SetUniform("model", modelMatrix);
    shaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());

    vao->DrawPrimitives();

    shaderProgram->Unbind();
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

    shaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/water");
    shaderProgram->Load();

    Log::SG_LOG_DEBUG("[WaterLayer::Init()] The WaterLayer was successfully initialized.");
}
