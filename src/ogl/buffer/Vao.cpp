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

#include "Vao.h"
#include "Assert.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Vao::Vao()
{
    Log::SG_LOG_DEBUG("[Vao::Vao()] Create Vao.");

    CreateId();
}

sg::ogl::buffer::Vao::~Vao() noexcept
{
    Log::SG_LOG_DEBUG("[Vao::~Vao()] Destruct Vao.");

    CleanUp();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::buffer::Vbo& sg::ogl::buffer::Vao::GetVbo()
{
    SG_ASSERT(m_vbo, "[Vao::GetVbo] Vbo does not exist.")
    return *m_vbo;
}

//-------------------------------------------------
// Bind / unbind
//-------------------------------------------------

void sg::ogl::buffer::Vao::Bind() const
{
    glBindVertexArray(id);
}

void sg::ogl::buffer::Vao::Unbind()
{
    glBindVertexArray(0);
}

//-------------------------------------------------
// Create Vbo
//-------------------------------------------------

void sg::ogl::buffer::Vao::CreateEmptyDynamicVbo(const uint32_t t_size, const int32_t t_drawCount)
{
    SG_ASSERT(!m_vbo, "[Vao::CreateEmptyDynamicVbo()] Vbo already exists.")

    Bind();

    m_vbo = std::make_unique<Vbo>();
    m_vbo->Bind();

    glBufferData(GL_ARRAY_BUFFER, t_size, nullptr, GL_DYNAMIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    m_vbo->AddFloatAttribute(0, 3, 11, 0);

    // enable location 1 (uv)
    m_vbo->AddFloatAttribute(1, 2, 11, 3);

    // enable location 2 (idColor)
    m_vbo->AddFloatAttribute(2, 3, 11, 5);

    // enable location 3 (normal)
    m_vbo->AddFloatAttribute(3, 3, 11, 8);

    Unbind();

    if (t_drawCount > 0)
    {
        drawCount = t_drawCount;
    }
}

void sg::ogl::buffer::Vao::CreateStaticVbo(const std::vector<float>& t_vertices, int32_t t_drawCount)
{
    SG_ASSERT(!m_vbo, "[Vao::CreateStaticVbo()] Vbo already exists.")

    Bind();

    m_vbo = std::make_unique<Vbo>();
    m_vbo->Bind();

    uint32_t floatsPerVertex{ 5 };

    glBufferData(GL_ARRAY_BUFFER, t_drawCount * floatsPerVertex * (uint32_t)sizeof(float), t_vertices.data(), GL_STATIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    m_vbo->AddFloatAttribute(0, 3, 5, 0);

    // enable location 1 (uv)
    m_vbo->AddFloatAttribute(1, 2, 5, 3);

    Unbind();

    if (t_drawCount > 0)
    {
        drawCount = t_drawCount;
    }
}

void sg::ogl::buffer::Vao::CreateStaticWaterVbo()
{
    SG_ASSERT(!m_vbo, "[Vao::CreateWaterVbo()] Vbo already exists.")

    Bind();

    m_vbo = std::make_unique<Vbo>();
    m_vbo->Bind();

    // x and z vertex positions, y is set to 0.0 in the vertex shader
    std::vector<float> vertices =
    {
         0.0f, 0.0f, // tl
         0.0f, 1.0f, // bl
         1.0f, 1.0f, // br
         0.0f, 0.0f, // tl
         1.0f, 1.0f, // br
         1.0f, 0.0f  // tr
    };

    drawCount = 6;
    uint32_t floatsPerVertex{ 2 };

    glBufferData(GL_ARRAY_BUFFER, drawCount * floatsPerVertex * (uint32_t)sizeof(float), vertices.data(), GL_STATIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    m_vbo->AddFloatAttribute(0, 2, 2, 0);

    Unbind();
}

//-------------------------------------------------
// Draw
//-------------------------------------------------

void sg::ogl::buffer::Vao::DrawPrimitives(const uint32_t t_drawMode, const int32_t t_first) const
{
    glDrawArrays(t_drawMode, t_first, drawCount);
}

void sg::ogl::buffer::Vao::DrawPrimitives(const uint32_t t_drawMode) const
{
    DrawPrimitives(t_drawMode, 0);
}

void sg::ogl::buffer::Vao::DrawPrimitives() const
{
    DrawPrimitives(GL_TRIANGLES);
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::buffer::Vao::CreateId()
{
    glGenVertexArrays(1, &id);
    SG_ASSERT(id, "[Vao::CreateId()] Error while creating a new Vao.")

    Log::SG_LOG_DEBUG("[Vao::CreateId()] A new Vao was created. The Id is {}.", id);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::buffer::Vao::CleanUp()
{
    Log::SG_LOG_DEBUG("[Vao::CleanUp()] Clean up Vao Id {}.", id);

    glDisableVertexAttribArray(0);

    Unbind();

    if (id)
    {
        glDeleteVertexArrays(1, &id);
    }

    Log::SG_LOG_DEBUG("[Vao::CleanUp()] Vao Id {} was deleted.", id);
}
