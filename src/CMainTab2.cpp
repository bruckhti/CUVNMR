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
#include "CMainTab2.h"
#include "afxdialogex.h"
#include "iomanip"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainTab2, CDialogEx)

CMainTab2::CMainTab2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_TAB2, pParent)
{
}

CMainTab2::~CMainTab2()
{
}

void CMainTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainTab2, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPCHI_TEMP, &CMainTab2::OnCbnSelchangeComboChiTemp)
	ON_EN_KILLFOCUS(IDC_EDIT_INDIVIDUAL_SUS_XX, &CMainTab2::OnIndividualChiEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_INDIVIDUAL_SUS_XY, &CMainTab2::OnIndividualChiEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_INDIVIDUAL_SUS_XZ, &CMainTab2::OnIndividualChiEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_INDIVIDUAL_SUS_YX, &CMainTab2::OnIndividualChiEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_INDIVIDUAL_SUS_YY, &CMainTab2::OnIndividualChiEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_INDIVIDUAL_SUS_YZ, &CMainTab2::OnIndividualChiEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_INDIVIDUAL_SUS_ZX, &CMainTab2::OnIndividualChiEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_INDIVIDUAL_SUS_ZY, &CMainTab2::OnIndividualChiEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_INDIVIDUAL_SUS_ZZ, &CMainTab2::OnIndividualChiEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_FITTED_ACTXX, &CMainTab2::OnSymmetryEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_FITTED_ACTXY, &CMainTab2::OnSymmetryEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_FITTED_ACTXZ, &CMainTab2::OnSymmetryEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_FITTED_ACTYX, &CMainTab2::OnSymmetryEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_FITTED_ACTYY, &CMainTab2::OnSymmetryEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_FITTED_ACTYZ, &CMainTab2::OnSymmetryEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_FITTED_ACTZX, &CMainTab2::OnSymmetryEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_FITTED_ACTZY, &CMainTab2::OnSymmetryEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_FITTED_ACTZZ, &CMainTab2::OnSymmetryEditKillfocus)
	ON_BN_CLICKED(IDC_BUTTON_SYM_RESET, &CMainTab2::OnBnClicked)
	ON_BN_CLICKED(IDC_BUTTON_FITTING, &CMainTab2::OnFitClicked)
	ON_BN_CLICKED(IDC_BUTTON_FITTOIND, &CMainTab2::OnBnClickedButtonFittoind)
	ON_BN_CLICKED(IDC_BUTTON_COMPTOIND, &CMainTab2::OnBnClickedButtonComptoind)
	ON_EN_KILLFOCUS(IDC_EDIT_TRUSTRADIUS, &CMainTab2::OnTrustEditKillfocus)
END_MESSAGE_MAP()

void CMainTab2::Initialize(DataBase* dataBase)
{
	m_data_base = dataBase;
}

void CMainTab2::Initialize_Tabs(CMainTab1* TAB)
{
	m_pTab1 = TAB;
}

void CMainTab2::Initialize_Graph(CGraphControl* Graph)
{
	m_GraphControl = Graph;
}

BOOL CMainTab2::OnInitDialog()
{
	m_IndividualChiIso = 0.0;
	m_IndividualChiAnisoScalar = 0.0;
	ResetChiAniso(m_IndividualChiAniso);
	m_CompChiIso = 0.0;
	m_CompChiAnisoScalar = 0.0;
	ResetChiAniso(m_CompChiAniso);
	m_FitChiIso = 0.0;
	m_FitChiAnisoScalar = 0.0;
	ResetChiAniso(m_FitChiAniso);
	m_EditIndividualSusXX.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_XX, this);
	m_EditIndividualSusXX.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusXY.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_XY, this);
	m_EditIndividualSusXY.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusXZ.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_XZ, this);
	m_EditIndividualSusXZ.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusYX.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_YX, this);
	m_EditIndividualSusYX.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusYY.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_YY, this);
	m_EditIndividualSusYY.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusYZ.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_YZ, this);
	m_EditIndividualSusYZ.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusZX.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_ZX, this);
	m_EditIndividualSusZX.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusZY.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_ZY, this);
	m_EditIndividualSusZY.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusZZ.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_ZZ, this);
	m_EditIndividualSusZZ.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusISO.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_ISO, this);
	m_EditIndividualSusISO.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusANISO.SubclassDlgItem(IDC_EDIT_INDIVIDUAL_SUS_ANISO, this);
	m_EditIndividualSusANISO.SetWindowTextW(_T("0.00000"));
	m_TextSusIso.SubclassDlgItem(IDC_STATIC_CHIISO, this);
	m_TextSusIso.SetWindowTextW(_T("\u03C7_iso:"));
	m_TextSusDip.SubclassDlgItem(IDC_STATIC_CHIDIP, this);
	m_TextSusDip.SetWindowTextW(_T("\u0394\u03C7:"));
	m_EditCompSusXX.SubclassDlgItem(IDC_EDIT_COMP_SUS_XX, this);
	m_EditCompSusXX.SetWindowTextW(_T("0.00000"));
	m_EditCompSusXY.SubclassDlgItem(IDC_EDIT_COMP_SUS_XY, this);
	m_EditCompSusXY.SetWindowTextW(_T("0.00000"));
	m_EditCompSusXZ.SubclassDlgItem(IDC_EDIT_COMP_SUS_XZ, this);
	m_EditCompSusXZ.SetWindowTextW(_T("0.00000"));
	m_EditCompSusYX.SubclassDlgItem(IDC_EDIT_COMP_SUS_YX, this);
	m_EditCompSusYX.SetWindowTextW(_T("0.00000"));
	m_EditCompSusYY.SubclassDlgItem(IDC_EDIT_COMP_SUS_YY, this);
	m_EditCompSusYY.SetWindowTextW(_T("0.00000"));
	m_EditCompSusYZ.SubclassDlgItem(IDC_EDIT_COMP_SUS_YZ, this);
	m_EditCompSusYZ.SetWindowTextW(_T("0.00000"));
	m_EditCompSusZX.SubclassDlgItem(IDC_EDIT_COMP_SUS_ZX, this);
	m_EditCompSusZX.SetWindowTextW(_T("0.00000"));
	m_EditCompSusZY.SubclassDlgItem(IDC_EDIT_COMP_SUS_ZY, this);
	m_EditCompSusZY.SetWindowTextW(_T("0.00000"));
	m_EditCompSusZZ.SubclassDlgItem(IDC_EDIT_COMP_SUS_ZZ, this);
	m_EditCompSusZZ.SetWindowTextW(_T("0.00000"));
	m_EditCompSusISO.SubclassDlgItem(IDC_EDIT_COMP_SUS_ISO, this);
	m_EditCompSusISO.SetWindowTextW(_T("0.00000"));
	m_EditCompSusANISO.SubclassDlgItem(IDC_EDIT_COMP_SUS_ANISO, this);
	m_EditCompSusANISO.SetWindowTextW(_T("0.00000"));
	m_TextSusIso2.SubclassDlgItem(IDC_STATIC_CHIISO2, this);
	m_TextSusIso2.SetWindowTextW(_T("\u03C7_iso:"));
	m_TextSusDip2.SubclassDlgItem(IDC_STATIC_CHIDIP2, this);
	m_TextSusDip2.SetWindowTextW(_T("\u0394\u03C7:"));
	m_ComboBoxCompChiTemp.SubclassDlgItem(IDC_COMBO_COMPCHI_TEMP, this);
	{
		CRect combo_rc;
		m_ComboBoxCompChiTemp.GetWindowRect(&combo_rc);
		m_ComboBoxCompChiTemp.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (10 * m_ComboBoxCompChiTemp.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
	}
	m_EditFitSusXX.SubclassDlgItem(IDC_EDIT_FITTED_SUSXX, this);
	m_EditFitSusXX.SetWindowTextW(_T("0.00000"));
	m_EditFitSusXY.SubclassDlgItem(IDC_EDIT_FITTED_SUSXY, this);
	m_EditFitSusXY.SetWindowTextW(_T("0.00000"));
	m_EditFitSusXZ.SubclassDlgItem(IDC_EDIT_FITTED_SUSXZ, this);
	m_EditFitSusXZ.SetWindowTextW(_T("0.00000"));
	m_EditFitSusYX.SubclassDlgItem(IDC_EDIT_FITTED_SUSYX, this);
	m_EditFitSusYX.SetWindowTextW(_T("0.00000"));
	m_EditFitSusYY.SubclassDlgItem(IDC_EDIT_FITTED_SUSYY, this);
	m_EditFitSusYY.SetWindowTextW(_T("0.00000"));
	m_EditFitSusYZ.SubclassDlgItem(IDC_EDIT_FITTED_SUSYZ, this);
	m_EditFitSusYZ.SetWindowTextW(_T("0.00000"));
	m_EditFitSusZX.SubclassDlgItem(IDC_EDIT_FITTED_SUSZX, this);
	m_EditFitSusZX.SetWindowTextW(_T("0.00000"));
	m_EditFitSusZY.SubclassDlgItem(IDC_EDIT_FITTED_SUSZY, this);
	m_EditFitSusZY.SetWindowTextW(_T("0.00000"));
	m_EditFitSusZZ.SubclassDlgItem(IDC_EDIT_FITTED_SUSZZ, this);
	m_EditFitSusZZ.SetWindowTextW(_T("0.00000"));
	m_EditFitSymXX.SubclassDlgItem(IDC_EDIT_FITTED_ACTXX, this);
	m_EditFitSymXX.SetWindowTextW(_T("11"));
	m_EditFitSymXY.SubclassDlgItem(IDC_EDIT_FITTED_ACTXY, this);
	m_EditFitSymXY.SetWindowTextW(_T("12"));
	m_EditFitSymXZ.SubclassDlgItem(IDC_EDIT_FITTED_ACTXZ, this);
	m_EditFitSymXZ.SetWindowTextW(_T("13"));
	m_EditFitSymYX.SubclassDlgItem(IDC_EDIT_FITTED_ACTYX, this);
	m_EditFitSymYX.SetWindowTextW(_T("21"));
	m_EditFitSymYY.SubclassDlgItem(IDC_EDIT_FITTED_ACTYY, this);
	m_EditFitSymYY.SetWindowTextW(_T("22"));
	m_EditFitSymYZ.SubclassDlgItem(IDC_EDIT_FITTED_ACTYZ, this);
	m_EditFitSymYZ.SetWindowTextW(_T("23"));
	m_EditFitSymZX.SubclassDlgItem(IDC_EDIT_FITTED_ACTZX, this);
	m_EditFitSymZX.SetWindowTextW(_T("31"));
	m_EditFitSymZY.SubclassDlgItem(IDC_EDIT_FITTED_ACTZY, this);
	m_EditFitSymZY.SetWindowTextW(_T("32"));
	m_EditFitSymZZ.SubclassDlgItem(IDC_EDIT_FITTED_ACTZZ, this);
	m_EditFitSymZZ.SetWindowTextW(_T("33"));
	m_Sym = std::vector<std::vector<int>>(3, std::vector<int>(3, 0));
	m_IndividualChi = std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0));
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_Sym[i][j] = (i + 1) * 10 + j + 1;
			m_IndividualChi[i][j] = 0.0;
		}
	}
	m_EditFitSusISO.SubclassDlgItem(IDC_EDIT_FITTED_SUS_ISO, this);
	m_EditFitSusISO.SetWindowTextW(_T("0.00000"));
	m_EditFitSusANISO.SubclassDlgItem(IDC_EDIT_FITTED_SUS_ANISO, this);
	m_EditFitSusANISO.SetWindowTextW(_T("0.00000"));
	m_TextSusIso3.SubclassDlgItem(IDC_STATIC_CHIISO3, this);
	m_TextSusIso3.SetWindowTextW(_T("\u03C7_iso:"));
	m_TextSusDip3.SubclassDlgItem(IDC_STATIC_CHIDIP3, this);
	m_TextSusDip3.SetWindowTextW(_T("\u0394\u03C7:"));
	m_EditTrustradius.SubclassDlgItem(IDC_EDIT_TRUSTRADIUS, this);
	m_EditTrustradius.SetWindowTextW(_T("0.1000"));
	return TRUE;
}

BOOL CMainTab2::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnIndividualChiEditKillfocus();
			OnSymmetryEditKillfocus();
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMainTab2::RefreshCompChi()
{
	m_ComboBoxCompChiTemp.ResetContent();
	CString formattedString;
	for (size_t i = 0; i < m_data_base->get_mol_chi_size(); i++)
	{
		formattedString.Format(_T("%.1f"), m_data_base->get_mol_chi_Temp(i));
		m_ComboBoxCompChiTemp.AddString(formattedString);
	}
	m_EditCompSusXX.SetWindowTextW(_T("0.00000"));
	m_EditCompSusXY.SetWindowTextW(_T("0.00000"));
	m_EditCompSusXZ.SetWindowTextW(_T("0.00000"));
	m_EditCompSusYX.SetWindowTextW(_T("0.00000"));
	m_EditCompSusYY.SetWindowTextW(_T("0.00000"));
	m_EditCompSusYZ.SetWindowTextW(_T("0.00000"));
	m_EditCompSusZX.SetWindowTextW(_T("0.00000"));
	m_EditCompSusZY.SetWindowTextW(_T("0.00000"));
	m_EditCompSusZZ.SetWindowTextW(_T("0.00000"));
	m_EditCompSusISO.SetWindowTextW(_T("0.00000"));
	m_EditCompSusANISO.SetWindowTextW(_T("0.00000"));
	ResetChiAniso(m_CompChiAniso);
}

void CMainTab2::OnCbnSelchangeComboChiTemp()
{
	if (m_ComboBoxCompChiTemp.GetCurSel() != -1)
	{
		std::vector<std::vector<double>> SetChi = m_data_base->get_mol_CHI(m_ComboBoxCompChiTemp.GetCurSel());
		CString formattedString;
		formattedString.Format(_T("%.5f"), SetChi[0][0]);
		m_EditCompSusXX.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), SetChi[0][1]);
		m_EditCompSusXY.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), SetChi[0][2]);
		m_EditCompSusXZ.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), SetChi[1][0]);
		m_EditCompSusYX.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), SetChi[1][1]);
		m_EditCompSusYY.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), SetChi[1][2]);
		m_EditCompSusYZ.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), SetChi[2][0]);
		m_EditCompSusZX.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), SetChi[2][1]);
		m_EditCompSusZY.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), SetChi[2][2]);
		m_EditCompSusZZ.SetWindowTextW(formattedString);
		m_CompChiIso = CalcChiIso(SetChi);
		m_CompChiAnisoScalar = CalcChiAniso(SetChi, m_CompChiAniso);
		formattedString.Format(_T("%.5f"), m_CompChiIso);
		m_EditCompSusISO.SetWindowTextW(formattedString);
		formattedString.Format(_T("%.5f"), m_CompChiAnisoScalar);
		m_EditCompSusANISO.SetWindowTextW(formattedString);
		m_pTab1->tab2_sim(true, false);
	}
}

void CMainTab2::OnIndividualChiEditKillfocus()
{
	CString edittext;
	CString formattedValue;
	std::vector<std::vector<double>> value(3, std::vector<double>(3, 0.0));
	m_EditIndividualSusXX.GetWindowTextW(edittext);
	value[0][0] = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value[0][0]);
	m_EditIndividualSusXX.SetWindowTextW(formattedValue);
	m_EditIndividualSusXY.GetWindowTextW(edittext);
	value[0][1] = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value[0][1]);
	m_EditIndividualSusXY.SetWindowTextW(formattedValue);
	m_EditIndividualSusXZ.GetWindowTextW(edittext);
	value[0][2] = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value[0][2]);
	m_EditIndividualSusXZ.SetWindowTextW(formattedValue);
	m_EditIndividualSusYX.GetWindowTextW(edittext);
	value[1][0] = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value[1][0]);
	m_EditIndividualSusYX.SetWindowTextW(formattedValue);
	m_EditIndividualSusYY.GetWindowTextW(edittext);
	value[1][1] = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value[1][1]);
	m_EditIndividualSusYY.SetWindowTextW(formattedValue);
	m_EditIndividualSusYZ.GetWindowTextW(edittext);
	value[1][2] = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value[1][2]);
	m_EditIndividualSusYZ.SetWindowTextW(formattedValue);
	m_EditIndividualSusZX.GetWindowTextW(edittext);
	value[2][0] = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value[2][0]);
	m_EditIndividualSusZX.SetWindowTextW(formattedValue);
	m_EditIndividualSusZY.GetWindowTextW(edittext);
	value[2][1] = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value[2][1]);
	m_EditIndividualSusZY.SetWindowTextW(formattedValue);
	m_EditIndividualSusZZ.GetWindowTextW(edittext);
	value[2][2] = _tstof(edittext);
	formattedValue.Format(_T("%.5f"), value[2][2]);
	m_EditIndividualSusZZ.SetWindowTextW(formattedValue);
	m_IndividualChiIso = CalcChiIso(value);
	m_IndividualChiAnisoScalar = CalcChiAniso(value, m_IndividualChiAniso);
	formattedValue.Format(_T("%.5f"), m_IndividualChiIso);
	m_EditIndividualSusISO.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), m_IndividualChiAnisoScalar);
	m_EditIndividualSusANISO.SetWindowTextW(formattedValue);
	m_IndividualChi = value;
	m_pTab1->tab2_sim(true, false);
}

void CMainTab2::OnSymmetryEditKillfocus()
{
	CString edittext;
	CString formattedValue;
	m_EditFitSymXX.GetWindowTextW(edittext);
	m_Sym[0][0] = _tstoi(edittext);
	m_EditFitSymXY.GetWindowTextW(edittext);
	m_Sym[0][1] = _tstoi(edittext);
	m_EditFitSymXZ.GetWindowTextW(edittext);
	m_Sym[0][2] = _tstoi(edittext);
	m_EditFitSymYX.GetWindowTextW(edittext);
	m_Sym[1][0] = _tstoi(edittext);
	m_EditFitSymYY.GetWindowTextW(edittext);
	m_Sym[1][1] = _tstoi(edittext);
	m_EditFitSymYZ.GetWindowTextW(edittext);
	m_Sym[1][2] = _tstoi(edittext);
	m_EditFitSymZX.GetWindowTextW(edittext);
	m_Sym[2][0] = _tstoi(edittext);
	m_EditFitSymZY.GetWindowTextW(edittext);
	m_Sym[2][1] = _tstoi(edittext);
	m_EditFitSymZZ.GetWindowTextW(edittext);
	m_Sym[2][2] = _tstoi(edittext);
	formattedValue.Format(_T("%d"), m_Sym[0][0]);
	m_EditFitSymXX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[0][1]);
	m_EditFitSymXY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[0][2]);
	m_EditFitSymXZ.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[1][0]);
	m_EditFitSymYX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[1][1]);
	m_EditFitSymYY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[1][2]);
	m_EditFitSymYZ.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[2][0]);
	m_EditFitSymZX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[2][1]);
	m_EditFitSymZY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[2][2]);
	m_EditFitSymZZ.SetWindowTextW(formattedValue);
}

void CMainTab2::OnBnClicked()
{
	CString formattedValue;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_Sym[i][j] = (i + 1) * 10 + j + 1;
		}
	}
	formattedValue.Format(_T("%d"), m_Sym[0][0]);
	m_EditFitSymXX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[0][1]);
	m_EditFitSymXY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[0][2]);
	m_EditFitSymXZ.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[1][0]);
	m_EditFitSymYX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[1][1]);
	m_EditFitSymYY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[1][2]);
	m_EditFitSymYZ.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[2][0]);
	m_EditFitSymZX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[2][1]);
	m_EditFitSymZY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%d"), m_Sym[2][2]);
	m_EditFitSymZZ.SetWindowTextW(formattedValue);
}

void CMainTab2::OnBnClickedButtonComptoind()
{
	CString copytext;
	m_EditCompSusXX.GetWindowTextW(copytext);
	m_EditIndividualSusXX.SetWindowTextW(copytext);
	m_EditCompSusXY.GetWindowTextW(copytext);
	m_EditIndividualSusXY.SetWindowTextW(copytext);
	m_EditCompSusXZ.GetWindowTextW(copytext);
	m_EditIndividualSusXZ.SetWindowTextW(copytext);
	m_EditCompSusYX.GetWindowTextW(copytext);
	m_EditIndividualSusYX.SetWindowTextW(copytext);
	m_EditCompSusYY.GetWindowTextW(copytext);
	m_EditIndividualSusYY.SetWindowTextW(copytext);
	m_EditCompSusYZ.GetWindowTextW(copytext);
	m_EditIndividualSusYZ.SetWindowTextW(copytext);
	m_EditCompSusZX.GetWindowTextW(copytext);
	m_EditIndividualSusZX.SetWindowTextW(copytext);
	m_EditCompSusZY.GetWindowTextW(copytext);
	m_EditIndividualSusZY.SetWindowTextW(copytext);
	m_EditCompSusZZ.GetWindowTextW(copytext);
	m_EditIndividualSusZZ.SetWindowTextW(copytext);
	OnIndividualChiEditKillfocus();
	m_pTab1->tab2_sim(true, false);
}

void CMainTab2::OnBnClickedButtonFittoind()
{
	CString copytext;
	m_EditFitSusXX.GetWindowTextW(copytext);
	m_EditIndividualSusXX.SetWindowTextW(copytext);
	m_EditFitSusXY.GetWindowTextW(copytext);
	m_EditIndividualSusXY.SetWindowTextW(copytext);
	m_EditFitSusXZ.GetWindowTextW(copytext);
	m_EditIndividualSusXZ.SetWindowTextW(copytext);
	m_EditFitSusYX.GetWindowTextW(copytext);
	m_EditIndividualSusYX.SetWindowTextW(copytext);
	m_EditFitSusYY.GetWindowTextW(copytext);
	m_EditIndividualSusYY.SetWindowTextW(copytext);
	m_EditFitSusYZ.GetWindowTextW(copytext);
	m_EditIndividualSusYZ.SetWindowTextW(copytext);
	m_EditFitSusZX.GetWindowTextW(copytext);
	m_EditIndividualSusZX.SetWindowTextW(copytext);
	m_EditFitSusZY.GetWindowTextW(copytext);
	m_EditIndividualSusZY.SetWindowTextW(copytext);
	m_EditFitSusZZ.GetWindowTextW(copytext);
	m_EditIndividualSusZZ.SetWindowTextW(copytext);
	OnIndividualChiEditKillfocus();
	m_pTab1->tab2_sim(true, false);
}

void CMainTab2::OnTrustEditKillfocus()
{
	CString edittext;
	m_EditTrustradius.GetWindowTextW(edittext);
	double value = _tstof(edittext);
	if (value <= 0.0001)
	{
		value = 0.1;
	}
	CString formattedValue;
	formattedValue.Format(_T("%.4f"), value);
	m_EditTrustradius.SetWindowTextW(formattedValue);
}

void CMainTab2::ResetChiAniso(std::vector<std::vector<double>>& CHI_ANISO)
{
	CHI_ANISO.clear();
	CHI_ANISO.emplace_back(std::vector<double>(3, 0.0));
	CHI_ANISO.emplace_back(std::vector<double>(3, 0.0));
	CHI_ANISO.emplace_back(std::vector<double>(3, 0.0));
}

double CMainTab2::CalcChiIso(std::vector<std::vector<double>>& CHI)
{
	return (1.0/3.0) * (CHI[0][0] + CHI[1][1] + CHI[2][2]);
}

double CMainTab2::CalcChiAniso(std::vector<std::vector<double>>& CHI, std::vector<std::vector<double>>& CHI_ANISO)
{
	ResetChiAniso(CHI_ANISO);
	double CHI_ISO = (1.0 / 3.0) * (CHI[0][0] + CHI[1][1] + CHI[2][2]);
	CHI_ANISO[0][0] = CHI[0][0] - CHI_ISO;
	CHI_ANISO[0][1] = CHI[0][1];
	CHI_ANISO[0][2] = CHI[0][2];
	CHI_ANISO[1][0] = CHI[1][0];
	CHI_ANISO[1][1] = CHI[1][1] - CHI_ISO;
	CHI_ANISO[1][2] = CHI[1][2];
	CHI_ANISO[2][0] = CHI[2][0];
	CHI_ANISO[2][1] = CHI[2][1];
	CHI_ANISO[2][2] = CHI[2][2] - CHI_ISO;
	double sum = pow(CHI_ANISO[0][0], 2.0) + pow(CHI_ANISO[0][1], 2.0) + pow(CHI_ANISO[0][2], 2.0);
	sum += pow(CHI_ANISO[1][0], 2.0) + pow(CHI_ANISO[1][1], 2.0) + pow(CHI_ANISO[1][2], 2.0);
	sum += pow(CHI_ANISO[2][0], 2.0) + pow(CHI_ANISO[2][1], 2.0) + pow(CHI_ANISO[2][2], 2.0);
	return sqrt(sum);
}


void CMainTab2::OnFitClicked()
{
	m_fitStep = 0;
	double temperature = m_pTab1->get_temperature();
	m_data_base->m_simulation.SetFitTemperature(temperature);
	std::vector<std::vector<double>> CHI_CONVERTED(3, std::vector<double>(3, 0.0));
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			CHI_CONVERTED[i][j] = m_data_base->convertCHItoA3(m_IndividualChi[i][j], temperature);
		}
	}
	m_data_base->set_fitted_Chi(CHI_CONVERTED);
	int state = m_pTab1->GetFittingState();
	if (!m_data_base->m_simulation.WeightsOK())
	{
		state = -5;
		AfxMessageBox(_T("All weights are zero! No atom group is considered in the fitting procedure. Aborting fitting."), MB_ICONERROR);
	}
	if (state == 0 || state == 1 || state == 2 || state == 3)
	{
		SetSusSymmetry();
		m_data_base->m_simulation.SetFitType(state);
		std::vector<std::vector<bool>> sym_entry_checked(3, std::vector<bool>(3, false));
		std::vector<double> param_chi;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				for (size_t k = 0; k < 3; k++)
				{
					for (size_t l = 0; l < 3; l++)
					{
						if (!(k == i && l == j))
						{
							if (m_Sym[k][l] == m_Sym[i][j])
							{
								sym_entry_checked[k][l] = true;
							}
						}
					}
				}
				if (!sym_entry_checked[i][j])
				{
					if (m_Sym[i][j] == 0)
					{
						//
					}
					else
					{
						param_chi.emplace_back(CHI_CONVERTED[i][j]);
					}
				}
				sym_entry_checked[i][j] = true;
			}
		}
		for (size_t i = 0; i < param_chi.size(); i++)
		{
			if (param_chi[i] == 0.0)
			{
				param_chi[i] += 0.000001;
			}
		}
		CButton* pRelCheckBox = (CButton*)GetDlgItem(IDC_CHECK_FIT_REL);
		bool relative_trust;
		if (pRelCheckBox != nullptr)
		{
			relative_trust = (pRelCheckBox->GetCheck() == BST_CHECKED);
		}
		else
		{
			relative_trust = true;
		}
		CString edittext;
		m_EditTrustradius.GetWindowTextW(edittext);
		double TrRa = (double)_tstof(edittext);
		m_data_base->m_simulation.Reset_computed();
		NelderMeadSimplex* Optimizer = new NelderMeadSimplex(m_data_base->m_simulation.RMSD, this);
		Optimizer->InitializeBreak("ShiftFit");
		Optimizer->set_convergence_criteria(5000, 0.00000001, 0.000000001);
		Optimizer->set_optimization_parameter(1.0, 1.0, 0.5, 0.5);
		Optimizer->fconv_type(1);
		if (Optimizer->downhillsimplex(param_chi, TrRa, relative_trust))
		{
			CString converged_after;
			converged_after.Format(_T("%d"), Optimizer->get_number_of_iterations());	
			CString optimal_value;
			optimal_value.Format(_T("%.9f"), Optimizer->get_result_value());
			AfxMessageBox(_T("converged after ") + converged_after + _T(" cycles (RMSD = ") + optimal_value + _T(")"), MB_ICONINFORMATION);
			std::vector<double> optimized_parameters(param_chi.size(), 0.0);
			for (size_t i = 0; i < param_chi.size(); i++)
			{
				optimized_parameters[i] = Optimizer->get_optimized_parameter((int)i);
			}
			m_data_base->m_simulation.chi_error(temperature);
			m_StatError->UpdateErrors(m_data_base->m_simulation.get_GroupsName(), m_data_base->m_simulation.get_chi_error(), m_data_base->m_simulation.get_signal_error(), m_data_base->m_simulation.get_chi2red(), m_data_base->m_simulation.get_r2(), m_data_base->m_simulation.get_r2adj(), m_data_base->m_simulation.get_error_list(), m_FitChiAnisoScalar, m_FitChiAniso, Optimizer->get_result_value());
			std::vector<std::vector<double>> tmp_anisoSI;
			double tmp_aniso_scalar_SI = CalcChiAniso(m_data_base->get_fitted_CHI(),tmp_anisoSI);
			m_StatError->UpdateErrorsSI(m_data_base->m_simulation.get_chi_errorSI(), tmp_anisoSI, tmp_aniso_scalar_SI, temperature);
			setFittedChi(GetChiBySym(optimized_parameters), false);
		}
		else
		{
			AfxMessageBox(_T("fit did not converge\n"));
			m_StatError->UpdateErrors({}, { {0.0, 0.0, 0.0}, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 } }, {}, 0.0, 0.0, 0.0, { 8 }, 0.0, {}, 0.0);
			m_StatError->UpdateErrorsSI({ {0.0, 0.0, 0.0}, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 } }, {}, 0.0, 0.0);
		}
		delete Optimizer;
		m_data_base->m_simulation.set_fitusage(false);
	}
	else if (state == -1)
	{
		AfxMessageBox(_T("A data set of diamagnetic shifts must be selected for this fitting procedure!"), MB_ICONWARNING);
	}
	else if (state == -2)
	{
		AfxMessageBox(_T("A data set of hyperfine coupling constants (HFC) must be selected for this fitting procedure!"), MB_ICONWARNING);
	}
	else if (state == -3)
	{
		AfxMessageBox(_T("A data set of fermi contact couplings (FC) must be selected for this fitting procedure!"), MB_ICONWARNING);
	}
	else if (state == -4)
	{
		AfxMessageBox(_T("No fitting procedure is possible with the selected simulation properties!"), MB_ICONERROR);
	}
}

void CMainTab2::setFittedChi(std::vector<std::vector<double>> FittedCHI, bool temporary)
{
	std::vector<std::vector<double>> convertedCHI(3, std::vector<double>(3, 0.0));
	double temperature = (double)m_pTab1->get_temperature();
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			convertedCHI[i][j] = m_data_base->convertCHItoCGI(FittedCHI[i][j], temperature);
		}
	}
	CString formattedValue;
	formattedValue.Format(_T("%.5f"), convertedCHI[0][0]);
	m_EditFitSusXX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), convertedCHI[0][1]);
	m_EditFitSusXY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), convertedCHI[0][2]);
	m_EditFitSusXZ.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), convertedCHI[1][0]);
	m_EditFitSusYX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), convertedCHI[1][1]);
	m_EditFitSusYY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), convertedCHI[1][2]);
	m_EditFitSusYZ.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), convertedCHI[2][0]);
	m_EditFitSusZX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), convertedCHI[2][1]);
	m_EditFitSusZY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), convertedCHI[2][2]);
	m_EditFitSusZZ.SetWindowTextW(formattedValue);
	m_FitChiIso = CalcChiIso(convertedCHI);
	m_FitChiAnisoScalar = CalcChiAniso(convertedCHI, m_FitChiAniso);
	formattedValue.Format(_T("%.5f"), m_FitChiIso);
	m_EditFitSusISO.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.5f"), m_FitChiAnisoScalar);
	m_EditFitSusANISO.SetWindowTextW(formattedValue);
	if (!temporary || m_fitStep % 10 == 0)
	{
		m_data_base->set_fitted_Chi(FittedCHI);
		m_pTab1->tab2_sim(true, true);
	}
	m_fitStep++;
}

void CMainTab2::SetSusSymmetry()
{
	m_data_base->m_simulation.SetSymmetry(m_Sym);
}

std::vector<std::vector<double>> CMainTab2::GetChiBySym(std::vector<double> parameters)
{
	std::vector<std::vector<double>> return_chi(3, std::vector<double>(3, 0.0));
	int sym_val = -1;
	std::vector<std::vector<bool>> sym_checked(3, std::vector<bool>(3, false));
	for (size_t n = 0; n < parameters.size(); n++)
	{
		sym_val = -1;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				if (m_Sym[i][j] == 0)
				{
					return_chi[i][j] = 0.0;
					sym_checked[i][j] = true;
				}
				if (sym_checked[i][j] == false)
				{
					if (sym_val == -1)
					{
						sym_val = m_Sym[i][j];
					}
					if (sym_val == m_Sym[i][j])
					{
						return_chi[i][j] = parameters[n];
						sym_checked[i][j] = true;
					}
				}
			}
		}
	}
	return return_chi;
}

std::vector<std::vector<double>> CMainTab2::GetIndividualChi(double temperature)
{
	std::vector<std::vector<double>> CHI_CONVERTED(3, std::vector<double>(3, 0.0));
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			CHI_CONVERTED[i][j] = m_data_base->convertCHItoA3(m_IndividualChi[i][j], temperature);
		}
	}
	return CHI_CONVERTED;
}

std::vector<std::vector<double>> CMainTab2::GetComputedChi(double temperature)
{
	std::vector<std::vector<double>> CHI_CONVERTED(3, std::vector<double>(3, 0.0));
	if (m_ComboBoxCompChiTemp.GetCurSel() == -1)
	{
		AfxMessageBox(_T("Please choose a temperature for the computed susceptibility tensor!\n...using NULL tensor..."), MB_ICONWARNING);
	}
	else
	{
		std::vector<std::vector<double>> SelectedCompChi = m_data_base->get_mol_CHI(m_ComboBoxCompChiTemp.GetCurSel());
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				CHI_CONVERTED[i][j] = m_data_base->convertCHItoA3(SelectedCompChi[i][j], temperature);
			}
		}
	}
	return CHI_CONVERTED;
}

std::vector<std::vector<double>> CMainTab2::GetFittedChi()
{
	return m_data_base->get_fitted_CHI();
}

void CMainTab2::ResetTab()
{
	m_EditIndividualSusXX.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusXY.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusXZ.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusYX.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusYY.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusYZ.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusZX.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusZY.SetWindowTextW(_T("0.00000"));
	m_EditIndividualSusZZ.SetWindowTextW(_T("0.00000"));
	OnIndividualChiEditKillfocus();
	m_ComboBoxCompChiTemp.SetCurSel(-1);
	m_ComboBoxCompChiTemp.ResetContent();
	m_EditCompSusXX.SetWindowTextW(_T("0.00000"));
	m_EditCompSusXY.SetWindowTextW(_T("0.00000"));
	m_EditCompSusXZ.SetWindowTextW(_T("0.00000"));
	m_EditCompSusYX.SetWindowTextW(_T("0.00000"));
	m_EditCompSusYY.SetWindowTextW(_T("0.00000"));
	m_EditCompSusYZ.SetWindowTextW(_T("0.00000"));
	m_EditCompSusZX.SetWindowTextW(_T("0.00000"));
	m_EditCompSusZY.SetWindowTextW(_T("0.00000"));
	m_EditCompSusZZ.SetWindowTextW(_T("0.00000"));
	m_EditCompSusISO.SetWindowTextW(_T("0.00000"));
	m_EditCompSusANISO.SetWindowTextW(_T("0.00000"));
	std::vector<std::vector<double>> empty_chi(3, std::vector<double>(3, 0.0));
	setFittedChi(empty_chi, false);
	OnBnClicked();
	m_EditTrustradius.SetWindowTextW(_T("0.1000"));
	OnTrustEditKillfocus();
	CButton* pRelCheckBox = (CButton*)GetDlgItem(IDC_CHECK_FIT_REL);
	pRelCheckBox->SetCheck(FALSE);
}

void CMainTab2::Initialize_StatErrorDlg(CStatError* dlg)
{
	m_StatError = dlg;
}

void CMainTab2::print_info(std::ofstream& file, int susc_type)
{
	double temperature = (double)m_pTab1->get_temperature();
	if (susc_type == 0)
	{ 
		file << "---------------------" << std::endl;
		file << "units: cm**3 K / mol" << std::endl;
		file << "---------------------" << std::endl;
		file << std::endl;
		file << "full tensor:" << std::endl;
		file << std::fixed << std::scientific << std::setprecision(12);
		for (size_t i = 0; i < 3; i++)
		{
			file << m_IndividualChi[i][0] << "\t" << m_IndividualChi[i][1] << "\t" << m_IndividualChi[i][2] << std::endl;
		}
		file << std::endl;
		file << "isotropic value\t" << m_IndividualChiIso << std::endl;
		file << "anisotropic value (frobenius norm)\t" << m_IndividualChiAnisoScalar << std::endl;
		file << std::endl;
		file << "anisotropic tensor:" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			file << m_IndividualChiAniso[i][0] << "\t" << m_IndividualChiAniso[i][1] << "\t" << m_IndividualChiAniso[i][2] << std::endl;
		}
		file << std::endl;
		file << std::endl;
		file << "-----------------------------------------------------------------------" << std::endl;
		file << "units: Angstroem**3 (at simulation temperature = " << temperature << " K)" << std::endl;
		file << "-----------------------------------------------------------------------" << std::endl;
		file << std::endl;
		file << "full tensor:" << std::endl;
		std::vector<std::vector<double>> IndividualChiSI = GetIndividualChi(temperature);
		for (size_t i = 0; i < 3; i++)
		{
			file << IndividualChiSI[i][0] << "\t" << IndividualChiSI[i][1] << "\t" << IndividualChiSI[i][2] << std::endl;
		}
		file << std::endl;
		
		double IndividualChiIsoSI = CalcChiIso(IndividualChiSI);
		std::vector<std::vector<double>> IndividualChiAnisoSI;
		double IndividualChiAnisoScalarSI = CalcChiAniso(IndividualChiSI,IndividualChiAnisoSI);
		file << "isotropic value\t" << IndividualChiIsoSI << std::endl;
		file << "anisotropic value (frobenius norm)\t" << IndividualChiAnisoScalarSI << std::endl;
		file << std::endl;
		file << "anisotropic tensor:" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			file << IndividualChiAnisoSI[i][0] << "\t" << IndividualChiAnisoSI[i][1] << "\t" << IndividualChiAnisoSI[i][2] << std::endl;
		}
		std::vector<double> e_val;
		std::string __warning;
		std::vector<std::vector<double>> e_vec = eigenvectors(IndividualChiSI, e_val, __warning);
		file << std::endl;
		file << std::endl;
		file << "-----------------------------------------------------------" << std::endl;
		file << "Eigenvectors (norm.) and eigenvalues in units: Angstroem**3" << std::endl;
		file << "-----------------------------------------------------------" << std::endl;
		file << std::endl;
		file << std::setw(12) << std::setfill(' ') << "Chi(tot)   ";
		for (size_t i = 0; i < 3; i++)
		{
			file << std::setw(20) << std::setfill(' ') << e_val[i];
		}
		file << std::endl;
		file << std::endl;
		file << "Orientation:" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			if (i == 0)
			{
				file << std::setw(12) << std::setfill(' ') << "X          ";
			}
			else if (i == 1)
			{
				file << std::setw(12) << std::setfill(' ') << "Y          ";
			}
			else
			{
				file << std::setw(12) << std::setfill(' ') << "Z          ";
			}
			for (size_t j = 0; j < 3; j++)
			{
				file << std::setw(20) << std::setfill(' ') << e_vec[i][j];
			}
			file << std::endl;
		}
		file << std::endl;
		if (__warning != "")
		{
			file << std::endl;
			file << "warnings:" << std::endl;
			file << __warning;
		}
	}
	else if (susc_type == 1)
	{
		if(m_ComboBoxCompChiTemp.GetCurSel() == -1)
		{
			file << "no \"computed\" spectrum was found";
		}
		else
		{
			std::vector<std::vector<double>> compSusTensor = m_data_base->get_mol_CHI(m_ComboBoxCompChiTemp.GetCurSel());
			file << "---------------------" << std::endl;
			file << "units: cm**3 K / mol" << std::endl;
			file << "---------------------" << std::endl;
			file << std::endl;
			file << "full tensor:" << std::endl;
			file << std::fixed << std::scientific << std::setprecision(12);
			for (size_t i = 0; i < 3; i++)
			{
				file << compSusTensor[i][0] << "\t" << compSusTensor[i][1] << "\t" << compSusTensor[i][2] << std::endl;
			}
			double compChiIso = CalcChiIso(compSusTensor);
			std::vector<std::vector<double>> compChiAniso;
			double compChiAnisoScalar = CalcChiAniso(compSusTensor, compChiAniso);
			file << std::endl;
			file << "isotropic value\t" << compChiIso << std::endl;
			file << "anisotropic value (frobenius norm)\t" << compChiAnisoScalar << std::endl;
			file << std::endl;
			file << "anisotropic tensor:" << std::endl;
			for (size_t i = 0; i < 3; i++)
			{
				file << compChiAniso[i][0] << "\t" << compChiAniso[i][1] << "\t" << compChiAniso[i][2] << std::endl;
			}
			file << std::endl;
			file << std::endl;
			CString CompTemp;
			m_ComboBoxCompChiTemp.GetWindowTextW(CompTemp);
			file << "-----------------------------------------------------------------------" << std::endl;
			file << "units: Angstroem**3 (at \"computation temperature\" = " << m_data_base->CStoS(CompTemp) << " K)" << std::endl;
			file << "-----------------------------------------------------------------------" << std::endl;
			file << std::endl;
			file << "full tensor:" << std::endl;
			double d_compTemp = (double)_tstof(CompTemp);
			std::vector<std::vector<double>> CompChiSI_compT = GetComputedChi(d_compTemp);
			for (size_t i = 0; i < 3; i++)
			{
				file << CompChiSI_compT[i][0] << "\t" << CompChiSI_compT[i][1] << "\t" << CompChiSI_compT[i][2] << std::endl;
			}
			file << std::endl;
			double CompChiIsoSI_compT = CalcChiIso(CompChiSI_compT);
			std::vector<std::vector<double>> CompChiAnisoSI_compT;
			double CompChiAnisoScalarSI_compT = CalcChiAniso(CompChiSI_compT, CompChiAnisoSI_compT);
			file << "isotropic value\t" << CompChiIsoSI_compT << std::endl;
			file << "anisotropic value (frobenius norm)\t" << CompChiAnisoScalarSI_compT << std::endl;
			file << std::endl;
			file << "anisotropic tensor:" << std::endl;
			for (size_t i = 0; i < 3; i++)
			{
				file << CompChiAnisoSI_compT[i][0] << "\t" << CompChiAnisoSI_compT[i][1] << "\t" << CompChiAnisoSI_compT[i][2] << std::endl;
			}
			file << std::endl;
			file << std::endl;

			file << "-----------------------------------------------------------------------" << std::endl;
			file << "units: Angstroem**3 (at simulation temperature = " << temperature << " K)" << std::endl;
			file << "-----------------------------------------------------------------------" << std::endl;
			file << std::endl;
			file << "full tensor:" << std::endl;
			std::vector<std::vector<double>> CompChiSI = GetComputedChi(temperature);
			for (size_t i = 0; i < 3; i++)
			{
				file << CompChiSI[i][0] << "\t" << CompChiSI[i][1] << "\t" << CompChiSI[i][2] << std::endl;
			}
			file << std::endl;

			double CompChiIsoSI = CalcChiIso(CompChiSI);
			std::vector<std::vector<double>> CompChiAnisoSI;
			double CompChiAnisoScalarSI = CalcChiAniso(CompChiSI, CompChiAnisoSI);
			file << "isotropic value\t" << CompChiIsoSI << std::endl;
			file << "anisotropic value (frobenius norm)\t" << CompChiAnisoScalarSI << std::endl;
			file << std::endl;
			file << "anisotropic tensor:" << std::endl;
			for (size_t i = 0; i < 3; i++)
			{
				file << CompChiAnisoSI[i][0] << "\t" << CompChiAnisoSI[i][1] << "\t" << CompChiAnisoSI[i][2] << std::endl;
			}
			std::vector<double> e_val;
			std::string __warning;
			std::vector<std::vector<double>> e_vec = eigenvectors(CompChiSI_compT, e_val, __warning);
			file << std::endl;
			file << std::endl;
			file << "----------------------------------------------------------------------------------" << std::endl;
			file << "Eigenvectors (norm.) and eigenvalues in units: Angstroem**3 (at comp. temperature)" << std::endl;
			file << "----------------------------------------------------------------------------------" << std::endl;
			file << std::endl;
			file << std::setw(12) << std::setfill(' ') << "Chi(tot)   ";
			for (size_t i = 0; i < 3; i++)
			{
				file << std::setw(20) << std::setfill(' ') << e_val[i];
			}
			file << std::endl;
			file << std::endl;
			file << "Orientation:" << std::endl;
			for (size_t i = 0; i < 3; i++)
			{
				if (i == 0)
				{
					file << std::setw(12) << std::setfill(' ') << "X          ";
				}
				else if (i == 1)
				{
					file << std::setw(12) << std::setfill(' ') << "Y          ";
				}
				else
				{
					file << std::setw(12) << std::setfill(' ') << "Z          ";
				}
				for (size_t j = 0; j < 3; j++)
				{
					file << std::setw(20) << std::setfill(' ') << e_vec[i][j];
				}
				file << std::endl;
			}
			file << std::endl;
			if (__warning != "")
			{
				file << std::endl;
				file << "warnings:" << std::endl;
				file << __warning;
			}
		}
	}
	else if (susc_type == 2)
	{
		file << "Tensor symmetry:" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			file << m_Sym[i][0] << "\t" << m_Sym[i][1] << "\t" << m_Sym[i][2] << std::endl;
		}
		file << std::endl;
		std::vector<std::vector<double>> fittedChiSI = m_data_base->get_fitted_CHI();
		std::vector<std::vector<double>> fittedChi(3, std::vector<double>(3, 0.0));
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				fittedChi[i][j] = m_data_base->convertCHItoCGI(fittedChiSI[i][j], temperature);
			}
		}
		file << "---------------------" << std::endl;
		file << "units: cm**3 K / mol" << std::endl;
		file << "---------------------" << std::endl;
		file << std::endl;
		file << "full tensor:" << std::endl;
		file << std::fixed << std::scientific << std::setprecision(12);
		for (size_t i = 0; i < 3; i++)
		{
			file << fittedChi[i][0] << "\t" << fittedChi[i][1] << "\t" << fittedChi[i][2] << std::endl;
		}
		file << std::endl;
		file << "isotropic value\t" << m_FitChiIso << std::endl;
		file << "anisotropic value (frobenius norm)\t" << m_FitChiAnisoScalar << std::endl;
		file << std::endl;
		file << "anisotropic tensor:" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			file << m_FitChiAniso[i][0] << "\t" << m_FitChiAniso[i][1] << "\t" << m_FitChiAniso[i][2] << std::endl;
		}
		file << std::endl;
		file << std::endl;
		file << "-----------------------------------------------------------------------" << std::endl;
		file << "units: Angstroem**3 (at simulation temperature = " << temperature << " K)" << std::endl;
		file << "-----------------------------------------------------------------------" << std::endl;
		file << std::endl;
		file << "full tensor:" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			file << fittedChiSI[i][0] << "\t" << fittedChiSI[i][1] << "\t" << fittedChiSI[i][2] << std::endl;
		}
		file << std::endl;
		double fittedChiIsoSI = CalcChiIso(fittedChiSI);
		std::vector<std::vector<double>> fittedChiAnisoSI;
		double fittedChiAnisoScalarSI = CalcChiAniso(fittedChiSI, fittedChiAnisoSI);
		file << "isotropic value\t" << fittedChiIsoSI << std::endl;
		file << "anisotropic value (frobenius norm)\t" << fittedChiAnisoScalarSI << std::endl;
		file << std::endl;
		file << "anisotropic tensor:" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			file << fittedChiAnisoSI[i][0] << "\t" << fittedChiAnisoSI[i][1] << "\t" << fittedChiAnisoSI[i][2] << std::endl;
		}
		std::vector<double> e_val;
		std::string __warning;
		std::vector<std::vector<double>> e_vec = eigenvectors(fittedChiSI, e_val, __warning);
		file << std::endl;
		file << std::endl;
		file << "-----------------------------------------------------------" << std::endl;
		file << "Eigenvectors (norm.) and eigenvalues in units: Angstroem**3" << std::endl;
		file << "-----------------------------------------------------------" << std::endl;
		file << std::endl;
		file << std::setw(12) << std::setfill(' ') << "Chi(tot)   ";
		if (e_vec.size() < 3)
		{
			file << "complex conjugate eigenvalues detected ... further real-valued processing stopped." << std::endl;
		}
		else
		{
			for (size_t i = 0; i < 3; i++)
			{
				file << std::setw(20) << std::setfill(' ') << e_val[i];
			}
		}
		file << std::endl;
		file << std::endl;
		file << "Orientation:" << std::endl;
		for (size_t i = 0; i < 3; i++)
		{
			if (i == 0)
			{
				file << std::setw(12) << std::setfill(' ') << "X          ";
			}
			else if (i == 1)
			{
				file << std::setw(12) << std::setfill(' ') << "Y          ";
			}
			else
			{
				file << std::setw(12) << std::setfill(' ') << "Z          ";
			}
			if (e_vec.size() > 2)
			{
				for (size_t j = 0; j < 3; j++)
				{
					file << std::setw(20) << std::setfill(' ') << e_vec[i][j];
				}
			}
			file << std::endl;
		}
		file << std::endl;
		if (__warning != "")
		{
			file << std::endl;
			file << "warnings:" << std::endl;
			file << __warning;
		}
	}
}