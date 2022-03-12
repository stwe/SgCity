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

#include <imgui.h>
#include <vector>
#include "map/Map.h"

namespace sg::gui
{
    enum class Action
    {
        RAISE,                  // raise terrain
        LOWER,                  // lower terrain
        MAKE_RESIDENTIAL_ZONE,  // make a residential zone
        MAKE_COMMERCIAL_ZONE,   // make a commercial zone
        MAKE_INDUSTRIAL_ZONE,   // make a industrial zone
        MAKE_TRAFFIC_ZONE,      // make a traffic zone
        CREATE_PLANT,           // create a tree
        INFO                    // get tile info
    };

    class MapEditGui
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        Action action{ Action::RAISE };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MapEditGui()
        {
            m_buttonTextures = CreateButtonTextures();
        }

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void RenderImGui(map::Map& t_map);

    protected:

    private:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * A text label identifying an element.
         * Can be used as a tooltip.
         */
        inline static const std::vector<std::string> BUTTON_NAMES
        {
            "Raise terrain",
            "Lower terrain",
            "Make residential zone",
            "Make commercial zone",
            "Make industrial zone",
            "Make traffic zone",
            "Create a plant",
            "Info"
        };

        static constexpr int FRAME_PADDING{ 1 };                                     // -1 == uses default padding (style.FramePadding)
        inline static const auto SIZE{ ImVec2(32.0f, 32.0f) };                 // Size of the image we want to make visible
        inline static const auto UV0{ ImVec2(0.0f, 0.0f) };                    // UV coordinates for lower-left
        inline static const auto BG{ ImVec4(0.0f, 0.0f, 0.0f, 1.0f) };   // Black background
        inline static const auto TINT{ ImVec4(1.0f, 1.0f, 1.0f, 1.0f) }; // No tint

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * A texture handle for each menu entry.
         */
        std::vector<ImTextureID> m_buttonTextures;

        /**
         * Indicates whether a menu item is active.
         */
        inline static std::vector<bool> m_buttons{ true, false, false, false, false, false, false, false };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Loads a texture for each menu entry.
         */
        static std::vector<void*> CreateButtonTextures();
    };
}
