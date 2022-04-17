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

        [[nodiscard]] virtual bool IsOnFrustum(const Frustum& t_cameraFrustum, const glm::vec3& t_position) const = 0;
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

    struct Sphere : BoundingVolume
    {
        glm::vec3 center{ 0.0f, 0.0f, 0.0f };
        float radius{ 0.0f };

        Sphere() = default;

        Sphere(const glm::vec3& t_inCenter, const float t_inRadius)
            : BoundingVolume{}, center{ t_inCenter }
            , radius{ t_inRadius }
        {}

        [[nodiscard]] bool IsOnOrForwardPlan(const Plan& t_plan) const override
        {
            return t_plan.GetSignedDistanceToPlan(center) > -radius;
        }

        [[nodiscard]] bool IsOnFrustum(const Frustum& t_cameraFrustum, const glm::vec3& t_position) const override
        {
            // Get global scale thanks to our transform
            // const glm::vec3 globalScale = t_transform.getGlobalScale();
            const auto globalScale{ glm::vec3(1.0f) };

            // Get our global center with process it with the global model matrix of our transform
            const glm::vec3 globalCenter{ math::Transform::CreateModelMatrix(t_position, glm::vec3(0.0f), glm::vec3(1.0f)) * glm::vec4(center, 1.0f) };

            // To wrap correctly our shape, we need the maximum scale scalar.
            const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

            // Max scale is assuming for the diameter. So, we need the half to apply it to our radius
            const Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

            // Check Firstly the result that have the most chance to faillure to avoid to call all functions.
            return (globalSphere.IsOnOrForwardPlan(t_cameraFrustum.leftFace) &&
                globalSphere.IsOnOrForwardPlan(t_cameraFrustum.rightFace) &&
                globalSphere.IsOnOrForwardPlan(t_cameraFrustum.farFace) &&
                globalSphere.IsOnOrForwardPlan(t_cameraFrustum.nearFace) &&
                globalSphere.IsOnOrForwardPlan(t_cameraFrustum.topFace) &&
                globalSphere.IsOnOrForwardPlan(t_cameraFrustum.bottomFace));
        }
    };
}
