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

#pragma once

#include <vector>
#include <string>
#include "map/Map.h"

namespace sg::gui
{
    enum class Action
    {
        RAISE,                  // raise terrain
        LOWER,                  // lower terrain
        MAKE_RESIDENTIAL_ZONE,  // make a residential zone
        MAKE_TRAFFIC_ZONE,      // make a traffic zone
        INFO                    // get terrain info
    };

    class MapEditGui
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        Action action{ Action::RAISE };

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void RenderImGui(map::Map& t_map);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::vector<std::string> m_buttonNames
        {
            "Raise terrain",
            "Lower terrain",
            "Make residential zone",
            "Make traffic zone",
            "Info"
        };

        std::vector<bool> m_buttons{ true, false, false, false, false };
    };
}
