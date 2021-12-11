#include "Tile.h"
#include <glm/vec3.hpp>

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Tile::Tile(const float t_mapX, const float t_mapZ)
    : m_mapX{ t_mapX }
    , m_mapZ{ t_mapZ }
{
    Init();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Tile::Init()
{
    const glm::vec3 tl{ m_mapX, DEFAULT_HEIGHT, m_mapZ };
    const glm::vec3 bl{ m_mapX, DEFAULT_HEIGHT, m_mapZ + 1 };
    const glm::vec3 br{ m_mapX + 1, DEFAULT_HEIGHT, m_mapZ + 1 };
    const glm::vec3 tr{ m_mapX + 1, DEFAULT_HEIGHT, m_mapZ };

    vertices =
    {
        // pos            // uv
        tl.x, tl.y, tl.z, 0.0f, 1.0f,
        bl.x, bl.y, bl.z, 0.0f, 0.0f,
        br.x, br.y, br.z, 1.0f, 0.0f,

        tl.x, tl.y, tl.z, 0.0f, 1.0f,
        br.x, br.y, br.z, 1.0f, 0.0f,
        tr.x, tr.y, tr.z, 1.0f, 1.0f
    };
}
