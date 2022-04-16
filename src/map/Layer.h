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
#include <vector>
#include "ogl/camera/Camera.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl
{
    class Window;
}

namespace sg::ogl::buffer
{
    class Vao;
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

        /**
         * The Window object.
         */
        std::shared_ptr<ogl::Window> window;

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

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Layer() = delete;

        /**
         * Constructs a new Layer object.
         *
         * @param t_window The Window object.
         * @param t_tileCount The number of tiles in x and z direction.
         */
        Layer(std::shared_ptr<ogl::Window> t_window, int t_tileCount);

        /**
         * Constructs a new Layer object.
         *
         * @param t_window The Window object.
         * @param t_tiles The Tile objects.
         */
        Layer(std::shared_ptr<ogl::Window> t_window, std::vector<std::shared_ptr<Tile>> t_tiles);

        Layer(const Layer& t_other) = delete;
        Layer(Layer&& t_other) noexcept = delete;
        Layer& operator=(const Layer& t_other) = delete;
        Layer& operator=(Layer&& t_other) noexcept = delete;

        virtual ~Layer() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        /**
         * Input handling.
         */
        virtual void Input() {}

        /**
         * Updates the Layer.
         */
        virtual void Update() {}

        /**
         * Render the Layer.
         *
         * @param t_camera The Camera object.
         * @param t_plane The clipping plane.
         */
        virtual void Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane = glm::vec4(0.0f)) const = 0;

        /**
         * Renders an ImGui window.
         */
        virtual void RenderImGui() {}

        //-------------------------------------------------
        // Listeners
        //-------------------------------------------------

        /**
         * On left mouse button pressed event handler.
         */
        virtual void OnLeftMouseButtonPressed() {}

        /**
         * On left mouse button released event handler.
         */
        virtual void OnLeftMouseButtonReleased() {}

        /**
         * On mouse moved event handler.
         */
        virtual void OnMouseMoved() {}

    protected:

    private:
        /**
         * Creates Layer tiles.
         */
        virtual void CreateTiles() = 0;
    };
}
