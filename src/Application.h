#pragma once

#include <memory>
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"
#include "gui/MapEditGui.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::map
{
    class Map;
    class Water;
    class RoadNetwork;
}

namespace sg::ogl::resource
{
    class Model;
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

        static constexpr auto FRAME_TIME{ 1.0 / 60.0 };

        /**
         * The number of tiles in x and z direction.
         */
        static constexpr auto TILE_COUNT{ 64 };

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
         * A Glfw window.
         */
        ogl::Window m_window{ 1024, 768, "SgCity Sandbox" };

        /**
         * A camera to move around the map.
         */
        ogl::camera::Camera m_camera{ glm::vec3(1.0f, 2.0f, 37.0f), -88.0f, -34.0f };

        /**
         * The map from tiles.
         */
        std::unique_ptr<map::Map> m_map;

        /**
         * The water surface.
         */
        std::unique_ptr<map::Water> m_water;

        /**
         * A tree obj model.
         */
        std::unique_ptr<ogl::resource::Model> m_treeModel;

        /**
         * The roads.
         */
        std::unique_ptr<map::RoadNetwork> m_roadNetwork;

        /**
         * Render widgets to edit the map.
         */
        gui::MapEditGui m_mapEditGui;

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

        //-------------------------------------------------
        // Game loop
        //-------------------------------------------------

        void GameLoop();

        //-------------------------------------------------
        // Frame
        //-------------------------------------------------

        void StartFrame();
        void EndFrame();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
