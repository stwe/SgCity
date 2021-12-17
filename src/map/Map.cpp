#include "Map.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/math/Transform.h"
#include "ogl/input/PickingTexture.h"
#include "ogl/input/MouseInput.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Map::Map(const int t_tileCount)
    : m_tileCount{ t_tileCount }
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

void sg::map::Map::RenderForMousePicking(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera)
{
    if (!m_pickingTexture)
    {
        m_pickingTexture = std::make_unique<ogl::input::PickingTexture>();
        m_pickingTexture->Init(t_window.GetWidth(), t_window.GetHeight());
    }

    m_pickingTexture->EnableWriting();

    ogl::OpenGL::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    ogl::OpenGL::Clear();

    m_mapVao->Bind();
    m_pickingShaderProgram->Bind();

    auto modelMatrix{ogl::math::Transform::CreateModelMatrix(
        glm::vec3(-2.0f, 0.0f, -2.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    )};

    m_pickingShaderProgram->SetUniform("model", modelMatrix);
    m_pickingShaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    m_pickingShaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());

    m_mapVao->DrawPrimitives();

    m_pickingShaderProgram->Unbind();
    m_mapVao->Unbind();

    m_pickingTexture->DisableWriting();
}

void sg::map::Map::Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera)
{
    ogl::OpenGL::EnableAlphaBlending();

    m_mapVao->Bind();
    m_mapShaderProgram->Bind();

    auto modelMatrix{ogl::math::Transform::CreateModelMatrix(
        glm::vec3(-2.0f, 0.0f, -2.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    )};

    m_mapShaderProgram->SetUniform("model", modelMatrix);
    m_mapShaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    m_mapShaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());
    //m_mapShaderProgram->SetUniform("col", glm::vec3(0.8, 0.1, 0.1));

    m_mapVao->DrawPrimitives();

    m_mapShaderProgram->Unbind();
    m_mapVao->Unbind();

    ogl::OpenGL::DisableBlending();
}

int sg::map::Map::GetTileObjectId()
{
    return m_pickingTexture->ReadId(
        ogl::input::MouseInput::GetInstance().GetX(),
        ogl::input::MouseInput::GetInstance().GetY());
}

void sg::map::Map::Raise(int t_tileObjectId)
{
    if (t_tileObjectId < 0 || t_tileObjectId >= m_tiles.size() - 1)
    {
        return;
    }

    auto& vertices{ m_tiles[t_tileObjectId]->vertices };

    vertices[1] += 0.5f;
    vertices[9] += 0.5f;
    vertices[17] += 0.5f;

    vertices[25] += 0.5f;
    vertices[33] += 0.5f;
    vertices[41] += 0.5f;

    //vbo.Bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    glBufferSubData(GL_ARRAY_BUFFER, t_tileObjectId * 192, 192, vertices.data());
    ogl::buffer::Vbo::Unbind();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Map::Init()
{
    for (auto z{ 0 }; z < m_tileCount; ++z)
    {
        for (auto x{ 0 }; x < m_tileCount; ++x)
        {
            auto tile{ std::make_unique<Tile>(
                static_cast<float>(x),
                static_cast<float>(z),
                m_tileCount
                ) };

            m_tiles.push_back(std::move(tile));
        }
    }

    m_mapVao = std::make_unique<ogl::buffer::Vao>();

    // todo: Vbo Größe in Bytes und Draw count ermitteln
    // 8 floats per vertex = 8 x 4 bytes pro float = 32 bytes
    // 6 vertices = 32 x 6 = 192 bytes per Tile
    // 4x4 Tiles = 16 tiles = 16x192 bytes = 3072 bytes

    const auto& vbo{ m_mapVao->AddEmptyVbo(m_tileCount * m_tileCount * 192, m_tileCount * m_tileCount * 6) };
    m_vboId = vbo.id;

    // add vertices to the VBO
    vbo.Bind();
    auto offset{ 0 };
    for (const auto& tile : m_tiles)
    {
        glBufferSubData(GL_ARRAY_BUFFER, offset * 192, 192, tile->vertices.data());
        offset++;
    }
    ogl::buffer::Vbo::Unbind();

    m_mapShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/sprite");
    m_mapShaderProgram->Load();

    m_pickingShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/picking");
    m_pickingShaderProgram->Load();
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::map::Map::CleanUp()
{
    Log::SG_LOG_DEBUG("[Map::CleanUp()] Clean up Map.");
}
