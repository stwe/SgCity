#pragma once

#include <glm/vec2.hpp>

namespace sg::iso
{
    class IsoUtil
    {
    public:
        static constexpr auto TILE_SIZE_WIDTH{ 64 };
        static constexpr auto TILE_SIZE_HEIGHT{ 32 };

        /*
        90 degrees: x = width - y - 1; y = x
        180 degrees: x = width - x - 1; y = height - y - 1
        270 degrees: x = y; y = height - x - 1
        */

        static glm::ivec2 ToScreen(int t_worldX, int t_worldY, int t_rotation = 0)
        {
            auto x{ t_worldX };
            auto y{ t_worldY };

            if (t_rotation == 90)
            {
                x = 12 - t_worldY - 1;
                y = t_worldX;
            }

            if (t_rotation == 180)
            {
                x = 12 - t_worldX - 1;
                y = 12 - t_worldY - 1;
            }

            if (t_rotation == 270)
            {
                x = t_worldY;
                y = 12 - t_worldX - 1;
            }

            return glm::ivec2(
                    // offset             + // to screen
                (6 * TILE_SIZE_WIDTH) + (x - y) * (TILE_SIZE_WIDTH / 2),
                (2 * TILE_SIZE_HEIGHT) + (x + y) * (TILE_SIZE_HEIGHT / 2)
                );
        }

    protected:

    private:
    };
}
