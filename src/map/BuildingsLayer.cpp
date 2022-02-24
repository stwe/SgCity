#include <glm/gtc/matrix_inverse.hpp>
#include "BuildingsLayer.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/resource/ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::BuildingsLayer::BuildingsLayer(const int t_tileCount, std::vector<std::shared_ptr<Tile>> t_tiles)
    : Layer(t_tileCount, std::move(t_tiles))
{
    Log::SG_LOG_DEBUG("[BuildingsLayer::BuildingsLayer()] Create BuildingsLayer.");

    Init();
}

sg::map::BuildingsLayer::~BuildingsLayer() noexcept
{
    Log::SG_LOG_DEBUG("[BuildingsLayer::~BuildingsLayer()] Destruct BuildingsLayer.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::BuildingsLayer::Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const
{
    if (!vao)
    {
        return;
    }

    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();

    auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram("/home/steffen/CLionProjects/SgCity/resources/shader/map") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", t_window.GetProjectionMatrix());

    const auto mv{ t_camera.GetViewMatrix() * modelMatrix };
    const auto n{ glm::inverseTranspose(glm::mat3(mv)) };
    shaderProgram.SetUniform("normalMatrix", n);

    const auto& texture{ ogl::resource::ResourceManager::LoadTexture("/home/steffen/CLionProjects/SgCity/resources/texture/r.png") };
    texture.BindForReading(GL_TEXTURE0);
    shaderProgram.SetUniform("diffuseMap", 0);

    vao->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();

    for (const auto& tile : m_buildingTiles)
    {
        m_buildingModel->Render(t_window, t_camera, glm::vec3(tile->mapX + 0.5f, 0.25f, tile->mapZ + 0.5f));
    }
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::BuildingsLayer::Init()
{
    Log::SG_LOG_DEBUG("[BuildingsLayer::Init()] Initialize the BuildingsLayer.");

    position = glm::vec3(0.0f);

    modelMatrix = ogl::math::Transform::CreateModelMatrix(
        position,
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    );

    CreateBuildingTiles();
    BuildingTilesToGpu();

    m_buildingModel = &sg::ogl::resource::ResourceManager::LoadModel("/home/steffen/CLionProjects/SgCity/resources/model/cube.obj");

    Log::SG_LOG_DEBUG("[BuildingsLayer::Init()] The BuildingsLayer was successfully initialized.");
}

void sg::map::BuildingsLayer::CreateBuildingTiles()
{
    auto i{ 0 };
    for (const auto& tile : tiles)
    {
        if (tile->type == Tile::TileType::RESIDENTIAL)
        {
            m_buildingTiles.push_back(std::move(CreateBuildingTile(*tile, i)));
            i++;
        }
    }
}

void sg::map::BuildingsLayer::BuildingTilesToGpu()
{
    if (m_buildingTiles.empty())
    {
        return;
    }

    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateEmptyDynamicVbo(tileCount * tileCount * Tile::BYTES_PER_TILE, static_cast<int>(m_buildingTiles.size()) * Tile::VERTICES_PER_TILE);

    for (const auto& buildingTile : m_buildingTiles)
    {
        buildingTile->VerticesToGpu(*vao);
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

std::unique_ptr<sg::map::BuildingTile> sg::map::BuildingsLayer::CreateBuildingTile(const sg::map::Tile& t_tile, int t_index)
{
    auto buildingTile{ std::make_unique<BuildingTile>() };
    buildingTile->vertices = t_tile.vertices;

    buildingTile->vertices[Tile::TL_1_Y] += 0.01f;
    buildingTile->vertices[Tile::BL_1_Y] += 0.01f;
    buildingTile->vertices[Tile::BR_1_Y] += 0.01f;

    buildingTile->vertices[Tile::TL_2_Y] += 0.01f;
    buildingTile->vertices[Tile::BR_2_Y] += 0.01f;
    buildingTile->vertices[Tile::TR_2_Y] += 0.01f;

    buildingTile->mapX = t_tile.mapX;
    buildingTile->mapZ = t_tile.mapZ;
    buildingTile->mapIndex = t_index;

    buildingTile->n = t_tile.n;
    buildingTile->s = t_tile.s;
    buildingTile->e = t_tile.e;
    buildingTile->w = t_tile.w;
    buildingTile->nw = t_tile.nw;
    buildingTile->ne = t_tile.ne;
    buildingTile->sw = t_tile.sw;
    buildingTile->se = t_tile.se;

    buildingTile->type = Tile::TileType::RESIDENTIAL;

    return buildingTile;
}
