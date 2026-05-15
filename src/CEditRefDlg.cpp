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
#include "CEditRefDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CEditRefDlg, CDialogEx)

CEditRefDlg::CEditRefDlg(DataBase* p_DataBase, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EDITREFERENCE, pParent)
{
	m_data_base = p_DataBase;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEditRefDlg::~CEditRefDlg()
{
}

void CEditRefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEditRefDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_ADDREFELEMENTS, &CEditRefDlg::OnCbnSelchangeComboAddrefelements)
	ON_EN_KILLFOCUS(IDC_EDIT_ADDREFERENCE, &CEditRefDlg::OnEnKillfocusEditAddreference)
END_MESSAGE_MAP()

BOOL CEditRefDlg::OnInitDialog()
{
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_ReferenceElementCombo.SubclassDlgItem(IDC_COMBO_ADDREFELEMENTS, this);
	{
		CRect combo_rc;
		m_ReferenceElementCombo.GetWindowRect(&combo_rc);
		m_ReferenceElementCombo.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (10 * m_ReferenceElementCombo.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
	}
	for (size_t i = 0; i < m_data_base->GetNMRnucleiSize(); i++)
	{
		m_ReferenceElementCombo.AddString(m_data_base->GetNMRnuclei(i));
	}
	m_EditReference.SubclassDlgItem(IDC_EDIT_ADDREFERENCE, this);
	m_EditReference.EnableWindow(FALSE);
	return TRUE;
}


void CEditRefDlg::OnCbnSelchangeComboAddrefelements()
{
	int idx = m_ReferenceElementCombo.GetCurSel();
	if (idx != -1)
	{
		m_ReferenceElementCombo.GetLBText(idx, selected_isotop);
		if (selected_isotop == _T("1H") || selected_isotop == _T("13C") || selected_isotop == _T("29Si"))
		{
			m_EditReference.EnableWindow(FALSE);
			double value = m_data_base->get_reference_by_isotope(selected_isotop);
			CString formattedValue;
			formattedValue.Format(_T("%.5f"), value);
			m_EditReference.SetWindowTextW(formattedValue);
		}
		else
		{
			m_EditReference.EnableWindow(TRUE);
			double value = m_data_base->get_reference_by_isotope(selected_isotop);
			CString formattedValue;
			formattedValue.Format(_T("%.5f"), value);
			m_EditReference.SetWindowTextW(formattedValue);
		}
	}
	else
	{
		m_EditReference.EnableWindow(FALSE);
		m_EditReference.SetWindowTextW(_T(""));
	}
}


void CEditRefDlg::OnEnKillfocusEditAddreference()
{
	CString edittext;
	CString formattedValue;
	m_EditReference.GetWindowTextW(edittext);
	double value = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value);
	m_EditReference.SetWindowTextW(formattedValue);
	m_data_base->set_additional_reference(selected_isotop, value);
}
