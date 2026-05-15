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
#include <vector>

class CIsotopeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIsotopeDlg)
public:
	CIsotopeDlg(std::vector<CString> isotopes, CWnd* pParent = nullptr);
	virtual ~CIsotopeDlg();
	CString m_ChosenIsotope;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ISOTOPE_DIALOG };
#endif
protected:
	CComboBox m_ComboBoxIsotope;
	std::vector<CString> m_isotopes;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboIsotope();
	DECLARE_MESSAGE_MAP()
};
