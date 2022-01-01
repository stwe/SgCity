#pragma once

#include <vector>
#include <string>
#include "map/Tile.h"
#include "map/Map.h"

namespace sg::gui
{
    enum class Action
    {
        RAISE,
        LOWER,
        SET_TRAFFIC,
        INFO
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

        void Render(const map::Map& t_map);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::vector<std::string> m_buttonNames{ "Raise terrain", "Lower terrain", "Build road", "Info" };
        std::vector<bool> m_buttons{ true, false, false, false };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static void TileInfo(const map::Tile& t_tile);
    };
}
