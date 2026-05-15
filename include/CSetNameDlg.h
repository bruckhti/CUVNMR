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

class CSetNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetNameDlg)
public:
	CSetNameDlg(CWnd* pParent = nullptr);
	virtual ~CSetNameDlg();
	CString GetInputText();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SET_GROUP_NAME };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	CString m_InputText;
	DECLARE_MESSAGE_MAP()
};
