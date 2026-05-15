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
#include "CSetNameDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CSetNameDlg, CDialogEx)

CSetNameDlg::CSetNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SET_GROUP_NAME, pParent)
{
}

CSetNameDlg::~CSetNameDlg()
{
}

void CSetNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GROUPNAME, m_InputText);
}

BEGIN_MESSAGE_MAP(CSetNameDlg, CDialogEx)
END_MESSAGE_MAP()

CString CSetNameDlg::GetInputText()
{
	return m_InputText;
}