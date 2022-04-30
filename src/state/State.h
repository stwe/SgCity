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

#include <memory>

//-------------------------------------------------
// State
//-------------------------------------------------

namespace sg::state
{
    /**
     * Forward declaration class StateStack.
     */
    class StateStack;

    /**
     * The base class for all states.
     */
    class State
    {
    public:
        //-------------------------------------------------
        // Types
        //-------------------------------------------------

        /**
         * A set of state Ids.
         */
        enum class Id
        {
            NONE,
            MENU,
            GAME,
            PAUSE
        };

        /**
         * Shared objects between all states.
         */
        struct Context
        {};

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * Works as a holder of shared objects between all states.
         */
        Context context;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        State() = delete;

        /**
         * Constructs a new State object.
         *
         * @param t_stateStack The StateStack object.
         * @param t_context The holder of shared objects.
         */
        State(std::shared_ptr<StateStack> t_stateStack, Context t_context);

        State(const State& t_other) = delete;
        State(State&& t_other) noexcept = delete;
        State& operator=(const State& t_other) = delete;
        State& operator=(State&& t_other) noexcept = delete;

        virtual ~State() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        virtual void Init() = 0;
        virtual void Input() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void RenderImGui() = 0;

    protected:
        //-------------------------------------------------
        // Stack operations
        //-------------------------------------------------

        void RequestStackPush(Id t_id);
        void RequestStackPop();
        void RequestStateClear();

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The StateStack object.
         */
        std::shared_ptr<StateStack> m_stateStack;
    };
}
