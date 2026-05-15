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
#ifndef CMAINTAB2_H
#define CMAINTAB2_H
#include "DataBase.h"
#include "CMainTab1.h"
#include "GraphControl.h"
#include "NelderMeadSimplex.h"
#include "CStatError.h"

class CMainTab1;

class CMainTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CMainTab2)
public:
	CMainTab2(CWnd* pParent = nullptr);
	virtual ~CMainTab2();
	void ResetTab();
	void Initialize(DataBase* dataBase);
	void Initialize_Tabs(CMainTab1* TAB);
	void Initialize_Graph(CGraphControl* Graph);
	void Initialize_StatErrorDlg(CStatError* dlg);
	void RefreshCompChi();
	std::vector<std::vector<double>> GetChiBySym(std::vector<double> parameters);
	void setFittedChi(std::vector<std::vector<double>> Fitted_CHI, bool temporary);
	std::vector<std::vector<double>> GetIndividualChi(double temperature);
	std::vector<std::vector<double>> GetComputedChi(double temperature);
	std::vector<std::vector<double>> GetFittedChi();
	void print_info(std::ofstream& file, int susc_type);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_TAB2 };
#endif
protected:
	DataBase* m_data_base;
	CMainTab1* m_pTab1;
	CGraphControl* m_GraphControl;
	CStatError* m_StatError;
	virtual void DoDataExchange(CDataExchange* pDX);
	CEdit m_EditIndividualSusXX;
	CEdit m_EditIndividualSusXY;
	CEdit m_EditIndividualSusXZ;
	CEdit m_EditIndividualSusYX;
	CEdit m_EditIndividualSusYY;
	CEdit m_EditIndividualSusYZ;
	CEdit m_EditIndividualSusZX;
	CEdit m_EditIndividualSusZY;
	CEdit m_EditIndividualSusZZ;
	CEdit m_EditIndividualSusISO;
	CEdit m_EditIndividualSusANISO;
	CStatic m_TextSusIso;
	CStatic m_TextSusDip;
	CEdit m_EditCompSusXX;
	CEdit m_EditCompSusXY;
	CEdit m_EditCompSusXZ;
	CEdit m_EditCompSusYX;
	CEdit m_EditCompSusYY;
	CEdit m_EditCompSusYZ;
	CEdit m_EditCompSusZX;
	CEdit m_EditCompSusZY;
	CEdit m_EditCompSusZZ;
	CEdit m_EditCompSusISO;
	CEdit m_EditCompSusANISO;
	CStatic m_TextSusIso2;
	CStatic m_TextSusDip2;
	CComboBox m_ComboBoxCompChiTemp;
	CEdit m_EditFitSusXX;
	CEdit m_EditFitSusXY;
	CEdit m_EditFitSusXZ;
	CEdit m_EditFitSusYX;
	CEdit m_EditFitSusYY;
	CEdit m_EditFitSusYZ;
	CEdit m_EditFitSusZX;
	CEdit m_EditFitSusZY;
	CEdit m_EditFitSusZZ;
	CEdit m_EditFitSusISO;
	CEdit m_EditFitSusANISO;
	CStatic m_TextSusIso3;
	CStatic m_TextSusDip3;
	CEdit m_EditFitSymXX;
	CEdit m_EditFitSymXY;
	CEdit m_EditFitSymXZ;
	CEdit m_EditFitSymYX;
	CEdit m_EditFitSymYY;
	CEdit m_EditFitSymYZ;
	CEdit m_EditFitSymZX;
	CEdit m_EditFitSymZY;
	CEdit m_EditFitSymZZ;
	CEdit m_EditTrustradius;
	std::vector<std::vector<int>> m_Sym;
	std::vector<std::vector<double>> m_IndividualChi;
	double m_IndividualChiIso;
	double m_IndividualChiAnisoScalar;
	std::vector<std::vector<double>> m_IndividualChiAniso;
	double m_CompChiIso;
	double m_CompChiAnisoScalar;
	std::vector<std::vector<double>> m_CompChiAniso;
	double m_FitChiIso;
	double m_FitChiAnisoScalar;
	std::vector<std::vector<double>> m_FitChiAniso;
	virtual BOOL OnInitDialog();
	double CalcChiIso(std::vector<std::vector<double>>& CHI);
	double CalcChiAniso(std::vector<std::vector<double>>& CHI, std::vector<std::vector<double>>& CHI_ANISO);
	void ResetChiAniso(std::vector<std::vector<double>>& CHI_ANISO);
	void SetSusSymmetry();
	int m_fitStep;
	afx_msg void OnIndividualChiEditKillfocus();
	afx_msg void OnSymmetryEditKillfocus();
	afx_msg void OnCbnSelchangeComboChiTemp();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClicked();
	afx_msg void OnFitClicked();
	afx_msg void OnBnClickedButtonFittoind();
	afx_msg void OnBnClickedButtonComptoind();
	afx_msg void OnTrustEditKillfocus();
	DECLARE_MESSAGE_MAP()
public:
};

#endif