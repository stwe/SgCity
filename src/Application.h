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
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"
#include "event/Event.h"
#include "eventpp/eventqueue.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::map
{
    class Map;
}

namespace sg::gui
{
    class MapEditGui;
}

namespace sg::ogl::resource
{
    class Skybox;
}

//-------------------------------------------------
// Application
//-------------------------------------------------

namespace sg
{
    /**
     * Represents the main application class of the game.
     */
    class Application
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * 60 updates per second.
         */
        static constexpr auto FRAME_TIME{ 1.0 / 60.0 };

        /**
         * The number of tiles in x and z direction.
         */
        static constexpr auto TILE_COUNT{ 64 };

        /**
         * The width of the screen.
         */
        static constexpr auto SCREEN_WIDTH{ 1024 };

        /**
         * The height of the screen.
         */
        static constexpr auto SCREEN_HEIGHT{ 768 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        /**
         * Constructs a new Application object.
         */
        Application();

        Application(const Application& t_other) = delete;
        Application(Application&& t_other) noexcept = delete;
        Application& operator=(const Application& t_other) = delete;
        Application& operator=(Application&& t_other) noexcept = delete;

        ~Application() noexcept;

        //-------------------------------------------------
        // Run
        //-------------------------------------------------

        void Run();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * A Glfw window.
         */
        ogl::Window m_window{ SCREEN_WIDTH, SCREEN_HEIGHT, "SgCity Sandbox" };

        /**
         * A camera to move around the map.
         */
        ogl::camera::Camera m_camera{ glm::vec3(0.0f, 10.0f, 14.0f), 45.0f, -26.0f };

        /**
         * The Map object.
         */
        std::unique_ptr<map::Map> m_map;

        /**
         * Widgets to edit the map layers.
         */
        std::unique_ptr<gui::MapEditGui> m_mapEditGui;

        /**
         * A Skybox object.
         */
        std::unique_ptr<ogl::resource::Skybox> m_skybox;

        /**
         * So that events are only executed once.
         */
        bool m_handleMouseEvent{ true };

        /**
         * The event queue.
         */
        eventpp::EventQueue<event::SgEventType, void(std::shared_ptr<event::SgEvent>)> m_eventQueue;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init();
        void Input();
        void Update();
        void Render();
        void RenderImGui();

        //-------------------------------------------------
        // Game loop
        //-------------------------------------------------

        void GameLoop();

        //-------------------------------------------------
        // Frame
        //-------------------------------------------------

        static void StartFrame();
        void EndFrame() const;

        //-------------------------------------------------
        // Logic helper
        //-------------------------------------------------

        void InputLeftMouseButton();
        void InputRightMouseButton();
        void InputKey();
    };
}
