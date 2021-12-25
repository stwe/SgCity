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

    // todo: nur einmal erstellen
    auto modelMatrix{ ogl::math::Transform::CreateModelMatrix(
        MAP_POSITION,
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    ) };

    m_pickingShaderProgram->SetUniform("model", modelMatrix);
    m_pickingShaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    m_pickingShaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());

    m_mapVao->DrawPrimitives();

    m_pickingShaderProgram->Unbind();
    m_mapVao->Unbind();

    m_pickingTexture->DisableWriting();
}

void sg::map::Map::Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const
{
    ogl::OpenGL::EnableFaceCulling();

    m_mapVao->Bind();
    m_mapShaderProgram->Bind();

    // todo: nur einmal erstellen
    auto modelMatrix{ ogl::math::Transform::CreateModelMatrix(
        MAP_POSITION,
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    ) };

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

    ogl::OpenGL::DisableFaceCulling();
}

//-------------------------------------------------
// Tiles
//-------------------------------------------------

int sg::map::Map::GetCurrentTileIdxUnderMouse()
{
    return m_pickingTexture->ReadMapIndex(
        ogl::input::MouseInput::GetInstance().GetX(),
        ogl::input::MouseInput::GetInstance().GetY());
}

//-------------------------------------------------
// Raise / lower terrain
//-------------------------------------------------

void sg::map::Map::HandleTileUpdate(const int t_mapIndex, const bool t_raise)
{
    if (t_mapIndex < 0 || t_mapIndex > m_tiles.size() - 1)
    {
        return;
    }

    auto& tile{ m_tiles[t_mapIndex] };

    t_raise ? tile->Raise() : tile->Lower();

    tile->UpdateNormal();
    tile->VerticesToGpu(*m_mapVao); // todo: das muss nur 1x ausgeführt werden

    UpdateNorthNeighbor(*tile);
    UpdateSouthNeighbor(*tile);
    UpdateEastNeighbor(*tile);
    UpdateWestNeighbor(*tile);

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
    CreateTiles();
    AddTileNeighbors();
    TilesToGpu();
    InitResources();
}

void sg::map::Map::CreateTiles()
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
}

void sg::map::Map::AddTileNeighbors()
{
    for (auto z{ 0 }; z < m_tileCount; ++z)
    {
        for (auto x{ 0 }; x < m_tileCount; ++x)
        {
            auto i{ static_cast<int>(z) * m_tileCount + static_cast<int>(x) };

            // regular grid
            if (z > 0)
            {
                m_tiles[i]->n = m_tiles[static_cast<int>(z - 1) * m_tileCount + static_cast<int>(x)].get();
            }

            if (z < m_tileCount - 1)
            {
                m_tiles[i]->s = m_tiles[static_cast<int>(z + 1) * m_tileCount + static_cast<int>(x)].get();
            }

            if (x > 0)
            {
                m_tiles[i]->w = m_tiles[static_cast<int>(z) * m_tileCount + static_cast<int>(x - 1)].get();
            }

            if (x < m_tileCount - 1)
            {
                m_tiles[i]->e = m_tiles[static_cast<int>(z) * m_tileCount + static_cast<int>(x + 1)].get();
            }

            // connect diagonally
            if (z > 0 && x < m_tileCount - 1)
            {
                m_tiles[i]->ne = m_tiles[static_cast<int>(z - 1) * m_tileCount + static_cast<int>(x + 1)].get();
            }

            if (z > 0 && x > 0)
            {
                m_tiles[i]->nw = m_tiles[static_cast<int>(z - 1) * m_tileCount + static_cast<int>(x - 1)].get();
            }

            if (z < m_tileCount - 1 && x > 0)
            {
                m_tiles[i]->sw = m_tiles[static_cast<int>(z + 1) * m_tileCount + static_cast<int>(x - 1)].get();
            }

            if (z < m_tileCount - 1 && x < m_tileCount - 1)
            {
                m_tiles[i]->se = m_tiles[static_cast<int>(z + 1) * m_tileCount + static_cast<int>(x + 1)].get();
            }
        }
    }
}

void sg::map::Map::TilesToGpu()
{
    m_mapVao = std::make_unique<ogl::buffer::Vao>();
    m_mapVao->CreateEmptyDynamicVbo(m_tileCount * m_tileCount * Tile::BYTES_PER_TILE, m_tileCount * m_tileCount * Tile::VERTICES_PER_TILE);

    for (const auto& tile : m_tiles)
    {
        tile->VerticesToGpu(*m_mapVao);
    }
}

void sg::map::Map::InitResources()
{
    // shader

    m_mapShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/map");
    m_mapShaderProgram->Load();

    m_pickingShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/picking");
    m_pickingShaderProgram->Load();

    // texture

    m_tileTexture = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/grass.jpg");
    m_tileTexture->Load();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::map::Map::UpdateNorthWestNeighbor(Tile& t_tile)
{
    if (t_tile.nw)
    {
        auto& tile{ t_tile.nw };
        auto& vertices{ tile->vertices };

        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::TL_1_Y] - vertices[Tile::BR_1_Y];
        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::TL_2_Y] - vertices[Tile::BR_2_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*m_mapVao);
    }
}

void sg::map::Map::UpdateNorthEastNeighbor(Tile& t_tile)
{
    if (t_tile.ne)
    {
        auto& tile{ t_tile.ne };
        auto& vertices{ tile->vertices };

        vertices[Tile::BL_1_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::BL_1_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*m_mapVao);
    }
}

void sg::map::Map::UpdateSouthWestNeighbor(Tile& t_tile)
{
    if (t_tile.sw)
    {
        auto& tile{ t_tile.sw };
        auto& vertices{ tile->vertices };

        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::TR_2_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*m_mapVao);
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

        tile->UpdateNormal();
        tile->VerticesToGpu(*m_mapVao);
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

        tile->UpdateNormal();
        tile->VerticesToGpu(*m_mapVao);
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

        vertices[Tile::TL_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::TL_2_Y];
        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::BR_2_Y] - vertices[Tile::TR_2_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*m_mapVao);
    }
}

void sg::map::Map::UpdateEastNeighbor(Tile& t_tile)
{
    if (t_tile.e)
    {
        auto& tile{ t_tile.e };
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

        tile->UpdateNormal();
        tile->VerticesToGpu(*m_mapVao);
    }
}

void sg::map::Map::UpdateWestNeighbor(Tile& t_tile)
{
    if (t_tile.w)
    {
        auto& tile{ t_tile.w };
        auto& vertices{ tile->vertices };

        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::BR_1_Y];

        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::BR_2_Y];
        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::TL_2_Y] - vertices[Tile::TR_2_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*m_mapVao);
    }
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::map::Map::CleanUp()
{
    Log::SG_LOG_DEBUG("[Map::CleanUp()] Clean up Map.");

    // ...
}
