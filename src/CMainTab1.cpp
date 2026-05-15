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
#include "CMainTab1.h"
#include "afxdialogex.h"
#include <iomanip>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainTab1, CDialogEx)

CMainTab1::CMainTab1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_TAB1, pParent)
{
}

CMainTab1::~CMainTab1()
{
}

void CMainTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainTab1, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPERATURE, &CMainTab1::OnTemperatureEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_MAGFIELD, &CMainTab1::OnMagfieldEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_PDA_POSX, &CMainTab1::OnPDAxEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_PDA_POSY, &CMainTab1::OnPDAyEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_PDA_POSZ, &CMainTab1::OnPDAzEditKillfocus)
	ON_CBN_SELCHANGE(IDC_COMBO_SHIELDING_DATA, &CMainTab1::OnCbnSelchangeComboShieldingData)
	ON_CBN_SELCHANGE(IDC_COMBO_FC_DATA, &CMainTab1::OnCbnSelchangeComboFCData)
	ON_CBN_SELCHANGE(IDC_COMBO_HFC_DATA, &CMainTab1::OnCbnSelchangeComboHFCData)
	ON_CBN_SELCHANGE(IDC_COMBO_NUCLEUS, &CMainTab1::OnCbnSelchangeComboNucleus)
	ON_BN_CLICKED(IDC_BUTTON_SIMULATE, &CMainTab1::OnBnClickedSimulate)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_DIAMAG, &CMainTab1::OnBnClickedRemoveDia)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_FERMI, &CMainTab1::OnBnClickedRemoveFermi)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_HFC, &CMainTab1::OnBnClickedRemoveHFC)
	ON_CBN_SELCHANGE(IDC_COMBO_SIM_FC, &CMainTab1::OnCbnSelchangeSHIFTComboBoxes)
	ON_CBN_SELCHANGE(IDC_COMBO_SIM_PC, &CMainTab1::OnCbnSelchangeSHIFTComboBoxes)
	ON_CBN_SELCHANGE(IDC_COMBO_SIM_SUSCEPTIBILITY, &CMainTab1::OnCbnSelchangeSHIFTComboBoxes)
	ON_CBN_SELCHANGE(IDC_COMBO_SIM_CURVETYPE, &CMainTab1::OnCbnSelchangeSHIFTComboBoxes)
	ON_CBN_SELCHANGE(IDC_COMBO_RELAXATION_CONTACT, &CMainTab1::OnCbnSelchangeFWHMComboBoxes)
	ON_CBN_SELCHANGE(IDC_COMBO_RELAXATION_DIPOLAR, &CMainTab1::OnCbnSelchangeFWHMComboBoxes)
	ON_CBN_SELCHANGE(IDC_COMBO_RELAXATION_CURIE, &CMainTab1::OnCbnSelchangeFWHMComboBoxes)
END_MESSAGE_MAP()

BOOL CMainTab1::OnInitDialog()
{
	m_bSimulation_On = false;
	m_bActivatedByButton = true;
	m_SimulateButton.SubclassDlgItem(IDC_BUTTON_SIMULATE, this);
	m_SimulateButton.ModifyStyle(0, BS_MULTILINE);
	m_EditTemperature.SubclassDlgItem(IDC_EDIT_TEMPERATURE, this);
	m_EditTemperature.SetWindowTextW(L"298");
	m_EditMagField.SubclassDlgItem(IDC_EDIT_MAGFIELD, this);
	m_EditMagField.SetWindowTextW(L"600");
	m_ComboBoxNucleus.SubclassDlgItem(IDC_COMBO_NUCLEUS, this);
	m_ComboBoxShieldingData.SubclassDlgItem(IDC_COMBO_SHIELDING_DATA, this);
	m_ComboBoxFCData.SubclassDlgItem(IDC_COMBO_FC_DATA, this);
	m_ComboBoxHFCData.SubclassDlgItem(IDC_COMBO_HFC_DATA, this);
	{
		CRect combo_rc;
		m_ComboBoxNucleus.GetWindowRect(&combo_rc);
		m_ComboBoxNucleus.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (10 * m_ComboBoxNucleus.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxShieldingData.GetWindowRect(&combo_rc);
		m_ComboBoxShieldingData.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (5 * m_ComboBoxShieldingData.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxFCData.GetWindowRect(&combo_rc);
		m_ComboBoxFCData.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (5 * m_ComboBoxFCData.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxHFCData.GetWindowRect(&combo_rc);
		m_ComboBoxHFCData.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (5 * m_ComboBoxHFCData.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
	}
	m_Edit_PDAx.SubclassDlgItem(IDC_EDIT_PDA_POSX, this);
	m_Edit_PDAx.SetWindowTextW(_T("0.00000"));
	m_Edit_PDAy.SubclassDlgItem(IDC_EDIT_PDA_POSY, this);
	m_Edit_PDAy.SetWindowTextW(_T("0.00000"));
	m_Edit_PDAz.SubclassDlgItem(IDC_EDIT_PDA_POSZ, this);
	m_Edit_PDAz.SetWindowTextW(_T("0.00000"));
	m_ComboBoxSimFC.SubclassDlgItem(IDC_COMBO_SIM_FC, this);
	m_ComboBoxSimPC.SubclassDlgItem(IDC_COMBO_SIM_PC, this);
	m_ComboBoxSimSusceptibility.SubclassDlgItem(IDC_COMBO_SIM_SUSCEPTIBILITY, this);
	m_ComboBoxSimCurvertype.SubclassDlgItem(IDC_COMBO_SIM_CURVETYPE, this);
	{
		CRect combo_rc;
		m_ComboBoxSimFC.GetWindowRect(&combo_rc);
		m_ComboBoxSimFC.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (2 * m_ComboBoxSimFC.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxSimFC.AddString(_T("Hyperfine"));
		m_ComboBoxSimFC.AddString(_T("Contact Couplings"));
		m_ComboBoxSimFC.SetCurSel(0);
		m_ComboBoxSimPC.GetWindowRect(&combo_rc);
		m_ComboBoxSimPC.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (2 * m_ComboBoxSimPC.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxSimPC.AddString(_T("Hyperfine"));
		m_ComboBoxSimPC.AddString(_T("Point Dipole"));
		m_ComboBoxSimPC.SetCurSel(0);
		m_ComboBoxSimSusceptibility.GetWindowRect(&combo_rc);
		m_ComboBoxSimSusceptibility.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (3 * m_ComboBoxSimSusceptibility.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxSimSusceptibility.AddString(_T("Individual"));
		m_ComboBoxSimSusceptibility.AddString(_T("Computed"));
		m_ComboBoxSimSusceptibility.AddString(_T("Fitted"));
		m_ComboBoxSimSusceptibility.SetCurSel(0);
		m_ComboBoxSimCurvertype.GetWindowRect(&combo_rc);
		m_ComboBoxSimCurvertype.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (2 * m_ComboBoxSimCurvertype.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxSimCurvertype.AddString(_T("Lorentzian"));
		m_ComboBoxSimCurvertype.AddString(_T("Gaussian"));
		m_ComboBoxSimCurvertype.SetCurSel(0);
	}
	m_ComboBoxRelaxationContact.SubclassDlgItem(IDC_COMBO_RELAXATION_CONTACT, this);
	m_ComboBoxRelaxationDipolar.SubclassDlgItem(IDC_COMBO_RELAXATION_DIPOLAR, this);
	m_ComboBoxRelaxationCurie.SubclassDlgItem(IDC_COMBO_RELAXATION_CURIE, this);
	{
		CRect combo_rc;
		m_ComboBoxRelaxationContact.GetWindowRect(&combo_rc);
		m_ComboBoxRelaxationContact.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (2 * m_ComboBoxRelaxationContact.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxRelaxationContact.AddString(_T("Default"));
		m_ComboBoxRelaxationContact.AddString(_T("None"));
		m_ComboBoxRelaxationContact.SetCurSel(0);
		m_ComboBoxRelaxationDipolar.GetWindowRect(&combo_rc);
		m_ComboBoxRelaxationDipolar.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (3 * m_ComboBoxRelaxationDipolar.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxRelaxationDipolar.AddString(_T("Hyperfine"));
		m_ComboBoxRelaxationDipolar.AddString(_T("Point Dipole"));
		m_ComboBoxRelaxationDipolar.AddString(_T("None"));
		m_ComboBoxRelaxationDipolar.SetCurSel(0);
		m_ComboBoxRelaxationCurie.GetWindowRect(&combo_rc);
		m_ComboBoxRelaxationCurie.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (4 * m_ComboBoxRelaxationCurie.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxRelaxationCurie.AddString(_T("Curie + CSA"));
		m_ComboBoxRelaxationCurie.AddString(_T("Isotropic Spin"));
		m_ComboBoxRelaxationCurie.AddString(_T("Isotropic Susc"));
		m_ComboBoxRelaxationCurie.AddString(_T("None"));
		m_ComboBoxRelaxationCurie.SetCurSel(0);
	}
	return TRUE;
}

BOOL CMainTab1::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnTemperatureEditKillfocus();
			OnMagfieldEditKillfocus();
			OnPDAxEditKillfocus();
			OnPDAyEditKillfocus();
			OnPDAzEditKillfocus();
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMainTab1::Initialize(DataBase* dataBase)
{
	m_data_base = dataBase;
	for (size_t i = 0; i < m_data_base->GetNMRnucleiSize(); i++)
	{
		m_ComboBoxNucleus.AddString(m_data_base->GetNMRnuclei(i));
	}
}

void CMainTab1::Initialize_Tabs2(CMainTab2* TAB2)
{
	m_pTab2 = TAB2;
}

void CMainTab1::Initialize_Tabs3(CMainTab3* TAB3)
{
	m_pTab3 = TAB3;
}

void CMainTab1::Initialize_Graph(CGraphControl* Graph)
{
	m_GraphControl = Graph;
}

void CMainTab1::OnTemperatureEditKillfocus()
{
	CString edittext;
	m_EditTemperature.GetWindowTextW(edittext);
	int value = _tstoi(edittext);
	CString formattedValue;
	formattedValue.Format(_T("%d"), value);
	m_EditTemperature.SetWindowTextW(formattedValue);
	if (formattedValue == _T("") || formattedValue == _T("0"))
	{
		edittext = _T("298");
		m_EditTemperature.SetWindowTextW(edittext);
	}
	m_pTab3->OnTempChange();
	if (m_bSimulation_On)
	{
		TurnSimOn(true, true, false, true);
	}
}

void CMainTab1::OnMagfieldEditKillfocus()
{
	CString edittext;
	m_EditMagField.GetWindowTextW(edittext);
	int value = _tstoi(edittext);
	CString formattedValue;
	formattedValue.Format(_T("%d"), value);
	m_EditMagField.SetWindowTextW(formattedValue);
	if (formattedValue == _T("") || formattedValue == _T("0"))
	{
		edittext = _T("600");
		m_EditMagField.SetWindowTextW(edittext);
	}
	m_pTab3->OnB0Change();
}

void CMainTab1::UpdateLoadedData(bool shielding, bool FC, bool HFC)
{
	if (shielding)
	{
		m_ComboBoxShieldingData.ResetContent();
		for (size_t i = 0; i < m_data_base->get_mol_d_orb_DataSize(); i++)
		{
			m_ComboBoxShieldingData.AddString(m_data_base->get_mol_d_orb_DataName(i));
		}
	}
	if (FC)
	{
		m_ComboBoxFCData.ResetContent();
		for (size_t i = 0; i < m_data_base->get_mol_FC_DataSize(); i++)
		{
			m_ComboBoxFCData.AddString(m_data_base->get_mol_FC_DataName(i));
		}
	}
	if (HFC)
	{
		m_ComboBoxHFCData.ResetContent();

		for (size_t i = 0; i < m_data_base->get_mol_HFC_DataSize(); i++)
		{
			m_ComboBoxHFCData.AddString(m_data_base->get_mol_HFC_DataName(i));
		}
	}
}

void CMainTab1::OnPDAxEditKillfocus()
{
	CString edittext;
	m_Edit_PDAx.GetWindowTextW(edittext);
	double doubleValue = _tstof(edittext);
	CString formattedValue;
	formattedValue.Format(_T("%.5f"), doubleValue);
	m_Edit_PDAx.SetWindowTextW(formattedValue);
	m_Edit_PDAy.GetWindowTextW(edittext);
	double valueY = (double)_tstof(edittext);
	m_Edit_PDAz.GetWindowTextW(edittext);
	double valueZ = (double)_tstof(edittext);
	m_data_base->setPDPositions(doubleValue, valueY, valueZ);
	m_data_base->m_simulation.SetPDDistance(m_data_base->CalculatePDDistances());
	if (m_bSimulation_On)
	{
		TurnSimOn(true, true, false, true);
	}
}

void CMainTab1::OnPDAyEditKillfocus()
{
	CString edittext;
	m_Edit_PDAy.GetWindowTextW(edittext);
	double doubleValue = _tstof(edittext);
	CString formattedValue;
	formattedValue.Format(_T("%.5f"), doubleValue);
	m_Edit_PDAy.SetWindowTextW(formattedValue);
	m_Edit_PDAx.GetWindowTextW(edittext);
	double valueX = (double)_tstof(edittext);
	m_Edit_PDAz.GetWindowTextW(edittext);
	double valueZ = (double)_tstof(edittext);
	m_data_base->setPDPositions(valueX, doubleValue, valueZ);
	m_data_base->m_simulation.SetPDDistance(m_data_base->CalculatePDDistances());
	if (m_bSimulation_On)
	{
		TurnSimOn(true, true, false, true);
	}
}

void CMainTab1::OnPDAzEditKillfocus()
{
	CString edittext;
	m_Edit_PDAz.GetWindowTextW(edittext);
	double doubleValue = _tstof(edittext);
	CString formattedValue;
	formattedValue.Format(_T("%.5f"), doubleValue);
	m_Edit_PDAz.SetWindowTextW(formattedValue);
	m_Edit_PDAx.GetWindowTextW(edittext);
	double valueX = (double)_tstof(edittext);
	m_Edit_PDAy.GetWindowTextW(edittext);
	double valueY = (double)_tstof(edittext);
	m_data_base->setPDPositions(valueX, valueY, doubleValue);
	m_data_base->m_simulation.SetPDDistance(m_data_base->CalculatePDDistances());
	if (m_bSimulation_On)
	{
		TurnSimOn(true, true, false, true);
	}
}

int CMainTab1::GetFittingState()
{
	if (m_ComboBoxShieldingData.GetCurSel() != -1)
	{
		if (m_ComboBoxSimFC.GetCurSel() == 0 && m_ComboBoxSimPC.GetCurSel() == 0)
		{
			if (m_ComboBoxHFCData.GetCurSel() != -1)
			{
				return 0;
			}
			else
			{
				return -2;
			}
		}
		else if (m_ComboBoxSimFC.GetCurSel() == 0 && m_ComboBoxSimPC.GetCurSel() != 0)
		{
			if (m_ComboBoxHFCData.GetCurSel() != -1)
			{
				return 1;
			}
			else
			{
				return -2;
			}
		}
		else if (m_ComboBoxSimFC.GetCurSel() != 0 && m_ComboBoxSimPC.GetCurSel() == 0)
		{
			if (m_ComboBoxHFCData.GetCurSel() != -1)
			{
				if (m_ComboBoxFCData.GetCurSel() != -1)
				{
					return 2;
				}
				else
				{
					return -3;
				}
			}
			else
			{
				return -2;
			}
		}
		else
		{
			return 3;
		}
	}
	return -1;
}

void CMainTab1::OnCbnSelchangeComboShieldingData()
{
	if (m_ComboBoxShieldingData.GetCurSel() != -1)
	{
		m_data_base->m_simulation.SetEssentials_dorb(m_data_base->get_mol_d_orb(m_ComboBoxShieldingData.GetCurSel()));
		m_data_base->m_simulation.setEssentials_Shielding(m_data_base->get_mol_shieldingTensors(m_ComboBoxShieldingData.GetCurSel()));
		if (m_bSimulation_On)
		{
			TurnSimOn(true, true, false, true);
		}
	}
	else
	{
		if (m_bSimulation_On)
		{
			TurnSimOff();
		}
	}
}

void CMainTab1::OnCbnSelchangeComboHFCData()
{
	if (m_ComboBoxHFCData.GetCurSel() != -1)
	{
		m_data_base->m_simulation.SetEssentials_HFC(m_data_base->get_mol_hfc(m_ComboBoxHFCData.GetCurSel()), m_data_base->get_mol_hfc_inHz(m_ComboBoxHFCData.GetCurSel()));
		if (m_bSimulation_On)
		{
			TurnSimOn(true, true, false, true);
		}
	}
	else
	{
		if (m_bSimulation_On)
		{
			TurnSimOff();
		}
	}
}

void CMainTab1::OnCbnSelchangeComboFCData()
{
	if (m_ComboBoxFCData.GetCurSel() != -1)
	{
		m_data_base->m_simulation.SetEssentials_FC(m_data_base->get_mol_fc(m_ComboBoxFCData.GetCurSel()));
		if (m_bSimulation_On)
		{
			TurnSimOn(true, true, false, true);
		}
	}
	else
	{
		if (m_bSimulation_On)
		{
			TurnSimOff();
		}
	}
}

void CMainTab1::OnCbnSelchangeSHIFTComboBoxes()
{
	if (m_bSimulation_On)
	{
		TurnSimOn(false, true, false, true);
	}
}

void CMainTab1::OnCbnSelchangeFWHMComboBoxes()
{
	if (m_bSimulation_On)
	{ 
		TurnSimOn(true, false, false, true);
	}
}

void CMainTab1::UnselectComboBoxData()
{
	m_ComboBoxShieldingData.SetCurSel(-1);
	m_ComboBoxHFCData.SetCurSel(-1);
	m_ComboBoxFCData.SetCurSel(-1);
	if (m_bSimulation_On)
	{
		TurnSimOff();
	}
}

double CMainTab1::get_temperature()
{
	CString edittext;
	m_EditTemperature.GetWindowTextW(edittext);
	int value = _tstoi(edittext);
	return (double)value;
}

void CMainTab1::resetPD()
{
	m_Edit_PDAx.SetWindowTextW(_T("0.00000"));
	m_Edit_PDAy.SetWindowTextW(_T("0.00000"));
	m_Edit_PDAz.SetWindowTextW(_T("0.00000"));
	m_data_base->setPDPositions(0.0, 0.0, 0.0);
}

double CMainTab1::get_MagField()
{
	CString edittext;
	m_EditMagField.GetWindowTextW(edittext);
	int value = _tstoi(edittext);
	return (double)value;
}

void CMainTab1::OnBnClickedSimulate()
{
	if (m_bSimulation_On)
	{
		TurnSimOff();
	}
	else
	{
		TurnSimOn(true, true, true, true);
	}
}

void CMainTab1::TurnSimOn(bool FWHMUpdate, bool ShiftUpdate, bool SimDatDlgReset, bool SimDatDlgUpdate)
{	
	if (m_ComboBoxNucleus.GetCurSel() != -1 && m_ComboBoxRelaxationContact.GetCurSel() != -1 && m_ComboBoxRelaxationDipolar.GetCurSel() != -1 && m_ComboBoxRelaxationCurie.GetCurSel() != -1)
	{
		m_data_base->m_simulation.SetMagneticField(get_MagField());
		m_data_base->m_simulation.SetSimTemperature(get_temperature());
		if (FWHMUpdate)
		{
			std::vector<double> Relaxation_times;
			double T1E = m_pTab3->GetT1E();
			double T2E = m_pTab3->GetT2E();
			double TM = m_pTab3->GetTM();
			double TR = m_pTab3->GetTR();
			Relaxation_times.emplace_back(1.0 / ((1.0 / T1E) + (1.0 / TR) + (1.0 / TM)));
			Relaxation_times.emplace_back(1.0 / ((1.0 / T2E) + (1.0 / TR) + (1.0 / TM)));
			Relaxation_times.emplace_back(1.0 / ((1.0 / T1E) + (1.0 / TM)));
			Relaxation_times.emplace_back(1.0 / ((1.0 / T2E) + (1.0 / TM)));
			Relaxation_times.emplace_back(1.0 / ((1.0 / TR) + (1.0 / TM)));
			m_data_base->m_simulation.SetRelaxationTimes(Relaxation_times);
		}
		if (m_ComboBoxSimSusceptibility.GetCurSel() != -1)
		{
			if (m_ComboBoxSimSusceptibility.GetCurSel() == 0)
			{
				m_data_base->m_simulation.SetSimChi(m_pTab2->GetIndividualChi(get_temperature()));
			}
			else if (m_ComboBoxSimSusceptibility.GetCurSel() == 1)
			{
				m_data_base->m_simulation.SetSimChi(m_pTab2->GetComputedChi(get_temperature()));
			}
			else if (m_ComboBoxSimSusceptibility.GetCurSel() == 2)
			{
				m_data_base->m_simulation.SetSimChi(m_pTab2->GetFittedChi());
			}
			else
			{
				m_data_base->m_simulation.SetSimChi(std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0)));
			}
			m_ComboBoxNucleus.GetWindowTextW(NucleusIsotope);
			w_element = m_data_base->GetAdditionalRefElements(m_ComboBoxNucleus.GetCurSel()).GetString();
			bool failed = false;
			if (ShiftUpdate)
			{
				if (m_ComboBoxSimFC.GetCurSel() == 0 && m_ComboBoxSimPC.GetCurSel() == 0)
				{
					if (m_ComboBoxShieldingData.GetCurSel() == -1 || m_ComboBoxHFCData.GetCurSel() == -1)
					{
						failed = true;
					}
					else
					{
						m_data_base->m_simulation.ComputeChemShift(0, w_element);
					}
				}
				else if (m_ComboBoxSimFC.GetCurSel() == 1 && m_ComboBoxSimPC.GetCurSel() == 0)
				{
					if (m_ComboBoxShieldingData.GetCurSel() == -1 || m_ComboBoxHFCData.GetCurSel() == -1 || m_ComboBoxFCData.GetCurSel() == -1)
					{
						failed = true;
					}
					else
					{
						m_data_base->m_simulation.ComputeChemShift(1, w_element);
					}
				}
				else if (m_ComboBoxSimFC.GetCurSel() == 0 && m_ComboBoxSimPC.GetCurSel() == 1)
				{
					if (m_ComboBoxShieldingData.GetCurSel() == -1 || m_ComboBoxHFCData.GetCurSel() == -1)
					{
						failed = true;
					}
					else
					{
						m_data_base->m_simulation.ComputeChemShift(2, w_element);
					}
				}
				else if (m_ComboBoxSimFC.GetCurSel() == 1 && m_ComboBoxSimPC.GetCurSel() == 1)
				{
					if (m_ComboBoxShieldingData.GetCurSel() == -1 || m_ComboBoxHFCData.GetCurSel() == -1 || m_ComboBoxFCData.GetCurSel() == -1)
					{
						failed = true;
					}
					else
					{
						m_data_base->m_simulation.ComputeChemShift(3, w_element);
					}
				}
			}
			if (!failed)
			{
				if (FWHMUpdate)
				{
					m_data_base->m_simulation.ComputeFWHM(m_ComboBoxRelaxationContact.GetCurSel(), m_ComboBoxRelaxationDipolar.GetCurSel(), m_ComboBoxRelaxationCurie.GetCurSel(), NucleusIsotope, w_element);
				}
				if (m_ComboBoxSimCurvertype.GetCurSel() != -1)
				{
					if (m_ComboBoxSimCurvertype.GetCurSel() == 0)
					{
						CurveType = 'L';
					}
					else
					{
						CurveType = 'G';
					}
					double spc_min = m_data_base->m_simulation.get_sim_min(w_element, CurveType, 1.0);
					double spc_max = m_data_base->m_simulation.get_sim_max(w_element, CurveType, 1.0);
					if (m_data_base->m_simulation.ComputeSpectrum(w_element, spc_min, spc_max, -1, m_GraphControl->GetMinPPM(), m_GraphControl->GetMaxPPM(), m_GraphControl->GetWindowStepSize(), CurveType))
					{
						m_GraphControl->load_graph(m_data_base->m_simulation.GetSimSpec(), true);
						m_bSimulation_On = true;
						m_SimulateButton.SetWindowTextW(_T("stop\r\n\simulation"));
						if (SimDatDlgReset)
						{
							m_SimDataDlg->ClearList();
						}
						if (SimDatDlgUpdate)
						{
							m_SimDataDlg->Update(m_data_base->m_simulation.get_GroupsName(), m_data_base->m_simulation.get_SimGroupIndices(), w_element, m_data_base->m_simulation.get_SimIsoOrb(), m_data_base->m_simulation.get_SimIsoFc(), m_data_base->m_simulation.get_SimIsoPcs(), m_data_base->m_simulation.get_fwhm());
							m_SimDataDlg->UpdateWindow();
						}
					}
					else
					{
						TurnSimOff();
						AfxMessageBox(_T("Failed to simulate spectrum"), MB_ICONERROR);
					}
				}
				else
				{
					TurnSimOff();
					AfxMessageBox(_T("Unexpected Error --Curvetype-- !"), MB_ICONERROR);
				}
			}
			else
			{
				TurnSimOff();
				AfxMessageBox(_T("missing imported data selection"), MB_ICONERROR);
			}
		}
		else
		{
			TurnSimOff();
			AfxMessageBox(_T("Unexpected Error --Susceptibility-- !"), MB_ICONERROR);
		}
	}
	else
	{
		TurnSimOff();
		AfxMessageBox(_T("Please choose a nucleus type for simulation!"), MB_ICONWARNING);
	}
}

void CMainTab1::tab3_sim_start()
{
	if (m_bSimulation_On)
	{
		TurnSimOn(true, false, false, true);
	}
}

void CMainTab1::GTensor_dlg_sim_start()
{
	if (m_bSimulation_On)
	{
		TurnSimOn(true, true, false, true);
	}
}

void CMainTab1::Graph_sim_start()
{
	if (m_bSimulation_On)
	{
		TurnSimOn(false, false, false, false);
	}
}

void CMainTab1::OnCbnSelchangeComboNucleus()
{
	TurnSimOff();
}

void CMainTab1::TurnSimOff()
{
	if (m_bSimulation_On)
	{
		m_GraphControl->load_graph(std::vector<std::pair<double, double>>(), true);
		m_bSimulation_On = false;
		m_SimulateButton.SetWindowTextW(_T("simulate"));
		m_SimDataDlg->ClearList();
	}
}

void CMainTab1::tab2_sim(bool start, bool fit)
{
	if (fit && start)
	{
		if (m_ComboBoxSimSusceptibility.GetCurSel() == 2 && m_bSimulation_On)
		{
			TurnSimOn(true, true, false, true);
			m_GraphControl->UpdateWindow();
		}
	}
	else
	{
		if (start)
		{
			if (m_bSimulation_On)
			{
				TurnSimOn(true, true, false, true);
			}
		}
		else
		{
			if (m_bSimulation_On)
			{
				TurnSimOff();
			}
		}
	}
}


void CMainTab1::Initialize_SimDataDlg(CDisplaySimDataDlg* dlg)
{
	m_SimDataDlg = dlg;
}

void CMainTab1::OnListItemRemoveSimUpdate()
{
	if (m_bSimulation_On)
	{
		TurnSimOn(true, true, true, true);
	}
}


void CMainTab1::OnBnClickedRemoveDia()
{
	TurnSimOff();
	int selection = m_ComboBoxShieldingData.GetCurSel();
	if (selection != -1)
	{
		m_data_base->remove_diamag(selection);
		m_ComboBoxShieldingData.DeleteString(selection);
		m_ComboBoxShieldingData.SetCurSel(-1);
	}
}

void CMainTab1::OnBnClickedRemoveFermi()
{
	TurnSimOff();
	int selection = m_ComboBoxFCData.GetCurSel();
	if (selection != -1)
	{
		m_data_base->remove_fc(selection);
		m_ComboBoxFCData.DeleteString(selection);
		m_ComboBoxFCData.SetCurSel(-1);
	}
}

void CMainTab1::OnBnClickedRemoveHFC()
{
	TurnSimOff();
	int selection = m_ComboBoxHFCData.GetCurSel();
	if (selection != -1)
	{
		m_data_base->remove_hfc(selection);
		m_ComboBoxHFCData.DeleteString(selection);
		m_ComboBoxHFCData.SetCurSel(-1);
	}
}

void CMainTab1::ResetTab()
{
	m_EditTemperature.SetWindowTextW(_T("298"));
	OnTemperatureEditKillfocus();
	m_ComboBoxNucleus.SetCurSel(-1);
	OnCbnSelchangeComboNucleus();
	m_EditMagField.SetWindowTextW(_T("600"));
	OnMagfieldEditKillfocus();
	m_ComboBoxShieldingData.SetCurSel(-1);
	m_ComboBoxShieldingData.ResetContent();
	m_ComboBoxFCData.SetCurSel(-1);
	m_ComboBoxFCData.ResetContent();
	m_ComboBoxHFCData.SetCurSel(-1);
	m_ComboBoxHFCData.ResetContent();
	m_ComboBoxSimFC.SetCurSel(0);
	m_ComboBoxSimPC.SetCurSel(0);
	m_ComboBoxSimSusceptibility.SetCurSel(0);
	m_ComboBoxSimCurvertype.SetCurSel(0);
	m_Edit_PDAx.SetWindowTextW(_T("0.00000"));
	m_Edit_PDAy.SetWindowTextW(_T("0.00000"));
	m_Edit_PDAz.SetWindowTextW(_T("0.00000"));
	OnPDAxEditKillfocus();
	m_ComboBoxRelaxationContact.SetCurSel(0);
	m_ComboBoxRelaxationDipolar.SetCurSel(0);
	m_ComboBoxRelaxationCurie.SetCurSel(0);
}

void CMainTab1::main_sim_stop()
{
	TurnSimOff();
}

CString CMainTab1::save_getNucIsotope()
{
	return NucleusIsotope;
}

std::wstring CMainTab1::save_getElement()
{
	return w_element;
}

char CMainTab1::save_getCurveType()
{
	return CurveType;
}

bool CMainTab1::save_isSimRunning()
{
	return m_bSimulation_On;
}

void CMainTab1::print_info(std::ofstream& file)
{
	file << std::endl;
	file << "----------------------" << std::endl;
	file << "Simulation parameters:" << std::endl;
	file << "----------------------" << std::endl;
	file << "Temperature: " << std::to_string(get_temperature()) << " K" << std::endl;
	double B0 = (get_MagField() * 1000000.0) / get_gyromagnetic_ratio("1H", -1, false);
	file << "Magnetic Field: " << std::to_string(B0) << " T (proton larmor precession: " << std::to_string(get_MagField()) << " MHz)" << std::endl;
	CString Text;
	m_ComboBoxSimFC.GetWindowTextW(Text);
	file << "Fermi Contact: " << m_data_base->CStoS(Text) << std::endl;
	m_ComboBoxSimPC.GetWindowTextW(Text);
	file << "Pseudo Contact: " << m_data_base->CStoS(Text) << std::endl;
	m_ComboBoxSimSusceptibility.GetWindowTextW(Text);
	file << "Used Susceptibility: " << m_data_base->CStoS(Text) << std::endl;
	m_ComboBoxSimCurvertype.GetWindowTextW(Text);
	file << "Curve Type: " << m_data_base->CStoS(Text) << std::endl;
	file << std::endl;
	file << "-------------------------------" << std::endl;
	file << "Relaxation theories considered:" << std::endl;
	file << "-------------------------------" << std::endl;
	m_ComboBoxRelaxationContact.GetWindowTextW(Text);
	file << "Contact Type: " << m_data_base->CStoS(Text) << std::endl;
	m_ComboBoxRelaxationDipolar.GetWindowTextW(Text);
	file << "Dipolar Type: " << m_data_base->CStoS(Text) << std::endl;
	m_ComboBoxRelaxationCurie.GetWindowTextW(Text);
	file << "Curie Type: " << m_data_base->CStoS(Text) << std::endl;
	file << std::endl;
	if (m_ComboBoxSimPC.GetCurSel() == 1 || m_ComboBoxRelaxationDipolar.GetCurSel() == 1 || m_ComboBoxRelaxationCurie.GetCurSel() == 2)
	{
		file << "Point-Dipole Approximation was used." << std::endl;
		CString PDA_x;
		m_Edit_PDAx.GetWindowTextW(PDA_x);
		CString PDA_y;
		m_Edit_PDAy.GetWindowTextW(PDA_y);
		CString PDA_z;
		m_Edit_PDAz.GetWindowTextW(PDA_z);
		file << "Point-Dipole Center : x = " << m_data_base->CStoS(PDA_x) << ", y = " << m_data_base->CStoS(PDA_y) << ", z = " << m_data_base->CStoS(PDA_z);
		file << std::endl;
	}
	file << "------------" << std::endl;
	file << "Active data: " << std::endl;
	file << "------------" << std::endl;
	m_ComboBoxShieldingData.GetWindowTextW(Text);
	file << "Diamagnetic Shifts: " << m_data_base->CStoS(Text) << std::endl;
	m_ComboBoxFCData.GetWindowTextW(Text);
	file << "DFT Fermi Contact: " << m_data_base->CStoS(Text) << std::endl;
	m_ComboBoxHFCData.GetWindowTextW(Text);
	file << "HyperFine Couplings (HFC): " << m_data_base->CStoS(Text) << std::endl;
	file << std::endl;
}


void CMainTab1::print_dorb(std::ofstream& file, std::wstring element)
{
	if (m_ComboBoxShieldingData.GetCurSel() >= 0)
	{
		std::string std_element(element.begin(), element.end());
		CString CStr_element(std_element.c_str());
		file << "Note: only chemical shieldings and shifts (orbital contributions) for the element " << std_element << " are listed. Units in ppm" << std::endl;
		std::vector<double> dorb_referenced_complete = m_data_base->m_simulation.get_mdorb();
		std::vector<std::vector<std::vector<double>>> shieldings_complete = m_data_base->get_mol_shieldingTensors(m_ComboBoxShieldingData.GetCurSel());
		double reference = 0;
		file << std::fixed << std::scientific << std::setprecision(12);
		file << std::endl;
		file << "reference: " << reference << " ppm /// warning: print of the respective reference shift not implemented yet" << std::endl;
		file << std::endl;
		std::vector<size_t> indices_to_print;
		for (size_t i = 0; i < m_data_base->get_mol_atom_count(); i++)
		{
			if (m_data_base->get_mol_element_by_index(i) == CStr_element)
			{
				indices_to_print.push_back(i);
			}
		}
		file << "isotropic chemical shifts (orbital contributions; referenced)" << std::endl;
		file << "index\tdelta_orb" << std::endl;
		for (size_t i = 0; i < indices_to_print.size(); i++)
		{
			file << std::to_string(indices_to_print[i]) << "\t" << dorb_referenced_complete[indices_to_print[i]] << std::endl;
		}
		file << std::endl;
		file << std::endl;
		file << "chemical shielding tensors (orbital contributions)" << std::endl;
		file << "index\tsigma_xx\tsigma_xy\tsigma_xz\tsigma_yx\tsigma_yy\tsigma_yz\tsigma_zx\tsigma_zy\tsigma_zz" << std::endl;
		for (size_t i = 0; i < indices_to_print.size(); i++)
		{
			file << std::to_string(indices_to_print[i]);
			for (size_t j = 0; j < 3; j++)
			{
				file << "\t" << shieldings_complete[indices_to_print[i]][j][0] << "\t" << shieldings_complete[indices_to_print[i]][j][1] << "\t" << shieldings_complete[indices_to_print[i]][j][2];
			}
			file << std::endl;
		}
	}
	else
	{
		file << "No chmical shielding data (orbital contributions) was selected";
	}
}

void CMainTab1::print_dfc(std::ofstream& file, std::wstring element)
{
	if (m_ComboBoxFCData.GetCurSel() >= 0)
	{
		std::string std_element(element.begin(), element.end());
		CString CStr_element(std_element.c_str());
		file << "Note: only fermi contact shifts for the element " << std_element << " are listed. Units in ppmK" << std::endl;
		std::vector<double> fc_complete = m_data_base->get_mol_fc(m_ComboBoxFCData.GetCurSel());
		file << std::endl;
		file << "index\tFC" << std::endl;
		file << std::fixed << std::scientific << std::setprecision(12);
		for (size_t i = 0; i < m_data_base->get_mol_atom_count(); i++)
		{
			if (m_data_base->get_mol_element_by_index(i) == CStr_element)
			{
				file << std::to_string(i) << "\t" << fc_complete[i] << std::endl;
			}
		}
	}
	else
	{
		file << "No FC data was selected";
	}
}

void CMainTab1::print_dhfc(std::ofstream& file, std::wstring element)
{
	if (m_ComboBoxHFCData.GetCurSel() >= 0)
	{
		std::string std_element(element.begin(), element.end());
		CString CStr_element(std_element.c_str());
		file << "Note: only hyperfine couplings for the element " << std_element << " are listed." << std::endl;
		file << std::endl;
		file << "Units in ppm/Angstroem**3" << std::endl;
		file << "index\thfc_xx\thfc_xy\thfc_xz\thfc_yx\thfc_yy\thfc_yz\thfc_zx\thfc_zy\thfc_zz" << std::endl;
		file << std::fixed << std::scientific << std::setprecision(12);
		std::vector<std::vector<std::vector<double>>> hfc_complete = m_data_base->get_mol_hfc(m_ComboBoxHFCData.GetCurSel());
		std::vector<std::vector<std::vector<double>>> hfc_inhz_complete = m_data_base->get_mol_hfc_inHz(m_ComboBoxHFCData.GetCurSel());
		std::vector<std::vector<double>> g_tensor_inverse = inverse_Matrix(m_data_base->get_g_tensor());
		if (g_tensor_inverse.size() == 1)
		{
			AfxMessageBox(_T("Computing inverse g-tensor failed. Using 1/g:e instead.\n\nPlease reset g-tensor."), MB_ICONWARNING);
			g_tensor_inverse = inverse_Matrix({ {g_e, 0.0, 0.0}, {0.0, g_e, 0.0}, {0.0, 0.0, g_e} });
		}
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				g_tensor_inverse[i][j] *= g_e;
			}
		}
		std::vector<std::vector<std::vector<double>>> tmp_hfc_complete;
		for (size_t i = 0; i < hfc_complete.size(); i++)
		{
			tmp_hfc_complete.push_back(MatrixMultiplication(hfc_complete[i], g_tensor_inverse));
		}
		hfc_complete.clear();
		hfc_complete = tmp_hfc_complete;
		std::vector<size_t> indices_to_print;
		for (size_t i = 0; i < m_data_base->get_mol_atom_count(); i++)
		{
			if (m_data_base->get_mol_element_by_index(i) == CStr_element)
			{
				indices_to_print.push_back(i);
			}
		}
		for (size_t i = 0; i < indices_to_print.size(); i++)
		{
			file << std::to_string(indices_to_print[i]);
			for (size_t j = 0; j < 3; j++)
			{
				file << "\t" << hfc_complete[indices_to_print[i]][j][0] << "\t" << hfc_complete[indices_to_print[i]][j][1] << "\t" << hfc_complete[indices_to_print[i]][j][2];
			}
			file << std::endl;
		}
		file << std::endl;
		file << std::endl;
		file << "Units in Hz" << std::endl;
		file << "index\thfc_xx\thfc_xy\thfc_xz\thfc_yx\thfc_yy\thfc_yz\thfc_zx\thfc_zy\thfc_zz" << std::endl;
		for (size_t i = 0; i < indices_to_print.size(); i++)
		{
			file << std::to_string(indices_to_print[i]);
			for (size_t j = 0; j < 3; j++)
			{
				file << "\t" << hfc_inhz_complete[indices_to_print[i]][j][0] << "\t" << hfc_inhz_complete[indices_to_print[i]][j][1] << "\t" << hfc_inhz_complete[indices_to_print[i]][j][2];
			}
			file << std::endl;
		}
	}
	else
	{
		file << "No HFC data was selected";
	}
}