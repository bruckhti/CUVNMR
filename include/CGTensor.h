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
#include "physical_constants.h"
#include "CMainTab1.h"

class CGTensor : public CDialogEx
{
	DECLARE_DYNAMIC(CGTensor)
public:
	CGTensor(DataBase* p_DataBase, CMainTab1* pTab1, CWnd* pParent = nullptr);
	virtual ~CGTensor();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GVALUE };
#endif
protected:
	DataBase* m_data_base;
	CMainTab1* m_Tab1;
	virtual void DoDataExchange(CDataExchange* pDX);
	CEdit m_EditGValXX;
	CEdit m_EditGValXY;
	CEdit m_EditGValXZ;
	CEdit m_EditGValYX;
	CEdit m_EditGValYY;
	CEdit m_EditGValYZ;
	CEdit m_EditGValZX;
	CEdit m_EditGValZY;
	CEdit m_EditGValZZ;
	std::vector<std::vector<double>> m_g_tensor;
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonGReset();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnGTensorEditKillfocus();
public:
};
