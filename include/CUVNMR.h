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
#define VERSION "1.0.0"
#ifndef __AFXWIN_H__
	#error "'pch.h' vor dieser Datei für PCH einschließen"
#endif
#include "resource.h"

class CCuvnmrApp : public CWinApp
{
public:
	CCuvnmrApp();
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CCuvnmrApp theApp;
