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
#include "Camera.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Camera::Camera() 
{
    m_near = 0.01f;
    m_far = 100;
    m_fov = 25;
    m_yaw = 0;
    m_pitch = 0;
    m_roll = 0;
    m_cam_pos = glm::vec3(0, 0, 12);
    m_height = 600;
    m_width = 600;
    updateOrientation();
}

void Camera::attach_shader(GLuint shader_id) 
{
    m_shader_id = shader_id;
}

void Camera::init() 
{
    m_projection_id = glGetUniformLocation(m_shader_id, "projection");
    m_view_id = glGetUniformLocation(m_shader_id, "view");
    m_view = glm::lookAt(m_cam_pos,m_cam_pos + glm::vec3(0, 0, -1),glm::vec3(0, 1, 0));
}

void Camera::setup() 
{
    glUniformMatrix4fv(m_projection_id, 1, GL_FALSE, glm::value_ptr(m_projection));
    updateOrientation();
    glUniformMatrix4fv(m_view_id, 1, GL_FALSE, glm::value_ptr(m_view));
}

void  Camera::resize(int w, int h) 
{
    m_aspect_ratio = 1.0f;
    if ((w > 0) && (h > 0)) {
        m_aspect_ratio = (w * 1.0f) / h;
        m_height = h;
        m_width = w;
    }
    m_projection = glm::perspective(glm::radians(2 * m_fov), m_aspect_ratio, m_near, m_far);
}

Ray Camera::transform(int px, int py, int w, int h) 
{
    float x = (2.0f * px) / w - 1;
    float y = 1 - (2.0f * py) / h;
    // Ref: https://gamedev.stackexchange.com/questions/12360/how-do-you-determine-which-object-surface-the-users-pointing-at-with-lwjgl/12370#12370
    double viewRatio = glm::tan((3.1428 / (180.f / m_fov)));
    float sX = x * viewRatio * m_aspect_ratio;
    float sY = y * viewRatio;
    glm::vec4 cameraSpaceNear = glm::vec4(sX * m_near, sY * m_near, -m_near, 1);
    glm::vec4 cameraSpaceFar = glm::vec4(sX * m_far, sY * m_far, -m_far, 1);
    glm::mat4 invView = glm::inverse(m_view);
    glm::vec4 worldSpaceNear = invView * cameraSpaceNear;
    glm::vec4 worldSpaceFar = invView * cameraSpaceFar;
    glm::vec3 rayPosition(worldSpaceNear.x, worldSpaceNear.y, worldSpaceNear.z);
    glm::vec3 rayDirection(worldSpaceFar.x - worldSpaceNear.x,
        worldSpaceFar.y - worldSpaceNear.y,
        worldSpaceFar.z - worldSpaceNear.z);
    rayDirection = glm::normalize(rayDirection);
    return Ray(rayPosition, rayDirection);
}

void Camera::setYaw(float ang) 
{
    m_yaw = glm::radians(ang);
    updateOrientation();
}

void Camera::setPitch(float ang) 
{
    m_pitch = glm::radians(ang);
    updateOrientation();
}

void Camera::setRoll(float ang) 
{
    m_roll = glm::radians(ang);
    updateOrientation();
}

float Camera::getYaw() 
{
    return glm::degrees(m_yaw);
}

float Camera::getPitch() 
{
    return glm::degrees(m_pitch);
}

float Camera::getRoll() 
{
    return glm::degrees(m_roll);
}

void Camera::updateOrientation() 
{
    glm::mat4 tranform = glm::mat4(1.0f);
    tranform = glm::rotate(tranform, m_roll, glm::vec3(0.0f, 1.0f, 0.0f));
    tranform = glm::rotate(tranform, m_yaw, glm::vec3(0.0f, 0.0f, 1.0f));
    tranform = glm::rotate(tranform, m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec4 result = tranform * glm::vec4(0, 0, -1, 1);
    m_dir.x = result.x;
    m_dir.y = result.y;
    m_dir.z = result.z;
    result = tranform * glm::vec4(0, 1, 0, 1);
    m_up.x = result.x;
    m_up.y = result.y;
    m_up.z = result.z;
    result = tranform * glm::vec4(1, 0, 0, 1);
    m_left.x = result.x;
    m_left.y = result.y;
    m_left.z = result.z;
    updateView();
}

void Camera::updateView() 
{
    m_view = glm::lookAt(m_cam_pos,m_cam_pos + m_dir,m_up);
}

float Camera::CalculateDisplacementAlongX(glm::vec2 end, glm::vec2 start)
{
    float dx = end.x - start.x;
    dx = dx / (m_width * 1.0f);
    dx = dx * m_fov;
    dx = 2 * dx * m_aspect_ratio;
    return dx;
}

float Camera::CalculateDisplacementAlongY(glm::vec2 end, glm::vec2 start)
{
    float dy = end.y - start.y;
    dy = dy / (m_height * 1.0f);
    dy = 2 * dy * m_fov;
    return dy;
}

void Camera::updateOrientation(glm::vec2 end, glm::vec2 start) 
{
    float dx = end.x - start.x;
    dx = dx / (m_width * 1.0f);
    dx = dx * m_fov;
    dx = 2 * dx * m_aspect_ratio;
    float dy = end.y - start.y;
    dy = dy / (m_height * 1.0f);
    dy = 2 * dy * m_fov;
    m_roll += glm::radians(dx);
    m_pitch += glm::radians(dy);
    updateOrientation();
}

glm::vec3 Camera::getPos() 
{
    return m_cam_pos;
}

void Camera::setPos(glm::vec3 pos) 
{
    m_cam_pos = pos;
    updateOrientation();
}

void Camera::updatePosition(glm::vec2 end, glm::vec2 start, float radius) 
{
    glm::vec2 delta = end - start;
    moveUpwards(2 * radius * delta.y / m_height);
    moveLeftwards(2 * radius * delta.x / m_height);
}

void Camera::moveUpwards(float stepSize) 
{
    m_cam_pos = m_cam_pos + stepSize * m_up;
    updateView();
}

void Camera::moveLeftwards(float stepSize) 
{
    m_cam_pos = m_cam_pos - stepSize * m_left;
    updateView();
}

void Camera::moveForwards(float stepSize) 
{
    if (m_cam_pos.z >= 1.3)
    {
        m_cam_pos = m_cam_pos + stepSize * m_dir;
    }
    if (m_cam_pos.z < 1.3)
    {
        m_cam_pos.z = 1.31;
    }
    updateView();
}
