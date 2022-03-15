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
#include "Vbo.h"
#include "Ebo.h"
#include "SgAssert.h"
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
    SG_ASSERT(!vbo, "[Vao::CreateEmptyDynamicVbo()] Vbo already exists.")

    Bind();

    vbo = std::make_unique<Vbo>();
    vbo->Bind();

    glBufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(t_size), nullptr, GL_DYNAMIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    vbo->AddFloatAttribute(0, 3, 12, 0);

    // enable location 1 (uv)
    vbo->AddFloatAttribute(1, 2, 12, 3);

    // enable location 2 (idColor)
    vbo->AddFloatAttribute(2, 3, 12, 5);

    // enable location 3 (normal)
    vbo->AddFloatAttribute(3, 3, 12, 8);

    // enable location 4 (textureNr)
    vbo->AddFloatAttribute(4, 1, 12, 11);

    Unbind();

    if (t_drawCount > 0)
    {
        drawCount = t_drawCount;
    }
}

void sg::ogl::buffer::Vao::CreateStaticVbo(const std::vector<float>& t_vertices, const int32_t t_drawCount)
{
    SG_ASSERT(!vbo, "[Vao::CreateStaticVbo()] Vbo already exists.")

    Bind();

    vbo = std::make_unique<Vbo>();
    vbo->Bind();

    constexpr int64_t floatsPerVertex{ 5 };

    glBufferData(GL_ARRAY_BUFFER, t_drawCount * floatsPerVertex * static_cast<int64_t>(sizeof(float)), t_vertices.data(), GL_STATIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    vbo->AddFloatAttribute(0, 3, 5, 0);

    // enable location 1 (uv)
    vbo->AddFloatAttribute(1, 2, 5, 3);

    Unbind();

    if (t_drawCount > 0)
    {
        drawCount = t_drawCount;
    }
}

void sg::ogl::buffer::Vao::CreateStaticWaterVbo()
{
    SG_ASSERT(!vbo, "[Vao::CreateWaterVbo()] Vbo already exists.")

    Bind();

    vbo = std::make_unique<Vbo>();
    vbo->Bind();

    // x and z vertex positions, y is set to 0.0 in the vertex shader
    const std::vector vertices =
    {
         0.0f, 0.0f, // tl
         0.0f, 1.0f, // bl
         1.0f, 1.0f, // br
         0.0f, 0.0f, // tl
         1.0f, 1.0f, // br
         1.0f, 0.0f  // tr
    };

    drawCount = 6;
    constexpr int64_t floatsPerVertex{ 2 };

    glBufferData(GL_ARRAY_BUFFER, drawCount * floatsPerVertex * static_cast<int64_t>(sizeof(float)), vertices.data(), GL_STATIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    vbo->AddFloatAttribute(0, 2, 2, 0);

    Unbind();
}

void sg::ogl::buffer::Vao::CreateSkyboxVbo()
{
    SG_ASSERT(!vbo, "[Vao::CreateSkyboxVbo()] Vbo already exists.")

    Bind();

    vbo = std::make_unique<Vbo>();
    vbo->Bind();

    const std::vector vertices
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    drawCount = 36;
    constexpr int64_t floatsPerVertex{ 3 };

    glBufferData(GL_ARRAY_BUFFER, drawCount * floatsPerVertex * static_cast<int64_t>(sizeof(float)), vertices.data(), GL_STATIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    vbo->AddFloatAttribute(0, 3, 3, 0);

    Unbind();
}

void sg::ogl::buffer::Vao::CreateGuiVbo()
{
    SG_ASSERT(!vbo, "[Vao::CreateGuiVbo()] Vbo already exists.")

    Bind();

    vbo = std::make_unique<Vbo>();
    vbo->Bind();

    // to render with GL_TRIANGLE_STRIP
    const std::vector vertices =
    {
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f
    };

    drawCount = 4;
    constexpr int64_t floatsPerVertex{ 2 };

    glBufferData(GL_ARRAY_BUFFER, drawCount * floatsPerVertex * static_cast<int64_t>(sizeof(float)), vertices.data(), GL_STATIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    vbo->AddFloatAttribute(0, 2, 2, 0);

    Unbind();
}

void sg::ogl::buffer::Vao::CreateModelVertexDataVbo(const std::vector<float>& t_vertices, const int32_t t_drawCount)
{
    SG_ASSERT(!vbo, "[Vao::CreateModelVertexDataVbo()] Vbo already exists.")

    Bind();

    vbo = std::make_unique<Vbo>();
    vbo->Bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(t_vertices.size()) * static_cast<int64_t>(sizeof(float)), t_vertices.data(), GL_STATIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    vbo->AddFloatAttribute(0, 3, 5, 0);

    // enable location 1 (uv)
    vbo->AddFloatAttribute(1, 2, 5, 3);

    Unbind();
    drawCount = t_drawCount;
}

void sg::ogl::buffer::Vao::CreateModelIndexBuffer(const std::vector<uint32_t>& t_indices)
{
    SG_ASSERT(!ebo, "[Vao::CreateModelIndexBuffer()] Ebo already exists.")
    SG_ASSERT(vbo, "[Vao::CreateModelIndexBuffer()] The Ebo should be created after the Vbo.")

    static constexpr auto ELEMENT_SIZE_IN_BYTES{ static_cast<int64_t>(sizeof(uint32_t)) };

    Bind();

    ebo = std::make_unique<Ebo>();
    ebo->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int64_t>(t_indices.size()) * ELEMENT_SIZE_IN_BYTES, t_indices.data(), GL_STATIC_DRAW);

    Unbind();
    drawCount = static_cast<int32_t>(t_indices.size());
}

//-------------------------------------------------
// Draw
//-------------------------------------------------

void sg::ogl::buffer::Vao::DrawPrimitives(const uint32_t t_drawMode, const int32_t t_first) const
{
    if (ebo)
    {
        glDrawElements(t_drawMode, drawCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(t_drawMode, t_first, drawCount);
    }
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

void sg::ogl::buffer::Vao::CleanUp() const
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
