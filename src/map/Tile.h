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
         * The unique color used for mouse picking.
         */
        glm::vec3 idColor{ glm::vec3(0.0f) };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Tile() = delete;
        Tile(float t_mapX, float t_mapZ);

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
