#pragma once

#include <memory>
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::gui
{
    enum class Action;
}

//-------------------------------------------------
// Map
//-------------------------------------------------

namespace sg::map
{
    /**
     * Forward declaration class WaterLayer.
     */
    class WaterLayer;

    /**
     * Forward declaration class TerrainLayer.
     */
    class TerrainLayer;

    /**
     * Forward declaration class RoadsLayer.
     */
    class RoadsLayer;

    /**
     * Forward declaration class Tile.
     */
    class Tile;

    /**
     * Represents the Map.
     */
    class Map
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * Value used for an invalid tile index.
         */
        static constexpr auto INVALID_TILE_INDEX{ -1 };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * Index in std::vector<std::shared_ptr<Tile>> of the current picked Tile.
         */
        int currentTileIndex{ INVALID_TILE_INDEX };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Map() = delete;

        /**
         * Constructs a new Map object.
         *
         * @param t_tileCount The number of tiles in x and z direction.
         */
        explicit Map(int t_tileCount);

        Map(const Map& t_other) = delete;
        Map(Map&& t_other) noexcept = delete;
        Map& operator=(const Map& t_other) = delete;
        Map& operator=(Map&& t_other) noexcept = delete;

        ~Map() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input();
        void Update(gui::Action t_action);
        void RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const;
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const;
        void RenderImGui() const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /*
         * The number of tiles in x and z direction.
         */
        int m_tileCount{ 0 };

        /**
         * The water surface.
         */
        std::unique_ptr<WaterLayer> m_waterLayer;

        /**
         * The Terrain Layer.
         */
        std::unique_ptr<TerrainLayer> m_terrainLayer;

        /**
         * Roads as a new Layer.
         */
        std::unique_ptr<RoadsLayer> m_roadsLayer;

        /**
         * Current Tile object.
         */
        std::shared_ptr<map::Tile> m_currentTile;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the Map.
         */
        void Init();

        //-------------------------------------------------
        // Mouse picking
        //-------------------------------------------------

        void UpdateCurrentTileIndex();
    };
}
