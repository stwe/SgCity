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

#include "Ebo.h"
#include "Assert.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Ebo::Ebo()
{
    Log::SG_LOG_DEBUG("[Ebo::Ebo()] Create Ebo.");

    CreateId();
}

sg::ogl::buffer::Ebo::~Ebo() noexcept
{
    Log::SG_LOG_DEBUG("[Ebo::~Ebo()] Destruct Ebo.");

    CleanUp();
}

//-------------------------------------------------
// Bind / unbind
//-------------------------------------------------

void sg::ogl::buffer::Ebo::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::buffer::Ebo::CreateId()
{
    glGenBuffers(1, &id);
    SG_ASSERT(id, "[Ebo::CreateId()] Error while creating a new Ebo.")

    Log::SG_LOG_DEBUG("[Ebo::CreateId()] A new Ebo was created. The Id is {}.", id);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::buffer::Ebo::CleanUp()
{
    Log::SG_LOG_DEBUG("[Ebo::CleanUp()] Clean up Ebo Id {}.", id);

    if (id)
    {
        glDeleteBuffers(1, &id);
        Log::SG_LOG_DEBUG("[Ebo::CleanUp()] Ebo Id {} was deleted.", id);
    }
}
