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

#include "state/State.h"

//-------------------------------------------------
// GameState
//-------------------------------------------------

namespace sg
{
    class GameState : public state::State
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        GameState() = delete;

        GameState(std::shared_ptr<state::StateStack> t_stateStack, const Context& t_context)
            : State(std::move(t_stateStack), t_context)
        {
        }

        GameState(const GameState& t_other) = delete;
        GameState(GameState&& t_other) noexcept = delete;
        GameState& operator=(const GameState& t_other) = delete;
        GameState& operator=(GameState&& t_other) noexcept = delete;

        ~GameState() noexcept override {}

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init() override {}
        void Input() override {}
        void Update() override {}
        void Render() override {}
        void RenderImGui() override {}

    protected:

    private:

    };
}
