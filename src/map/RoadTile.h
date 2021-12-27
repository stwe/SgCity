#pragma once

#include <cstdint>
#include "Tile.h"

namespace sg::map
{
    class RoadTile : public Tile
    {
    public:
        /**
         * Possible Road Neighbours.
         * Each flag can set by using the OR operator.
         */
        enum RoadNeighbours : uint8_t
        {
            NORTH = 1,
            EAST = 2,
            SOUTH = 4,
            WEST = 8
        };

        /**
         * The value corresponds to the index in the texture atlas.
         */
        enum class RoadType
        {
            ROAD_V = 0,
            ROAD_H = 1,
            ROAD_C1 = 4,
            ROAD_T1 = 5,
            ROAD_C2 = 6,
            ROAD_T2 = 8,
            ROAD_X = 9,
            ROAD_T3 = 10,
            ROAD_C3 = 12,
            ROAD_T4 = 13,
            ROAD_C4 = 14,
        };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The orientation of the road.
         */
        RoadType roadType{ RoadType::ROAD_V };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RoadTile() = delete;

        RoadTile(float t_mapX, float t_mapZ);

        RoadTile(const RoadTile& t_other) = delete;
        RoadTile(RoadTile&& t_other) noexcept = delete;
        RoadTile& operator=(const RoadTile& t_other) = delete;
        RoadTile& operator=(RoadTile&& t_other) noexcept = delete;

        virtual ~RoadTile() noexcept;

    protected:

    private:

    };
}
