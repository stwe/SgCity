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
#include <cstdint>

//-------------------------------------------------
// Vao
//-------------------------------------------------

namespace sg::ogl::buffer
{
    /**
     * Forward declaration class Vbo.
     */
    class Vbo;

    /**
     * Forward declaration class Ebo.
     */
    class Ebo;

    /**
     * Represents a Vertex Array Object.
     */
    class Vao
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The handle of the Vao.
         */
        uint32_t id{ 0 };

        /**
         * Number of vertices to draw.
         */
        int32_t drawCount{ 0 };

        /**
         * A Vbo object.
         */
        std::unique_ptr<Vbo> vbo;

        /**
         * An Ebo object (Indexbuffer).
         */
        std::unique_ptr<Ebo> ebo;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Vao();

        Vao(const Vao& t_other) = delete;
        Vao(Vao&& t_other) noexcept = delete;
        Vao& operator=(const Vao& t_other) = delete;
        Vao& operator=(Vao&& t_other) noexcept = delete;

        ~Vao() noexcept;

        //-------------------------------------------------
        // Bind / unbind
        //-------------------------------------------------

        void Bind() const;
        static void Unbind();

        //-------------------------------------------------
        // Create Vbo
        //-------------------------------------------------

        /**
         * Binds this Vao and creates an empty dynamic Vbo.
         * Allocate memory and *not* fill it.
         *
         * Bufferlayout:
         * -------------
         * location 0 (position)  3 floats
         * location 1 (uv)        2 floats
         * location 2 (idColor)   3 floats
         * location 3 (normal)    3 floats
         * location 4 (textureNr) 1 float
         * location 5 (selected)  1 float
         *
         * @param t_size Specifies the size in bytes of the buffer object's new data store.
         * @param t_drawCount Sets the number of vertices to be render if the value is greater than zero.
         */
        void CreateEmptyDynamicVbo(uint32_t t_size, int32_t t_drawCount = 0);

        /**
         * Binds this Vao and creates a static Vbo. The given vertices data are copied into the Vbo.
         *
         * Bufferlayout
         * ------------
         * location 0 (position) 3 floats
         * location 1 (uv)       2 floats
         *
         * @param t_vertices The vertices to copy.
         * @param t_drawCount Sets the number of vertices to be render if the value is greater than zero.
         */
        void CreateStaticVbo(const std::vector<float>& t_vertices, int32_t t_drawCount = 0);

        /**
         * Creates a simple square.
         * Binds this Vao and creates a static Vbo.
         *
         * Bufferlayout
         * ------------
         * location 0 (position) 2 floats
         */
        void CreateStaticWaterVbo();

        /**
         * Binds this Vao and creates a static Vbo for a Skybox.
         *
         * Bufferlayout
         * ------------
         * location 0 (position) 3 floats
         */
        void CreateSkyboxVbo();

        /**
         * Creates a simple square for a Gui.
         * Binds this Vao and creates a static Vbo.
         *
         * Bufferlayout
         * ------------
         * location 0 (position) 2 floats
         */
        void CreateGuiVbo();

        // todo:

        void CreateModelVertexDataVbo(const std::vector<float>& t_vertices, int32_t t_drawCount);
        void CreateModelIndexBuffer(const std::vector<uint32_t>& t_indices);

        //-------------------------------------------------
        // Draw
        //-------------------------------------------------

        void DrawPrimitives(uint32_t t_drawMode, int32_t t_first) const;
        void DrawPrimitives(uint32_t t_drawMode) const;
        void DrawPrimitives() const;

    protected:

    private:
        //-------------------------------------------------
        // Create
        //-------------------------------------------------

        void CreateId();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp() const;
    };
}
