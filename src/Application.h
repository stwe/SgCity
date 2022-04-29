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
#include "ini/ini.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::city
{
    class City;
}

namespace sg::map
{
    class Map;
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

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

#if defined(_WIN64)
        inline static const inih::INIReader INI{ "./config.ini" };
        inline static const std::string RESOURCES_PATH{ INI.Get<std::string>("win64", "resources_path") };
#else
        inline static const inih::INIReader INI{ "/home/steffen/CLionProjects/SgCity/config.ini" }; // NOLINT(cert-err58-cpp)
        inline static const std::string RESOURCES_PATH{ INI.Get<std::string>("linux", "resources_path") }; // NOLINT(cert-err58-cpp)
#endif

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
        std::shared_ptr<map::Map> m_map;

        /**
         * A Skybox object.
         */
        std::unique_ptr<ogl::resource::Skybox> m_skybox;

        /**
         * A City object.
         */
        std::unique_ptr<city::City> m_city;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init();
        void Input() const;
        void Update();
        void Render();
        void RenderImGui() const;

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
