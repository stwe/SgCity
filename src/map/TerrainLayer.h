#pragma once

#include "Layer.h"
#include "gui/MapEditGui.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::resource
{
    class Texture;
}

namespace sg::ogl::input
{
    class PickingTexture;
}

//-------------------------------------------------
// TerrainLayer
//-------------------------------------------------

namespace sg::map
{
    class TerrainLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TerrainLayer() = delete;
        explicit TerrainLayer(int t_tileCount);

        TerrainLayer(const TerrainLayer& t_other) = delete;
        TerrainLayer(TerrainLayer&& t_other) noexcept = delete;
        TerrainLayer& operator=(const TerrainLayer& t_other) = delete;
        TerrainLayer& operator=(TerrainLayer&& t_other) noexcept = delete;

        ~TerrainLayer() noexcept override;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        int Update(gui::MapEditGui::Action t_action);
        void RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const override;

        //-------------------------------------------------
        // Mouse picking
        //-------------------------------------------------

        /**
         * Get the mapIndex of the Tile under current mouse position.
         *
         * @return The mapIndex.
         */
        [[nodiscard]] int GetCurrentTileIdxUnderMouse() const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The texture for each Tile.
         */
        std::unique_ptr<ogl::resource::Texture> m_tileTexture;

        /**
         * A ShaderProgram object used to draw the TerrainLayer for mouse picking.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_pickingShaderProgram;

        /**
         * An object holding the Fbo for mouse picking.
         */
        std::unique_ptr<ogl::input::PickingTexture> m_pickingTexture;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the class.
         */
        void Init();

        /**
         * Create all Tiles.
         */
        void CreateTiles();

        /**
         * Finds the neighbors for every Tile.
         */
        void AddTileNeighbors();

        /**
         * Stores vertices of all Tiles in a Vbo.
         */
        void TilesToGpu();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void UpdateNorthNeighbor(Tile& t_tile);
        void UpdateSouthNeighbor(Tile& t_tile);
        void UpdateWestNeighbor(Tile& t_tile);
        void UpdateEastNeighbor(Tile& t_tile);

        void UpdateNorthWestNeighbor(Tile& t_tile);
        void UpdateNorthEastNeighbor(Tile& t_tile);
        void UpdateSouthWestNeighbor(Tile& t_tile);
        void UpdateSouthEastNeighbor(Tile& t_tile);
    };
}
