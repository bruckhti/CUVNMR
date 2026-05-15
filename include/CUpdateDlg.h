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
#include "GitHubUpdater.h"

class CUpdateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateDlg)
public:
	CUpdateDlg(CWnd* pParent = nullptr);
	virtual ~CUpdateDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_UPDATES };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
	CStatic m_UpdateText;
	CButton m_update_button;
	CButton m_cancel_button;
	std::wstring update_path;
	std::string HASH;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
