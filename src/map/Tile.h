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

        /**
         * Position (3 Floats), Uv (2 Floats), Id color (3 Floats), Normal (3 Floats)
         * -> 11 Floats x 4 Bytes per Float -> 44 Bytes per Vertex
         * -> 6 Vertices per Tile = 264 Bytes
         */
        static constexpr auto BYTES_PER_TILE{ 264 };

        /**
         * The default height of the Tile.
         */
        static constexpr auto DEFAULT_HEIGHT{ 0.0f };

        /*
            tl       tr
            +--------+
            |  +   2 |
            |    +   |
            | 1    + |
            +--------+
            bl       br

            1) tl, bl, br
            2) tl, br, tr
        */

        // Height value index of each vertex

        static constexpr auto TL_1_Y{ 1 };
        static constexpr auto BL_1_Y{ 12 };
        static constexpr auto BR_1_Y{ 23 };

        static constexpr auto TL_2_Y{ 34 };
        static constexpr auto BR_2_Y{ 45 };
        static constexpr auto TR_2_Y{ 56 };

        // Start index of normals

        static constexpr auto TL_1_N_START_INDEX{ 8 };
        static constexpr auto BL_1_N_START_INDEX{ 19 };
        static constexpr auto BR_1_N_START_INDEX{ 30 };

        static constexpr auto TL_2_N_START_INDEX{ 41 };
        static constexpr auto BR_2_N_START_INDEX{ 52 };
        static constexpr auto TR_2_N_START_INDEX{ 63 };

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

        /**
         * The index of this Tile in the Map array.
         */
        int mapIndex{ 0 };

        /**
         * A unique color used for mouse picking.
         */
        glm::vec3 idColor{ glm::vec3(0.0f) };

        // Neighbors

        // todo: -> shared_ptr statt raw pointer

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

        /**
         * Constructs a new Tile object.
         *
         * @param t_mapX The top left x position of the Tile in local space.
         * @param t_mapZ The top left z position of the Tile in local space.
         * @param t_mapIndex The index of this Tile in the Map array.
         */
        Tile(float t_mapX, float t_mapZ, int t_mapIndex);

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
        void InitObjectIdColor();
    };
}
