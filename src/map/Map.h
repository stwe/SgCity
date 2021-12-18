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

        void RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);

        int GetTileObjectId();

        void Raise(int t_tileObjectId);

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
         *
         */
        uint32_t m_vboId{ 0 };

        /**
         * A ShaderProgram object used to draw the Map.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_mapShaderProgram;

        /**
         * A ShaderProgram object used for mouse picking.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_pickingShaderProgram;

        /**
         *
         */
        std::unique_ptr<ogl::input::PickingTexture> m_pickingTexture;

        std::unique_ptr<ogl::resource::Texture> m_tileTexture;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        glm::vec3 CalcNormal(Tile& t_tile);

        void UpdateTile(Tile& t_tile);

        void UpdateNorthNeighbor(Tile& t_tile);
        void UpdateSouthNeighbor(Tile& t_tile);
        void UpdateWestNeighbor(Tile& t_tile);
        void UpdateEastNeighbor(Tile& t_tile);

        void UpdateNorthWestNeighbor(Tile& t_tile);
        void UpdateNorthEastNeighbor(Tile& t_tile);

        void UpdateSouthWestNeighbor(Tile& t_tile);
        void UpdateSouthEastNeighbor(Tile& t_tile);

        void UpdateVertices(const std::vector<float>& t_vertices, int t_offset) const;

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
