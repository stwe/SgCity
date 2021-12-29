#include <glm/gtc/matrix_inverse.hpp>
#include "RoadNetwork.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/resource/Texture.h"
#include "ogl/math/Transform.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::RoadNetwork::RoadNetwork(const sg::map::Map& t_map)
{
    Log::SG_LOG_DEBUG("[RoadNetwork::RoadNetwork()] Create RoadNetwork.");

    Init(t_map);
}

sg::map::RoadNetwork::~RoadNetwork() noexcept
{
    Log::SG_LOG_DEBUG("[RoadNetwork::~RoadNetwork()] Destruct RoadNetwork.");

    CleanUp();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::RoadNetwork::Render(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera) const
{
    ogl::OpenGL::EnableFaceCulling();

    m_roadsVao->Bind();
    m_roadsShaderProgram->Bind();

    m_roadsShaderProgram->SetUniform("model", m_roadsModelMatrix);
    m_roadsShaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    m_roadsShaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());

    const auto mv{ t_camera.GetViewMatrix() * m_roadsModelMatrix };
    const auto n{ glm::inverseTranspose(glm::mat3(mv)) };
    m_roadsShaderProgram->SetUniform("normalMatrix", n);

    m_roadsTexture->BindForReading(GL_TEXTURE0);
    m_roadsShaderProgram->SetUniform("diffuseMap", 0);

    m_roadsVao->DrawPrimitives();

    m_roadsShaderProgram->Unbind();
    m_roadsVao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::RoadNetwork::Init(const sg::map::Map& t_map)
{
    Log::SG_LOG_DEBUG("[RoadNetwork::Init()] Initialize the roads.");

    m_roadsModelMatrix = ogl::math::Transform::CreateModelMatrix(
        ROAD_NETWORK_POSITION,
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    );

    CreateTiles(t_map);
    TilesToGpu();
    InitResources();

    Log::SG_LOG_DEBUG("[RoadNetwork::Init()] The roads was successfully initialized.");
}

void sg::map::RoadNetwork::CreateTiles(const sg::map::Map& t_map)
{
    std::vector<float> roadNetworkVertices;

    auto i{ 0 };
    /*
    for (auto& tile : t_map.GetTiles())
    {
        if (tile->type == Tile::TileType::TRAFFIC)
        {
            auto roadTile{ std::make_unique<Tile>() };
            roadTile->vertices = tile->vertices;

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

            roadTile->mapX = tile->mapX;
            roadTile->mapZ = tile->mapZ;
            roadTile->mapIndex = i;

            roadTile->n = tile->n;
            roadTile->s = tile->s;
            roadTile->e = tile->e;
            roadTile->w = tile->w;
            roadTile->nw = tile->nw;
            roadTile->ne = tile->ne;
            roadTile->sw = tile->sw;
            roadTile->se = tile->se;

            roadTile->type = Tile::TileType::TRAFFIC;

            m_tiles.push_back(std::move(roadTile));

            i++;
        }
    }
    */
}

void sg::map::RoadNetwork::TilesToGpu()
{
    m_roadsVao = std::make_unique<ogl::buffer::Vao>();
    m_roadsVao->CreateEmptyDynamicVbo(m_tiles.size() * Tile::BYTES_PER_TILE, static_cast<int>(m_tiles.size()) * Tile::VERTICES_PER_TILE);

    for (const auto& tile : m_tiles)
    {
        tile->VerticesToGpu(*m_roadsVao);
    }
}

void sg::map::RoadNetwork::InitResources()
{
    // shader

    m_roadsShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/map");
    m_roadsShaderProgram->Load();

    // texture

    m_roadsTexture = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/roads.png");
    m_roadsTexture->Load();
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::map::RoadNetwork::CleanUp()
{
    Log::SG_LOG_DEBUG("[RoadNetwork::CleanUp()] Clean up RoadNetwork.");

    // ...
}
