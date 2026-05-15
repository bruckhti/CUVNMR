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
#include "CStatError.h"
#include "afxdialogex.h"
#include <iomanip>

IMPLEMENT_DYNAMIC(CStatError, CDialogEx)

CStatError::CStatError(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_STATERROR, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CStatError::~CStatError()
{
}

void CStatError::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStatError, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CStatError::OnInitDialog()
{
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_EditErrorChiXX.SubclassDlgItem(IDC_EDIT_ERRORCHIXX, this);
	m_EditErrorChiXX.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiXY.SubclassDlgItem(IDC_EDIT_ERRORCHIXY, this);
	m_EditErrorChiXY.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiXZ.SubclassDlgItem(IDC_EDIT_ERRORCHIXZ, this);
	m_EditErrorChiXZ.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiYX.SubclassDlgItem(IDC_EDIT_ERRORCHIYX, this);
	m_EditErrorChiYX.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiYY.SubclassDlgItem(IDC_EDIT_ERRORCHIYY, this);
	m_EditErrorChiYY.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiYZ.SubclassDlgItem(IDC_EDIT_ERRORCHIYZ, this);
	m_EditErrorChiYZ.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiZX.SubclassDlgItem(IDC_EDIT_ERRORCHIZX, this);
	m_EditErrorChiZX.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiZY.SubclassDlgItem(IDC_EDIT_ERRORCHIZY, this);
	m_EditErrorChiZY.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiZZ.SubclassDlgItem(IDC_EDIT_ERRORCHIZZ, this);
	m_EditErrorChiZZ.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiIso.SubclassDlgItem(IDC_EDIT_ERRORCHI_ISO, this);
	m_EditErrorChiIso.SetWindowTextW(_T("0.00000"));
	m_EditErrorChiAniso.SubclassDlgItem(IDC_EDIT_ERRORCHI_ANISO, this);
	m_EditErrorChiAniso.SetWindowTextW(_T("0.00000"));
	m_StaticChiQuadRed.SubclassDlgItem(IDC_STATIC_CHI2RED, this);
	m_StaticChiQuadRed.SetWindowTextW(_T("0.00000"));
	m_StaticR2.SubclassDlgItem(IDC_STATIC_R2, this);
	m_StaticR2.SetWindowTextW(_T("0.00000"));
	m_StaticR2adj.SubclassDlgItem(IDC_STATIC_R2ADJ, this);
	m_StaticR2adj.SetWindowTextW(_T("0.00000"));
	m_ListBoxErrorInfo.SubclassDlgItem(IDC_LIST_ERROR_INFORMATION, this);
	m_StaticChiIso.SubclassDlgItem(IDC_STATIC_ERRORCHIISO_TEXT, this);
	m_StaticChiIso.SetWindowTextW(_T("\u03C7_iso:"));
	m_StaticChiAniso.SubclassDlgItem(IDC_STATIC_ERRORCHIANISO_TEXT, this);
	m_StaticChiAniso.SetWindowTextW(_T("\u0394\u03C7:"));
	m_StaticChiQuadRed_Text.SubclassDlgItem(IDC_STATIC_CHIQUADRED_TEXT, this);
	m_StaticChiQuadRed_Text.SetWindowTextW(_T("\u03C7\u00B2 red. (ppm\u00B2):"));
	m_StaticR2_Text.SubclassDlgItem(IDC_STATIC_R2_TEXT, this);
	m_StaticR2_Text.SetWindowTextW(_T("R\u00B2:"));
	m_StaticR2adj_Text.SubclassDlgItem(IDC_STATIC_R2ADJ_TEXT, this);
	m_StaticR2adj_Text.SetWindowTextW(_T("R\u00B2 adj.:"));
	m_ListSignalError.SubclassDlgItem(IDC_LIST_SIGNALERROR, this);
	m_ListSignalError.SetExtendedStyle(m_ListSignalError.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | HDS_NOSIZING);
	m_ListSignalError.InsertColumn(0, L"Group", LVCFMT_LEFT, 80);
	m_ListSignalError.InsertColumn(1, L"\u03C3(\u03B4_calc) (ppm)", LVCFMT_LEFT, 100);
	UpdateErrors({}, { {0.0, 0.0, 0.0}, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 } }, {}, 0.0, 0.0, 0.0, {}, 0.0, {}, 0.0);
	UpdateErrorsSI({ {0.0, 0.0, 0.0}, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 } }, {}, 0.0, 0.0);
	return TRUE;
}

BOOL CStatError::PreTranslateMessage(MSG* pMsg)
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

void CStatError::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CStatError::UpdateErrors(std::vector<std::wstring> GroupNames, std::vector<std::vector<double>> ChiError, std::vector<double> signal_error, double chi2red, double r2, double r2adj, std::vector<int> error_list, double Chi_aniso_scalar, std::vector<std::vector<double>> Chi_aniso, double RMSD)
{
	m_save_GroupNames.clear();
	m_save_GroupNames = GroupNames;
	m_save_ChiError.clear();
	m_save_ChiError = ChiError;
	m_save_signal_error.clear();
	m_save_signal_error = signal_error;
	m_save_RMSD = RMSD;
	m_save_chi2red = chi2red;
	m_save_r2 = r2;
	m_save_r2adj = r2adj;
	CString formattedString;
	formattedString.Format(_T("%.5f"), ChiError[0][0]);
	m_EditErrorChiXX.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), ChiError[0][1]);
	m_EditErrorChiXY.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), ChiError[0][2]);
	m_EditErrorChiXZ.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), ChiError[1][0]);
	m_EditErrorChiYX.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), ChiError[1][1]);
	m_EditErrorChiYY.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), ChiError[1][2]);
	m_EditErrorChiYZ.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), ChiError[2][0]);
	m_EditErrorChiZX.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), ChiError[2][1]);
	m_EditErrorChiZY.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), ChiError[2][2]); 
	m_EditErrorChiZZ.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), chi2red);
	m_StaticChiQuadRed.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), r2);
	m_StaticR2.SetWindowTextW(formattedString);
	formattedString.Format(_T("%.5f"), r2adj);
	m_StaticR2adj.SetWindowTextW(formattedString);
	m_ListBoxErrorInfo.ResetContent();
	m_save_error_list.clear();
	for (auto& code : error_list)
	{
		m_ListBoxErrorInfo.AddString(GetErrorMessage(code));
		m_save_error_list.push_back(GetErrorMessageStd(code));
	}
	if (signal_error.size() == 0)
	{
		m_ListSignalError.DeleteAllItems();
	}
	else
	{
		m_ListSignalError.DeleteAllItems();
		for (size_t i = 0; i < signal_error.size(); i++)
		{
			m_ListSignalError.InsertItem((int)i, GroupNames[i].c_str());
			formattedString.Format(_T("%.5f"), signal_error[i]);
			m_ListSignalError.SetItemText((int)i, 1, formattedString);
		}
	}
	if (Chi_aniso_scalar == 0.0 && Chi_aniso.size() == 0)
	{
		m_EditErrorChiIso.SetWindowTextW(_T("0.00000"));
		m_EditErrorChiAniso.SetWindowTextW(_T("0.00000"));
	}
	else
	{
		double var_chi_iso = (1.0 / 9.0) * (pow(ChiError[0][0], 2.0), pow(ChiError[1][1], 2.0), pow(ChiError[2][2], 2.0));
		double var_chi_aniso = 0.0;
		for (size_t i = 0; i < 3; i++)
		{
			double inner = 0.0;
			inner += (4.0 / 3.0) * Chi_aniso[i][i];
			for (size_t j = 0; j < 3; j++)
			{
				if (j != i)
				{
					inner -= (2.0 / 3.0) * Chi_aniso[j][j];
				}
			}
			var_chi_aniso += pow(((1.0 / (2.0 * Chi_aniso_scalar)) * inner), 2.0) * pow(ChiError[i][i], 2.0);
		}
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				if (i != j)
				{
					var_chi_aniso += pow((Chi_aniso[i][j] / Chi_aniso_scalar), 2.0) * pow(ChiError[i][j], 2.0);
				}
			}
		}
		double sigma_chi_iso = 0.0;
		double sigma_chi_aniso = 0.0;
		if (var_chi_iso >= 0)
		{
			sigma_chi_iso = sqrt(var_chi_iso);
		}
		else
		{
			m_ListBoxErrorInfo.AddString(_T("warning: \u03C3(\u03C7_iso)\u00B2 is negative."));
			m_save_error_list.emplace_back("warning: \u03C3(\u03C7_iso)\u00B2 is negative.");
		}
		m_save_ChiIsoError = sigma_chi_iso;
		if (var_chi_aniso >= 0)
		{
			sigma_chi_aniso = sqrt(var_chi_aniso);
		}
		else
		{
			m_ListBoxErrorInfo.AddString(_T("warning: \u03C3(\u0394\u03C7)\u00B2 is negative."));
			m_save_error_list.emplace_back("warning: \u03C3(\u0394\u03C7)\u00B2 is negative.");
		}
		m_save_ChiAnisoScalarError = sigma_chi_aniso;
		formattedString.Format(_T("%.5f"), sigma_chi_iso);
		m_EditErrorChiIso.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), sigma_chi_aniso);
		m_EditErrorChiAniso.SetWindowTextW(formattedString);
	}
}

void CStatError::UpdateErrorsSI(std::vector<std::vector<double>> ChiErrorSI, std::vector<std::vector<double>> Chi_anisoSI, double Chi_aniso_scalar_SI, double temperature)
{
	m_save_ChiErrorSI.clear();
	m_save_ChiErrorSI = ChiErrorSI;
	m_save_temperature = temperature;
	if (temperature != 0.0)
	{
		double var_chi_iso_SI = (1.0 / 9.0) * (pow(ChiErrorSI[0][0], 2.0), pow(ChiErrorSI[1][1], 2.0), pow(ChiErrorSI[2][2], 2.0));
		double var_chi_aniso_SI = 0.0;
		for (size_t i = 0; i < 3; i++)
		{
			double inner = 0.0;
			inner += (4.0 / 3.0) * Chi_anisoSI[i][i];
			for (size_t j = 0; j < 3; j++)
			{
				if (j != i)
				{
					inner -= (2.0 / 3.0) * Chi_anisoSI[j][j];
				}
			}
			var_chi_aniso_SI += pow(((1.0 / (2.0 * Chi_aniso_scalar_SI)) * inner), 2.0) * pow(ChiErrorSI[i][i], 2.0);
		}
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				if (i != j)
				{
					var_chi_aniso_SI += pow((Chi_anisoSI[i][j] / Chi_aniso_scalar_SI), 2.0) * pow(ChiErrorSI[i][j], 2.0);
				}
			}
		}
		m_save_ChiIsoErrorSI = 0.0;
		m_save_ChiAnisoScalarErrorSI = 0.0;
		if (var_chi_iso_SI >= 0)
		{
			m_save_ChiIsoError = sqrt(var_chi_iso_SI);
		}
		if (var_chi_aniso_SI >= 0)
		{
			m_save_ChiAnisoScalarError = sqrt(var_chi_aniso_SI);
		}
	}
	else
	{
		m_save_ChiIsoErrorSI = 0.0;
		m_save_ChiAnisoScalarErrorSI = 0.0;
	}
}

CString CStatError::GetErrorMessage(int error_code)
{
	CString error_string = _T("");
	switch (error_code)
	{
	case 1:
		error_string = _T("warning: det[I(\u03B8)] = 0. Continued with I(\u03B8)+\u03BB\xD835\xDC08.");
		break;
	case 2:
		error_string = _T("Error: det[I(\u03B8)+\u03BB\xD835\xDC08] = 0. Failed to compute Cov(\u0398).");
		break;
	case 3:
		error_string = _T("warning: DOF \u2264 0. \u03B8 is overparameterized. Reduced \u03C7\u00B2 was set to 1.");
		break;
	case 4:
		error_string = _T("warning: \u03C3(\u03B8)\u00B2 is negative for \u03C7\u2096\u2097 with l = k.");
		break;
	case 5:
		error_string = _T("warning: \u03C3(y_n)\u00B2 is negative for Var(y_n)\u2096\u2097 with l = k.");
		break;
	case 6:
		error_string = _T("warning: SST = 0. R\u00B2 cannot be calculated.");
		break;
	case 7:
		error_string = _T("warning: R\u00B2_adj cannot be calculated.");
		break;
	case 8:
		error_string = _T("error: Fit not converged.");
		break;
	default:
		break;
	};
	return error_string;
}

std::string CStatError::GetErrorMessageStd(int error_code)
{
	std::string error_string = "";
	switch (error_code)
	{
	case 1:
		error_string = "warning: det[I(theta)] = 0. Continued with I(theta)+lamda*IdentityMatrix.";
		break;
	case 2:
		error_string = "Error: det[I(theta)+lamda*IdentityMatrix] = 0. Failed to compute Cov(THETA).";
		break;
	case 3:
		error_string = "warning: DOF <= 0. theta is overparameterized. Reduced Chi**2 was set to 1.";
		break;
	case 4:
		error_string = "warning: sigma(theta)**2 is negative for Chi_kl with l = k.";
		break;
	case 5:
		error_string = "warning: sigma(y_n)**2 is negative for Var(y_n)_kl with l = k.";
		break;
	case 6:
		error_string = "warning: SST = 0. R**2 cannot be calculated.";
		break;
	case 7:
		error_string = "warning: R**2_adj cannot be calculated.";
		break;
	case 8:
		error_string = "error: Fit not converged.";
		break;
	default:
		break;
	};
	return error_string;
}

void CStatError::print_errors(std::ofstream& file)
{
	file << "---------------------" << std::endl;
	file << "units: cm**3 K / mol" << std::endl;
	file << "---------------------" << std::endl;
	file << std::endl;
	file << "standard error of fitted susceptibility tensor:" << std::endl;
	file << std::fixed << std::scientific << std::setprecision(12);
	for (size_t i = 0; i < 3; i++)
	{
		file << m_save_ChiError[i][0] << "\t" << m_save_ChiError[i][1] << "\t" << m_save_ChiError[i][2] << std::endl;
	}
	file << std::endl;
	file << "isotropic value\t" << m_save_ChiIsoError << std::endl;
	file << "anisotropic value (frobenius norm)\t" << m_save_ChiAnisoScalarError << std::endl;
	file << std::endl;
	file << std::endl;
	file << "----------------------------------------------------------------" << std::endl;
	file << "units: Angstroem**3 (at fit temperature = " << m_save_temperature << " K)" << std::endl;
	file << "----------------------------------------------------------------" << std::endl;
	file << std::endl;
	file << "standard error of fitted susceptibility tensor:" << std::endl;
	for (size_t i = 0; i < 3; i++)
	{
		file << m_save_ChiErrorSI[i][0] << "\t" << m_save_ChiErrorSI[i][1] << "\t" << m_save_ChiErrorSI[i][2] << std::endl;
	}
	file << std::endl;
	file << "isotropic value\t" << m_save_ChiIsoErrorSI << std::endl;
	file << "anisotropic value (frobenius norm)\t" << m_save_ChiAnisoScalarErrorSI << std::endl;
	file << std::endl;
	file << std::endl;
	file << "----------------" << std::endl;
	file << "goodness of fit" << std::endl;
	file << "----------------" << std::endl;
	file << std::endl;
	file << "RMSD = " << m_save_RMSD << std::endl;
	file << "reduced chi**2 = " << m_save_chi2red << std::endl;
	file << "R**2 = " << m_save_r2 << std::endl;
	file << "adjusted R**2 = " << m_save_r2adj << std::endl;
	file << std::endl;
	file << std::endl;
	if (m_save_GroupNames.size() > 0)
	{
		file << "--------------------------------------" << std::endl;
		file << "standard error of simulated data (ppm)" << std::endl;
		file << "--------------------------------------" << std::endl;
		file << std::endl;
		file << "GroupNames\terror" << std::endl;
		for (size_t i = 0; i < m_save_GroupNames.size(); i++)
		{
			std::string name(m_save_GroupNames[i].begin(), m_save_GroupNames[i].end());
			if (m_save_signal_error.size() == 0)
			{
				file << name << "\t" << "WARNING: computing signal error ranges failed ..." << std::endl;
			}
			else
			{
				file << name << "\t" << m_save_signal_error[i] << std::endl;
			}
		}
	}
	if (m_save_error_list.size() > 0)
	{
		file << std::endl;
		file << std::endl;
		file << "--------------------" << std::endl;
		file << "Warnings and Errors" << std::endl;
		file << "--------------------" << std::endl;
		file << std::endl;
		for (size_t i = 0; i < m_save_error_list.size(); i++)
		{
			file << m_save_error_list[i] << std::endl;
		}
	}
}