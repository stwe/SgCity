#include "Tile.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Tile::Tile(const float t_mapX, const float t_mapZ, const int t_mapIndex)
    : mapX{ t_mapX }
    , mapZ{ t_mapZ }
    , mapIndex{ t_mapIndex }
{
    Init();
}

sg::map::Tile::~Tile() noexcept
{
    // ...
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

    InitObjectIdColor();

    vertices =
    {
        // pos            // uv       // id color                      // normal
        tl.x, tl.y, tl.z, 0.0f, 1.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
        bl.x, bl.y, bl.z, 0.0f, 0.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
        br.x, br.y, br.z, 1.0f, 0.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,

        tl.x, tl.y, tl.z, 0.0f, 1.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
        br.x, br.y, br.z, 1.0f, 0.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
        tr.x, tr.y, tr.z, 1.0f, 1.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
    };
}

void sg::map::Tile::InitObjectIdColor()
{
    // convert index into an RGB color
    int r = (mapIndex & 0x000000FF) >> 0;
    int g = (mapIndex & 0x0000FF00) >> 8;
    int b = (mapIndex & 0x00FF0000) >> 16;

    idColor = glm::vec3(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f);
}
