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
#include "OpenGLRenderer.h"
#include "DataBase.h"

class CMoleculeViewerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMoleculeViewerDlg)
public:
	CMoleculeViewerDlg(DataBase* dataBase, CWnd* pParent = nullptr);
	virtual ~CMoleculeViewerDlg();
	void setList(CListCtrl* list_main);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void StartOpenGL();
	OpenGlRenderer m_OGL_Window;
	CStatic m_PictCtrl;
	bool bOpenGLWindowsExists;
	CListCtrl* g_list_main;
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	DataBase* m_data_base;
};
