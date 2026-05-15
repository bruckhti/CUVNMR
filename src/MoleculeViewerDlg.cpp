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
#include "MoleculeViewerDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMoleculeViewerDlg, CDialogEx)

CMoleculeViewerDlg::CMoleculeViewerDlg(DataBase* dataBase, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIEW_DIALOG, pParent)
{
	bOpenGLWindowsExists = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_data_base = dataBase;
}

CMoleculeViewerDlg::~CMoleculeViewerDlg()
{
}

void CMoleculeViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEWLARGE, m_PictCtrl);
}

BEGIN_MESSAGE_MAP(CMoleculeViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

BOOL CMoleculeViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	StartOpenGL();
	return TRUE;
}

void CMoleculeViewerDlg::OnPaint()
{
	if (!IsIconic())
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMoleculeViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMoleculeViewerDlg::StartOpenGL()
{
	CRect rect(19, 42, 1218 / 2, 668 / 2);
	m_PictCtrl.GetWindowRect(rect);
	ScreenToClient(rect);
	if (bOpenGLWindowsExists)
	{
		m_OGL_Window.DestroyScene();
		bOpenGLWindowsExists = false;
	}
	if (!bOpenGLWindowsExists)
	{
		m_OGL_Window.attach_data_base(m_data_base);
		m_OGL_Window.CreateGLContext(rect, (LPCWSTR)"coco2", this);
		m_OGL_Window.PrepareScene();
		m_OGL_Window.setList(g_list_main);

		bOpenGLWindowsExists = true;
	}
}

void CMoleculeViewerDlg::setList(CListCtrl* list_main) 
{
	g_list_main = list_main;
}
