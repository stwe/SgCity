#include <glm/gtc/matrix_inverse.hpp>
#include "Map.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/resource/Texture.h"
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

    const auto mv{ t_camera.GetViewMatrix() * modelMatrix };
    const auto n{ glm::inverseTranspose(glm::mat3(mv)) };
    m_mapShaderProgram->SetUniform("normalMatrix", n);

    m_tileTexture->BindForReading(GL_TEXTURE0);
    m_mapShaderProgram->SetUniform("diffuseMap", 0);

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
    if (t_tileObjectId < 0 || t_tileObjectId > m_tiles.size() - 1)
    {
        return;
    }

    auto& tile{ m_tiles[t_tileObjectId] };
    UpdateTile(*tile);

    UpdateNorthNeighbor(*tile);
    UpdateSouthNeighbor(*tile);
    UpdateWestNeighbor(*tile);
    UpdateEastNeighbor(*tile);

    UpdateNorthEastNeighbor(*tile);
    UpdateNorthWestNeighbor(*tile);
    UpdateSouthEastNeighbor(*tile);
    UpdateSouthWestNeighbor(*tile);
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
                z * m_tileCount + x
                ) };

            m_tiles.push_back(std::move(tile));
        }
    }

    for (auto z{ 0 }; z < m_tileCount; ++z)
    {
        for (auto x{ 0 }; x < m_tileCount; ++x)
        {
            // regular grid
            if (z > 0)
            {
                auto i{ static_cast<int>(z) * m_tileCount + static_cast<int>(x) };
                m_tiles[i]->n = m_tiles[static_cast<int>(z - 1) * m_tileCount + static_cast<int>(x)].get();
            }

            if (z < m_tileCount - 1)
            {
                auto i{ static_cast<int>(z) * m_tileCount + static_cast<int>(x) };
                m_tiles[i]->s = m_tiles[static_cast<int>(z + 1) * m_tileCount + static_cast<int>(x)].get();
            }

            if (x > 0)
            {
                auto i{ static_cast<int>(z) * m_tileCount + static_cast<int>(x) };
                m_tiles[i]->e = m_tiles[static_cast<int>(z) * m_tileCount + static_cast<int>(x - 1)].get();
            }

            if (x < m_tileCount - 1)
            {
                auto i{ static_cast<int>(z) * m_tileCount + static_cast<int>(x) };
                m_tiles[i]->w = m_tiles[static_cast<int>(z) * m_tileCount + static_cast<int>(x + 1)].get();
            }

            // connect diagonally
            if (z > 0 && x < m_tileCount - 1)
            {
                auto i{ static_cast<int>(z) * m_tileCount + static_cast<int>(x) };
                m_tiles[i]->ne = m_tiles[static_cast<int>(z - 1) * m_tileCount + static_cast<int>(x + 1)].get();
            }

            if (z > 0 && x > 0)
            {
                auto i{ static_cast<int>(z) * m_tileCount + static_cast<int>(x) };
                m_tiles[i]->nw = m_tiles[static_cast<int>(z - 1) * m_tileCount + static_cast<int>(x - 1)].get();
            }

            if (z < m_tileCount - 1 && x > 0)
            {
                auto i{ static_cast<int>(z) * m_tileCount + static_cast<int>(x) };
                m_tiles[i]->sw = m_tiles[static_cast<int>(z + 1) * m_tileCount + static_cast<int>(x - 1)].get();
            }

            if (z < m_tileCount - 1 && x < m_tileCount - 1)
            {
                auto i{ static_cast<int>(z) * m_tileCount + static_cast<int>(x) };
                m_tiles[i]->se = m_tiles[static_cast<int>(z + 1) * m_tileCount + static_cast<int>(x + 1)].get();
            }
        }
    }

    m_mapVao = std::make_unique<ogl::buffer::Vao>();

    // 11 floats per vertex = 11 x 4 bytes pro float = 44 bytes
    // 6 vertices = 44 x 6 = 264 bytes per Tile

    m_mapVao->CreateEmptyDynamicVbo(m_tileCount * m_tileCount * Tile::BYTES_PER_TILE, m_tileCount * m_tileCount * 6);

    // add vertices to the VBO
    m_mapVao->GetVbo().Bind();
    auto offset{ 0 };
    for (const auto& tile : m_tiles)
    {
        glBufferSubData(GL_ARRAY_BUFFER, offset * Tile::BYTES_PER_TILE, Tile::BYTES_PER_TILE, tile->vertices.data());
        offset++;
    }
    ogl::buffer::Vbo::Unbind();

    m_mapShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/sprite");
    m_mapShaderProgram->Load();

    m_pickingShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/picking");
    m_pickingShaderProgram->Load();

    m_tileTexture = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/tile.png");
    m_tileTexture->Load();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

glm::vec3 sg::map::Map::CalcNormal(Tile& t_tile)
{
    auto& vertices{ t_tile.vertices };

    // read out positions
    auto v0{ glm::vec3(vertices[0], vertices[1], vertices[2]) };
    auto v1{ glm::vec3(vertices[11], vertices[12], vertices[13]) };
    auto v2{ glm::vec3(vertices[22], vertices[23], vertices[24]) };
    auto v3{ glm::vec3(vertices[55], vertices[56], vertices[57]) };

    // store position in array
    std::vector<glm::vec3> vertex = { v0, v1, v2, v3 };

    // calc normal
    glm::vec3 normal{ 0.0f, 0.0f, 0.0f };
    for (auto i{ 0 }; i < 4; ++i)
    {
        auto j{ (i + 1) % 4 };
        normal.x += (vertex[i].y - vertex[j].y) * (vertex[i].z + vertex[j].z);
        normal.y += (vertex[i].z - vertex[j].z) * (vertex[i].x + vertex[j].x);
        normal.z += (vertex[i].x - vertex[j].x) * (vertex[i].y + vertex[j].y);
    }

    return glm::normalize(normal);
}

void sg::map::Map::UpdateTile(Tile& t_tile)
{
    auto& vertices{ t_tile.vertices };

    vertices[Tile::TL_1_Y] += 0.5f;
    vertices[Tile::BL_1_Y] += 0.5f;
    vertices[Tile::BR_1_Y] += 0.5f;

    vertices[Tile::TL_2_Y] += 0.5f;
    vertices[Tile::BR_2_Y] += 0.5f;
    vertices[Tile::TR_2_Y] += 0.5f;

    auto normal{ CalcNormal(t_tile) };

    vertices[Tile::TL_1_N_START_INDEX] = normal.x;
    vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
    vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

    vertices[Tile::BL_1_N_START_INDEX] = normal.x;
    vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
    vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

    vertices[Tile::BR_1_N_START_INDEX] = normal.x;
    vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
    vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

    vertices[Tile::TL_2_N_START_INDEX] = normal.x;
    vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
    vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

    vertices[Tile::BR_2_N_START_INDEX] = normal.x;
    vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
    vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

    vertices[Tile::TR_2_N_START_INDEX] = normal.x;
    vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
    vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;

    UpdateVertices(vertices, t_tile.mapIndex);
}

void sg::map::Map::UpdateNorthWestNeighbor(Tile& t_tile)
{
    if (t_tile.nw)
    {
        auto& tile{ t_tile.nw };
        auto& vertices{ tile->vertices };

        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::TL_1_Y] - vertices[Tile::BR_1_Y];
        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::TL_1_Y] - vertices[Tile::BR_2_Y];

        auto normal{ CalcNormal(*tile) };

        vertices[Tile::TL_1_N_START_INDEX] = normal.x;
        vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BL_1_N_START_INDEX] = normal.x;
        vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_1_N_START_INDEX] = normal.x;
        vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TL_2_N_START_INDEX] = normal.x;
        vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_2_N_START_INDEX] = normal.x;
        vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TR_2_N_START_INDEX] = normal.x;
        vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;

        UpdateVertices(vertices, tile->mapIndex);
    }
}

void sg::map::Map::UpdateNorthEastNeighbor(Tile& t_tile)
{
    if (t_tile.ne)
    {
        auto& tile{ t_tile.ne };
        auto& vertices{ tile->vertices };

        /*
        tl.      tr           tl
         |  .  2
         | 1   .
        bl------ br   Raise   bl     br

        tl.      tr
         |  .  2
         | 1   .
        bl------ br
        */

        vertices[Tile::BL_1_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::BL_1_Y];

        auto normal{ CalcNormal(*tile) };

        vertices[Tile::TL_1_N_START_INDEX] = normal.x;
        vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BL_1_N_START_INDEX] = normal.x;
        vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_1_N_START_INDEX] = normal.x;
        vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TL_2_N_START_INDEX] = normal.x;
        vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_2_N_START_INDEX] = normal.x;
        vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TR_2_N_START_INDEX] = normal.x;
        vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;

        UpdateVertices(vertices, tile->mapIndex);
    }
}

// todo

void sg::map::Map::UpdateSouthWestNeighbor(Tile& t_tile)
{
    if (t_tile.sw)
    {
        auto& tile{ t_tile.sw };
        auto& vertices{ tile->vertices };

        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::TR_2_Y];

        auto normal{ CalcNormal(*tile) };

        vertices[Tile::TL_1_N_START_INDEX] = normal.x;
        vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BL_1_N_START_INDEX] = normal.x;
        vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_1_N_START_INDEX] = normal.x;
        vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TL_2_N_START_INDEX] = normal.x;
        vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_2_N_START_INDEX] = normal.x;
        vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TR_2_N_START_INDEX] = normal.x;
        vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;

        UpdateVertices(vertices, tile->mapIndex);
    }
}

void sg::map::Map::UpdateSouthEastNeighbor(sg::map::Tile& t_tile)
{
    if (t_tile.se)
    {
        auto& tile{ t_tile.se };
        auto& vertices{ tile->vertices };

        vertices[Tile::TL_1_Y] += t_tile.vertices[Tile::BR_1_Y] - vertices[Tile::TL_1_Y];
        vertices[Tile::TL_2_Y] += t_tile.vertices[Tile::BR_2_Y] - vertices[Tile::TL_2_Y];

        auto normal{ CalcNormal(*tile) };

        vertices[Tile::TL_1_N_START_INDEX] = normal.x;
        vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BL_1_N_START_INDEX] = normal.x;
        vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_1_N_START_INDEX] = normal.x;
        vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TL_2_N_START_INDEX] = normal.x;
        vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_2_N_START_INDEX] = normal.x;
        vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TR_2_N_START_INDEX] = normal.x;
        vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;

        UpdateVertices(vertices, tile->mapIndex);
    }
}

void sg::map::Map::UpdateNorthNeighbor(Tile& t_tile)
{
    if (t_tile.n)
    {
        auto& tile{ t_tile.n };
        auto& vertices{ tile->vertices };

        /*
        tl.      tr
         |  .  2
         | 1   .
        bl------ br   Raise

        tl.      tr
         |  .  2
         | 1   .
        bl------ br
        */

        vertices[Tile::BL_1_Y] += t_tile.vertices[Tile::TL_1_Y] - vertices[Tile::BL_1_Y];
        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::BR_1_Y];

        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::BR_2_Y];

        auto normal{ CalcNormal(*tile) };

        vertices[Tile::TL_1_N_START_INDEX] = normal.x;
        vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BL_1_N_START_INDEX] = normal.x;
        vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_1_N_START_INDEX] = normal.x;
        vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TL_2_N_START_INDEX] = normal.x;
        vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_2_N_START_INDEX] = normal.x;
        vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TR_2_N_START_INDEX] = normal.x;
        vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;

        UpdateVertices(vertices, tile->mapIndex);
    }
}

void sg::map::Map::UpdateSouthNeighbor(Tile& t_tile)
{
    if (t_tile.s)
    {
        auto& tile{ t_tile.s };
        auto& vertices{ tile->vertices };

        /*
        tl.      tr
         |  .  2
         | 1   .
        bl------ br

        tl.      tr
         |  .  2
         | 1   .
        bl------ br   Raise
        */

        vertices[Tile::TL_1_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::TL_1_Y];

        vertices[Tile::TL_2_Y] += t_tile.vertices[Tile::BR_1_Y] - vertices[Tile::TL_2_Y];
        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::BR_2_Y] - vertices[Tile::TR_2_Y];

        auto normal{ CalcNormal(*tile) };

        vertices[Tile::TL_1_N_START_INDEX] = normal.x;
        vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BL_1_N_START_INDEX] = normal.x;
        vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_1_N_START_INDEX] = normal.x;
        vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TL_2_N_START_INDEX] = normal.x;
        vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_2_N_START_INDEX] = normal.x;
        vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TR_2_N_START_INDEX] = normal.x;
        vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;

        UpdateVertices(vertices, tile->mapIndex);
    }
}

void sg::map::Map::UpdateWestNeighbor(Tile& t_tile)
{
    if (t_tile.w)
    {
        auto& tile{ t_tile.w };
        auto& vertices{ tile->vertices };

        /*
        tl.      tr   tl
         |  .  2            raise
         | 1   .
        bl------ br   bl

        tl.      tr
         |  .  2
         | 1   .
        bl------ br   Raise
        */

        vertices[Tile::TL_1_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::TL_1_Y];
        vertices[Tile::BL_1_Y] += t_tile.vertices[Tile::BR_1_Y] - vertices[Tile::BL_1_Y];

        vertices[Tile::TL_2_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::TL_2_Y];

        auto normal{ CalcNormal(*tile) };

        vertices[Tile::TL_1_N_START_INDEX] = normal.x;
        vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BL_1_N_START_INDEX] = normal.x;
        vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_1_N_START_INDEX] = normal.x;
        vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TL_2_N_START_INDEX] = normal.x;
        vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_2_N_START_INDEX] = normal.x;
        vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TR_2_N_START_INDEX] = normal.x;
        vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;

        UpdateVertices(vertices, tile->mapIndex);
    }
}

void sg::map::Map::UpdateEastNeighbor(Tile& t_tile)
{
    if (t_tile.e)
    {
        auto& tile{ t_tile.e };
        auto& vertices{ tile->vertices };

        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::TL_1_Y] - vertices[Tile::BR_1_Y];

        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::BR_2_Y];
        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::TL_2_Y] - vertices[Tile::TR_2_Y];

        auto normal{ CalcNormal(*tile) };

        vertices[Tile::TL_1_N_START_INDEX] = normal.x;
        vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BL_1_N_START_INDEX] = normal.x;
        vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_1_N_START_INDEX] = normal.x;
        vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TL_2_N_START_INDEX] = normal.x;
        vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::BR_2_N_START_INDEX] = normal.x;
        vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

        vertices[Tile::TR_2_N_START_INDEX] = normal.x;
        vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
        vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;

        UpdateVertices(vertices, tile->mapIndex);
    }
}

void sg::map::Map::UpdateVertices(const std::vector<float>& t_vertices, const int t_offset) const
{
    m_mapVao->GetVbo().Bind();
    glBufferSubData(GL_ARRAY_BUFFER, t_offset * Tile::BYTES_PER_TILE, Tile::BYTES_PER_TILE, t_vertices.data());
    ogl::buffer::Vbo::Unbind();
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::map::Map::CleanUp()
{
    Log::SG_LOG_DEBUG("[Map::CleanUp()] Clean up Map.");

    // ...
}
