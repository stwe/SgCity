#include <glm/gtc/matrix_inverse.hpp>
#include "RoadsLayer.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/resource/Texture.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::RoadsLayer::RoadsLayer(const int t_tileCount, std::vector<std::shared_ptr<Tile>> t_tiles)
    : Layer(t_tileCount, std::move(t_tiles))
{
    Log::SG_LOG_DEBUG("[RoadsLayer::RoadsLayer()] Create RoadsLayer.");

    Init();
}

sg::map::RoadsLayer::~RoadsLayer() noexcept
{
    Log::SG_LOG_DEBUG("[RoadsLayer::~RoadsLayer()] Destruct RoadsLayer.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::RoadsLayer::Render(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera) const
{
    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();
    shaderProgram->Bind();

    shaderProgram->SetUniform("model", modelMatrix);
    shaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());

    const auto mv{ t_camera.GetViewMatrix() * modelMatrix };
    const auto n{ glm::inverseTranspose(glm::mat3(mv)) };
    shaderProgram->SetUniform("normalMatrix", n);

    m_roadsTexture->BindForReading(GL_TEXTURE0);
    shaderProgram->SetUniform("diffuseMap", 0);

    vao->DrawPrimitives();

    shaderProgram->Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::RoadsLayer::Init()
{
    Log::SG_LOG_DEBUG("[RoadsLayer::Init()] Initialize the RoadsLayer.");

    position = glm::vec3(0.0f);

    modelMatrix = ogl::math::Transform::CreateModelMatrix(
        position,
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    );

    TilesToGpu();

    shaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/map");
    shaderProgram->Load();

    m_roadsTexture = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/roads.png");
    m_roadsTexture->Load();

    Log::SG_LOG_DEBUG("[RoadsLayer::Init()] The RoadsLayer was successfully initialized.");
}

void sg::map::RoadsLayer::TilesToGpu()
{
    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateEmptyDynamicVbo(tileCount * tileCount * Tile::BYTES_PER_TILE, tileCount * tileCount * Tile::VERTICES_PER_TILE);

    for (const auto& tile : tiles)
    {
        tile->VerticesToGpu(*vao);
    }
}
