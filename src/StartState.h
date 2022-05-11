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
// Forward declarations
//-------------------------------------------------

namespace sg::city
{
    class City;
}

//-------------------------------------------------
// StartState
//-------------------------------------------------

namespace sg
{
    /**
     * State to setup a new city.
     */
    class StartState : public state::State
    {
    public:
        //-------------------------------------------------
        // Types
        //-------------------------------------------------

        /**
         * Menu actions.
         */
        enum class Action
        {
            NONE,        // default
            MAIN_MENU,   // back to the main menu (MenuState)
            CREATE_CITY  // play a city
        };

        /**
         * The level of difficulty.
         */
        enum class Level
        {
            EASY, MEDIUM, HARD
        };

        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * Possible map sizes.
         */
        static constexpr std::array<int, 3> MAP_SIZES{ 128, 256, 512 };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The current menu action.
         */
        Action action{ Action::NONE };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        StartState() = delete;

        /**
         * Constructs a new CityState object.
         *
         * @param t_id The unique identifier.
         * @param t_stateStack The parent StateStack object.
         * @param t_context The holder of shared objects.
         */
        StartState(Id t_id, state::StateStack* t_stateStack, std::shared_ptr<Context> t_context);

        StartState(const StartState& t_other) = delete;
        StartState(StartState&& t_other) noexcept = delete;
        StartState& operator=(const StartState& t_other) = delete;
        StartState& operator=(StartState&& t_other) noexcept = delete;

        ~StartState() noexcept override;

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
        // Member
        //-------------------------------------------------

        /**
         * Name name of the city.
         */
        char m_cityName[96]{ "Musterstadt" };

        /**
         * The size of the map.
         */
        int m_mapSize{ 0 };

        /**
         * The level of difficulty.
         */
        Level m_level{ Level::EASY };
    };
}
