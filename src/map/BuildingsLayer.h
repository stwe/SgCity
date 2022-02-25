#pragma once

#include "Layer.h"
#include "BuildingTile.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::resource
{
    class Model;
}

//-------------------------------------------------
// BuildingsLayer
//-------------------------------------------------

namespace sg::map
{
    class BuildingsLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        BuildingsLayer() = delete;
        BuildingsLayer(int t_tileCount, std::vector<std::shared_ptr<Tile>> t_tiles);

        BuildingsLayer(const BuildingsLayer& t_other) = delete;
        BuildingsLayer(BuildingsLayer&& t_other) noexcept = delete;
        BuildingsLayer& operator=(const BuildingsLayer& t_other) = delete;
        BuildingsLayer& operator=(BuildingsLayer&& t_other) noexcept = delete;

        ~BuildingsLayer() noexcept override;

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
         * A Model object.
         */
        std::shared_ptr<sg::ogl::resource::Model> m_model;

        /**
         * An array holding the BuildingTile objects.
         */
        std::vector<std::shared_ptr<BuildingTile>> m_buildingTiles;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the class.
         */
        void Init();

        /**
         * Create tiles.
         */
        void CreateBuildingTiles();

        /**
         * Stores vertices in a Vbo.
         */
        void BuildingTilesToGpu();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static std::unique_ptr<BuildingTile> CreateBuildingTile(const Tile& t_tile, int t_index);
    };
}
