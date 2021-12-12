#pragma once

#include <vector>
#include <memory>
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"

namespace sg::ogl::buffer
{
    class Vao;
}

namespace sg::ogl::resource
{
    class ShaderProgram;
}

namespace sg::ogl::input
{
    class PickingTexture;
}

namespace sg::map
{
    class Tile;

    /**
     * Represents the Map.
     */
    class Map
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Map();

        Map(const Map& t_other) = delete;
        Map(Map&& t_other) noexcept = delete;
        Map& operator=(const Map& t_other) = delete;
        Map& operator=(Map&& t_other) noexcept = delete;

        ~Map() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);

        int GetTileObjectId();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * A container holding the Tile objects.
         */
        std::vector<std::unique_ptr<Tile>> m_tiles;

        /**
         * A Vao object for the Map.
         */
        std::unique_ptr<ogl::buffer::Vao> m_mapVao;

        /**
         * A ShaderProgram object used to draw the Map.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_mapShaderProgram;

        /**
         * A ShaderProgram object used for mouse picking.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_pickingShaderProgram;

        /**
         *
         */
        std::unique_ptr<ogl::input::PickingTexture> m_pickingTexture;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
