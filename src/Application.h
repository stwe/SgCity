#pragma once

#include <memory>
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"
#include "ogl/resource/Model.h"

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
        ogl::Window m_window{ 1024, 768, "SgCity Sandbox" };

        /**
         * A camera to move around the map.
         */
        ogl::camera::Camera m_camera{ glm::vec3(5.0f, 6.0f, 31.0f), -90.0f, -45.0f };

        /**
         * The Map object.
         */
        std::unique_ptr<map::Map> m_map;

        /**
         * Widgets to edit the map layers.
         */
        std::unique_ptr<gui::MapEditGui> m_mapEditGui;

        /**
         * So that events are only executed once.
         */
        bool m_handleMouseEvent{ true };

        std::unique_ptr<ogl::resource::Model> m_model;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init();
        void Input();
        void Update();
        void Render() const;

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
