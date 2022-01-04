#pragma once

#include <string>
#include <memory>
#include <vector>
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
    class Texture;
}

//-------------------------------------------------
// Model
//-------------------------------------------------

namespace sg::ogl::resource
{
    class Model
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Model() = delete;

        /**
         * Constructs a new Model object.
         *
         * @param t_path The path to the model file.
         */
        explicit Model(std::string t_path);

        Model(const Model& t_other) = delete;
        Model(Model&& t_other) noexcept = delete;
        Model& operator=(const Model& t_other) = delete;
        Model& operator=(Model&& t_other) noexcept = delete;

        ~Model() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        /**
         * Renders the model.
         *
         * @param t_window The Window object.
         * @param t_camera The Camera object.
         * @param t_position The position in local space.
         */
        void Render(
            const ogl::Window& t_window,
            const ogl::camera::Camera& t_camera,
            const glm::vec3& t_position
            ) const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The path to the model file.
         */
        std::string m_path;

        /**
         * A Vao for each Shape.
         */
        std::vector<std::unique_ptr<ogl::buffer::Vao>> m_vaos;

        /**
         * A ShaderProgram object used to draw the model.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_modelShaderProgram;

        /**
         * The Texture for the model.
         */
        std::unique_ptr<ogl::resource::Texture> m_modelTexture;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp() const;
    };
}
