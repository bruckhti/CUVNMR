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
#include "DataBase.h"

class ViewPropertiesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ViewPropertiesDlg)
public:
	ViewPropertiesDlg(DataBase* data_base, CWnd* pParent = nullptr);
	virtual ~ViewPropertiesDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW_PROPERTIES_DIALOG };
#endif
private:
	DataBase* m_data_base;
	COLORREF getColorRef(glm::vec3 col);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	std::wstring m_prev_sphere_radius;
	std::wstring m_prev_conn_thresh;
	std::wstring m_prev_cylinder_radius;
	afx_msg void OnCBN1Change();
	afx_msg void OnCBN2Change();
	afx_msg void OnEditSphereRadiusKillfocus();
	afx_msg void OnEditCNNThresholdKillfocus();
	afx_msg void OnEditCNNThreshold2Killfocus();
	afx_msg void OnBnClickedReset();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboSphereName();
	afx_msg void OnCbnSelchangeComboCnnNameA();
	afx_msg void OnCbnSelchangeComboCnnNameB();
	CComboBox g_view_properties_combo_sphere_name;
	virtual BOOL OnInitDialog();
	CComboBox g_view_properties_combo_conn_start;
	CComboBox g_view_properties_combo_conn_end;
	CMFCColorButton g_view_properties_color_sphere;
	CEdit g_view_properties_conn_thresh;
	CEdit g_view_properties_sphere_radius;
	CMFCColorButton g_view_properties_bg_col;
	afx_msg void OnEnChangeComboSphereRadius();
	afx_msg void OnEnChangeComboCnnThreshold();
	afx_msg void OnBnClickedOk2();
	CEdit g_view_properties_cylinder_radius;
	afx_msg void OnEnChangeComboCnnThreshold2();
};
