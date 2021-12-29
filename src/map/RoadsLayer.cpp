#include "RoadsLayer.h"
#include "Log.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::RoadsLayer::RoadsLayer(std::vector<std::shared_ptr<Tile>> t_tiles)
    : Layer(std::move(t_tiles))
{
    Log::SG_LOG_DEBUG("[RoadsLayer::RoadsLayer()] Create RoadsLayer.");
}

sg::map::RoadsLayer::~RoadsLayer() noexcept
{
    Log::SG_LOG_DEBUG("[RoadsLayer::~RoadsLayer()] Destruct RoadsLayer.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::RoadsLayer::Render(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera) const
{

}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::RoadsLayer::Init()
{

}
