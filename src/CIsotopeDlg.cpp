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
#include "CIsotopeDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CIsotopeDlg, CDialogEx)

CIsotopeDlg::CIsotopeDlg(std::vector<CString> isotopes, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ISOTOPE_DIALOG, pParent)
{
	m_isotopes = isotopes;
}

CIsotopeDlg::~CIsotopeDlg()
{
}

void CIsotopeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIsotopeDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_ISOTOPE_SELECTION, &CIsotopeDlg::OnCbnSelchangeComboIsotope)
END_MESSAGE_MAP()

BOOL CIsotopeDlg::OnInitDialog()
{
	m_ComboBoxIsotope.SubclassDlgItem(IDC_COMBO_ISOTOPE_SELECTION, this);
	{
		CRect combo_rc;
		m_ComboBoxIsotope.GetWindowRect(&combo_rc);
		m_ComboBoxIsotope.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (5 * m_ComboBoxIsotope.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
	}
	for (size_t i = 0; i < m_isotopes.size(); i++)
	{
		m_ComboBoxIsotope.AddString(m_isotopes[i]);
	}
	m_ComboBoxIsotope.SetCurSel(0);
	m_ChosenIsotope = m_isotopes[0];
	return TRUE;
}

void CIsotopeDlg::OnCbnSelchangeComboIsotope()
{
	if (m_ComboBoxIsotope.GetCurSel() != -1)
	{
		m_ChosenIsotope = m_isotopes[m_ComboBoxIsotope.GetCurSel()];
	}
	else
	{
		m_ComboBoxIsotope.SetCurSel(0);
		m_ChosenIsotope = m_isotopes[0];
	}
}