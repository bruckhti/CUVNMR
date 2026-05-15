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
#include "CEditAddOrb.h"
#include "afxdialogex.h"
#define IDC_EDIT_CONTROL_LIST 1

BEGIN_MESSAGE_MAP(CListCtrlCEAO, CListCtrl)
	ON_EN_KILLFOCUS(1, &CListCtrlCEAO::OnTemporaryEditKillfocus)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

void CListCtrlCEAO::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
	GetParent()->SendMessage(WM_USER + 1, (WPARAM)nSBCode, (LPARAM)nPos);
}

void CListCtrlCEAO::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	GetParent()->SendMessage(WM_USER + 1, (WPARAM)nSBCode, (LPARAM)nPos);
}

void CListCtrlCEAO::OnTemporaryEditKillfocus()
{
	GetParent()->SendMessage(WM_USER + 2);
}

BOOL CListCtrlCEAO::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		GetParent()->SendMessage(WM_USER + 3, pMsg->wParam, pMsg->lParam);
	}
	return CListCtrl::PreTranslateMessage(pMsg);
}

IMPLEMENT_DYNAMIC(CEditAddOrb, CDialogEx)

CEditAddOrb::CEditAddOrb(DataBase* dataBase, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADDDORB, pParent)
{
	m_data_base = dataBase;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEditAddOrb::~CEditAddOrb()
{
}

void CEditAddOrb::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEditAddOrb, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT_DORB, &CEditAddOrb::OnCbnSelectChangeDORB)
	ON_CBN_SELCHANGE(IDC_COMBO_ADDANISO, &CEditAddOrb::OnCbnSelectChangeAddAniso)
	ON_CBN_SELCHANGE(IDC_COMBO_APPLYREF, &CEditAddOrb::OnCbnSelectChangeApplyReference)
	ON_CBN_SELCHANGE(IDC_COMBO_SHOWBY, &CEditAddOrb::OnCbnSelectChangeShowBy)
	ON_BN_CLICKED(IDC_BUTTON_ADDNEWDORB, &CEditAddOrb::OnBnClickedAddDORB)
	ON_EN_KILLFOCUS(IDC_EDIT_RENAME_DORB, &CEditAddOrb::OnEditOrbNameKillfocus)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BODY_EDITDORB, &CEditAddOrb::OnNMDblclkdOrbList)
	ON_MESSAGE(WM_USER + 1, &CEditAddOrb::OnListScroll)
	ON_MESSAGE(WM_USER + 2, &CEditAddOrb::OnListEditKillfocus)
	ON_MESSAGE(WM_USER + 3, &CEditAddOrb::OnListEditKillfocus)
END_MESSAGE_MAP()

BOOL CEditAddOrb::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (m_edit_list.GetSafeHwnd())
			{
				if (m_edit_list.IsWindowVisible())
				{
					KillEditControl();
				}
			}
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	if (pMsg->message == WM_HSCROLL || pMsg->message == WM_VSCROLL)
	{
		CWnd* pWnd = GetDlgItem(IDC_LIST_BODY_EDITDORB);
		if (pWnd && pMsg->hwnd == pWnd->GetSafeHwnd())
		{
			SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CEditAddOrb::OnInitDialog()
{
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_ComboBoxSelectOrb.SubclassDlgItem(IDC_COMBO_SELECT_DORB, this);
	m_ComboBoxShowBy.SubclassDlgItem(IDC_COMBO_SHOWBY, this);
	m_ComboBoxApplyRef.SubclassDlgItem(IDC_COMBO_APPLYREF, this);
	m_ComboBoxAddAniso.SubclassDlgItem(IDC_COMBO_ADDANISO, this);
	{
		CRect combo_rc;
		m_ComboBoxSelectOrb.GetWindowRect(&combo_rc);
		m_ComboBoxSelectOrb.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (5 * m_ComboBoxSelectOrb.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxShowBy.GetWindowRect(&combo_rc);
		m_ComboBoxShowBy.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (2 * m_ComboBoxShowBy.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxApplyRef.GetWindowRect(&combo_rc);
		m_ComboBoxApplyRef.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (2 * m_ComboBoxApplyRef.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxAddAniso.GetWindowRect(&combo_rc);
		m_ComboBoxAddAniso.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (5 * m_ComboBoxAddAniso.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
	}
	m_EditRenamedOrb.SubclassDlgItem(IDC_EDIT_RENAME_DORB, this);
	m_dOrbList.SubclassDlgItem(IDC_LIST_BODY_EDITDORB, this);
	m_dOrbListHeader.SubclassDlgItem(IDC_LIST_HEAD_EDITDORB, this);
	m_dOrbList.SetExtendedStyle(m_dOrbList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | HDS_NOSIZING);
	m_dOrbList.InsertColumn(0, L"Atom/Group", LVCFMT_LEFT, 80);
	m_dOrbList.InsertColumn(1, L"Atom No.", LVCFMT_LEFT, 70);
	m_dOrbList.InsertColumn(2, L"Element", LVCFMT_LEFT, 60);
	m_dOrbList.InsertColumn(3, L"\u03B4_orb (ppm)", LVCFMT_LEFT, 110);
	m_dOrbList.ModifyStyle(0, LVS_NOCOLUMNHEADER);
	m_dOrbListHeader.InsertColumn(0, L"Atom/Group", LVCFMT_LEFT, 81);
	m_dOrbListHeader.InsertColumn(1, L"Atom No.", LVCFMT_LEFT, 70);
	m_dOrbListHeader.InsertColumn(2, L"Element", LVCFMT_LEFT, 60);
	m_dOrbListHeader.InsertColumn(3, L"\u03B4_orb (ppm)", LVCFMT_LEFT, 110);
	m_ComboBoxAddAniso.AddString(_T("none"));
	for (size_t i = 0; i < m_data_base->get_mol_d_orb_DataSize(); i++)
	{
		tmp_Names.emplace_back(m_data_base->get_mol_d_orb_DataName(i));
		m_ComboBoxSelectOrb.AddString(tmp_Names[i]);
		m_ComboBoxAddAniso.AddString(tmp_Names[i]);
		tmp_sigma_iso.emplace_back(m_data_base->get_mol_d_orb_raw(i));
		tmp_shielding_tensors.emplace_back(m_data_base->get_mol_shieldingTensors(i));
		m_changed_entries.emplace_back(false);
		m_applyAniso.emplace_back(-1);
	}
	m_ComboBoxAddAniso.SetCurSel(0);
	m_ComboBoxAddAniso.EnableWindow(FALSE);
	m_ComboBoxShowBy.AddString(_T("Atoms"));
	m_ComboBoxShowBy.AddString(_T("Groups"));
	m_ComboBoxShowBy.SetCurSel(0);
	m_ComboBoxShowBy.EnableWindow(FALSE);
	m_ComboBoxApplyRef.AddString(_T("None"));
	m_ComboBoxApplyRef.AddString(_T("Isotope"));
	m_ComboBoxApplyRef.SetCurSel(0);
	m_ComboBoxApplyRef.EnableWindow(FALSE);
	m_EditRenamedOrb.EnableWindow(FALSE);
	m_ButtonAddOrb.SubclassDlgItem(IDC_BUTTON_ADDNEWDORB, this);
	if (m_data_base->is_molecule_set())
	{
		m_ButtonAddOrb.EnableWindow(TRUE);
	}
	else
	{
		m_ButtonAddOrb.EnableWindow(FALSE);
	}
	m_TextSigmaOrbSelect.SubclassDlgItem(IDC_STATIC_DORBSELECT_TEXT, this);
	m_TextSigmaOrbSelect.SetWindowTextW(_T("Select \u03C3_orb Data:"));
	m_TextAdaptSigma.SubclassDlgItem(IDC_STATIC_ADAPTSIGMA, this);
	m_TextAdaptSigma.SetWindowTextW(_T("Use \u0394\u03C3_orb:"));
	return TRUE;
}

void CEditAddOrb::OnCbnSelectChangeAddAniso()
{
	int selectionOrb = m_ComboBoxSelectOrb.GetCurSel();
	int selectionAniso = m_ComboBoxAddAniso.GetCurSel();
	if (selectionOrb != -1 && selectionAniso != -1)
	{
		m_applyAniso[selectionOrb] = selectionAniso - 1;
		int num_atoms = m_data_base->get_mol_atom_count();
		if (m_applyAniso[selectionOrb] == -1)
		{
			for (size_t i = 0; i < tmp_shielding_tensors[selectionOrb].size(); i++)
			{
				for (auto& row : tmp_shielding_tensors[selectionOrb][i])
				{
					for (auto& entry : row)
					{
						entry = 0.0;
					}
				}
			}
			for (size_t i = 0; i < tmp_shielding_tensors[selectionOrb].size(); i++)
			{
				double iso_corr = tmp_sigma_iso[selectionOrb][i];
				tmp_shielding_tensors[selectionOrb][i][0][0] += iso_corr;
				tmp_shielding_tensors[selectionOrb][i][1][1] += iso_corr;
				tmp_shielding_tensors[selectionOrb][i][1][1] += iso_corr;
			}
		}
		else
		{
			for (size_t i = 0; i < tmp_shielding_tensors[selectionOrb].size(); i++)
			{
				tmp_shielding_tensors[selectionOrb][i].clear();
				tmp_shielding_tensors[selectionOrb][i] = tmp_shielding_tensors[m_applyAniso[selectionOrb]][i];
				double iso_corr = tmp_sigma_iso[selectionOrb][i] - tmp_sigma_iso[m_applyAniso[selectionOrb]][i];
				tmp_shielding_tensors[selectionOrb][i][0][0] += iso_corr;
				tmp_shielding_tensors[selectionOrb][i][1][1] += iso_corr;
				tmp_shielding_tensors[selectionOrb][i][2][2] += iso_corr;
			}
		}
	}
}

void CEditAddOrb::OnCbnSelectChangeDORB()
{
	_tmp_elements.clear();
	_tmp_isotope.clear();
	int selection = m_ComboBoxSelectOrb.GetCurSel();
	if (selection != -1)
	{
		m_EditRenamedOrb.SetWindowTextW(tmp_Names[selection]);
		m_EditRenamedOrb.EnableWindow(TRUE);
		m_ComboBoxShowBy.EnableWindow(TRUE);
		m_ComboBoxApplyRef.SetCurSel(0);
		m_ComboBoxApplyRef.EnableWindow(TRUE);
		m_ComboBoxAddAniso.SetCurSel(m_applyAniso[selection] + 1);
		if (selection >= m_data_base->get_mol_d_orb_DataSize())
		{
			m_ComboBoxAddAniso.EnableWindow(TRUE);
		}
		else
		{
			m_ComboBoxAddAniso.EnableWindow(FALSE);
		}
		UpdateList(false);
	}
	else
	{
		m_EditRenamedOrb.SetWindowTextW(_T(""));
		m_EditRenamedOrb.EnableWindow(FALSE);
		m_ComboBoxShowBy.EnableWindow(FALSE);
		m_ComboBoxApplyRef.SetCurSel(0);
		m_ComboBoxApplyRef.EnableWindow(FALSE);
		m_ComboBoxAddAniso.SetCurSel(0);
		m_ComboBoxAddAniso.EnableWindow(FALSE);
		m_dOrbList.DeleteAllItems();
	}
}

void CEditAddOrb::OnBnClickedAddDORB()
{
	size_t num_entries = m_changed_entries.size();
	int num_atoms = m_data_base->get_mol_atom_count();
	tmp_Names.emplace_back(_T("New Entry"));
	tmp_sigma_iso.emplace_back(std::vector<double>(num_atoms, 0.0));
	tmp_shielding_tensors.emplace_back(std::vector<std::vector<std::vector<double>>>(num_atoms, std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0))));
	m_changed_entries.emplace_back(true);
	m_applyAniso.emplace_back(-1);
	m_ComboBoxSelectOrb.AddString(tmp_Names[num_entries]);
	m_ComboBoxSelectOrb.SetCurSel(num_entries);
	OnCbnSelectChangeDORB();
}

void CEditAddOrb::OnEditOrbNameKillfocus()
{
	int selectionOrb = m_ComboBoxSelectOrb.GetCurSel();
	if (selectionOrb != -1)
	{
		CString newName;
		m_EditRenamedOrb.GetWindowTextW(newName);
		m_ComboBoxSelectOrb.DeleteString(selectionOrb);
		m_ComboBoxSelectOrb.InsertString(selectionOrb, newName);
		tmp_Names[selectionOrb] = newName;
		m_ComboBoxSelectOrb.SetCurSel(selectionOrb);
	}
}

void CEditAddOrb::OnCbnSelectChangeApplyReference()
{ 
	UpdateList(true);
}

void CEditAddOrb::OnCbnSelectChangeShowBy()
{
	UpdateList(false);
}

void CEditAddOrb::UpdateList(bool retainListScroll)
{
	int scrollPosition = m_dOrbList.GetScrollPos(SB_VERT);
	int selectionRefType = m_ComboBoxApplyRef.GetCurSel();
	if (selectionRefType != -1)
	{
		int selection_show_by = m_ComboBoxShowBy.GetCurSel();
		int selectionOrb = m_ComboBoxSelectOrb.GetCurSel();
		if (selectionRefType == 1 && _tmp_elements.size() == 0)
		{
			bool _new_isotope = true;
			for (size_t i = 0; i < tmp_sigma_iso[selectionOrb].size(); i++)
			{
				_new_isotope = true;
				for (size_t j = 0; j < _tmp_elements.size(); j++)
				{
					if (_tmp_elements[j] == m_data_base->get_mol_element_by_index(i))
					{
						_new_isotope = false;
						break;
					}
				}
				if (_new_isotope)
				{
					_tmp_elements.emplace_back(m_data_base->get_mol_element_by_index(i));
					int index = m_data_base->IsNMRactiveIsotope(_tmp_elements[_tmp_elements.size() - 1]);
					if (index == -1)
					{
						CIsotopeDlg IsotopeSelectionDlg(m_data_base->GetIsotopesOfType(_tmp_elements[_tmp_elements.size() - 1]));
						IsotopeSelectionDlg.DoModal();
						_tmp_isotope.emplace_back(IsotopeSelectionDlg.m_ChosenIsotope);
					}
					else if (index == -2)
					{
						_tmp_isotope.emplace_back(_T("error"));
					}
					else
					{
						_tmp_isotope.emplace_back(m_data_base->GetNMRnuclei(index));
					}
				}
			}
		}
		m_dOrbList.DeleteAllItems();
		if (selection_show_by == 0)
		{
			for (size_t atom = 0; atom < tmp_sigma_iso[selectionOrb].size(); atom++)
			{
				double diamagn_shift = tmp_sigma_iso[selectionOrb][atom];
				if (selectionRefType == 1)
				{
					double sigma_ref = 0.0;
					for (size_t i = 0; i < _tmp_elements.size(); i++)
					{
						if (_tmp_elements[i] == m_data_base->get_mol_element_by_index(atom))
						{
							sigma_ref = m_data_base->get_reference_by_isotope(_tmp_isotope[i]) * 1E-6;
							break;
						}
					}
					double sigma_calc = diamagn_shift * 1E-6;
					diamagn_shift = ((sigma_ref - sigma_calc) / (1.0 - sigma_ref)) * 1E6;
				}
				else
				{
					diamagn_shift *= -1.0;
				}
				CString dOrb_shift_string;
				dOrb_shift_string.Format(_T("%.5f"), diamagn_shift);
				m_dOrbList.InsertItem((int)atom, m_data_base->get_mol_element_by_index(atom) + std::to_wstring(atom).c_str());
				m_dOrbList.SetItemText((int)atom, 1, std::to_wstring(atom).c_str());
				m_dOrbList.SetItemText((int)atom, 2, m_data_base->get_mol_element_by_index(atom));
				m_dOrbList.SetItemText((int)atom, 3, dOrb_shift_string);
			}
		}
		else if (selection_show_by == 1)
		{
			for (size_t group = 0; group < m_data_base->m_simulation.get_num_groups(); group++)
			{
				m_dOrbList.InsertItem((int)group, m_data_base->m_simulation.get_GroupsName_byIndex(group));
				std::vector<int> tmp_group_indices = m_data_base->m_simulation.get_GroupsIndices_byIndex(group);
				CString IndicesString;
				CString FormattedString;
				double mean_dOrb = 0.0;
				for (size_t index = 0; index < tmp_group_indices.size(); index++)
				{
					if (index == 0)
					{
						IndicesString.Format(_T("%d"), tmp_group_indices[index]);
					}
					else
					{
						FormattedString.Format(_T("%d"), tmp_group_indices[index]);
						IndicesString += _T(",") + FormattedString;
					}
					mean_dOrb += tmp_sigma_iso[selectionOrb][tmp_group_indices[index]];
				}
				mean_dOrb /= (double)tmp_group_indices.size();
				if (selectionRefType == 1)
				{
					double sigma_ref = 0.0;
					for (size_t i = 0; i < _tmp_elements.size(); i++)
					{
						if (_tmp_elements[i] == m_data_base->m_simulation.get_GroupsElement_byIndex(group))
						{
							sigma_ref = m_data_base->get_reference_by_isotope(_tmp_isotope[i]) * 1E-6;
							break;
						}
					}
					double sigma_calc = mean_dOrb * 1E-6;
					mean_dOrb = ((sigma_ref - sigma_calc) / (1.0 - sigma_ref)) * 1E6;
				}
				else
				{
					mean_dOrb *= -1.0;
				}
				m_dOrbList.SetItemText((int)group, 1, IndicesString);
				m_dOrbList.SetItemText((int)group, 2, m_data_base->m_simulation.get_GroupsElement_byIndex(group));
				CString dOrb_shift_string;
				dOrb_shift_string.Format(_T("%.5f"), mean_dOrb);
				m_dOrbList.SetItemText((int)group, 3, dOrb_shift_string);
			}
		}
	}
	if (retainListScroll && m_dOrbList.GetItemCount() > 23)
	{
		m_dOrbList.SetScrollPos(SB_VERT, scrollPosition, TRUE);
		m_dOrbList.EnsureVisible(scrollPosition + 22, TRUE);
		UpdateWindow();
	}	
}

void CEditAddOrb::OnNMDblclkdOrbList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pItem = (LPNMITEMACTIVATE)pNMHDR;
	if (m_dOrbList.GetSafeHwnd())
	{
		if (m_dOrbList.IsWindowVisible())
		{
			KillEditControl();
		}
	}
	if (pItem->iSubItem == 3 && m_dOrbList.GetItemCount() != 0)
	{
		CRect rect;
		m_dOrbList.GetSubItemRect(pItem->iItem, pItem->iSubItem, LVIR_LABEL, rect);
		int columnWidth = m_dOrbList.GetColumnWidth(pItem->iSubItem);
		rect.right = rect.left + columnWidth;
		CString currentValue = m_dOrbList.GetItemText(pItem->iItem, 3);
		if (!m_edit_list.GetSafeHwnd())
		{
			m_edit_list.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER, rect, &m_dOrbList, IDC_EDIT_CONTROL_LIST);
		}
		m_edit_list.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
		m_edit_list.SetWindowTextW(currentValue);
		m_edit_list.SetFocus();
		m_edit_list.SetSel(0, -1);
		m_edit_list.SetDlgCtrlID(1);
		m_edit_list.ShowWindow(SW_SHOW);
		m_lastRow_list = pItem->iItem;
	}
	*pResult = 0;
}

void CEditAddOrb::KillEditControl()
{
	int selectionRefType = m_ComboBoxApplyRef.GetCurSel();
	if (m_edit_list.GetSafeHwnd())
	{
		if (m_edit_list.IsWindowVisible())
		{
			CString newValue;
			m_edit_list.SetSel(0, 0);
			Invalidate();
			m_edit_list.GetWindowTextW(newValue);
			m_edit_list.ShowWindow(SW_HIDE);
			m_edit_list.DestroyWindow();
			if (m_lastRow_list != -1)
			{
				CString element = m_dOrbList.GetItemText(m_lastRow_list, 2);
				double doubleValue = _tstof(newValue);
				newValue.Format(_T("%.5f"), doubleValue);
				m_dOrbList.SetItemText(m_lastRow_list, 3, newValue);
				if (selectionRefType == 1)
				{
					double sigma_ref = 0.0;
					for (size_t i = 0; i < _tmp_elements.size(); i++)
					{
						if (_tmp_elements[i] == element)
						{
							sigma_ref = m_data_base->get_reference_by_isotope(_tmp_isotope[i]) * 1E-6;
							break;
						}
					}
					double delta_calc = doubleValue * 1E-6;
					doubleValue = (sigma_ref - delta_calc * (1.0 - sigma_ref)) * 1E6;;
				}
				else
				{
					doubleValue *= -1.0;
				}
				int selection_show_by = m_ComboBoxShowBy.GetCurSel();
				int selectionOrb = m_ComboBoxSelectOrb.GetCurSel();
				if (selection_show_by == 0)
				{
					double corr_iso_value = doubleValue - tmp_sigma_iso[selectionOrb][m_lastRow_list];
					tmp_sigma_iso[selectionOrb][m_lastRow_list] = doubleValue;
					tmp_shielding_tensors[selectionOrb][m_lastRow_list][0][0] += corr_iso_value;
					tmp_shielding_tensors[selectionOrb][m_lastRow_list][1][1] += corr_iso_value;
					tmp_shielding_tensors[selectionOrb][m_lastRow_list][2][2] += corr_iso_value;
				}
				else if (selection_show_by == 1)
				{
					std::vector<int> tmp_group_indices = m_data_base->m_simulation.get_GroupsIndices_byIndex(m_lastRow_list);
					double corr_iso_value;
					for (size_t index = 0; index < tmp_group_indices.size(); index++)
					{
						corr_iso_value = doubleValue - tmp_sigma_iso[selectionOrb][tmp_group_indices[index]];
						tmp_sigma_iso[selectionOrb][tmp_group_indices[index]] = doubleValue;
						tmp_shielding_tensors[selectionOrb][tmp_group_indices[index]][0][0] += corr_iso_value;
						tmp_shielding_tensors[selectionOrb][tmp_group_indices[index]][1][1] += corr_iso_value;
						tmp_shielding_tensors[selectionOrb][tmp_group_indices[index]][2][2] += corr_iso_value;
					}
				}
			}
		}
	}
}

LRESULT CEditAddOrb::OnListScroll(WPARAM wParam, LPARAM lParam)
{
	KillEditControl();
	return 0;
}

LRESULT CEditAddOrb::OnListEditKillfocus(WPARAM wParam, LPARAM lParam)
{
	KillEditControl();
	return 0;
}

LRESULT CEditAddOrb::OnListMouseScroll(WPARAM wParam, LPARAM lParam)
{
	KillEditControl();
	return 0;
}

std::vector<std::vector<std::vector<double>>> CEditAddOrb::get_new_shielding_tensors(size_t index)
{
	if (index < tmp_Names.size())
	{
		return tmp_shielding_tensors[index];
	}
}

std::vector<double> CEditAddOrb::get_new_sigma_iso(size_t index)
{
	if (index < tmp_Names.size())
	{
		return tmp_sigma_iso[index];
	}
}

CString CEditAddOrb::get_new_Name(size_t index)
{
	if (index < tmp_Names.size())
	{
		return tmp_Names[index];
	}
}

size_t CEditAddOrb::sizet()
{
	return tmp_Names.size();
}