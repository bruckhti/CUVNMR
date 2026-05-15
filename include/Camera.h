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
#include <GL/glew.h>
#include <GL/wglew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Ray.h"

class Camera
{
public:
	Camera();
	void attach_shader(GLuint shader_id);
	void init();
	void setup();
	void resize(int w, int h);
	Ray Camera::transform(int px, int py, int w, int h);
	glm::vec3 getPos();
	void setPos(glm::vec3 pos);
	void setYaw(float ang);
	void setPitch(float ang);
	void setRoll(float ang);
	float getYaw();
	float getPitch();
	float getRoll();
	void updateOrientation(glm::vec2 end, glm::vec2 start);
	void updatePosition(glm::vec2 end, glm::vec2 start, float radius);
	void moveForwards(float stepSize);
	float Camera::CalculateDisplacementAlongX(glm::vec2 end, glm::vec2 start);
	float Camera::CalculateDisplacementAlongY(glm::vec2 end, glm::vec2 start);
private:
	GLuint m_shader_id;
	GLuint m_projection_id;
	glm::mat4 m_projection;
	float m_near;
	float m_far;
	float m_fov;
	float m_aspect_ratio;
	GLuint m_view_id;
	glm::mat4 m_view;
	glm::vec3 m_cam_pos;
	float m_yaw;
	float m_pitch;
	float m_roll;
	glm::vec3 m_dir;
	glm::vec3 m_up;
	glm::vec3 m_left;
	long m_width;
	long m_height;
	void updateView();
	void updateOrientation();
	void moveUpwards(float stepSize);
	void moveLeftwards(float stepSize);
};

