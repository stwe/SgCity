#pragma once

#include "Layer.h"
#include "RoadTile.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::resource
{
    class Texture;
}

namespace sg::gui
{
    enum class Action;
}

//-------------------------------------------------
// RoadsLayer
//-------------------------------------------------

namespace sg::map
{
    class RoadsLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RoadsLayer() = delete;
        RoadsLayer(int t_tileCount, std::vector<std::shared_ptr<Tile>> t_tiles);

        RoadsLayer(const RoadsLayer& t_other) = delete;
        RoadsLayer(RoadsLayer&& t_other) noexcept = delete;
        RoadsLayer& operator=(const RoadsLayer& t_other) = delete;
        RoadsLayer& operator=(RoadsLayer&& t_other) noexcept = delete;

        ~RoadsLayer() noexcept override;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update(gui::Action t_action, int t_tileIndex);
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const override;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The roads texture atlas.
         */
        std::unique_ptr<ogl::resource::Texture> m_roadsTexture;

        /**
         * An array holding the RoadTile objects.
         */
        std::vector<std::shared_ptr<RoadTile>> m_roadTiles;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the class.
         */
        void Init();

        /**
         * Create all RoadTiles.
         */
        void CreateRoadTiles();

        /**
         * Stores vertices of all RoadTiles in a Vbo.
         */
        void RoadTilesToGpu();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static std::unique_ptr<RoadTile> CreateRoadTile(const Tile& t_tile, int t_index);

        static void UpdateTexture(RoadTile& t_roadTile);

        static bool CheckTerrainForRoad(const Tile& t_tile);
    };
}
