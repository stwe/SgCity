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

#include "Mesh.h"
#include "SgAssert.h"
#include "ogl/buffer/Vao.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Mesh::Mesh()
    : vao{ std::make_unique<buffer::Vao>() }
{
    SG_ASSERT(vao, "[Mesh::Mesh()] Null pointer.")
    Log::SG_LOG_DEBUG("[Mesh::Mesh()] Create Mesh.");
}

sg::ogl::resource::Mesh::Mesh(std::string t_name)
    : vao{ std::make_unique<buffer::Vao>() }
    , name{ std::move(t_name) }
{
    SG_ASSERT(vao, "[Mesh::Mesh()] Null pointer.")
    Log::SG_LOG_DEBUG("[Mesh::Mesh()] Create Mesh with name {}.", t_name);
}

sg::ogl::resource::Mesh::~Mesh() noexcept
{
    Log::SG_LOG_DEBUG("[Mesh::Mesh()] Destruct Mesh.");
}

//-------------------------------------------------
// Draw methods created for convenience
//-------------------------------------------------

void sg::ogl::resource::Mesh::InitDraw() const
{
    vao->Bind();
}

void sg::ogl::resource::Mesh::DrawPrimitives(const uint32_t t_drawMode) const
{
    vao->DrawPrimitives(t_drawMode);
}

void sg::ogl::resource::Mesh::DrawInstanced(const int32_t t_instanceCount, const uint32_t t_drawMode) const
{
    //vao->DrawInstanced(t_instanceCount, t_drawMode);
}

void sg::ogl::resource::Mesh::EndDraw()
{
    buffer::Vao::Unbind();
}
