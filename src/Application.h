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

namespace sg::ogl
{
    class Window;
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
         * A Window object.
         */
        std::shared_ptr<ogl::Window> m_window;

        /**
         * A Camera object.
         */
        std::unique_ptr<ogl::camera::Camera> m_camera;

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
    };
}
