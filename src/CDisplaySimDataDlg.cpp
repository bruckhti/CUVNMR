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
#include "CDisplaySimDataDlg.h"
#include "afxdialogex.h"
#include "iomanip"

IMPLEMENT_DYNAMIC(CDisplaySimDataDlg, CDialogEx)

CDisplaySimDataDlg::CDisplaySimDataDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DISPLAY_SIM_DATA, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDisplaySimDataDlg::~CDisplaySimDataDlg()
{
}

void CDisplaySimDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDisplaySimDataDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CDisplaySimDataDlg::OnInitDialog()
{
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_SimDataList.SubclassDlgItem(IDC_LIST_SIMDATA_BODY, this);
	m_SimDataList_Head.SubclassDlgItem(IDC_LIST_SIMDATA_HEAD, this);
	m_SimDataList.SetExtendedStyle(m_SimDataList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | HDS_NOSIZING);
	m_SimDataList.InsertColumn(0, L"Group", LVCFMT_LEFT, 80);
	m_SimDataList.InsertColumn(1, L"Element", LVCFMT_LEFT, 80);
	m_SimDataList.InsertColumn(2, L"\u03B4_orb (ppm)", LVCFMT_LEFT, 100);
	m_SimDataList.InsertColumn(3, L"\u03B4_fc (ppm)", LVCFMT_LEFT, 100);
	m_SimDataList.InsertColumn(4, L"\u03B4_pcs (ppm)", LVCFMT_LEFT, 100);
	m_SimDataList.InsertColumn(5, L"\u03B4_tot (ppm)", LVCFMT_LEFT, 100);
	m_SimDataList.InsertColumn(6, L"\u0394\u03BD\u00BD (ppm)", LVCFMT_LEFT, 100);
	m_SimDataList.ModifyStyle(0, LVS_NOCOLUMNHEADER);
	m_SimDataList_Head.InsertColumn(0, L"Group", LVCFMT_LEFT, 81);
	m_SimDataList_Head.InsertColumn(1, L"Element", LVCFMT_LEFT, 80);
	m_SimDataList_Head.InsertColumn(2, L"\u03B4_orb (ppm)", LVCFMT_LEFT, 100);
	m_SimDataList_Head.InsertColumn(3, L"\u03B4_fc (ppm)", LVCFMT_LEFT, 100);
	m_SimDataList_Head.InsertColumn(4, L"\u03B4_pcs (ppm)", LVCFMT_LEFT, 100);
	m_SimDataList_Head.InsertColumn(5, L"\u03B4_tot (ppm)", LVCFMT_LEFT, 100);
	m_SimDataList_Head.InsertColumn(6, L"\u0394\u03BD\u00BD (ppm)", LVCFMT_LEFT, 100);
	return TRUE;
}

BOOL CDisplaySimDataDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDisplaySimDataDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CDisplaySimDataDlg::ClearList()
{
	m_SimDataList.DeleteAllItems();
	m_saveGroupNames.clear();
	m_saveGroupIndex.clear();
	m_save_dorb.clear();
	m_save_dfc.clear();
	m_save_dpcs.clear();
	m_save_dv.clear();
}

void CDisplaySimDataDlg::Update(std::vector<std::wstring> GroupNames, std::vector<size_t> GroupIndex, std::wstring Elements, std::vector<double> dorb, std::vector<double> dfc, std::vector<double> dpcs, std::vector<double> dv)
{
	CString formattedValue;
	double dhf = 0.0;
	m_saveGroupNames.clear();
	m_saveGroupNames = GroupNames;
	m_saveGroupIndex.clear();
	m_saveGroupIndex = GroupIndex;
	m_save_dorb.clear();
	m_save_dorb = dorb;
	m_save_dfc.clear();
	m_save_dfc = dfc;
	m_save_dpcs.clear();
	m_save_dpcs = dpcs;
	m_save_dv.clear();
	m_save_dv = dv;
	m_saveElement = Elements;
	if (m_SimDataList.GetItemCount() == 0)
	{
		for (size_t i = 0; i < GroupIndex.size(); i++)
		{
			m_SimDataList.InsertItem((int)i, GroupNames[GroupIndex[i]].c_str());
			m_SimDataList.SetItemText((int)i, 1, Elements.c_str());
			formattedValue.Format(_T("%.5f"), dorb[i]);
			m_SimDataList.SetItemText((int)i, 2, formattedValue);
			formattedValue.Format(_T("%.5f"), dfc[i]);
			m_SimDataList.SetItemText((int)i, 3, formattedValue);
			formattedValue.Format(_T("%.5f"), dpcs[i]);
			m_SimDataList.SetItemText((int)i, 4, formattedValue);
			dhf = dorb[i] + dfc[i] + dpcs[i];
			formattedValue.Format(_T("%.5f"), dhf);
			m_SimDataList.SetItemText((int)i, 5, formattedValue);
			formattedValue.Format(_T("%.5f"), dv[i]);
			m_SimDataList.SetItemText((int)i, 6, formattedValue);
		}
	}
	else if (m_SimDataList.GetItemCount() == GroupIndex.size())
	{
		for (size_t i = 0; i < dorb.size(); i++)
		{
			m_SimDataList.SetItemText((int)i, 0, GroupNames[GroupIndex[i]].c_str());
			m_SimDataList.SetItemText((int)i, 1, Elements.c_str());
			formattedValue.Format(_T("%.5f"), dorb[i]);
			m_SimDataList.SetItemText((int)i, 2, formattedValue);
			formattedValue.Format(_T("%.5f"), dfc[i]);
			m_SimDataList.SetItemText((int)i, 3, formattedValue);
			formattedValue.Format(_T("%.5f"), dpcs[i]);
			m_SimDataList.SetItemText((int)i, 4, formattedValue);
			dhf = dorb[i] + dfc[i] + dpcs[i];
			formattedValue.Format(_T("%.5f"), dhf);
			m_SimDataList.SetItemText((int)i, 5, formattedValue);
			formattedValue.Format(_T("%.5f"), dv[i]);
			m_SimDataList.SetItemText((int)i, 6, formattedValue);
		}
	}
	else
	{
		AfxMessageBox(_T("unexpected error"), MB_ICONERROR);
	}
}


void CDisplaySimDataDlg::print_info(std::ofstream& file)
{
	file << "Group\tElement\tdelta_orb (ppm)\tdelta_fc (ppm)\tdelta_pcs(ppm)\tdelta_hf(ppm)\tFWHM (ppm)" << std::endl;
	file << std::fixed << std::scientific << std::setprecision(12);
	for (size_t i = 0; i < m_saveGroupIndex.size(); i++)
	{
		double dhf = m_save_dorb[i] + m_save_dfc[i] + m_save_dpcs[i];
		std::string name(m_saveGroupNames[m_saveGroupIndex[i]].begin(), m_saveGroupNames[m_saveGroupIndex[i]].end());
		std::string element(m_saveElement.begin(), m_saveElement.end());
		file << name << "\t" << element << "\t" << m_save_dorb[i] << "\t" << m_save_dfc[i] << "\t" << m_save_dpcs[i] << "\t" << dhf << "\t" << m_save_dv[i] << std::endl;
	}
}