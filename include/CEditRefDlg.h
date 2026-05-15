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

class CEditRefDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditRefDlg)
public:
	CEditRefDlg(DataBase* p_DataBase, CWnd* pParent = nullptr);
	virtual ~CEditRefDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDITREFERENCE };
#endif
protected:
	DataBase* m_data_base;
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	CComboBox m_ReferenceElementCombo;
	CEdit m_EditReference;
	CString selected_isotop;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCbnSelchangeComboAddrefelements();
	afx_msg void OnEnKillfocusEditAddreference();
public:
};
