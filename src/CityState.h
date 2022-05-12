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

namespace sg::ogl::camera
{
    class Camera;
}

namespace sg::ogl::resource
{
    class Skybox;
}

//-------------------------------------------------
// CityState
//-------------------------------------------------

namespace sg
{
    /**
     * Represents all the stuff to play a city.
     */
    class CityState : public state::State
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
            NONE,      // default
            MAIN_MENU  // skip the StartState and go back to the MenuState
        };

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

        CityState() = delete;

        /**
         * Constructs a new CityState object.
         *
         * @param t_id The unique identifier.
         * @param t_stateStack The parent StateStack object.
         * @param t_context The holder of shared objects.
         */
        CityState(Id t_id, state::StateStack* t_stateStack, std::shared_ptr<Context> t_context);

        CityState(const CityState& t_other) = delete;
        CityState(CityState&& t_other) noexcept = delete;
        CityState& operator=(const CityState& t_other) = delete;
        CityState& operator=(CityState&& t_other) noexcept = delete;

        ~CityState() noexcept override;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input() override;
        void Update() override;
        void PreRender() override;
        void Render() override;
        void RenderImGui() override;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * To show an about window.
         */
        bool m_showAbout{ false };

        /**
         * A camera to move around.
         */
        std::unique_ptr<ogl::camera::Camera> m_camera;

        /**
         * A skybox for a good looking environment.
         */
        std::unique_ptr<ogl::resource::Skybox> m_skybox;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // ImGui
        //-------------------------------------------------

        /**
         * The main menu bar on the top.
         */
        void ShowMainMenuBar();

        /**
         * The menu for build the city.
         */
        void ShowGameMenu();
    };
}
