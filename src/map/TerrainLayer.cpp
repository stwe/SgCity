#include <glm/gtc/matrix_inverse.hpp>
#include "TerrainLayer.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/resource/Texture.h"
#include "ogl/input/PickingTexture.h"
#include "ogl/input/MouseInput.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::TerrainLayer::TerrainLayer(const int t_tileCount)
    : Layer(t_tileCount)
{
    Log::SG_LOG_DEBUG("[TerrainLayer::TerrainLayer()] Create TerrainLayer.");

    Init();
}

sg::map::TerrainLayer::~TerrainLayer() noexcept
{
    Log::SG_LOG_DEBUG("[TerrainLayer::~TerrainLayer()] Destruct TerrainLayer.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

int sg::map::TerrainLayer::Update(gui::MapEditGui::Action t_action)
{
    auto index{ GetCurrentTileIdxUnderMouse() };
    if (index < 0 || index > tiles.size() - 1)
    {
        return -1; // todo: constant
    }

    auto& tile{ *tiles[index] };

    if (t_action == gui::MapEditGui::Action::SET_TRAFFIC)
    {
        return index;
    }

    if (t_action == gui::MapEditGui::Action::RAISE)
    {
        tile.Raise();
    }

    if (t_action == gui::MapEditGui::Action::LOWER)
    {
        tile.Lower();
    }

    tile.UpdateNormal();
    tile.VerticesToGpu(*vao);

    UpdateNorthNeighbor(tile);
    UpdateSouthNeighbor(tile);
    UpdateEastNeighbor(tile);
    UpdateWestNeighbor(tile);

    UpdateNorthEastNeighbor(tile);
    UpdateNorthWestNeighbor(tile);
    UpdateSouthEastNeighbor(tile);
    UpdateSouthWestNeighbor(tile);

    return index;
}

void sg::map::TerrainLayer::RenderForMousePicking(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera)
{
    if (!m_pickingTexture)
    {
        m_pickingTexture = std::make_unique<ogl::input::PickingTexture>();
        m_pickingTexture->Init(t_window.GetWidth(), t_window.GetHeight());
    }

    m_pickingTexture->EnableWriting();

    ogl::OpenGL::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    ogl::OpenGL::Clear();

    vao->Bind();
    m_pickingShaderProgram->Bind();

    m_pickingShaderProgram->SetUniform("model", modelMatrix);
    m_pickingShaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    m_pickingShaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());

    vao->DrawPrimitives();

    m_pickingShaderProgram->Unbind();
    vao->Unbind();

    m_pickingTexture->DisableWriting();
}

void sg::map::TerrainLayer::Render(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera) const
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

    m_tileTexture->BindForReading(GL_TEXTURE0);
    shaderProgram->SetUniform("diffuseMap", 0);

    vao->DrawPrimitives();

    shaderProgram->Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
}

//-------------------------------------------------
// Mouse picking
//-------------------------------------------------

int sg::map::TerrainLayer::GetCurrentTileIdxUnderMouse() const
{
    return m_pickingTexture->ReadMapIndex(
        ogl::input::MouseInput::GetInstance().GetX(),
        ogl::input::MouseInput::GetInstance().GetY());
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::TerrainLayer::Init()
{
    Log::SG_LOG_DEBUG("[TerrainLayer::Init()] Initialize the TerrainLayer.");

    position = glm::vec3(0.0f);

    modelMatrix = ogl::math::Transform::CreateModelMatrix(
        position,
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    );

    CreateTiles();
    AddTileNeighbors();
    TilesToGpu();

    shaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/map");
    shaderProgram->Load();

    m_tileTexture = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/grass.jpg");
    m_tileTexture->Load();

    m_pickingShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/picking");
    m_pickingShaderProgram->Load();

    Log::SG_LOG_DEBUG("[TerrainLayer::Init()] The TerrainLayer was successfully initialized.");
}

void sg::map::TerrainLayer::CreateTiles()
{
    for (auto z{ 0 }; z < tileCount; ++z)
    {
        for (auto x{ 0 }; x < tileCount; ++x)
        {
            auto index{ z * tileCount + x };
            auto tile{ std::make_unique<Tile>( // is stored as shared_ptr later
                static_cast<float>(x),
                static_cast<float>(z),
                index
            ) };

            tiles.push_back(std::move(tile));
        }
    }
}

void sg::map::TerrainLayer::AddTileNeighbors()
{
    for (auto z{ 0 }; z < tileCount; ++z)
    {
        for (auto x{ 0 }; x < tileCount; ++x)
        {
            const auto i{ static_cast<int>(z) * tileCount + static_cast<int>(x) };

            // regular grid
            if (z > 0)
            {
                tiles[i]->n = tiles[static_cast<int>(z - 1) * tileCount + static_cast<int>(x)];
            }

            if (z < tileCount - 1)
            {
                tiles[i]->s = tiles[static_cast<int>(z + 1) * tileCount + static_cast<int>(x)];
            }

            if (x > 0)
            {
                tiles[i]->w = tiles[static_cast<int>(z) * tileCount + static_cast<int>(x - 1)];
            }

            if (x < tileCount - 1)
            {
                tiles[i]->e = tiles[static_cast<int>(z) * tileCount + static_cast<int>(x + 1)];
            }

            // connect diagonally
            if (z > 0 && x < tileCount - 1)
            {
                tiles[i]->ne = tiles[static_cast<int>(z - 1) * tileCount + static_cast<int>(x + 1)];
            }

            if (z > 0 && x > 0)
            {
                tiles[i]->nw = tiles[static_cast<int>(z - 1) * tileCount + static_cast<int>(x - 1)];
            }

            if (z < tileCount - 1 && x > 0)
            {
                tiles[i]->sw = tiles[static_cast<int>(z + 1) * tileCount + static_cast<int>(x - 1)];
            }

            if (z < tileCount - 1 && x < tileCount - 1)
            {
                tiles[i]->se = tiles[static_cast<int>(z + 1) * tileCount + static_cast<int>(x + 1)];
            }
        }
    }
}

void sg::map::TerrainLayer::TilesToGpu()
{
    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateEmptyDynamicVbo(tileCount * tileCount * Tile::BYTES_PER_TILE, tileCount * tileCount * Tile::VERTICES_PER_TILE);

    for (const auto& tile : tiles)
    {
        tile->VerticesToGpu(*vao);
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::map::TerrainLayer::UpdateNorthNeighbor(sg::map::Tile& t_tile)
{
    if (t_tile.n)
    {
        const auto& tile{ t_tile.n };
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
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateSouthNeighbor(sg::map::Tile& t_tile)
{
    if (t_tile.s)
    {
        const auto& tile{ t_tile.s };
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
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateWestNeighbor(sg::map::Tile& t_tile)
{
    if (t_tile.w)
    {
        const auto& tile{ t_tile.w };
        auto& vertices{ tile->vertices };

        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::BR_1_Y];

        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::BR_2_Y];
        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::TL_2_Y] - vertices[Tile::TR_2_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateEastNeighbor(sg::map::Tile& t_tile)
{
    if (t_tile.e)
    {
        const auto& tile{ t_tile.e };
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
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateNorthWestNeighbor(sg::map::Tile& t_tile)
{
    if (t_tile.nw)
    {
        const auto& tile{ t_tile.nw };
        auto& vertices{ tile->vertices };

        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::TL_1_Y] - vertices[Tile::BR_1_Y];
        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::TL_2_Y] - vertices[Tile::BR_2_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateNorthEastNeighbor(sg::map::Tile& t_tile)
{
    if (t_tile.ne)
    {
        const auto& tile{ t_tile.ne };
        auto& vertices{ tile->vertices };

        vertices[Tile::BL_1_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::BL_1_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateSouthWestNeighbor(sg::map::Tile& t_tile)
{
    if (t_tile.sw)
    {
        const auto& tile{ t_tile.sw };
        auto& vertices{ tile->vertices };

        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::TR_2_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateSouthEastNeighbor(sg::map::Tile& t_tile)
{
    if (t_tile.se)
    {
        const auto& tile{ t_tile.se };
        auto& vertices{ tile->vertices };

        vertices[Tile::TL_1_Y] += t_tile.vertices[Tile::BR_1_Y] - vertices[Tile::TL_1_Y];
        vertices[Tile::TL_2_Y] += t_tile.vertices[Tile::BR_2_Y] - vertices[Tile::TL_2_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}
