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
#include "GraphControl.h"
#include "DataBase.h"
#include <vector>
#include "CMainTab1.h"
#include "CMainTab2.h"
#include "OpenGLRenderer.h"
#include "CMainTab3.h"
#include "CAbout.h"
#include "CDisplaySimDataDlg.h"
#include "CEnsembleDlg.h"
#include "CEditRefDlg.h"
#include "CStatError.h"
#include "CGTensor.h"

class CCuvetteDlg : public CDialogEx
{
public:
	CCuvetteDlg(DataBase* dataBase, CWnd* pParent = nullptr);
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnFileImport();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUVETTE_DIALOG };
#endif
	CButton m_CheckBoxGraph1;
	CButton m_CheckBoxGraph2;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	DataBase* m_data_base;
	CMainTab1 m_tab1;
	CMainTab2 m_tab2;
	CMainTab3 m_tab3;
	HICON m_hIcon;
	afx_msg void StartOpenGL();
	OpenGlRenderer m_OGL_Window;
	CStatic m_PictCtrl;
	bool bOpenGLWindowsExists;
	CListCtrl g_main_list;
	CEdit g_edit_list;
	CSliderCtrl g_Slider_list;
	int g_lastRow_list;
	int g_lastCol_list;
	CListCtrl g_header_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCbnSelchangeComboReference();
	afx_msg void OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHdnBegintrack(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedHideExpSpc();
	afx_msg void OnBnClickedRemoveExpSpc();
	void KillEditControl();
	afx_msg void OnEditReferenceShifts();
	afx_msg void OnEditdOrb();
	afx_msg void OnFileNew();
	afx_msg void OnFileExit();
	afx_msg void OnVersionAbout();
	afx_msg void ShowSimDataDlg();
	afx_msg void ShowStatErrorDlg();
	afx_msg void OnCreateEnsemble();
	afx_msg void OnCreateEmptyHFC();
	afx_msg void OnExport();
	afx_msg void OnEditGTensor();
	DECLARE_MESSAGE_MAP()
private:
	CGraphControl m_GraphControl;
	CComboBox m_ComboBoxReferenceSpectrum;
	CTabCtrl m_TabCtrl_Main;
	CDisplaySimDataDlg m_SimDataDlg;
	CStatError m_StatErrorDlg;
	void UpdateReferenceComboBox();
public:
	afx_msg void OnTcnSelchangeMaintab(NMHDR* pNMHDR, LRESULT* pResult);
};
