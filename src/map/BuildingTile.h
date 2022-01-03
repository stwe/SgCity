#pragma once

#include "Tile.h"

namespace sg::map
{
    class BuildingTile : public Tile
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        BuildingTile();

        BuildingTile(const BuildingTile& t_other) = delete;
        BuildingTile(BuildingTile&& t_other) noexcept = delete;
        BuildingTile& operator=(const BuildingTile& t_other) = delete;
        BuildingTile& operator=(BuildingTile&& t_other) noexcept = delete;

        ~BuildingTile() noexcept override;

    protected:

    private:

    };
}
