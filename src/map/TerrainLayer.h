#pragma once

#include "Layer.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::input
{
    class PickingTexture;
}

namespace sg::gui
{
    enum class Action;
}

//-------------------------------------------------
// TerrainLayer
//-------------------------------------------------

namespace sg::map
{
    /**
     * Represents the TerrainLayer.
     */
    class TerrainLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TerrainLayer() = delete;

        /**
         * Constructs a new TerrainLayer object.
         *
         * @param t_tileCount The number of tiles in x and z direction.
         */
        explicit TerrainLayer(int t_tileCount);

        TerrainLayer(const TerrainLayer& t_other) = delete;
        TerrainLayer(TerrainLayer&& t_other) noexcept = delete;
        TerrainLayer& operator=(const TerrainLayer& t_other) = delete;
        TerrainLayer& operator=(TerrainLayer&& t_other) noexcept = delete;

        ~TerrainLayer() noexcept override;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] const auto& GetPickingTexture() const { return *m_pickingTexture; }

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input();
        void Update(gui::Action t_action, int t_currentTileIndex);
        void RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const override;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

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
