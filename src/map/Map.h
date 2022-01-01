#pragma once

#include <memory>
#include "gui/MapEditGui.h"
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"

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

        void Update(gui::MapEditGui::Action t_action);
        void RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const;
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const;

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

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the Map.
         */
        void Init();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
