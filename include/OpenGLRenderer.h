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
#define GLEW_STATIC
#pragma comment(lib, "opengl32.lib")
#include <GL/glew.h>
#include <GL/wglew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <vector>
#include "Point.h"
#include "Camera.h"
#include "DataBase.h"

class OpenGlRenderer : public CWnd
{
	DECLARE_DYNAMIC(OpenGlRenderer)
public:
	OpenGlRenderer();
	virtual ~OpenGlRenderer();
	bool CreateGLContext(CRect rect, LPCWSTR classNameStr, CWnd* parent);
	void PrepareScene();
	void Reshape(UINT nType, int w, int h);
	void DrawScene();
	void DestroyScene();
	void SetViewPortSize(int ViewPortWidth, int ViewPortHeight);
	CRect& GetWinRect() { return m_WindowOrig; };
	int& GetViewPortWidth() { return m_iViewPortWidth; };
	int& GetViewPortHeight() { return m_iViewPortHeight; };
	void attach_data_base(DataBase* data_base);
	void setPrimary();
	void setList(CListCtrl* list_main);
private:
	float m_epsilon;
	int	m_iViewPortWidth, m_iViewPortHeight;
	HDC m_hdc;
	bool m_r_mouse_button_down;
	float m_r_mouse_start_radius;
	bool m_primary_view;
	HGLRC m_hrc;
	CPalette* m_pPal;
	BOOL CreateRGBPalette(HDC hDC);
	static unsigned char   m_oneto8[2];
	static unsigned char   m_twoto8[4];
	static unsigned char   m_threeto8[8];
	static int             m_defaultOverride[13];
	static PALETTEENTRY    m_defaultPalEntry[20];
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
	CRect m_rect;
	CRect m_WindowOld;
	CRect m_WindowOrig;
	GLuint m_shader_id;
	GLuint m_model_color_id;
	GLuint m_light_dir_id;
	GLuint m_model_id;
	void LoadShaders();
	std::vector<glm::vec3> m_sphere_vertices;
	GLuint m_vao_sphere;
	void init_sphere();
	void draw_point(Point* pnt);
	void draw_points();
	std::vector<glm::vec3> m_cylinder_vertices;
	GLuint m_vao_cylinder;
	void init_cylinder();
	void draw_cylinder(Point* pntA, Point* pntB, float dist);
	void draw_cylinders();
	void init_light();
	void init_gl();
	void setup_rendering();
	void draw_gl();
	void GetSize(int& p_width, int& p_height);
	DataBase* m_data_base;
	glm::vec2 m_r_mouse_start_ang;
	glm::vec3 m_r_mouse_start_at;
	glm::vec2 m_r_mouse_start_pos;
	bool m_r_mouse_mode_rot;
	bool m_r_mouse_rotation_done;
	void updateDrag();
	WNDCLASSEX m_wc_ex;
	LPCWSTR m_class_name_str;
	CListCtrl* g_list_main;
	int FormattedErrorAfxMsgBox(const char* p_msg);
	afx_msg void OnAddSelectedAsGroup();
	afx_msg void OnAddSelectedAsIndividuals();
	afx_msg void OnViewProperties();
	afx_msg void OnCloseWindow();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMenu0DeselectAll();
};


