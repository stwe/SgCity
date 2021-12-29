#include "Map.h"
#include "Log.h"
#include "WaterLayer.h"
#include "TerrainLayer.h"
#include "RoadsLayer.h"

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

void sg::map::Map::Update(bool t_raise)
{
    m_terrainLayer->Update(t_raise);
}

void sg::map::Map::RenderForMousePicking(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera) const
{
    m_terrainLayer->RenderForMousePicking(t_window, t_camera);
}

void sg::map::Map::Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const
{
    m_terrainLayer->Render(t_window, t_camera);
    m_waterLayer->Render(t_window, t_camera);
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Map::Init()
{
    Log::SG_LOG_DEBUG("[Map::Init()] Initialize the map.");

    m_waterLayer = std::make_unique<WaterLayer>(m_tileCount);
    m_terrainLayer = std::make_unique<TerrainLayer>(m_tileCount);

    // todo: use same tiles as terrain
    m_roadsLayer = std::make_unique<RoadsLayer>(m_terrainLayer->tiles);

    Log::SG_LOG_DEBUG("[Map::Init()] The map was successfully initialized.");
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::map::Map::CleanUp()
{
    Log::SG_LOG_DEBUG("[Map::CleanUp()] Clean up Map.");

    // ...
}
