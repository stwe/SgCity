#include <utility>
#include "Layer.h"
#include "Log.h"
#include "Tile.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Layer::Layer()
{
    Log::SG_LOG_DEBUG("[Layer::Layer()] Create Layer.");
}

sg::map::Layer::Layer(std::vector<std::shared_ptr<Tile>> t_tiles)
    : tiles{ std::move(t_tiles) }
{
    Log::SG_LOG_DEBUG("[Layer::Layer()] Create Layer.");
}

sg::map::Layer::~Layer() noexcept
{
    Log::SG_LOG_DEBUG("[Layer::~Layer()] Destruct Layer.");
}
