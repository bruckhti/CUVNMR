/*
* Copyright (c) 2026 Tim Bruckhoff / Affiliation: Ruprecht-Karls-University Heidelberg, Germany
*
* This source code is licensed under the EUPL v1.2.
* See the LICENSE file in the repository root directory for full details.
*
* CITATION:
* If you use this software for your research, please cite it as
* described in the CITATION.cff file located in the root directory.
*
* SPDX-License-Identifier: EUPL-1.2
*/

#pragma once
#include <iostream>
#include <array>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Ray
{
public:
    Ray() = default;
    Ray(const glm::vec3& origin, const glm::vec3& direction)
    {
        data[0] = origin; data[1] = direction;
    }
    Ray& setRay(const glm::vec3& origin, const glm::vec3& direction)
    {
        data[0] = origin; data[1] = direction; return (*this);
    }
    Ray& setOrigin(const glm::vec3& origin) { data[0] = origin; return (*this); }
    Ray& setDirection(const glm::vec3& direction) { data[1] = direction; return (*this); }
    [[nodiscard]] glm::vec3 origin() const { return data[0]; }
    [[nodiscard]] glm::vec3 direction() const { return data[1]; }
    [[nodiscard]] glm::vec3  pointAtParam(float t) const { return data[0] + t * data[1]; }
    Ray& makeUnitDirection()
    {
        data[1] = glm::normalize(data[1]);
        return (*this);
    }
    Ray& moveOriginForward(float delta_t)
    {
        data[0] += delta_t * data[1];
        return (*this);
    }
    float pointDistance(glm::vec3 pnt) 
	{
        glm::vec3 u = pnt - origin();
        glm::vec3 puv = glm::dot(direction(), u) * direction();
        glm::vec3 qp = origin() + puv;
        return glm::distance(pnt, qp);
    }
private:
    std::array<glm::vec3, 2> data;
};
