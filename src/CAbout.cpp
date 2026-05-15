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
#include "CAbout.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CAbout, CDialogEx)

CAbout::CAbout(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ABOUT, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CAbout::~CAbout()
{
}

void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAbout, CDialogEx)
END_MESSAGE_MAP()

BOOL CAbout::OnInitDialog()
{
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	CStatic* pPictureCtrl = (CStatic*)GetDlgItem(IDC_STATIC_LOGOPICTURE);
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_LOGO), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (pPictureCtrl && hBmp)
	{
		pPictureCtrl->SetBitmap(hBmp);
	}
	return TRUE;
}