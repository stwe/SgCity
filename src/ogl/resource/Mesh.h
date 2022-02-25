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
#include <string>
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::buffer
{
    class Vao;
}

//-------------------------------------------------
// Mesh
//-------------------------------------------------

namespace sg::ogl::resource
{
    /**
     * Forward declaration struct Material.
     */
    struct Material;

    /**
     * Represents a Mesh.
     */
    class Mesh
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The Vao of the Mesh.
         */
        std::unique_ptr<buffer::Vao> vao;

        /**
         * The name of the Mesh.
         */
        std::string name;

        /**
         * The default Material of the Mesh.
         */
        std::shared_ptr<Material> defaultMaterial;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Mesh();

        explicit Mesh(std::string t_name);

        Mesh(const Mesh& t_other) = delete;
        Mesh(Mesh&& t_other) noexcept = delete;
        Mesh& operator=(const Mesh& t_other) = delete;
        Mesh& operator=(Mesh&& t_other) noexcept = delete;

        ~Mesh() noexcept;

        //-------------------------------------------------
        // Draw - methods created for convenience
        //-------------------------------------------------

        /**
         * Bind Vao.
         */
        void InitDraw() const;

        /**
         * Calls glDrawElements or glDrawArrays to render.
         *
         * @param t_drawMode Specifies what kind of primitives to render.
         */
        void DrawPrimitives(uint32_t t_drawMode = GL_TRIANGLES) const;

        /**
         * Calls glDrawElementsInstanced or glDrawArraysInstanced to render.
         *
         * @param t_instanceCount Specifies the number of instances to be rendered.
         * @param t_drawMode Specifies what kind of primitives to render.
         */
        void DrawInstanced(int32_t t_instanceCount, uint32_t t_drawMode = GL_TRIANGLES) const;

        /**
         * Unbind Vao.
         */
        static void EndDraw();

    protected:

    private:

    };
}
