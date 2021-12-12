#include "Tile.h"
#include <glm/vec3.hpp>

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Tile::Tile(const float t_mapX, const float t_mapZ)
    : mapX{ t_mapX }
    , mapZ{ t_mapZ }
{
    Init();
}

sg::map::Tile::~Tile() noexcept
{

}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Tile::Init()
{
    const glm::vec3 tl{ mapX, DEFAULT_HEIGHT, mapZ };
    const glm::vec3 bl{ mapX, DEFAULT_HEIGHT, mapZ + 1 };
    const glm::vec3 br{ mapX + 1, DEFAULT_HEIGHT, mapZ + 1 };
    const glm::vec3 tr{ mapX + 1, DEFAULT_HEIGHT, mapZ };

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
