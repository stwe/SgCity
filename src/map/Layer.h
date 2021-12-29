#pragma once

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
}

//-------------------------------------------------
// Layer
//-------------------------------------------------

namespace sg::map
{
    /**
     * Forward declaration class Tile.
     */
    class Tile;

    /**
     * Represents the Layer.
     */
    class Layer
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /*
         * The number of tiles in x and z direction.
         */
        int tileCount{ 0 };

        /**
         * An array holding the Tile objects.
         */
        std::vector<std::shared_ptr<Tile>> tiles;

        /**
         * The position of the Layer in world space.
         */
        glm::vec3 position{ glm::vec3(0.0f) };

        /**
         * The model matrix of the Layer.
         */
        glm::mat4 modelMatrix{ glm::mat4(1.0f) };

        /**
         * A Vao object which contains a Vbo with vertex data.
         */
        std::unique_ptr<ogl::buffer::Vao> vao;

        /**
         * A ShaderProgram object used to draw the Layer.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> shaderProgram;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Layer();
        explicit Layer(std::vector<std::shared_ptr<Tile>> t_tiles);

        Layer(const Layer& t_other) = delete;
        Layer(Layer&& t_other) noexcept = delete;
        Layer& operator=(const Layer& t_other) = delete;
        Layer& operator=(Layer&& t_other) noexcept = delete;

        virtual ~Layer() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        virtual void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const = 0;

    protected:

    private:

    };
}
