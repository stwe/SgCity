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
#include "state/State.h"

//-------------------------------------------------
// MenuState
//-------------------------------------------------

namespace sg
{
    class MenuState : public state::State
    {
    public:
        //-------------------------------------------------
        // Types
        //-------------------------------------------------

        enum class Action
        {
            NONE, LOAD_CITY, START_CITY, EDIT_MAP, QUIT
        };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        Action action{ Action::NONE };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MenuState() = delete;

        MenuState(Id t_id, state::StateStack* t_stateStack, const Context& t_context);

        MenuState(const MenuState& t_other) = delete;
        MenuState(MenuState&& t_other) noexcept = delete;
        MenuState& operator=(const MenuState& t_other) = delete;
        MenuState& operator=(MenuState&& t_other) noexcept = delete;

        ~MenuState() noexcept override;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input() override;
        void Update() override;
        void Render() override;
        void RenderImGui() override;

    protected:

    private:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        static constexpr std::array<std::string_view, 5> BUTTON_NAMES
        {
            "None",
            "Load Saved City",
            "Start New City",
            "Edit New Map",
            "Quit",
        };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::vector<bool> m_buttons
        {
            true, false, false, false, false
        };
    };
}
