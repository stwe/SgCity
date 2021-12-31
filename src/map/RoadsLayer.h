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

        void CreateRoadTiles();

        /**
         * Stores vertices of all RoadTiles in a Vbo.
         */
        void RoadTilesToGpu();
    };
}