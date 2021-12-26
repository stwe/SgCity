#include "RoadNetwork.h"
#include "Tile.h"
#include "Log.h"
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

    for (auto& tile : t_map.GetTiles())
    {
        if (tile->type == Tile::TileType::TRAFFIC)
        {
            // !! copy !! vertices
            auto vertices{ tile->vertices };

            // insert the RoadTile vertices at the end of the container with all vertices
            roadNetworkVertices.insert(roadNetworkVertices.end(), vertices.begin(), vertices.end());
        }
    }

    // calculate the number of RoadTiles
    //const auto nrTiles{ static_cast<int>(roadNetworkVertices.size()) / Tile::FLOATS_PER_TILE };

    /*
    if (nrTiles > 0)
    {
        // update draw count
        m_roadNetworkMesh->GetVao().SetDrawCount(nrTiles * tile::Tile::VERTICES_PER_TILE);

        // update Vbo
        ogl::buffer::Vbo::BindVbo(m_vboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nrTiles * tile::Tile::SIZE_IN_BYTES_PER_TILE, roadNetworkVertices.data());
        ogl::buffer::Vbo::UnbindVbo();
    }
    */
}

void sg::map::RoadNetwork::TilesToGpu()
{

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
