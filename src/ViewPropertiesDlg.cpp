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
#include "CUVNMR.h"
#include "ViewPropertiesDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(ViewPropertiesDlg, CDialogEx)

ViewPropertiesDlg::ViewPropertiesDlg(DataBase* data_base, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIEW_PROPERTIES_DIALOG, pParent)
{
	m_data_base = data_base;
	m_prev_sphere_radius = L"";
	m_prev_conn_thresh = L"";
	m_prev_cylinder_radius = L"";
}

ViewPropertiesDlg::~ViewPropertiesDlg()
{
}

void ViewPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SPHERE_NAME, g_view_properties_combo_sphere_name);
	DDX_Control(pDX, IDC_COMBO_CNN_NAME_A, g_view_properties_combo_conn_start);
	DDX_Control(pDX, IDC_COMBO_CNN_NAME_B, g_view_properties_combo_conn_end);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, g_view_properties_color_sphere);
	DDX_Control(pDX, IDC_COMBO_CNN_THRESHOLD, g_view_properties_conn_thresh);
	DDX_Control(pDX, IDC_COMBO_SPHERE_RADIUS, g_view_properties_sphere_radius);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, g_view_properties_bg_col);
	DDX_Control(pDX, IDC_COMBO_CNN_THRESHOLD2, g_view_properties_cylinder_radius);
}

BEGIN_MESSAGE_MAP(ViewPropertiesDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_SPHERE_NAME, &ViewPropertiesDlg::OnCbnSelchangeComboSphereName)
	ON_CBN_SELCHANGE(IDC_COMBO_CNN_NAME_A, &ViewPropertiesDlg::OnCbnSelchangeComboCnnNameA)
	ON_CBN_SELCHANGE(IDC_COMBO_CNN_NAME_B, &ViewPropertiesDlg::OnCbnSelchangeComboCnnNameB)
	ON_EN_CHANGE(IDC_COMBO_SPHERE_RADIUS, &ViewPropertiesDlg::OnEnChangeComboSphereRadius)
	ON_EN_CHANGE(IDC_COMBO_CNN_THRESHOLD, &ViewPropertiesDlg::OnEnChangeComboCnnThreshold)
	ON_EN_CHANGE(IDC_COMBO_CNN_THRESHOLD2, &ViewPropertiesDlg::OnEnChangeComboCnnThreshold2)
	ON_EN_KILLFOCUS(IDC_COMBO_SPHERE_RADIUS, &ViewPropertiesDlg::OnEditSphereRadiusKillfocus)
	ON_EN_KILLFOCUS(IDC_COMBO_CNN_THRESHOLD, &ViewPropertiesDlg::OnEditCNNThresholdKillfocus)
	ON_EN_KILLFOCUS(IDC_COMBO_CNN_THRESHOLD2, &ViewPropertiesDlg::OnEditCNNThreshold2Killfocus)
	ON_BN_CLICKED(IDOK2, &ViewPropertiesDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDRESET, &ViewPropertiesDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &ViewPropertiesDlg::OnCBN1Change)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON2, &ViewPropertiesDlg::OnCBN2Change)
END_MESSAGE_MAP()

void ViewPropertiesDlg::OnCbnSelchangeComboSphereName()
{
	int sel = g_view_properties_combo_sphere_name.GetCurSel();
	CString str;
	g_view_properties_combo_sphere_name.GetLBText(sel, str);
	PointName* pntName = m_data_base->getPointName(str.GetString());
	COLORREF col = getColorRef(pntName->getColor());
	g_view_properties_color_sphere.SetColor(col);
	float radius = pntName->getRadius();
	std::wstring radiusStr = std::to_wstring(radius);
	g_view_properties_sphere_radius.SetWindowTextW(radiusStr.c_str());
}

void ViewPropertiesDlg::OnCbnSelchangeComboCnnNameA()
{
	int selA = g_view_properties_combo_conn_start.GetCurSel();
	int selB = g_view_properties_combo_conn_end.GetCurSel();
	CString strA, strB;
	g_view_properties_combo_conn_start.GetLBText(selA, strA);
	g_view_properties_combo_conn_end.GetLBText(selB, strB);
	PointConnection* conn = m_data_base->getConnection(strA.GetString(), strB.GetString());
	std::wstring threshStr = std::to_wstring(conn->getThreshold());
	g_view_properties_conn_thresh.SetWindowTextW(threshStr.c_str());
}


void ViewPropertiesDlg::OnCbnSelchangeComboCnnNameB()
{
	int selA = g_view_properties_combo_conn_start.GetCurSel();
	int selB = g_view_properties_combo_conn_end.GetCurSel();
	CString strA, strB;
	g_view_properties_combo_conn_start.GetLBText(selA, strA);
	g_view_properties_combo_conn_end.GetLBText(selB, strB);
	PointConnection* conn = m_data_base->getConnection(strA.GetString(), strB.GetString());
	std::wstring threshStr = std::to_wstring(conn->getThreshold());
	g_view_properties_conn_thresh.SetWindowTextW(threshStr.c_str());
}

BOOL ViewPropertiesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	COLORREF col = getColorRef(m_data_base->getBackgroundColor());
	g_view_properties_bg_col.SetColor(col);
	size_t num = m_data_base->getNumPointNames();
	for (int i = 0; i < num; i++) {
		PointName* pntName = m_data_base->getPointName(i);
		std::wstring pntNameStr = pntName->getName();
		LPCTSTR string(pntNameStr.c_str());
		g_view_properties_combo_sphere_name.AddString(string);
		g_view_properties_combo_conn_start.AddString(string);
		g_view_properties_combo_conn_end.AddString(string);
	}
	if (num > 0) 
	{
		g_view_properties_combo_sphere_name.SetCurSel(0);
		g_view_properties_combo_conn_start.SetCurSel(0);
		g_view_properties_combo_conn_end.SetCurSel(0);
		CString str;
		g_view_properties_combo_sphere_name.GetLBText(0, str);
		PointName* pntName = m_data_base->getPointName(str.GetString());
		col = getColorRef(pntName->getColor());
		g_view_properties_color_sphere.SetColor(col);
		float radius = pntName->getRadius();
		std::wstring radiusStr = std::to_wstring(radius);
		g_view_properties_sphere_radius.SetWindowTextW(radiusStr.c_str());
		m_prev_sphere_radius = radiusStr;
		CString strA, strB;
		g_view_properties_combo_conn_start.GetLBText(0, strA);
		g_view_properties_combo_conn_end.GetLBText(0, strB);
		PointConnection* conn = m_data_base->getConnection(strA.GetString(), strB.GetString());
		std::wstring threshStr = std::to_wstring(conn->getThreshold());
		g_view_properties_conn_thresh.SetWindowTextW(threshStr.c_str());
		m_prev_conn_thresh = threshStr;
		float radiusCylinder = m_data_base->getCylinderRadius();
		std::wstring radiusCylinderStr = std::to_wstring(radiusCylinder);
		g_view_properties_cylinder_radius.SetWindowTextW(radiusCylinderStr.c_str());
		m_prev_cylinder_radius = radiusStr;
	}
	return TRUE;
}

COLORREF ViewPropertiesDlg::getColorRef(glm::vec3 col) 
{
	int r = col.r * 255;
	int g = col.g * 255;
	int b = col.b * 255;
	return RGB(r, g, b);
}

void ViewPropertiesDlg::OnEnChangeComboSphereRadius()
{
	CString str;
	g_view_properties_sphere_radius.GetWindowTextW(str);
	std::wstring wstr(str);
	try 
	{
		size_t next_ind;
		float val = std::stof(wstr.c_str(), &next_ind);
		if (next_ind != wstr.size()) 
		{
			int start, end;
			g_view_properties_sphere_radius.GetSel(start, end);
			g_view_properties_sphere_radius.SetWindowTextW(m_prev_sphere_radius.c_str());
			if (start > 0) 
			{
				start--;
				end--;
			}
			g_view_properties_sphere_radius.SetSel(start, end);
		}
		else 
		{
			m_prev_sphere_radius = wstr;
		}
	}
	catch (...) 
	{
		int start, end;
		g_view_properties_sphere_radius.GetSel(start, end);
		g_view_properties_sphere_radius.SetWindowTextW(m_prev_sphere_radius.c_str());
		if (start > 0) 
		{
			start--;
			end--;
		}
		g_view_properties_sphere_radius.SetSel(start, end);
	}
}


void ViewPropertiesDlg::OnEnChangeComboCnnThreshold()
{
	CString str;
	g_view_properties_conn_thresh.GetWindowTextW(str);
	std::wstring wstr(str);
	try 
	{
		size_t next_ind;
		float val = std::stof(wstr.c_str(), &next_ind);
		if (next_ind != wstr.size()) 
		{
			int start, end;
			g_view_properties_conn_thresh.GetSel(start, end);
			g_view_properties_conn_thresh.SetWindowTextW(m_prev_conn_thresh.c_str());
			if (start > 0) 
			{
				start--;
				end--;
			}
			g_view_properties_conn_thresh.SetSel(start, end);
		}
		else 
		{
			m_prev_conn_thresh = wstr;
		}
	}
	catch (...) 
	{
		int start, end;
		g_view_properties_conn_thresh.GetSel(start, end);
		g_view_properties_conn_thresh.SetWindowTextW(m_prev_conn_thresh.c_str());
		if (start > 0) 
		{
			start--;
			end--;
		}
		g_view_properties_conn_thresh.SetSel(start, end);
	}
}


void ViewPropertiesDlg::OnBnClickedOk2()
{
	m_data_base->saveViewerProperties();
}


void ViewPropertiesDlg::OnEnChangeComboCnnThreshold2()
{
	CString str;
	g_view_properties_cylinder_radius.GetWindowTextW(str);
	std::wstring wstr(str);
	try 
	{
		size_t next_ind;
		float val = std::stof(wstr.c_str(), &next_ind);
		if (next_ind != wstr.size()) 
		{
			int start, end;
			g_view_properties_cylinder_radius.GetSel(start, end);
			g_view_properties_cylinder_radius.SetWindowTextW(m_prev_cylinder_radius.c_str());
			if (start > 0) 
			{
				start--;
				end--;
			}
			g_view_properties_cylinder_radius.SetSel(start, end);
		}
		else 
		{
			m_prev_cylinder_radius = wstr;
		}
	}
	catch (...) 
	{
		int start, end;
		g_view_properties_cylinder_radius.GetSel(start, end);
		g_view_properties_cylinder_radius.SetWindowTextW(m_prev_cylinder_radius.c_str());
		if (start > 0) 
		{
			start--;
			end--;
		}
		g_view_properties_cylinder_radius.SetSel(start, end);
	}
}


void ViewPropertiesDlg::OnCBN1Change()
{
	CString str;
	int sel = g_view_properties_combo_sphere_name.GetCurSel();
	g_view_properties_combo_sphere_name.GetLBText(sel, str);
	PointName* pntName = m_data_base->getPointName(str.GetString());
	COLORREF col = g_view_properties_color_sphere.GetColor();
	float r = GetRValue(col) / 255.0f;
	float g = GetGValue(col) / 255.0f;
	float b = GetBValue(col) / 255.0f;
	glm::vec3 colVec = glm::vec3(r, g, b);
	pntName->setColor(colVec);
}

void ViewPropertiesDlg::OnBnClickedReset()
{
	m_data_base->loadDefaultViewerProperties();
}

void ViewPropertiesDlg::OnCBN2Change()
{
	COLORREF col = g_view_properties_bg_col.GetColor();
	float r = GetRValue(col) / 255.0f;
	float g = GetGValue(col) / 255.0f;
	float b = GetBValue(col) / 255.0f;
	glm::vec3 colVec = glm::vec3(r, g, b);
	m_data_base->setBackgroundColor(colVec);
}

void ViewPropertiesDlg::OnEditSphereRadiusKillfocus()
{
	CString str;
	g_view_properties_sphere_radius.GetWindowTextW(str);
	std::wstring wstr(str);
	float radius = std::stof(wstr.c_str());
	int sel = g_view_properties_combo_sphere_name.GetCurSel();
	g_view_properties_combo_sphere_name.GetLBText(sel, str);
	PointName* pntName = m_data_base->getPointName(str.GetString());
	pntName->setRadius(radius);
}

void ViewPropertiesDlg::OnEditCNNThresholdKillfocus()
{
	CString str;
	g_view_properties_conn_thresh.GetWindowTextW(str);
	std::wstring wstr(str);
	float thresh = std::stof(wstr.c_str());
	int selA = g_view_properties_combo_conn_start.GetCurSel();
	int selB = g_view_properties_combo_conn_end.GetCurSel();
	CString strA, strB;
	g_view_properties_combo_conn_start.GetLBText(selA, strA);
	g_view_properties_combo_conn_end.GetLBText(selB, strB);
	PointConnection* conn = m_data_base->getConnection(strA.GetString(), strB.GetString());
	conn->setThreshold(thresh);
}

void ViewPropertiesDlg::OnEditCNNThreshold2Killfocus()
{
	CString str;
	g_view_properties_cylinder_radius.GetWindowTextW(str);
	std::wstring wstr(str);
	float radiusCylinder = std::stof(wstr.c_str());
	m_data_base->setCylinderRadius(radiusCylinder);
}