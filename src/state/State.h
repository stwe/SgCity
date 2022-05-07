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
#include <array>
#include <string_view>

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl
{
    class Window;
}

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
         * The unique identifiers of the states.
         */
        enum class Id
        {
            MENU, // MenuState
            GAME, // GameState
            ALL   // is used when all states are meant
        };

        /**
         * The unique identifiers of the states as string.
         */
        static constexpr std::array<std::string_view, 3> STATE_IDS
        {
            "MENU", "GAME", "ALL"
        };

        /**
         * Shared objects between all states.
         */
        struct Context
        {
            explicit Context(std::shared_ptr<ogl::Window> t_window)
                : window{ std::move(t_window) }
            {}

            std::shared_ptr<ogl::Window> window;
        };

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
         * @param t_id The unique identifier.
         * @param t_stateStack The StateStack object.
         * @param t_context The holder of shared objects.
         */
        State(Id t_id, StateStack* t_stateStack, Context t_context);

        State(const State& t_other) = delete;
        State(State&& t_other) noexcept = delete;
        State& operator=(const State& t_other) = delete;
        State& operator=(State&& t_other) noexcept = delete;

        virtual ~State() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] Id GetId() const { return m_id; }

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        virtual void Init() = 0;
        virtual void Input() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void RenderImGui() = 0;

        //-------------------------------------------------
        // Frame
        //-------------------------------------------------

        virtual void StartFrame();
        virtual void EndFrame() const;

    protected:
        //-------------------------------------------------
        // Stack operations
        //-------------------------------------------------

        void RequestStackPush(Id t_id) const;
        void RequestStackPop() const;
        void RequestStateClear() const;

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The unique identifier of this state.
         */
        Id m_id;

        /**
         * Pointer to the parent state stack.
         */
        StateStack* m_stateStack{ nullptr };
    };
}
