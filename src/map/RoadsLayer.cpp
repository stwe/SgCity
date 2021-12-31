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

void sg::map::RoadsLayer::Update(sg::gui::MapEditGui::Action t_action, const int t_tileIndex)
{
    Log::SG_LOG_DEBUG("[RoadsLayer::Update()] Update Tile to a road {}.", t_tileIndex);

    const auto& tile{ *tiles[t_tileIndex] };
    auto i = m_roadTiles.size();
    m_roadTiles.push_back(std::move(CreateRoadTile(tile, i)));

    if (!vao)
    {
        vao = std::make_unique<ogl::buffer::Vao>();
        vao->CreateEmptyDynamicVbo(tileCount * tileCount * Tile::BYTES_PER_TILE, static_cast<int>(m_roadTiles.size()) * Tile::VERTICES_PER_TILE);
    }

    if (vao)
    {
        for (const auto& roadTile : m_roadTiles)
        {
            roadTile->VerticesToGpu(*vao);
        }

        vao->drawCount = static_cast<int>(m_roadTiles.size()) * Tile::VERTICES_PER_TILE;
    }
}

void sg::map::RoadsLayer::Render(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera) const
{
    if (!vao)
    {
        return;
    }

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

    CreateRoadTiles();
    RoadTilesToGpu();

    shaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/map");
    shaderProgram->Load();

    m_roadsTexture = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/roads.png");
    m_roadsTexture->Load();

    Log::SG_LOG_DEBUG("[RoadsLayer::Init()] The RoadsLayer was successfully initialized.");
}

void sg::map::RoadsLayer::CreateRoadTiles()
{
    auto i{ 0 };
    for (const auto& tile : tiles)
    {
        if (tile->type == Tile::TileType::TRAFFIC)
        {
            m_roadTiles.push_back(std::move(CreateRoadTile(*tile, i)));
            i++;
        }
    }
}

void sg::map::RoadsLayer::RoadTilesToGpu()
{
    if (m_roadTiles.empty())
    {
        return;
    }

    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateEmptyDynamicVbo(tileCount * tileCount * Tile::BYTES_PER_TILE, static_cast<int>(m_roadTiles.size()) * Tile::VERTICES_PER_TILE);

    for (const auto& roadTile : m_roadTiles)
    {
        roadTile->VerticesToGpu(*vao);
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

std::unique_ptr<sg::map::RoadTile> sg::map::RoadsLayer::CreateRoadTile(const sg::map::Tile& t_tile, const int t_index)
{
    auto roadTile{ std::make_unique<RoadTile>() };
    roadTile->vertices = t_tile.vertices;

    // y
    roadTile->vertices[Tile::TL_1_Y] += 0.01f;
    roadTile->vertices[Tile::BL_1_Y] += 0.01f;
    roadTile->vertices[Tile::BR_1_Y] += 0.01f;

    roadTile->vertices[Tile::TL_2_Y] += 0.01f;
    roadTile->vertices[Tile::BR_2_Y] += 0.01f;
    roadTile->vertices[Tile::TR_2_Y] += 0.01f;

    // uv
    roadTile->vertices[4] = 1.0f / 4.0f;
    roadTile->vertices[25] = 1.0f / 4.0f;

    roadTile->vertices[37] = 1.0f / 4.0f;
    roadTile->vertices[47] = 1.0f / 4.0f;
    roadTile->vertices[58] = 1.0f / 4.0f;
    roadTile->vertices[59] = 1.0f / 4.0f;

    roadTile->mapX = t_tile.mapX;
    roadTile->mapZ = t_tile.mapZ;
    roadTile->mapIndex = t_index;

    roadTile->n = t_tile.n;
    roadTile->s = t_tile.s;
    roadTile->e = t_tile.e;
    roadTile->w = t_tile.w;
    roadTile->nw = t_tile.nw;
    roadTile->ne = t_tile.ne;
    roadTile->sw = t_tile.sw;
    roadTile->se = t_tile.se;

    roadTile->type = Tile::TileType::TRAFFIC;

    return roadTile;
}
