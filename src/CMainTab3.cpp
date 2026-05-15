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
#include "CMainTab3.h"
#include "afxdialogex.h"
#include "physical_constants.h"
#include <fstream>
#include "iomanip"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainTab3, CDialogEx)

CMainTab3::CMainTab3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_TAB3, pParent)
{
}

CMainTab3::~CMainTab3()
{
}

void CMainTab3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainTab3, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_T1E_VAL, &CMainTab3::OnT1EEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_T1E_EXPONENT, &CMainTab3::OnExpT1EEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_T2E_VAL, &CMainTab3::OnT2EEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_T2E_EXPONENT, &CMainTab3::OnExpT2EEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_TM_VAL, &CMainTab3::OnTMEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_TM_EXPONENT, &CMainTab3::OnExpTMEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_TR_VAL, &CMainTab3::OnTREditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_TR_EXPONENT, &CMainTab3::OnExpTREditKillfocus)
	ON_CBN_SELCHANGE(IDC_COMBO_REL_SOLVENT, &CMainTab3::OnCbnSelchangeSolvent)
	ON_BN_CLICKED(IDC_BUTTON_GET_SPHERICAL_VOL, &CMainTab3::OnBnClickedSphereVol)
	ON_BN_CLICKED(IDC_BUTTON_GET_ELLIPSOID_VOL, &CMainTab3::OnBnClickedEllipsoidVol)
END_MESSAGE_MAP()

void CMainTab3::Initialize_TAB1(CMainTab1* pTab1)
{
	m_pTab1 = pTab1;
	double B0 = (m_pTab1->get_MagField() * 1000000.0) / get_gyromagnetic_ratio("1H", -1, false);
	m_omegaE = -(get_gyromagnetic_ratio("E", -1, false) * B0);
	m_C = calculateC();
	m_D = calculateD();
}

BOOL CMainTab3::OnInitDialog()
{
	m_temperature = 298.0;
	m_viscosity = 1.0;
	m_mol_sphere_radius = 1.0;
	m_sphere_radius = 1.0;
	m_coordinates_for_fit.push_back({ 1.0, 1.0, 1.0 });
	m_SliderT1E.SubclassDlgItem(IDC_SLIDER_RELAX_T1E, this);
	m_SliderT2E.SubclassDlgItem(IDC_SLIDER_RELAX_T2E, this);
	m_SliderTM.SubclassDlgItem(IDC_SLIDER_RELAX_TM, this);
	m_SliderTR.SubclassDlgItem(IDC_SLIDER_RELAX_TR, this);
	m_SliderT1E.SetRange(0, 6000);
	m_SliderT1E.SetPos(0);
	m_SliderT2E.SetRange(0, 6000);
	m_SliderT2E.SetPos(0);
	m_SliderTM.SetRange(0, 6000);
	m_SliderTM.SetPos(0);
	m_SliderTR.SetRange(0, 5000);
	m_SliderTR.SetPos(0);
	T1E = 1E-13;
	T2E = 1E-13;
	TM = 1E-10;
	TR = 1E-11;
	m_EditValT1E.SubclassDlgItem(IDC_EDIT_T1E_VAL, this);
	m_EditValT2E.SubclassDlgItem(IDC_EDIT_T2E_VAL, this);
	m_EditValTM.SubclassDlgItem(IDC_EDIT_TM_VAL, this);
	m_EditValTR.SubclassDlgItem(IDC_EDIT_TR_VAL, this);
	m_EditValT1E.SetWindowTextW(_T("1.00"));
	m_EditValT2E.SetWindowTextW(_T("1.00"));
	m_EditValTM.SetWindowTextW(_T("1.00"));
	m_EditValTR.SetWindowTextW(_T("1.00"));
	m_EditExponentT1E.SubclassDlgItem(IDC_EDIT_T1E_EXPONENT, this);
	m_EditExponentT2E.SubclassDlgItem(IDC_EDIT_T2E_EXPONENT, this);
	m_EditExponentTM.SubclassDlgItem(IDC_EDIT_TM_EXPONENT, this);
	m_EditExponentTR.SubclassDlgItem(IDC_EDIT_TR_EXPONENT, this);
	m_EditExponentT1E.SetWindowTextW(_T("-13"));
	m_EditExponentT2E.SetWindowTextW(_T("-13"));
	m_EditExponentTM.SetWindowTextW(_T("-10"));
	m_EditExponentTR.SetWindowTextW(_T("-11"));
	m_solvent = { _T("none"),_T("water"),_T("acetonitrile"),_T("methanol"),_T("benzonitrile"),_T("ethanol"),_T("acetone"),_T("pyridine"),_T("o-dichlorobenzene"),_T("dichloromethane"),_T("tetrahydrofuran"),
		_T("fluorobenzene"),_T("chloroform"),_T("toluene"),_T("benzene"),_T("hexafluorobenzene"),_T("methylcyclohexane"),_T("cyclohexane"),_T("cyclopentane"),_T("hexane"),_T("pentane") };
	m_eta0 = { 0.0, 0.963918, 38.11, 8.37, 14.53, 4.25, 18.38, 12.29, 21.17, 24.84, 20.84, 18.66, 26.25, 14.83, 9.71, 5.86, 12.98, 7.15, 16.18, 13.17, 23.97 };
	m_eta1 = { 0.0, 2053.36619, 651.83407, 1243.82934, 1331.03672, 1648.96314, 838.60601, 1282.69917, 1235.60698, 837.84789, 919.95083, 1008.63135, 900.03509, 1082.6685,
		1231.11191, 1838.38041, 1183.10627, 1439.69132, 965.73807, 935.94424, 665.89508 };
	m_ComboBoxRelSolvent.SubclassDlgItem(IDC_COMBO_REL_SOLVENT, this);
	{
		CRect combo_rc;
		m_ComboBoxRelSolvent.GetWindowRect(&combo_rc);
		m_ComboBoxRelSolvent.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (10 * m_ComboBoxRelSolvent.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		for (size_t i = 0; i < m_solvent.size(); i++)
		{
			m_ComboBoxRelSolvent.AddString(m_solvent[i]);
		}
		m_ComboBoxRelSolvent.SetCurSel(0);
	}
	m_EditSphereRadius.SubclassDlgItem(IDC_EDIT_SPHERE_RADIUS, this);
	double volume = (4.0 / 3.0) * PI * pow(1.0, 3.0);
	CString formatted_text;
	formatted_text.Format(_T("%.4f"), volume);
	m_EditSphereRadius.SetWindowTextW(formatted_text);
	m_CubicAngstrom.SubclassDlgItem(IDC_STATIC_CUBIC_ANGSTROM, this);
	m_CubicAngstrom.SetWindowTextW(_T("\u212B**3"));
	return TRUE;
}

BOOL CMainTab3::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnT1EEditKillfocus();
			OnT2EEditKillfocus();
			OnTMEditKillfocus();
			OnTREditKillfocus();
			OnExpT1EEditKillfocus();
			OnExpT2EEditKillfocus();
			OnExpTMEditKillfocus();
			OnExpTREditKillfocus();
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMainTab3::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int Slider_nPos;
	double sliderValue;
	CString baseValue_str;
	CString expValue_str;
	bool slided = false;
	if (pScrollBar->GetSafeHwnd() == m_SliderT1E.GetSafeHwnd())
	{
		Slider_nPos = m_SliderT1E.GetPos();
		sliderValue = 1E-13 * (pow(10.0, floor(Slider_nPos / 1000.0)) * (1.0 + (Slider_nPos % 1000) * 0.009));
		T1E = sliderValue;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValT1E.SetWindowTextW(baseValue_str);
		m_EditExponentT1E.SetWindowTextW(expValue_str);
		m_C = calculateC();
		T2E = calculateT2E();
		sliderValue = T2E;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValT2E.SetWindowTextW(baseValue_str);
		m_EditExponentT2E.SetWindowTextW(expValue_str);
		SetT2ESlider(T2E, false);
		slided = true;
	}
	else if (pScrollBar->GetSafeHwnd() == m_SliderT2E.GetSafeHwnd())
	{
		Slider_nPos = m_SliderT2E.GetPos();
		sliderValue = 1E-13 * (pow(10.0, floor(Slider_nPos / 1000.0)) * (1.0 + (Slider_nPos % 1000) * 0.009));
		T2E = sliderValue;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValT2E.SetWindowTextW(baseValue_str);
		m_EditExponentT2E.SetWindowTextW(expValue_str);
		m_D = calculateD();
		if (m_D < 0.0)
		{
			m_D = 0.0;
			T1E = 0.5 * T2E;
			m_C = calculateC();
			sliderValue = T1E;
			expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
			baseValue_str.Format(_T("%.2f"), sliderValue);
			m_EditValT1E.SetWindowTextW(baseValue_str);
			m_EditExponentT1E.SetWindowTextW(expValue_str);
			SetT1ESlider(T1E, false);
		}
		slided = true;
	}
	else if (pScrollBar->GetSafeHwnd() == m_SliderTM.GetSafeHwnd())
	{
		Slider_nPos = m_SliderTM.GetPos();
		sliderValue = 1E-10 * (pow(10.0, floor(Slider_nPos / 1000.0)) * (1.0 + (Slider_nPos % 1000) * 0.009));
		TM = sliderValue;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValTM.SetWindowTextW(baseValue_str);
		m_EditExponentTM.SetWindowTextW(expValue_str);
		T2E = calculateT2E();
		sliderValue = T2E;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValT2E.SetWindowTextW(baseValue_str);
		m_EditExponentT2E.SetWindowTextW(expValue_str);
		SetT2ESlider(T2E, false);
		slided = true;
	}
	else if (pScrollBar->GetSafeHwnd() == m_SliderTR.GetSafeHwnd())
	{ 
		Slider_nPos = m_SliderTR.GetPos();
		sliderValue = 1E-11 * (pow(10.0, floor(Slider_nPos / 1000.0)) * (1.0 + (Slider_nPos % 1000) * 0.009));
		TR = sliderValue;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValTR.SetWindowTextW(baseValue_str);
		m_EditExponentTR.SetWindowTextW(expValue_str);
		int selection = m_ComboBoxRelSolvent.GetCurSel();
		if (selection != -1 && selection != 0)
		{
			m_sphere_radius = pow(((3.0 * k_B * m_temperature * TR) / (4.0 * PI * m_viscosity)), (1.0 / 3.0)) * pow(10.0, 10.0);
			double sphere_volume = (4.0/3.0) * PI * pow(m_sphere_radius, 3.0);
			CString formatted_volume_str;
			formatted_volume_str.Format(_T("%.4f"), sphere_volume);
			m_EditSphereRadius.SetWindowTextW(formatted_volume_str);
		}
		T1E = calculateT1E();
		sliderValue = T1E;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValT1E.SetWindowTextW(baseValue_str);
		m_EditExponentT1E.SetWindowTextW(expValue_str);
		SetT1ESlider(T1E, false);
		T2E = calculateT2E();
		sliderValue = T2E;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValT2E.SetWindowTextW(baseValue_str);
		m_EditExponentT2E.SetWindowTextW(expValue_str);
		SetT2ESlider(T2E, false);
		slided = true;
	}
	m_pTab1->tab3_sim_start();
}

int CMainTab3::ScientificNumber(double& value)
{
	int exponent = 0;
	if (value <= 0.0)
	{
		value = 1.0;
		return exponent;
	}
	while (value >= 10.0)
	{
		value /= 10.0;
		exponent++;
	}
	while (value < 0.99999 && value > 0.0)
	{
		value *= 10.0;
		exponent--;
	}
	return exponent;
}

double CMainTab3::GetT1E()
{
	return T1E;
}

double CMainTab3::GetT2E()
{
	return T2E;
}

double CMainTab3::GetTM()
{
	return TM;
}

double CMainTab3::GetTR()
{
	return TR;
}

void CMainTab3::OnT1EEditKillfocus()
{
	CString editVal;
	CString editExp;
	m_EditValT1E.GetWindowTextW(editVal);
	m_EditExponentT1E.GetWindowTextW(editExp);
	double value = _tstof(editVal);
	int exponent = _tstoi(editExp);
	if (value <= 0.0)
	{
		value = 1.0;
		CString formattedText;
		formattedText.Format(_T("%.2f"), value);
		m_EditValT1E.SetWindowTextW(formattedText);
	}
	value = value * pow(10.0, exponent);
	SetT1ESlider(value, false);
	m_C = calculateC();
	CString expValue_str;
	CString baseValue_str;
	T2E = calculateT2E();
	double SliderValue = T2E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT2E.SetWindowTextW(baseValue_str);
	m_EditExponentT2E.SetWindowTextW(expValue_str);
	SetT2ESlider(T2E, true);
}

void CMainTab3::OnT2EEditKillfocus()
{
	CString editVal;
	CString editExp;
	m_EditValT2E.GetWindowTextW(editVal);
	m_EditExponentT2E.GetWindowTextW(editExp);
	double value = _tstof(editVal);
	int exponent = _tstoi(editExp);
	if (value <= 0.0)
	{
		value = 1.0;
		CString formattedText;
		formattedText.Format(_T("%.2f"), value);
		m_EditValT2E.SetWindowTextW(formattedText);
	}
	value = value * pow(10.0, exponent);
	m_D = calculateD();
	if (m_D < 0.0)
	{
		SetT2ESlider(value, false);
		m_D = 0.0;
		T1E = 0.5 * T2E;
		m_C = calculateC();
		double sliderValue = T1E;
		CString expValue_str, baseValue_str;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValT1E.SetWindowTextW(baseValue_str);
		m_EditExponentT1E.SetWindowTextW(expValue_str);
		SetT1ESlider(T1E, true);
	}
	else
	{
		SetT2ESlider(value, true);
	}
}

void CMainTab3::OnTMEditKillfocus()
{
	CString editVal;
	CString editExp;
	m_EditValTM.GetWindowTextW(editVal);
	m_EditExponentTM.GetWindowTextW(editExp);
	double value = _tstof(editVal);
	int exponent = _tstoi(editExp);
	if (value <= 0.0)
	{
		value = 1.0;
		CString formattedText;
		formattedText.Format(_T("%.2f"), value);
		m_EditValTM.SetWindowTextW(formattedText);
	}
	value = value * pow(10.0, exponent);
	SetTMSlider(value, false);
	CString expValue_str;
	CString baseValue_str;
	T2E = calculateT2E();
	double SliderValue = T2E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT2E.SetWindowTextW(baseValue_str);
	m_EditExponentT2E.SetWindowTextW(expValue_str);
	SetT2ESlider(T2E, true);
}

void CMainTab3::OnTREditKillfocus()
{
	CString editVal;
	CString editExp;
	m_EditValTR.GetWindowTextW(editVal);
	m_EditExponentTR.GetWindowTextW(editExp);
	double value = _tstof(editVal);
	int exponent = _tstoi(editExp);
	if (value <= 0.0)
	{
		value = 1.0;
		CString formattedText;
		formattedText.Format(_T("%.2f"), value);
		m_EditValTR.SetWindowTextW(formattedText);
	}
	value = value * pow(10.0, exponent);
	SetTRSlider(value, false);
	int selection = m_ComboBoxRelSolvent.GetCurSel();
	if (selection != -1 && selection != 0)
	{
		m_sphere_radius = pow(((3.0 * k_B * m_temperature * TR) / (4.0 * PI * m_viscosity)), (1.0 / 3.0)) * pow(10.0, 10.0);
		double sphere_volume = (4.0 / 3.0) * PI * pow(m_sphere_radius, 3.0);
		CString formatted_volume_str;
		formatted_volume_str.Format(_T("%.4f"), sphere_volume);
		m_EditSphereRadius.SetWindowTextW(formatted_volume_str);
	}
	CString expValue_str;
	CString baseValue_str;
	T1E = calculateT1E();
	double SliderValue = T1E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT1E.SetWindowTextW(baseValue_str);
	m_EditExponentT1E.SetWindowTextW(expValue_str);
	SetT1ESlider(T1E, false);
	T2E = calculateT2E();
	SliderValue = T2E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT2E.SetWindowTextW(baseValue_str);
	m_EditExponentT2E.SetWindowTextW(expValue_str);
	SetT2ESlider(T2E, true);
}

void CMainTab3::OnExpT1EEditKillfocus()
{
	CString editVal;
	CString editExp;
	m_EditValT1E.GetWindowTextW(editVal);
	m_EditExponentT1E.GetWindowTextW(editExp);
	double value = _tstof(editVal);
	int exponent = _tstoi(editExp);
	CString formattedText;
	formattedText.Format(_T("%d"), exponent);
	m_EditExponentT1E.SetWindowTextW(formattedText);
	value = value * pow(10.0, exponent);
	SetT1ESlider(value, false);
	m_C = calculateC();
	CString expValue_str;
	CString baseValue_str;
	T2E = calculateT2E();
	double SliderValue = T2E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT2E.SetWindowTextW(baseValue_str);
	m_EditExponentT2E.SetWindowTextW(expValue_str);
	SetT2ESlider(T2E, true);
}

void CMainTab3::OnExpT2EEditKillfocus()
{
	CString editVal;
	CString editExp;
	m_EditValT2E.GetWindowTextW(editVal);
	m_EditExponentT2E.GetWindowTextW(editExp);
	double value = _tstof(editVal);
	int exponent = _tstoi(editExp);
	CString formattedText;
	formattedText.Format(_T("%d"), exponent);
	m_EditExponentT2E.SetWindowTextW(formattedText);
	value = value * pow(10.0, exponent);
	m_D = calculateD();
	if (m_D < 0.0)
	{
		SetT2ESlider(value, false);
		m_D = 0.0;
		T1E = 0.5 * T2E;
		m_C = calculateC();
		double sliderValue = T1E;
		CString expValue_str, baseValue_str;
		expValue_str.Format(_T("%d"), ScientificNumber(sliderValue));
		baseValue_str.Format(_T("%.2f"), sliderValue);
		m_EditValT1E.SetWindowTextW(baseValue_str);
		m_EditExponentT1E.SetWindowTextW(expValue_str);
		SetT1ESlider(T1E, true);
	}
	else
	{
		SetT2ESlider(value, true);
	}
}

void CMainTab3::OnExpTMEditKillfocus()
{
	CString editVal;
	CString editExp;
	m_EditValTM.GetWindowTextW(editVal);
	m_EditExponentTM.GetWindowTextW(editExp);
	double value = _tstof(editVal);
	int exponent = _tstoi(editExp);
	CString formattedText;
	formattedText.Format(_T("%d"), exponent);
	m_EditExponentTM.SetWindowTextW(formattedText);
	value = value * pow(10.0, exponent);
	SetTMSlider(value, false);
	CString expValue_str;
	CString baseValue_str;
	T2E = calculateT2E();
	double SliderValue = T2E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT2E.SetWindowTextW(baseValue_str);
	m_EditExponentT2E.SetWindowTextW(expValue_str);
	SetT2ESlider(T2E, true);
}

void CMainTab3::OnExpTREditKillfocus()
{
	CString editVal;
	CString editExp;
	m_EditValTR.GetWindowTextW(editVal);
	m_EditExponentTR.GetWindowTextW(editExp);
	double value = _tstof(editVal);
	int exponent = _tstoi(editExp);
	CString formattedText;
	formattedText.Format(_T("%d"), exponent);
	m_EditExponentTR.SetWindowTextW(formattedText);
	value = value * pow(10.0, exponent);
	SetTRSlider(value, false);
	int selection = m_ComboBoxRelSolvent.GetCurSel();
	if (selection != -1 && selection != 0)
	{
		m_sphere_radius = pow(((3.0 * k_B * m_temperature * TR) / (4.0 * PI * m_viscosity)), (1.0 / 3.0)) * pow(10.0, 10.0);
		double sphere_volume = (4.0 / 3.0) * PI * pow(m_sphere_radius, 3.0);
		CString formatted_volume_str;
		formatted_volume_str.Format(_T("%.4f"), sphere_volume);
		m_EditSphereRadius.SetWindowTextW(formatted_volume_str);
	}
	CString expValue_str;
	CString baseValue_str;
	T1E = calculateT1E();
	double SliderValue = T1E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT1E.SetWindowTextW(baseValue_str);
	m_EditExponentT1E.SetWindowTextW(expValue_str);
	SetT1ESlider(T1E, false);
	T2E = calculateT2E();
	SliderValue = T2E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT2E.SetWindowTextW(baseValue_str);
	m_EditExponentT2E.SetWindowTextW(expValue_str);
	SetT2ESlider(T2E, true);
}

void CMainTab3::SetT1ESlider(double value, bool updateSim)
{
	T1E = value;
	m_SliderT1E.SetPos(GetSliderPosByValue(value, 0, 6000, 1E-13));
	if (updateSim)
	{
		m_pTab1->tab3_sim_start();
	}
}

void CMainTab3::SetT2ESlider(double value, bool updateSim)
{
	T2E = value;
	m_SliderT2E.SetPos(GetSliderPosByValue(value, 0, 6000, 1E-13));
	if (updateSim)
	{
		m_pTab1->tab3_sim_start();
	}
}

void CMainTab3::SetTMSlider(double value, bool updateSim)
{
	TM = value;
	m_SliderTM.SetPos(GetSliderPosByValue(value, 0, 6000, 1E-10));
	if (updateSim)
	{
		m_pTab1->tab3_sim_start();
	}
}

void CMainTab3::SetTRSlider(double value, bool updateSim)
{
	TR = value;
	m_SliderTR.SetPos(GetSliderPosByValue(value, 0, 5000, 1E-11));
	if (updateSim)
	{
		m_pTab1->tab3_sim_start();
	}
}

int CMainTab3::GetSliderPosByValue(double value, int slider_min, int slider_max, double min_E)
{
	double sliderValue_min = min_E * (pow(10.0, floor(slider_min / 1000.0)) * (1.0 + (slider_min % 1000) * 0.009));
	double sliderValue_max = min_E * (pow(10.0, floor(slider_max / 1000.0)) * (1.0 + (slider_max % 1000) * 0.009));
	if (value <= sliderValue_min)
	{
		return slider_min;
	}
	else if (value >= sliderValue_max)
	{
		return slider_max;
	}
	else
	{
		int counter = 1000;
		int i = 1000;
		bool run = true;
		while (run)
		{
			if (counter != 1)
			{
				if (value <= (min_E * (pow(10.0, floor(i / 1000.0)) * (1.0 + (i % 1000) * 0.009))))
				{
					i -= counter;
					counter /= 10;
				}
				else
				{
					i += counter;
				}
			}
			else
			{
				if (value >= (min_E * (pow(10.0, floor(i / 1000.0)) * (1.0 + (i % 1000) * 0.009))) || value < (min_E * (pow(10.0, floor((i + 1) / 1000.0)) * (1.0 + ((i + 1) % 1000) * 0.009))))
				{
					run = false;
					return i;
				}
				else
				{
					i += counter;
				}
			}
		}
	}
}

void CMainTab3::OnB0Change()
{
	double B0 = (m_pTab1->get_MagField() * 1000000.0) / get_gyromagnetic_ratio("1H", -1, false);
	m_omegaE = -(get_gyromagnetic_ratio("E", -1, false) * B0);
	CString expValue_str;
	CString baseValue_str;
	T1E = calculateT1E();
	double SliderValue = T1E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT1E.SetWindowTextW(baseValue_str);
	m_EditExponentT1E.SetWindowTextW(expValue_str);
	SetT1ESlider(T1E, false);
	T2E = calculateT2E();
	SliderValue = T2E;
	expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
	baseValue_str.Format(_T("%.2f"), SliderValue);
	m_EditValT2E.SetWindowTextW(baseValue_str);
	m_EditExponentT2E.SetWindowTextW(expValue_str);
	SetT2ESlider(T2E, true);
}

void CMainTab3::OnCbnSelchangeSolvent()
{
	int selection = m_ComboBoxRelSolvent.GetCurSel();
	if (selection != -1)
	{
		if (selection == 0)
		{
			//
		}
		else
		{
			m_viscosity = m_eta0[selection] * exp(m_eta1[selection] / m_temperature);
			m_viscosity /= 1000.0;
			TR = (4.0 * PI * m_viscosity * pow((m_sphere_radius * pow(10.0, -10.0)), 3.0)) / (3.0 * k_B * m_temperature);
			CString expValue_str;
			CString baseValue_str;
			double SliderValue = TR;
			expValue_str.Format(_T("% d"), ScientificNumber(SliderValue));
			baseValue_str.Format(_T("%.2f"), SliderValue);
			m_EditValTR.SetWindowTextW(baseValue_str);
			m_EditExponentTR.SetWindowTextW(expValue_str);
			SetTRSlider(TR, false);
			T1E = calculateT1E();
			SliderValue = T1E;
			expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
			baseValue_str.Format(_T("%.2f"), SliderValue);
			m_EditValT1E.SetWindowTextW(baseValue_str);
			m_EditExponentT1E.SetWindowTextW(expValue_str);
			SetT1ESlider(T1E, false);
			T2E = calculateT2E();
			SliderValue = T2E;
			expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
			baseValue_str.Format(_T("%.2f"), SliderValue);
			m_EditValT2E.SetWindowTextW(baseValue_str);
			m_EditExponentT2E.SetWindowTextW(expValue_str);
			SetT2ESlider(T2E, true);
		}
	}
}

void CMainTab3::OnTempChange()
{
	m_temperature = m_pTab1->get_temperature();
	int selection = m_ComboBoxRelSolvent.GetCurSel();
	if (selection != -1 && selection != 0)
	{
		m_viscosity = m_eta0[selection] * exp(m_eta1[selection] / m_temperature);
		m_viscosity /= 1000.0;
		TR = (4.0 * PI * m_viscosity * pow((m_sphere_radius * pow(10.0, -10.0)), 3.0)) / (3.0 * k_B * m_temperature);
		CString expValue_str;
		CString baseValue_str;
		double SliderValue = TR;
		expValue_str.Format(_T("% d"), ScientificNumber(SliderValue));
		baseValue_str.Format(_T("%.2f"), SliderValue);
		m_EditValTR.SetWindowTextW(baseValue_str);
		m_EditExponentTR.SetWindowTextW(expValue_str);
		SetTRSlider(TR, false);
		T1E = calculateT1E();
		SliderValue = T1E;
		expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
		baseValue_str.Format(_T("%.2f"), SliderValue);
		m_EditValT1E.SetWindowTextW(baseValue_str);
		m_EditExponentT1E.SetWindowTextW(expValue_str);
		SetT1ESlider(T1E, false);
		T2E = calculateT2E();
		SliderValue = T2E;
		expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
		baseValue_str.Format(_T("%.2f"), SliderValue);
		m_EditValT2E.SetWindowTextW(baseValue_str);
		m_EditExponentT2E.SetWindowTextW(expValue_str);
		SetT2ESlider(T2E, true);
	}
}

void CMainTab3::SetMoleculeSphereRadius(double radius)
{
	m_mol_sphere_radius = radius;
	m_sphere_radius = radius; 
	double volume = (4.0 / 3.0) * PI * pow(m_sphere_radius, 3.0);
	CString formatted_text;
	formatted_text.Format(_T("%.4f"), volume);
	m_EditSphereRadius.SetWindowTextW(formatted_text);
	m_ComboBoxRelSolvent.SetCurSel(0);
}

void CMainTab3::OnBnClickedSphereVol()
{
	m_sphere_radius = m_mol_sphere_radius;
	double volume = (4.0 / 3.0) * PI * pow(m_sphere_radius, 3.0);
	print_vol = volume;
	CString formatted_text;
	formatted_text.Format(_T("%.4f"), volume);
	m_EditSphereRadius.SetWindowTextW(formatted_text);
	int selection = m_ComboBoxRelSolvent.GetCurSel();
	if (selection != -1 && selection != 0)
	{
		TR = (4.0 * PI * m_viscosity * pow((m_sphere_radius * pow(10.0, -10.0)), 3.0)) / (3.0 * k_B * m_temperature);
		CString expValue_str;
		CString baseValue_str;
		double SliderValue = TR;
		expValue_str.Format(_T("% d"), ScientificNumber(SliderValue));
		baseValue_str.Format(_T("%.2f"), SliderValue);
		m_EditValTR.SetWindowTextW(baseValue_str);
		m_EditExponentTR.SetWindowTextW(expValue_str);
		SetTRSlider(TR, false);
		T1E = calculateT1E();
		SliderValue = T1E;
		expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
		baseValue_str.Format(_T("%.2f"), SliderValue);
		m_EditValT1E.SetWindowTextW(baseValue_str);
		m_EditExponentT1E.SetWindowTextW(expValue_str);
		SetT1ESlider(T1E, false);
		T2E = calculateT2E();
		SliderValue = T2E;
		expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
		baseValue_str.Format(_T("%.2f"), SliderValue);
		m_EditValT2E.SetWindowTextW(baseValue_str);
		m_EditExponentT2E.SetWindowTextW(expValue_str);
		SetT2ESlider(T2E, true);
	}
}

void CMainTab3::OnBnClickedEllipsoidVol()
{
	double new_volume = 1.0;
	if (m_coordinates_for_fit.size() > 1)
	{
		std::vector<double> center = { 0.0, 0.0, 0.0 };
		for (size_t i = 0; i < m_coordinates_for_fit.size(); i++)
		{
			center[0] += m_coordinates_for_fit[i][0];
			center[1] += m_coordinates_for_fit[i][1];
			center[2] += m_coordinates_for_fit[i][2];
		}
		center[0] /= static_cast<int>(m_coordinates_for_fit.size());
		center[1] /= static_cast<int>(m_coordinates_for_fit.size());
		center[2] /= static_cast<int>(m_coordinates_for_fit.size());
		double SphereRadius = m_mol_sphere_radius * 2;
		NelderMeadSimplex* SmallestSphereOptimizer = new NelderMeadSimplex(ComputeSmallestSphere, nullptr);
		SmallestSphereOptimizer->InitializeBreak("BoxFit");
		SmallestSphereOptimizer->set_convergence_criteria(5000, 1E-8, 1E-8);
		SmallestSphereOptimizer->set_optimization_parameter(1.0, 1.0, 0.5, 0.5);
		SmallestSphereOptimizer->fconv_type(1);
		std::vector<double> parameterset1 = { center[0], center[1], center[2] };
		if (SmallestSphereOptimizer->downhillsimplex(parameterset1, 0.2, false))
		{
			center[0] = SmallestSphereOptimizer->get_optimized_parameter(0);
			center[0] = SmallestSphereOptimizer->get_optimized_parameter(1);
			center[0] = SmallestSphereOptimizer->get_optimized_parameter(2);
			SphereRadius = SmallestSphereOptimizer->get_result_value();
			double theta = 0.0;
			double phi = 0.0;
			double tau = 0.0;
			std::vector<double> parameterset2 = { theta, phi };
			NelderMeadSimplex* GroundOptimizer = new NelderMeadSimplex(GroundMolecule, nullptr);
			GroundOptimizer->InitializeBreak("BoxFit");
			GroundOptimizer->set_convergence_criteria(5000, 1E-8, 1E-8);
			GroundOptimizer->set_optimization_parameter(1.0, 1.0, 0.5, 0.5);
			GroundOptimizer->fconv_type(1);
			if (GroundOptimizer->downhillsimplex(parameterset2, 0.1, false))
			{
				theta = GroundOptimizer->get_optimized_parameter(0);
				phi = GroundOptimizer->get_optimized_parameter(1);
				for (size_t i = 0; i < m_positioned_coordinates.size(); i++)
				{
					m_positioned_coordinates[i][0] = cos(phi) * m_positioned_coordinates[i][0] + sin(theta) * sin(phi) * m_positioned_coordinates[i][1] + cos(theta) * sin(phi) * m_positioned_coordinates[i][2];
					m_positioned_coordinates[i][1] = cos(theta) * m_positioned_coordinates[i][1] - sin(theta) * m_positioned_coordinates[i][2];
					m_positioned_coordinates[i][2] = -sin(phi) * m_positioned_coordinates[i][0] + sin(theta) * cos(phi) * m_positioned_coordinates[i][1] + cos(theta) * cos(phi) * m_positioned_coordinates[i][2];
				}
				parameterset2[0] = 0.0;
				parameterset2[1] = 0.0;
				NelderMeadSimplex* GroundOnOptimizer = new NelderMeadSimplex(GroundOnMolecule, nullptr);
				GroundOnOptimizer->InitializeBreak("BoxFit");
				GroundOnOptimizer->set_convergence_criteria(5000, 1E-8, 1E-8);
				GroundOnOptimizer->set_optimization_parameter(1.0, 1.0, 0.5, 0.5);
				GroundOnOptimizer->fconv_type(1);
				if (GroundOnOptimizer->downhillsimplex(parameterset2, 0.1, false))
				{
					std::vector<double> top_plane;
					top_plane = ComputePlane({ 1.0,1.0,1.0 }, { -1.0, 1.0, 1.0 }, { 1.0, -1.0, 1.0 }, -GroundOnOptimizer->get_optimized_parameter(0), -GroundOnOptimizer->get_optimized_parameter(1), 0.0);
					std::vector<double> FLBR(4, 0.0);
					double ground = INFINITY;
					for (size_t i = 0; i < m_positioned_coordinates.size(); i++)
					{
						if (m_positioned_coordinates[i][0] > FLBR[0])
						{
							FLBR[0] = m_positioned_coordinates[i][0];
						}
						if (m_positioned_coordinates[i][1] < FLBR[1])
						{
							FLBR[1] = m_positioned_coordinates[i][1];
						}
						if (m_positioned_coordinates[i][0] < FLBR[2])
						{
							FLBR[2] = m_positioned_coordinates[i][2];
						}
						if (m_positioned_coordinates[i][1] > FLBR[3])
						{
							FLBR[3] = m_positioned_coordinates[i][1];
						}
						if (m_positioned_coordinates[i][2] < ground)
						{
							ground = m_positioned_coordinates[i][2];
						}
					}
					std::vector<std::vector<double>> TopEdges(4);
					TopEdges[0] = { FLBR[0], FLBR[3], ComputeMissingPlaneCoordinate(3, FLBR[0], FLBR[3], 0.0, top_plane) };
					TopEdges[1] = { FLBR[0], FLBR[1], ComputeMissingPlaneCoordinate(3, FLBR[0], FLBR[1], 0.0, top_plane) };
					TopEdges[2] = { FLBR[2], FLBR[1], ComputeMissingPlaneCoordinate(3, FLBR[2], FLBR[1], 0.0, top_plane) };
					TopEdges[3] = { FLBR[2], FLBR[3], ComputeMissingPlaneCoordinate(3, FLBR[2], FLBR[3], 0.0, top_plane) };
					std::vector<std::vector<double>> BottomEdges(4);
					BottomEdges[0] = { FLBR[0], FLBR[3], ground };
					BottomEdges[1] = { FLBR[0], FLBR[1], ground };
					BottomEdges[2] = { FLBR[2], FLBR[1], ground };
					BottomEdges[3] = { FLBR[2], FLBR[3], ground };
					std::vector<double> centereight(3, 0.0);
					for (size_t i = 0; i < 4; i++)
					{
						center[0] += TopEdges[i][0] + BottomEdges[i][0];
						center[1] += TopEdges[i][1] + BottomEdges[i][1];
						center[2] += TopEdges[i][2] + BottomEdges[i][2];
					}
					center[0] /= 8.0;
					center[1] /= 8.0;
					center[2] /= 8.0;
					std::vector<double> tetraeder_volumes(18, 0.0);
					tetraeder_volumes[0] = ComputeVolumeTetraeder(center, BottomEdges[0], TopEdges[0], TopEdges[1]);
					tetraeder_volumes[1] = ComputeVolumeTetraeder(center, TopEdges[1], BottomEdges[1], BottomEdges[0]);
					tetraeder_volumes[2] = ComputeVolumeTetraeder(center, BottomEdges[3], TopEdges[3], TopEdges[2]);
					tetraeder_volumes[3] = ComputeVolumeTetraeder(center, TopEdges[2], BottomEdges[2], BottomEdges[3]);
					tetraeder_volumes[4] = ComputeVolumeTetraeder(center, TopEdges[1], BottomEdges[1], BottomEdges[2]);
					tetraeder_volumes[5] = ComputeVolumeTetraeder(center, TopEdges[1], TopEdges[2], BottomEdges[2]);
					tetraeder_volumes[6] = ComputeVolumeTetraeder(center, TopEdges[0], BottomEdges[3], BottomEdges[0]);
					tetraeder_volumes[7] = ComputeVolumeTetraeder(center, TopEdges[0], TopEdges[3], BottomEdges[3]);
					tetraeder_volumes[8] = ComputeVolumeTetraeder(center, TopEdges[0], TopEdges[1], TopEdges[3]);
					tetraeder_volumes[9] = ComputeVolumeTetraeder(center, TopEdges[1], TopEdges[2], TopEdges[3]);
					tetraeder_volumes[10] = ComputeVolumeTetraeder(center, BottomEdges[0], BottomEdges[2], BottomEdges[3]);
					tetraeder_volumes[11] = ComputeVolumeTetraeder(center, BottomEdges[0], BottomEdges[2], BottomEdges[1]);
					new_volume = 0.0;
					for (size_t i = 0; i < 12; i++)
					{
						new_volume += tetraeder_volumes[i];
					}
				}
				else
				{
					AfxMessageBox(_T("Unable to find top plane."));
				}
				delete GroundOnOptimizer;
			}
			else
			{
				AfxMessageBox(_T("Unable to find smallest enclosing box."));
			}
			delete GroundOptimizer;
		}
		else
		{
			AfxMessageBox(_T("Unable to find the center of the molecule."));
		}
		delete SmallestSphereOptimizer;
	}
	CString formatted_text;
	formatted_text.Format(_T("%.4f"), new_volume);
	print_vol = new_volume;
	m_EditSphereRadius.SetWindowTextW(formatted_text);
	m_sphere_radius = pow((3.0 * new_volume) / (4.0 * PI), 1.0 / 3.0);
	int selection = m_ComboBoxRelSolvent.GetCurSel();
	if (selection != -1 && selection != 0)
	{
		TR = (4.0 * PI * m_viscosity * pow((m_sphere_radius * pow(10.0, -10.0)), 3.0)) / (3.0 * k_B * m_temperature);
		CString expValue_str;
		CString baseValue_str;
		double SliderValue = TR;
		expValue_str.Format(_T("% d"), ScientificNumber(SliderValue));
		baseValue_str.Format(_T("%.2f"), SliderValue);
		m_EditValTR.SetWindowTextW(baseValue_str);
		m_EditExponentTR.SetWindowTextW(expValue_str);
		SetTRSlider(TR, false);
		T1E = (TR * (1.0 + pow((m_omegaE * TR), 2.0))) / m_C;
		SliderValue = T1E;
		expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
		baseValue_str.Format(_T("%.2f"), SliderValue);
		m_EditValT1E.SetWindowTextW(baseValue_str);
		m_EditExponentT1E.SetWindowTextW(expValue_str);
		SetT1ESlider(T1E, false);
		T2E = 1.0 / ((1.0 / T1E) + (m_D / TM));
		SliderValue = T2E;
		expValue_str.Format(_T("%d"), ScientificNumber(SliderValue));
		baseValue_str.Format(_T("%.2f"), SliderValue);
		m_EditValT2E.SetWindowTextW(baseValue_str);
		m_EditExponentT2E.SetWindowTextW(expValue_str);
		SetT2ESlider(T2E, true);
	}
}

std::vector<std::vector<double>> CMainTab3::m_coordinates_for_fit;
std::vector<std::vector<double>> CMainTab3::m_positioned_coordinates;
double CMainTab3::tmp_volume;

void CMainTab3::SetMoleculeCoordinates(std::vector<std::vector<double>> xyz)
{
	m_coordinates_for_fit.clear();
	m_coordinates_for_fit = xyz;
}

double CMainTab3::ComputeSmallestSphere(std::vector<double> parameters)
{
	std::vector<double> center = { parameters[0], parameters[1], parameters[2] };
	double radius = 0.0;
	double tmp_radius;
	std::vector<std::vector<double>> p(m_coordinates_for_fit.size(), std::vector<double>(3,0.0));
	for (size_t i = 0; i < m_coordinates_for_fit.size(); i++)
	{
		p[i][0] = m_coordinates_for_fit[i][0] - center[0];
		p[i][1] = m_coordinates_for_fit[i][1] - center[1];
		p[i][2] = m_coordinates_for_fit[i][2] - center[2];
		tmp_radius = sqrt(pow(p[i][0], 2.0) + pow(p[i][1], 2.0) + pow(p[i][2], 2.0));
		if (tmp_radius > radius)
		{
			radius = tmp_radius;
		}
	}
	m_positioned_coordinates = p;
	return radius;
}

double CMainTab3::GroundMolecule(std::vector<double> parameters)
{
	double theta = parameters[0];
	double phi = parameters[1];
	std::vector<std::vector<double>> q(m_positioned_coordinates.size(), std::vector<double>(3, 0.0));
	for (size_t i = 0; i < m_positioned_coordinates.size(); i++)
	{
		q[i][0] = cos(phi) * m_positioned_coordinates[i][0] + sin(theta) * sin(phi) * m_positioned_coordinates[i][1] + cos(theta) * sin(phi) * m_positioned_coordinates[i][2];
		q[i][1] = cos(theta) * m_positioned_coordinates[i][1] - sin(theta) * m_positioned_coordinates[i][2];
		q[i][2] = -sin(phi) * m_positioned_coordinates[i][0] + sin(theta) * cos(phi) * m_positioned_coordinates[i][1] + cos(theta) * cos(phi) * m_positioned_coordinates[i][2];
	}
	double lowest_coordinate = INFINITY;
	for (size_t i = 0; i < q.size(); i++)
	{
		if (q[i][2] < lowest_coordinate)
		{
			lowest_coordinate = q[i][2];
		}
	}
	return abs(lowest_coordinate);
}

double CMainTab3::GroundOnMolecule(std::vector<double> parameters)
{
	double theta = parameters[0];
	double phi = parameters[1];
	std::vector<std::vector<double>> q(m_positioned_coordinates.size(), std::vector<double>(3, 0.0));
	for (size_t i = 0; i < m_positioned_coordinates.size(); i++)
	{
		q[i][0] = cos(phi) * m_positioned_coordinates[i][0] + sin(theta) * sin(phi) * m_positioned_coordinates[i][1] + cos(theta) * sin(phi) * m_positioned_coordinates[i][2];
		q[i][1] = cos(theta) * m_positioned_coordinates[i][1] - sin(theta) * m_positioned_coordinates[i][2];
		q[i][2] = -sin(phi) * m_positioned_coordinates[i][0] + sin(theta) * cos(phi) * m_positioned_coordinates[i][1] + cos(theta) * cos(phi) * m_positioned_coordinates[i][2];
	}
	double highest_coordinate = -INFINITY;
	for (size_t i = 0; i < q.size(); i++)
	{
		if (q[i][2] > highest_coordinate)
		{
			highest_coordinate = q[i][2];
		}
	}
	return abs(highest_coordinate);
}

std::vector<double> CMainTab3::ComputePlane(std::vector<double> Pj, std::vector<double> Pi, std::vector<double> Pk, double theta, double phi, double tau)
{
	std::vector<double> Plane(4);
	std::vector<std::vector<double>> RotMat(3, std::vector<double>(3, 0.0));
	RotMat[0][0] = cos(phi) * cos(tau);
	RotMat[0][1] = sin(theta) * sin(phi) * cos(tau) - cos(theta) * sin(tau);
	RotMat[0][2] = cos(theta) * sin(phi) * cos(tau) + sin(theta) * sin(tau);
	RotMat[1][0] = cos(phi) * sin(tau);
	RotMat[1][1] = sin(theta) * sin(phi) * sin(tau) + cos(theta) * cos(tau);
	RotMat[1][2] = cos(theta) * sin(phi) * sin(tau) - sin(theta) * cos(tau);
	RotMat[2][0] = -sin(phi);
	RotMat[2][1] = sin(theta) * cos(phi);
	RotMat[2][2] = cos(theta) * cos(phi);
	std::vector<double> projectedPj(3);
	projectedPj[0] = RotMat[0][0] * Pj[0] + RotMat[0][1] * Pj[1] + RotMat[0][2] * Pj[2];
	projectedPj[1] = RotMat[1][0] * Pj[0] + RotMat[1][1] * Pj[1] + RotMat[1][2] * Pj[2];
	projectedPj[2] = RotMat[2][0] * Pj[0] + RotMat[2][1] * Pj[1] + RotMat[2][2] * Pj[2];
	std::vector<double> projectedPi(3);
	projectedPi[0] = RotMat[0][0] * Pi[0] + RotMat[0][1] * Pi[1] + RotMat[0][2] * Pi[2];
	projectedPi[1] = RotMat[1][0] * Pi[0] + RotMat[1][1] * Pi[1] + RotMat[1][2] * Pi[2];
	projectedPi[2] = RotMat[2][0] * Pi[0] + RotMat[2][1] * Pi[1] + RotMat[2][2] * Pi[2];
	std::vector<double> projectedPk(3);
	projectedPk[0] = RotMat[0][0] * Pk[0] + RotMat[0][1] * Pk[1] + RotMat[0][2] * Pk[2];
	projectedPk[1] = RotMat[1][0] * Pk[0] + RotMat[1][1] * Pk[1] + RotMat[1][2] * Pk[2];
	projectedPk[2] = RotMat[2][0] * Pk[0] + RotMat[2][1] * Pk[1] + RotMat[2][2] * Pk[2];
	std::vector<double> a(3, 0.0);
	std::vector<double> b(3, 0.0);
	for (size_t i = 0; i < 3; i++)
	{
		a[i] = projectedPj[i] - projectedPi[i];
		b[i] = projectedPk[i] - projectedPi[i];
	}
	Plane[0] = a[1] * b[2] - a[2] * b[1];
	Plane[1] = a[2] * b[0] - a[0] * b[2];
	Plane[2] = a[0] * b[1] - a[1] * b[0];
	Plane[3] = 0.0 - Plane[0] * projectedPj[0] - Plane[1] * projectedPj[1] - Plane[2] * projectedPj[2];
	return Plane;
}

double CMainTab3::ComputeMissingPlaneCoordinate(int xyz_index, double x, double y, double z, std::vector<double> Plane)
{
	double return_value = 0.0;
	if (xyz_index == 1)
	{
		return_value = (0.0 - Plane[3] - Plane[1] * y - Plane[2] * z) / Plane[0];
	}
	else if (xyz_index == 2)
	{
		return_value = (0.0 - Plane[3] - Plane[0] * x - Plane[2] * z) / Plane[1];
	}
	else if (xyz_index == 3)
	{
		return_value = (0.0 - Plane[3] - Plane[0] * x - Plane[1] * y) / Plane[2];
	}
	return return_value;
}

double CMainTab3::ComputeVolumeTetraeder(std::vector<double>& Pi, std::vector<double>& Pj, std::vector<double>& Pk, std::vector<double>& Pl)
{
	std::vector<double> vij(3, 0.0);
	std::vector<double> vik(3, 0.0);
	std::vector<double> vil(3, 0.0);
	for (size_t i = 0; i < 3; i++)
	{
		vij[i] = Pj[i] - Pi[i];
		vik[i] = Pk[i] - Pi[i];
		vil[i] = Pl[i] - Pi[i];
	}
	std::vector<double> cross_prod = { vik[1] * vil[2] - vik[2] * vil[1], vik[2] * vil[0] - vik[0] * vil[2], vik[0] * vil[1] - vik[1] * vil[0] };
	double product = vij[0] * cross_prod[0] + vij[1] * cross_prod[1] + vij[2] * cross_prod[2];
	return (1.0 / 6.0) * abs(product);
}



double CMainTab3::calculateT1E()
{
	double R = 0.0;
	R += TR / (1.0 + pow((m_omegaE * TR), 2.0));
	R += 4.0 * TR / (1.0 + 4.0 * pow((m_omegaE * TR), 2.0));
	return 1.0 / (R * m_C);
}

double CMainTab3::calculateT2E()
{
	return (1.0 / ((1.0 / (2.0 * T1E)) + (m_D / TM)));
}

double CMainTab3::calculateC()
{
	double R = 0.0;
	R += TR / (1.0 + pow((m_omegaE * TR), 2.0));
	R += 4.0 * TR / (1.0 + 4.0 * pow((m_omegaE * TR), 2.0));
	return 1.0 / (R * T1E);
}

double CMainTab3::calculateD()
{
	return ((1.0 / T2E) - (1.0 / (2.0 * T1E))) * TM;
}

void CMainTab3::ResetTab()
{
	m_temperature = 298.0;
	m_viscosity = 1.0;
	m_mol_sphere_radius = 1.0;
	m_sphere_radius = 1.0;
	m_coordinates_for_fit.clear();
	m_coordinates_for_fit.push_back({ 1.0, 1.0, 1.0 });
	m_ComboBoxRelSolvent.SetCurSel(0);
	double B0 = (m_pTab1->get_MagField() * 1000000.0) / get_gyromagnetic_ratio("1H", -1, false);
	m_omegaE = -(get_gyromagnetic_ratio("E", -1, false) * B0);
	m_SliderT1E.SetPos(0);
	m_SliderT2E.SetPos(0);
	m_SliderTM.SetPos(0);
	m_SliderTR.SetPos(0);
	T1E = 1E-13;
	T2E = 1E-13;
	TM = 1E-10;
	TR = 1E-11;
	m_C = calculateC();
	m_D = calculateD();
	m_EditValT1E.SetWindowTextW(_T("1.00"));
	m_EditValT2E.SetWindowTextW(_T("1.00"));
	m_EditValTM.SetWindowTextW(_T("1.00"));
	m_EditValTR.SetWindowTextW(_T("1.00"));
	m_EditExponentT1E.SetWindowTextW(_T("-13"));
	m_EditExponentT2E.SetWindowTextW(_T("-13"));
	m_EditExponentTM.SetWindowTextW(_T("-10"));
	m_EditExponentTR.SetWindowTextW(_T("-11"));
	double volume = (4.0 / 3.0) * PI * pow(1.0, 3.0);
	CString formatted_text;
	formatted_text.Format(_T("%.4f"), volume);
	m_EditSphereRadius.SetWindowTextW(formatted_text);
	OnCbnSelchangeSolvent();
}

void CMainTab3::print_info(std::ofstream& file)
{
	file << std::fixed << std::scientific << std::setprecision(12);
	file << "----------------------" << std::endl;
	file << "Relaxation Parameters:" << std::endl;
	file << "----------------------" << std::endl;
	file << "T1E = " << GetT1E() << " s" << std::endl;
	file << "T2E = " << GetT2E() << " s" << std::endl;
	file << "TM = " << GetTM() << " s" << std::endl;
	file << "TR = " << GetTR() << " s" << std::endl;
	file << "C = " << m_C << std::endl;
	file << "D = " << m_D << std::endl;
	file << "Veff = " << print_vol << " Angstroem**3" << std::endl;
	file << "molecule radius = " << m_mol_sphere_radius << " Angstroem**3" << std::endl;
	CString Text;
	m_ComboBoxRelSolvent.GetWindowTextW(Text);
	std::wstring wstr = Text.GetString();
	std::string stdstr(wstr.begin(), wstr.end());
	file << "Solvent: " << stdstr << std::endl;
	file << "viscosity = " << m_viscosity << " mPa*s" << std::endl;
	file << std::endl;
	file << "Relaxation times:" << std::endl;
	double tau_dip_t1e = (1.0 / ((1.0 / T1E) + (1.0 / TR) + (1.0 / TM)));
	double tau_dip_t2e = (1.0 / ((1.0 / T2E) + (1.0 / TR) + (1.0 / TM)));
	double tau_con_t1e = (1.0 / ((1.0 / T1E) + (1.0 / TM)));
	double tau_con_t2e = (1.0 / ((1.0 / T2E) + (1.0 / TM)));
	double tau_curie = (1.0 / ((1.0 / TR) + (1.0 / TM)));
	file << "  tau_dip_t1e = " << tau_dip_t1e << " s" << std::endl;
	file << "  tau_dip_t2e = " << tau_dip_t2e << " s" << std::endl;
	file << "  tau_con_t1e = " << tau_con_t1e << " s" << std::endl;
	file << "  tau_con_t2e = " << tau_con_t2e << " s" << std::endl;
	file << "  tau_curie = " << tau_curie << std::endl;
	file << std::endl;
	file << std::endl;
}