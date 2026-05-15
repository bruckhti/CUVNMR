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
#include "CIsotopeDlg.h"

class CListCtrlCEAO : public CListCtrl
{
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTemporaryEditKillfocus();
	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
};

class CEditAddOrb : public CDialogEx
{
	DECLARE_DYNAMIC(CEditAddOrb)
public:
	CEditAddOrb(DataBase* dataBase, CWnd* pParent = nullptr);
	virtual ~CEditAddOrb();
	std::vector<std::vector<std::vector<double>>> get_new_shielding_tensors(size_t index);
	std::vector<double> get_new_sigma_iso(size_t index);
	CString get_new_Name(size_t index);
	size_t sizet();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADDDORB };
#endif
protected:
	HICON m_hIcon;
	DataBase* m_data_base;
	CStatic m_TextSigmaOrbSelect;
	CStatic m_TextAdaptSigma;
	CComboBox m_ComboBoxSelectOrb;
	CComboBox m_ComboBoxShowBy;
	CComboBox m_ComboBoxApplyRef;
	CComboBox m_ComboBoxAddAniso;
	CEdit m_EditRenamedOrb;
	CListCtrlCEAO m_dOrbList;
	CListCtrl m_dOrbListHeader;
	CEdit m_edit_list;
	int m_lastRow_list;
	CButton m_ButtonAddOrb;
	std::vector<std::vector<std::vector<std::vector<double>>>> tmp_shielding_tensors;
	std::vector<std::vector<double>> tmp_sigma_iso;
	std::vector<CString> tmp_Names;
	std::vector<bool> m_changed_entries;
	std::vector<size_t> m_applyAniso;
	std::vector<CString> _tmp_elements;
	std::vector<CString> _tmp_isotope;
	void UpdateList(bool retainListScroll);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	void KillEditControl();
	afx_msg void OnCbnSelectChangeDORB();
	afx_msg void OnBnClickedAddDORB();
	afx_msg void OnCbnSelectChangeAddAniso();
	afx_msg void OnEditOrbNameKillfocus();
	afx_msg void OnCbnSelectChangeApplyReference();
	afx_msg void OnCbnSelectChangeShowBy();
	afx_msg void OnNMDblclkdOrbList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListScroll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT CEditAddOrb::OnListEditKillfocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListMouseScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
