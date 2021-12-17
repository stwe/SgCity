#pragma once

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

namespace sg::map
{
    class Terrain
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Terrain() = delete;
        explicit Terrain(int t_vertexCount);

        Terrain(const Terrain& t_other) = delete;
        Terrain(Terrain&& t_other) noexcept = delete;
        Terrain& operator=(const Terrain& t_other) = delete;
        Terrain& operator=(Terrain&& t_other) noexcept = delete;

        ~Terrain() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init();
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        int m_vertexCount{ 0 };

        /**
         * A Vao object for the Terrain.
         */
        std::unique_ptr<ogl::buffer::Vao> m_terrainVao;

        /**
         * A ShaderProgram object used to draw the Map.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_terrainShaderProgram;
    };
}
