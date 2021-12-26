#pragma once

#include <vector>
#include <memory>
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::buffer
{
    class Vao;
}

namespace sg::ogl::resource
{
    class ShaderProgram;
    class Texture;
}

namespace sg::ogl::input
{
    class PickingTexture;
}

//-------------------------------------------------
// Map
//-------------------------------------------------

namespace sg::map
{
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
         * The position of the Map in world space.
         */
        static constexpr auto MAP_POSITION{ glm::vec3(0.0f) };

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

        /**
         * Renders the Map in a Fbo.
         *
         * @param t_window The Window object.
         * @param t_camera The Camera object.
         */
        void RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);

        /**
         * Renders the Map.
         *
         * @param t_window The Window object.
         * @param t_camera The Camera object.
         */
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const;

        //-------------------------------------------------
        // Tiles
        //-------------------------------------------------

        /**
         * Get the mapIndex of the Tile under current mouse position.
         *
         * @return The mapIndex.
         */
        int GetCurrentTileIdxUnderMouse();

        //-------------------------------------------------
        // Raise / lower terrain
        //-------------------------------------------------

        /**
         * Raise and lower the terrain.
         *
         * @param t_mapIndex The map index of the Tile to be raised/lowered.
         * @param t_raise True if raise.
         */
        void HandleTileUpdate(int t_mapIndex, bool t_raise);

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
         * An array holding the Tile objects.
         */
        std::vector<std::unique_ptr<Tile>> m_tiles;

        /**
         * A Vao object which contains a Vbo with map vertex data.
         */
        std::unique_ptr<ogl::buffer::Vao> m_mapVao;

        /**
         * A ShaderProgram object used to draw the Map.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_mapShaderProgram;

        /**
         * A ShaderProgram object used to draw the Map for mouse picking.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_pickingShaderProgram;

        /**
         * An object holding the Fbo for mouse picking.
         */
        std::unique_ptr<ogl::input::PickingTexture> m_pickingTexture;

        /**
         * The texture for each Tile.
         */
        std::unique_ptr<ogl::resource::Texture> m_tileTexture;

        /**
         * The model matrix of the Map.
         */
        glm::mat4 m_mapModelMatrix{ glm::mat4(1.0f) };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the Map.
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

        /**
         * Initializes Shader and Textures.
         */
        void InitResources();

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

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
