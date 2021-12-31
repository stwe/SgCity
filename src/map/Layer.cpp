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

sg::map::Layer::Layer(const int t_tileCount)
    : tileCount{ t_tileCount }
{
    Log::SG_LOG_DEBUG("[Layer::Layer()] Create Layer.");
}

sg::map::Layer::Layer(const int t_tileCount, std::vector<std::shared_ptr<Tile>> t_tiles)
    : tileCount{ t_tileCount }
    , tiles{ std::move(t_tiles) }
{
    Log::SG_LOG_DEBUG("[Layer::Layer()] Create Layer.");
}

sg::map::Layer::~Layer() noexcept
{
    Log::SG_LOG_DEBUG("[Layer::~Layer()] Destruct Layer.");
}
