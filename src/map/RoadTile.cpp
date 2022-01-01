#include "RoadTile.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::RoadTile::RoadTile()
{
    // ...
}

sg::map::RoadTile::~RoadTile() noexcept
{
    // ...
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

bool sg::map::RoadTile::DetermineRoadType()
{
    uint8_t roadNeighbours{ 0 };

    if (n && n->type == TileType::TRAFFIC)
    {
        roadNeighbours = NORTH;
    }

    if (e && e->type == TileType::TRAFFIC)
    {
        roadNeighbours |= EAST;
    }

    if (s && s->type == TileType::TRAFFIC)
    {
        roadNeighbours |= SOUTH;
    }

    if (w && w->type == TileType::TRAFFIC)
    {
        roadNeighbours |= WEST;
    }

    RoadType newRoadType;
    switch (roadNeighbours)
    {
        case 0:                                   // keine Nachbarn
        case 1: newRoadType = RoadType::ROAD_V;   // Norden
            break;
        case 2: newRoadType = RoadType::ROAD_H;   // Osten
            break;
        case 3: newRoadType = RoadType::ROAD_C3;  // Norden - Osten
            break;
        case 4:                                   // Sueden
        case 5: newRoadType = RoadType::ROAD_V;   // Sueden - Norden
            break;
        case 6: newRoadType = RoadType::ROAD_C1;  // Sueden - Osten
            break;
        case 7: newRoadType = RoadType::ROAD_T2;  // Norden - Osten - Sueden
            break;
        case 8: newRoadType = RoadType::ROAD_H;   // Westen
            break;
        case 9: newRoadType = RoadType::ROAD_C4;  // Westen - Norden
            break;
        case 10: newRoadType = RoadType::ROAD_H;  // Westen - Osten
            break;
        case 11: newRoadType = RoadType::ROAD_T4; // Westen - Osten - Norden
            break;
        case 12: newRoadType = RoadType::ROAD_C2; // Westen - Sueden
            break;
        case 13: newRoadType = RoadType::ROAD_T3; // Westen - Sueden - Norden
            break;
        case 14: newRoadType = RoadType::ROAD_T1; // Westen - Sueden - Osten
            break;
        case 15: newRoadType = RoadType::ROAD_X;
            break;
        default: newRoadType = RoadType::ROAD_V;
    }

    const auto oldRoadType{ roadType };
    roadType = newRoadType;

    return oldRoadType != newRoadType;
}
