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
#include "framework.h"
#include "CUVNMR.h"
#include "CuvetteDlg.h"
#include "DataBase.h"
#include "physical_constants.h"
#include "CUpdateDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCuvnmrApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CCuvnmrApp::CCuvnmrApp()
{
}

CCuvnmrApp theApp;

BOOL CCuvnmrApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	AfxEnableControlContainer();
	CWinApp::InitInstance();
	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("cuvnmr"));
	CUpdateDlg updtdlg;
	INT_PTR update_response = updtdlg.DoModal();
	if (update_response != IDOK)
	{
		initialize_physical_constants();
		DataBase data_base;
		CCuvetteDlg dlg(&data_base);
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
	}
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}
#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif
	return FALSE;
}





