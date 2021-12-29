#pragma once

#include <vector>
#include "Map.h"

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

//-------------------------------------------------
// RoadNetwork
//-------------------------------------------------

namespace sg::map
{
    /**
     * Forward declaration class Tile.
     */
    class Tile;

    /**
     * Represents the RoadNetwork.
     */
    class RoadNetwork
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * The position of the RoadNetwork in world space.
         */
        static constexpr auto ROAD_NETWORK_POSITION{ glm::vec3(0.0f) };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        /**
         * Constructs a new RoadNetwork object.
         *
         * @param t_map The Map object.
         */
        explicit RoadNetwork(const Map& t_map);

        RoadNetwork(const RoadNetwork& t_other) = delete;
        RoadNetwork(RoadNetwork&& t_other) noexcept = delete;
        RoadNetwork& operator=(const RoadNetwork& t_other) = delete;
        RoadNetwork& operator=(RoadNetwork&& t_other) noexcept = delete;

        ~RoadNetwork() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        /**
         * Renders the RoadNetwork.
         *
         * @param t_window The Window object.
         * @param t_camera The Camera object.
         */
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * An array holding the Tile objects.
         */
        std::vector<std::unique_ptr<Tile>> m_tiles;

        /**
         * A Vao object which contains a Vbo with roads vertex data.
         */
        std::unique_ptr<ogl::buffer::Vao> m_roadsVao;

        /**
         * A ShaderProgram object used to draw the RoadNetwork.
         */
        std::unique_ptr<ogl::resource::ShaderProgram> m_roadsShaderProgram;

        /**
         * The roads texture atlas.
         */
        std::unique_ptr<ogl::resource::Texture> m_roadsTexture;

        /**
         * The model matrix of the RoadNetwork.
         */
        glm::mat4 m_roadsModelMatrix{ glm::mat4(1.0f) };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the RoadNetwork.
         */
        void Init(const sg::map::Map& t_map);

        /**
         * Create all Tiles.
         */
        void CreateTiles(const sg::map::Map& t_map);

        /**
         * Stores vertices of all Tiles in a Vbo.
         */
        void TilesToGpu();

        /**
         * Initializes Shader and Textures.
         */
        void InitResources();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
