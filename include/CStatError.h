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

class CStatError : public CDialogEx
{
	DECLARE_DYNAMIC(CStatError)
public:
	CStatError(CWnd* pParent = nullptr);
	virtual ~CStatError();
	void UpdateErrors(std::vector<std::wstring> GroupNames, std::vector<std::vector<double>> ChiError, std::vector<double> signal_error, double chi2red, double r2, double r2adj, std::vector<int> error_list, double Chi_aniso_scalar, std::vector<std::vector<double>> Chi_aniso, double RMSD);
	void UpdateErrorsSI(std::vector<std::vector<double>> ChiErrorSI, std::vector<std::vector<double>> Chi_anisoSI, double Chi_aniso_scalar_SI, double temperature);
	void print_errors(std::ofstream& file);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STATERROR };
#endif
protected:
	HICON m_hIcon;
	CEdit m_EditErrorChiXX;
	CEdit m_EditErrorChiXY;
	CEdit m_EditErrorChiXZ;
	CEdit m_EditErrorChiYX;
	CEdit m_EditErrorChiYY;
	CEdit m_EditErrorChiYZ;
	CEdit m_EditErrorChiZX;
	CEdit m_EditErrorChiZY;
	CEdit m_EditErrorChiZZ;
	CEdit m_EditErrorChiIso;
	CEdit m_EditErrorChiAniso;
	CStatic m_StaticChiQuadRed;
	CStatic m_StaticR2;
	CStatic m_StaticR2adj;
	CStatic m_StaticChiQuadRed_Text;
	CStatic m_StaticR2_Text;
	CStatic m_StaticR2adj_Text;
	CStatic m_StaticChiIso;
	CStatic m_StaticChiAniso;
	CListBox m_ListBoxErrorInfo;
	CListCtrl m_ListSignalError;
	std::vector<std::wstring> m_save_GroupNames;
	std::vector<std::vector<double>> m_save_ChiError;
	std::vector<std::vector<double>> m_save_ChiErrorSI;
	std::vector<double> m_save_signal_error;
	double m_save_RMSD;
	double m_save_chi2red;
	double m_save_r2;
	double m_save_r2adj;
	std::vector<std::string> m_save_error_list;
	double m_save_ChiIsoError;
	double m_save_ChiAnisoScalarError;
	double m_save_ChiIsoErrorSI;
	double m_save_ChiAnisoScalarErrorSI;
	double m_save_temperature;
	CString GetErrorMessage(int error_code);
	std::string GetErrorMessageStd(int error_code);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};
