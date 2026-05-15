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
#include "CUpdateDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CUpdateDlg, CDialogEx)

CUpdateDlg::CUpdateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UPDATES, pParent)
{
}

CUpdateDlg::~CUpdateDlg()
{
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUpdateDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUpdateDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CUpdateDlg::OnInitDialog()
{
	m_UpdateText.SubclassDlgItem(IDC_STATIC_UPDATE_INFO_TEXT, this);
	m_update_button.SubclassDlgItem(IDOK, this);
	m_cancel_button.SubclassDlgItem(IDCANCEL, this);
	m_update_button.EnableWindow(FALSE);
	m_cancel_button.EnableWindow(FALSE);
	m_UpdateText.SetWindowTextW(_T("searching for updates, please wait ..."));
	CString response_text;
	std::wstring updateMSG = CheckForUpdatesOnly(VERSION, L"raw.githubusercontent.com", L"/bruckhti/CUVNMR/main/version", L"CUVNMR-Agent", update_path, HASH);
	if (updateMSG == L"1")
	{
		response_text = _T("Updates are available. Do you want to download and istall the update now?");
		m_UpdateText.SetWindowTextW(response_text);
		m_update_button.EnableWindow(TRUE);
		m_cancel_button.EnableWindow(TRUE);
	}
	else if (updateMSG == L"")
	{
		response_text = _T("No Updates");
		m_UpdateText.SetWindowTextW(response_text);
		EndDialog(IDCANCEL);
	}
	else
	{
		response_text = CString(_T("Error: ")) + updateMSG.c_str();
		m_UpdateText.SetWindowTextW(response_text);
		m_cancel_button.EnableWindow(TRUE);
	}
	return TRUE;
}

void CUpdateDlg::OnBnClickedOk()
{
	std::wstring updateMSG;
	m_UpdateText.SetWindowTextW(_T("please wait..."));
	updateMSG = DownloadAndVerify(VERSION, L"raw.githubusercontent.com", L"/bruckhti/CUVNMR/main/version", L"github.com", update_path, HASH, L"CUVNMR-Agent");
	if (updateMSG != L"")
	{
		AfxMessageBox(updateMSG.c_str(), MB_ICONERROR);
	}
	CDialogEx::OnOK();
}
