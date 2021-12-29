#pragma once

#include "Layer.h"

//-------------------------------------------------
// WaterLayer
//-------------------------------------------------

namespace sg::map
{
    class WaterLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        WaterLayer() = delete;
        explicit WaterLayer(int t_tileCount);

        WaterLayer(const WaterLayer& t_other) = delete;
        WaterLayer(WaterLayer&& t_other) noexcept = delete;
        WaterLayer& operator=(const WaterLayer& t_other) = delete;
        WaterLayer& operator=(WaterLayer&& t_other) noexcept = delete;

        ~WaterLayer() noexcept override;

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
