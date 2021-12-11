#pragma once

#include <vector>
#include <memory>

namespace sg::ogl::buffer
{
    class Vao;
}

namespace sg::map
{
    class Tile;

    class Map
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Map();

        Map(const Map& t_other) = delete;
        Map(Map&& t_other) noexcept = delete;
        Map& operator=(const Map& t_other) = delete;
        Map& operator=(Map&& t_other) noexcept = delete;

        ~Map() noexcept = default;

    protected:

    private:
        std::vector<std::unique_ptr<Tile>> m_tiles;
        std::unique_ptr<ogl::buffer::Vao> m_vao;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();
    };
}
