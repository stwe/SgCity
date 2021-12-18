#pragma once

#include <vector>
#include <glm/vec3.hpp>

namespace sg::map
{
    /**
     * Represents a Tile Object.
     */
    class Tile
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        static constexpr auto DEFAULT_HEIGHT{ 0.0f };

        static constexpr auto TL_1_Y{ 1 };
        static constexpr auto BL_1_Y{ 12 };
        static constexpr auto BR_1_Y{ 23 };

        static constexpr auto TL_2_Y{ 34 };
        static constexpr auto BR_2_Y{ 45 };
        static constexpr auto TR_2_Y{ 56 };

        static constexpr auto TL_1_N{ 8 };
        static constexpr auto BL_1_N{ 19 };
        static constexpr auto BR_1_N{ 30 };

        static constexpr auto TL_2_N{ 41 };
        static constexpr auto BR_2_N{ 52 };
        static constexpr auto TR_2_N{ 63 };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * Vertices of the Tile.
         */
        std::vector<float> vertices;

        /**
         * The top left x position of the Tile in local space.
         */
        float mapX{ 0.0f };

        /**
         * The top left z position of the Tile in local space.
         */
        float mapZ{ 0.0f };

        int tileCount{ 0 };

        /**
         * The unique color used for mouse picking.
         */
        glm::vec3 idColor{ glm::vec3(0.0f) };

        Tile* n{ nullptr };
        Tile* s{ nullptr };
        Tile* e{ nullptr };
        Tile* w{ nullptr };

        Tile* nw{ nullptr };
        Tile* ne{ nullptr };
        Tile* sw{ nullptr };
        Tile* se{ nullptr };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Tile() = delete;
        Tile(float t_mapX, float t_mapZ, int t_tileCount);

        Tile(const Tile& t_other) = delete;
        Tile(Tile&& t_other) noexcept = delete;
        Tile& operator=(const Tile& t_other) = delete;
        Tile& operator=(Tile&& t_other) noexcept = delete;

        ~Tile() noexcept;

    protected:

    private:
        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();
        void CreateObjectIdColor();
    };
}
