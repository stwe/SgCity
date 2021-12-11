#pragma once

#include <vector>

namespace sg::map
{
    class Tile
    {
    public:
        std::vector<float> vertices;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Tile() = delete;
        Tile(float t_mapX, float t_mapZ);

        Tile(const Tile& t_other) = delete;
        Tile(Tile&& t_other) noexcept = delete;
        Tile& operator=(const Tile& t_other) = delete;
        Tile& operator=(Tile&& t_other) noexcept = delete;

        ~Tile() noexcept = default;

    protected:

    private:
        static constexpr auto DEFAULT_HEIGHT{ 0.0f };

        float m_mapX{ 0.0f };
        float m_mapZ{ 0.0f };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();
    };
}
