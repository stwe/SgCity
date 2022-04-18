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

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
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
    SG_ASSERT(t_size, "[Vao::CreateEmptyDynamicVbo()] Invalid size given.")

    Bind();

    vbo = std::make_unique<Vbo>();
    vbo->Bind();

    glBufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(t_size), nullptr, GL_DYNAMIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    vbo->AddFloatAttribute(0, 3, 13, 0);

    // enable location 1 (uv)
    vbo->AddFloatAttribute(1, 2, 13, 3);

    // enable location 2 (idColor)
    vbo->AddFloatAttribute(2, 3, 13, 5);

    // enable location 3 (normal)
    vbo->AddFloatAttribute(3, 3, 13, 8);

    // enable location 4 (textureNr)
    vbo->AddFloatAttribute(4, 1, 13, 11);

    // enable location 5 (selected)
    vbo->AddFloatAttribute(5, 1, 13, 12);

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
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    drawCount = 6;
    constexpr int64_t floatsPerVertex{ 2 };

    glBufferData(GL_ARRAY_BUFFER, drawCount * floatsPerVertex * static_cast<int64_t>(sizeof(float)), vertices.data(), GL_STATIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    vbo->AddFloatAttribute(0, 2, 2, 0);

    Unbind();
}

void sg::ogl::buffer::Vao::CreateStaticSkyboxVbo()
{
    SG_ASSERT(!vbo, "[Vao::CreateStaticSkyboxVbo()] Vbo already exists.")

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

void sg::ogl::buffer::Vao::CreateStaticGuiVbo()
{
    SG_ASSERT(!vbo, "[Vao::CreateStaticGuiVbo()] Vbo already exists.")

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

void sg::ogl::buffer::Vao::CreateStaticModelVertexDataVbo(const std::vector<float>& t_vertices, const int32_t t_drawCount)
{
    SG_ASSERT(!vbo, "[Vao::CreateStaticModelVertexDataVbo()] Vbo already exists.")
    SG_ASSERT(!t_vertices.empty(), "[Vao::CreateStaticModelVertexDataVbo()] No vertices given.")
    SG_ASSERT(t_drawCount, "[Vao::CreateStaticModelVertexDataVbo()] Invalid draw count given.")

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

void sg::ogl::buffer::Vao::CreateStaticSphereVbos(const float t_radius, const int t_slices, const int t_stacks)
{
    SG_ASSERT(!ebo, "[Vao::CreateStaticSphereVbos()] Ebo already exists.")

    // the vbo member is not used here, but shouldn't exist anyway
    SG_ASSERT(!vbo, "[Vao::CreateStaticSphereVbos()] Vbo already exists.")

    constexpr auto pi{ 3.1415926535897932384626433832795f };
    constexpr auto _2pi{ 2.0f * pi };

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoords;

    // vertices
    for (auto i{ 0 }; i <= t_stacks; ++i)
    {
        auto v{ static_cast<float>(i) / static_cast<float>(t_stacks) };
        const auto phi{ v * pi };

        for (auto j{ 0 }; j <= t_slices; ++j)
        {
            auto u{ static_cast<float>(j) / static_cast<float>(t_slices) };
            const auto theta{ u * _2pi };

            const auto x{ cos(theta) * sin(phi) };
            const auto y{ cos(phi) };
            const auto z{ sin(theta) * sin(phi) };

            positions.push_back(glm::vec3(x, y, z) * t_radius);
            normals.emplace_back(x, y, z);
            textureCoords.emplace_back(u, v);
        }
    }

    // indices
    std::vector<uint32_t> indices;
    for (auto i{ 0 }; i < t_slices * t_stacks + t_slices; ++i)
    {
        indices.push_back(i);
        indices.push_back(i + t_slices + 1);
        indices.push_back(i + t_slices);

        indices.push_back(i + t_slices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    Bind();

    // create Vbos
    // todo
    uint32_t vbos[3];
    glGenBuffers(3, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(positions.size()) * static_cast<int64_t>(sizeof(glm::vec3)), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(normals.size()) * static_cast<int64_t>(sizeof(glm::vec3)), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, static_cast<void*>(nullptr));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(textureCoords.size()) * static_cast<int64_t>(sizeof(glm::vec2)), textureCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));
    glEnableVertexAttribArray(2);

    // create Ebo
    ebo = std::make_unique<Ebo>();
    ebo->Bind();
    constexpr auto elementSizeInBytes{ static_cast<int64_t>(sizeof(uint32_t)) };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int64_t>(indices.size()) * elementSizeInBytes, indices.data(), GL_STATIC_DRAW);

    Unbind();
    drawCount = static_cast<int32_t>(indices.size());
}

//-------------------------------------------------
// Create Ebo
//-------------------------------------------------

void sg::ogl::buffer::Vao::CreateModelIndexBuffer(const std::vector<uint32_t>& t_indices)
{
    SG_ASSERT(!ebo, "[Vao::CreateModelIndexBuffer()] Ebo already exists.")
    SG_ASSERT(vbo, "[Vao::CreateModelIndexBuffer()] The Ebo should be created after the Vbo.")
    SG_ASSERT(!t_indices.empty(), "[Vao::CreateModelIndexBuffer()] No indices given.")

    constexpr auto elementSizeInBytes{ static_cast<int64_t>(sizeof(uint32_t)) };

    Bind();

    ebo = std::make_unique<Ebo>();
    ebo->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int64_t>(t_indices.size()) * elementSizeInBytes, t_indices.data(), GL_STATIC_DRAW);

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
