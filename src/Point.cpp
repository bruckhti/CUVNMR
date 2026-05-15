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

#include "pch.h"
#include "Point.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PointName::PointName(size_t index, std::wstring name, glm::vec3 col, float radius) :
	m_index(index),
	m_name(name),
	m_color(col),
	m_radius(radius)
{
}

size_t PointName::getIndex() 
{
	return m_index;
}

std::wstring PointName::getName() 
{
	return m_name;
}

glm::vec3 PointName::getColor() 
{
	return m_color;
}

void PointName::setColor(glm::vec3 val) 
{
	m_color = val;
}

float PointName::getRadius() 
{
	return m_radius;
}
void PointName::setRadius(float val)
{
	m_radius = val;
}

PointConnection::PointConnection(size_t index, PointName* A, PointName* B, float thresh) :
	m_index(index),
	m_a(A),
	m_b(B),
	m_threshold(thresh)
{
}

PointName* PointConnection::getPointA() 
{
	return m_a;
}

PointName* PointConnection::getPointB() 
{
	return m_b;
}

float PointConnection::getThreshold() 
{
	return m_threshold;
}

void PointConnection::setThreshold(float val) 
{
	m_threshold = val;
}

size_t PointConnection::getIndex() 
{
	return m_index;
}
