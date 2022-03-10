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
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::buffer
{
    class Vao;
}

namespace sg::gui
{
    enum class Action;
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

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        /**
         * Constructs a new Layer object.
         */
        Layer();

        /**
         * Constructs a new Layer object.
         *
         * @param t_tileCount The number of tiles in x and z direction.
         */
        explicit Layer(int t_tileCount);

        /**
         * Constructs a new Layer object.
         *
         * @param t_tiles The Tile objects.
         */
        explicit Layer(std::vector<std::shared_ptr<Tile>> t_tiles);

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
         * Updates a Layer tile.
         *
         * @param t_action Indicates what should be done with the given tile.
         * @param t_tile The tile.
         */
        virtual void UpdateTile(gui::Action t_action, Tile& t_tile);

        /**
         * Render the Layer.
         *
         * @param t_window The Window object.
         * @param t_camera The Camera object.
         */
        virtual void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const = 0;

    protected:

    private:

        /**
         * Creates Layer tiles.
         */
        virtual void CreateTiles() = 0;
    };
}
