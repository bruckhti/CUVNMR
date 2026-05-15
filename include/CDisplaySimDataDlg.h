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
#include <string>
#include <fstream>

class CDisplaySimDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplaySimDataDlg)
public:
	CDisplaySimDataDlg(CWnd* pParent = nullptr);
	virtual ~CDisplaySimDataDlg();
	void Update(std::vector<std::wstring> GroupNames, std::vector<size_t> GroupIndex, std::wstring Elements, std::vector<double> dorb, std::vector<double> dfc, std::vector<double> dpcs, std::vector<double> dv);
	void ClearList();
	void print_info(std::ofstream& file);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DISPLAY_SIM_DATA };
#endif
protected:
	HICON m_hIcon;
	CListCtrl m_SimDataList;
	CListCtrl m_SimDataList_Head;
	std::vector<std::wstring> m_saveGroupNames;
	std::vector<size_t> m_saveGroupIndex;
	std::vector<double> m_save_dorb;
	std::vector<double> m_save_dfc;
	std::vector<double> m_save_dpcs;
	std::vector<double> m_save_dv;
	std::wstring m_saveElement;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};
