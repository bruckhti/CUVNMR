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
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>

class PointName
{
public:
	PointName(size_t index, std::wstring name, glm::vec3 col, float radius);
	size_t getIndex();
	std::wstring getName();
	glm::vec3 getColor();
	void setColor(glm::vec3 val);
	float getRadius();
	void setRadius(float val);
private:
	size_t m_index;
	std::wstring m_name;
	glm::vec3 m_color;
	float m_radius;
};

struct Point 
{
	PointName* name;
	glm::vec3 pos;
	bool isSelected = false;
};

class PointConnection
{
public:
	PointConnection(size_t index, PointName* A, PointName* B, float thresh);
	size_t getIndex();
	PointName* getPointA();
	PointName* getPointB();
	float getThreshold();
	void setThreshold(float val);
private:
	size_t m_index;
	PointName* m_a;
	PointName* m_b;
	float m_threshold;
};