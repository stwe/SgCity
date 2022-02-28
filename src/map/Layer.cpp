// This file is part of the SgCity project.
//
// Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <utility>
#include "Layer.h"
#include "Log.h"
#include "Tile.h"
#include "ogl/buffer/Vao.h"

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
