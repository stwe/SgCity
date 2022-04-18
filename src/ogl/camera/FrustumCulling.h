// This file is part of the SgCity project.
//
// Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include "ogl/math/Transform.h"

//-------------------------------------------------
// FrustumCulling
//-------------------------------------------------

namespace sg::ogl::camera
{
    /**
     * Represent a plan.
     */
    struct Plan
    {
        /**
         * A normal vector.
         */
        glm::vec3 normal{ 0.0f, 1.0f, 0.0f };

        /**
         * Distance from origin to the nearest point in the plan.
         */
        float distance{0.0f};

        Plan() = default;

        Plan(const glm::vec3& t_p1, const glm::vec3& t_normal)
            : normal{ glm::normalize(t_normal) }
            , distance{ glm::dot(normal, t_p1) }
        {}

        [[nodiscard]] float GetSignedDistanceToPlan(const glm::vec3& t_point) const
        {
            return glm::dot(normal, t_point) - distance;
        }
    };

    /**
     * Represent a frustum.
     */
    struct Frustum
    {
        Plan topFace;
        Plan bottomFace;

        Plan rightFace;
        Plan leftFace;

        Plan farFace;
        Plan nearFace;
    };

    struct BoundingVolume
    {
        virtual ~BoundingVolume() noexcept = default;

        [[nodiscard]] virtual bool IsOnFrustum(
            const Frustum& t_cameraFrustum,
            const glm::vec3& t_position,
            const glm::vec3& t_rotation = glm::vec3(0.0f),
            const glm::vec3& t_scale = glm::vec3(1.0f)
        ) const = 0;

        [[nodiscard]] virtual bool IsOnOrForwardPlan(const Plan& t_plan) const = 0;

        [[nodiscard]] bool IsOnFrustum(const Frustum& t_cameraFrustum) const
        {
            return (IsOnOrForwardPlan(t_cameraFrustum.leftFace) &&
                IsOnOrForwardPlan(t_cameraFrustum.rightFace) &&
                IsOnOrForwardPlan(t_cameraFrustum.topFace) &&
                IsOnOrForwardPlan(t_cameraFrustum.bottomFace) &&
                IsOnOrForwardPlan(t_cameraFrustum.nearFace) &&
                IsOnOrForwardPlan(t_cameraFrustum.farFace));
        }
    };

    struct SphereVolume : BoundingVolume
    {
        glm::vec3 center{ 0.0f, 0.0f, 0.0f };
        float radius{ 0.0f };

        SphereVolume() = default;

        SphereVolume(const glm::vec3& t_inCenter, const float t_inRadius)
            : BoundingVolume{}, center{ t_inCenter }
            , radius{ t_inRadius }
        {}

        [[nodiscard]] bool IsOnOrForwardPlan(const Plan& t_plan) const override
        {
            return t_plan.GetSignedDistanceToPlan(center) > -radius;
        }

        [[nodiscard]] bool IsOnFrustum(
            const Frustum& t_cameraFrustum,
            const glm::vec3& t_position,
            const glm::vec3& t_rotation = glm::vec3(0.0f),
            const glm::vec3& t_scale = glm::vec3(1.0f)
        ) const override
        {
            const glm::vec3 transformMatrix{ math::Transform::CreateModelMatrix(
                t_position, t_rotation, t_scale) * glm::vec4(center, 1.0f)
            };

            const SphereVolume volume(transformMatrix, radius * 0.5f);

            return (volume.IsOnOrForwardPlan(t_cameraFrustum.leftFace) &&
                volume.IsOnOrForwardPlan(t_cameraFrustum.rightFace) &&
                volume.IsOnOrForwardPlan(t_cameraFrustum.farFace) &&
                volume.IsOnOrForwardPlan(t_cameraFrustum.nearFace) &&
                volume.IsOnOrForwardPlan(t_cameraFrustum.topFace) &&
                volume.IsOnOrForwardPlan(t_cameraFrustum.bottomFace));
        }
    };
}
