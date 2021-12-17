#include "Tile.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Tile::Tile(const float t_mapX, const float t_mapZ, int t_tileCount)
    : mapX{ t_mapX }
    , mapZ{ t_mapZ }
    , tileCount{ t_tileCount }
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

    CreateObjectIdColor();

    vertices =
    {
        // pos            // uv       // id color
        tl.x, tl.y, tl.z, 0.0f, 1.0f, idColor.x, idColor.y, idColor.z,
        bl.x, bl.y, bl.z, 0.0f, 0.0f, idColor.x, idColor.y, idColor.z,
        br.x, br.y, br.z, 1.0f, 0.0f, idColor.x, idColor.y, idColor.z,

        tl.x, tl.y, tl.z, 0.0f, 1.0f, idColor.x, idColor.y, idColor.z,
        br.x, br.y, br.z, 1.0f, 0.0f, idColor.x, idColor.y, idColor.z,
        tr.x, tr.y, tr.z, 1.0f, 1.0f, idColor.x, idColor.y, idColor.z,
    };
}

void sg::map::Tile::CreateObjectIdColor()
{
    auto i{ static_cast<int>(mapZ) * tileCount + static_cast<int>(mapX) };

    // convert "i", the integer mesh Id, into an RGB color
    int r = (i & 0x000000FF) >> 0;
    int g = (i & 0x0000FF00) >> 8;
    int b = (i & 0x00FF0000) >> 16;

    idColor = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
}
