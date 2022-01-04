#pragma once

#include <vector>
#include <string>
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

        void RenderImGui(map::Map& t_map);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::vector<std::string> m_buttonNames{ "Raise terrain", "Lower terrain", "Build road", "Info" };
        std::vector<bool> m_buttons{ true, false, false, false };
    };
}
