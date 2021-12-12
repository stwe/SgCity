#include "Map.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/math/Transform.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Map::Map()
{
    Log::SG_LOG_DEBUG("[Map::Map()] Create Map.");

    Init();
}

sg::map::Map::~Map() noexcept
{
    Log::SG_LOG_DEBUG("[Map::~Map()] Destruct Map.");

    CleanUp();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::Map::Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera)
{
    ogl::OpenGL::EnableAlphaBlending();

    m_vao->Bind();
    m_shaderProgram->Bind();

    auto modelMatrix{ogl::math::Transform::CreateModelMatrix(
        glm::vec3(-2.0f, 0.0f, -2.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    )};

    m_shaderProgram->SetUniform("model", modelMatrix);
    m_shaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    m_shaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());

    m_vao->DrawPrimitives();

    m_shaderProgram->Unbind();
    m_vao->Unbind();

    ogl::OpenGL::DisableBlending();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Map::Init()
{
    for (auto z{ 0 }; z < 4; ++z)
    {
        for (auto x{ 0 }; x < 4; ++x)
        {
            auto tile{ std::make_unique<Tile>(
                static_cast<float>(x),
                static_cast<float>(z)
                ) };

            m_tiles.push_back(std::move(tile));
        }
    }

    m_vao = std::make_unique<ogl::buffer::Vao>();

    // todo: Vbo Größe in Bytes und Draw count ermitteln
    // 5 floats per vertex = 5 x 4 bytes = 20 bytes
    // 6 vertices = 20 x 6 = 120 bytes per Tile
    // 4x4 Tiles = 16 tiles = 16x120 bytes = 1920 bytes

    const auto& vbo{ m_vao->AddEmptyVbo(16 * 120, 16 * 6) };

    // add vertices to the VBO
    vbo.Bind();
    auto offset{ 0 };
    for (const auto& tile : m_tiles)
    {
        glBufferSubData(GL_ARRAY_BUFFER, offset * 120, 120, tile->vertices.data());
        offset++;
    }
    ogl::buffer::Vbo::Unbind();

    m_shaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/sprite");
    m_shaderProgram->Load();
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::map::Map::CleanUp()
{
    Log::SG_LOG_DEBUG("[Map::CleanUp()] Clean up Map.");


}
