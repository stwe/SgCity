#include "Map.h"
#include "Tile.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Map::Map()
{
    Init();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Map::Init()
{
    for (auto z{ 0 }; z < 4; ++z)
    {
        for (auto x{ 0 }; x < 4; ++x)
        {
            auto tile{ std::make_unique<Tile>(
                static_cast<float>(x),
                static_cast<float>(z)
                ) };

            m_tiles.push_back(std::move(tile));
        }
    }

    m_vao = std::make_unique<ogl::buffer::Vao>();

    // todo: Vbo Größe in Bytes und Draw count ermitteln
    // 5 floats per vertex = 5 x 4 bytes = 20 bytes
    // 6 vertices = 20 x 6 = 120 bytes per Tile
    // 4x4 Tiles = 16 tiles = 16x120 bytes = 1920 bytes

    const auto& vbo{ m_vao->AddEmptyVbo(16 * 120, 16 * 6) };

    // add vertices to the VBO
    vbo.Bind();
    auto offset{ 0 };
    for (const auto& tile : m_tiles)
    {
        glBufferSubData(GL_ARRAY_BUFFER, offset * 120, 120, tile->vertices.data());
        offset++;
    }
    ogl::buffer::Vbo::Unbind();
}
