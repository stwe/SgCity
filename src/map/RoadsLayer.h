#pragma once

#include "Layer.h"

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
        explicit RoadsLayer(std::vector<std::shared_ptr<Tile>> t_tiles);

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
        // Init
        //-------------------------------------------------

        void Init();
    };
}
