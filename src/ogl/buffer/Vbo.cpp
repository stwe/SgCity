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

#include "Vbo.h"
#include "SgAssert.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Vbo::Vbo()
{
    Log::SG_LOG_DEBUG("[Vbo::Vbo()] Create Vbo.");

    CreateId();
}

sg::ogl::buffer::Vbo::~Vbo() noexcept
{
    Log::SG_LOG_DEBUG("[Vbo::~Vbo()] Destruct Vbo.");

    CleanUp();
}

//-------------------------------------------------
// Bind / unbind
//-------------------------------------------------

void sg::ogl::buffer::Vbo::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void sg::ogl::buffer::Vbo::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-------------------------------------------------
// Attributes
//-------------------------------------------------

void sg::ogl::buffer::Vbo::AddFloatAttribute(const uint32_t t_index,
                                             const int32_t t_nrOfFloatComponents,
                                             const int32_t t_nrOfAllFloats,
                                             const uint64_t t_startPoint) const
{
    Bind();

    glEnableVertexAttribArray(t_index);
    glVertexAttribPointer(
        t_index,
        t_nrOfFloatComponents,
        GL_FLOAT,
        GL_FALSE,
        t_nrOfAllFloats * static_cast<int32_t>(sizeof(float)),
        reinterpret_cast<uintptr_t*>(t_startPoint * sizeof(float))
    );

    Unbind();
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::buffer::Vbo::CreateId()
{
    glGenBuffers(1, &id);
    SG_ASSERT(id, "[Vbo::CreateId()] Error while creating a new Vbo.")

    Log::SG_LOG_DEBUG("[Vbo::CreateId()] A new Vbo was created. The Id is {}.", id);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::buffer::Vbo::CleanUp()
{
    Log::SG_LOG_DEBUG("[Vbo::CleanUp()] Clean up Vbo Id {}.", id);

    Unbind();

    if (id)
    {
        glDeleteBuffers(1, &id);
        Log::SG_LOG_DEBUG("[Vbo::CleanUp()] Vbo Id {} was deleted.", id);
    }
}
