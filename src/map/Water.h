#pragma once

#include <memory>
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::buffer
{
    class Vao;
}

namespace sg::ogl::resource
{
    class ShaderProgram;
}

//-------------------------------------------------
// Water
//-------------------------------------------------

namespace sg::map
{
    /**
     * Represents the Water.
     */
    class Water
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * The position of the Water in world space.
         */
        static constexpr auto WATER_POSITION{ glm::vec3(0.0f, -0.8f, 0.0f) };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Water() = delete;

        /**
         * Constructs a new Water object.
         *
         * @param t_tileCount The number of tiles in x and z direction.
         */
        explicit Water(int t_tileCount);

        Water(const Water& t_other) = delete;
        Water(Water&& t_other) noexcept = delete;
        Water& operator=(const Water& t_other) = delete;
        Water& operator=(Water&& t_other) noexcept = delete;

        ~Water() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        /**
         * Renders a water surface.
         *
         * @param t_window The Window object.
         * @param t_camera The Camera object.
         */
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /*
         * The number of tiles in x and z direction.
         */
        int m_tileCount{ 0 };

        /**
         * A Vao object which contains a Vbo with water vertex data.
         */
        std::unique_ptr<ogl::buffer::Vao> m_waterVao;

        /**
         * A ShaderProgram object used to draw the water surface.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_waterShaderProgram;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the Water.
         */
        void Init();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
