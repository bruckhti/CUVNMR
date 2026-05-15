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
#include "Simulation.h"
#include "CMainTab2.h"
#include "physical_constants.h"
#include "iomanip"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSimulation, CWnd)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CSimulation::set_fitusage(bool reset)
{
	if(reset)
	{
		m_save_exp_dorb.clear();
		m_save_weight.clear();
		m_save_elements.clear();
		m_save_groupNames.clear();
	}
	else
	{
		m_save_exp_dorb.clear();
		m_save_weight.clear();
		m_save_elements.clear();
		m_save_groupNames.clear();
		for (size_t i = 0; i < m_listedWeight.size(); i++)
		{
			if (m_listedWeight[i] != 0.0)
			{
				m_save_weight.push_back(m_listedWeight[i]);
				m_save_exp_dorb.push_back(m_listedExpShift[i]);
				std::string tmp_name(m_listedGroupName[i].begin(), m_listedGroupName[i].end());;
				std::string tmp_element(m_listedElement[i].begin(), m_listedElement[i].end());;
				m_save_groupNames.push_back(tmp_name);
				m_save_elements.push_back(tmp_element);
			}
		}
	}
}

void CSimulation::reset_error()
{
	m_chi_error.clear();
	m_chi_error_cgi.clear();
	m_signal_error.clear();
	m_chi_error = std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0));
	m_chi_error_cgi = m_chi_error;
	error_list.clear();
	m_chi2_quad = 0.0;
	m_SStot = 0.0;
	m_SSreg = 0.0;
	m_SSres = 0.0;
	m_R2 = 0.0;
	m_R2corr = 0.0;
}

void CSimulation::Reset_listed()
{
	m_listedGroupName.clear();
	m_listedGroupIndices.clear();
	m_listedElement.clear();
	m_listedExpShift.clear();
	m_listedWeight.clear();
	m_listedSimulate.clear();
	ListSize = 0;
	m_main_list->DeleteAllItems();
	m_FWHM.clear();
	m_chemShift_Sim.clear();
	UpdateList();
}

void CSimulation::Reset_computed()
{
	m_computedOrb.clear();
	m_computedHF.clear();
	m_computedFC.clear();
	m_computedPC.clear();
	m_computedTotal.clear();
	m_computedLineWidth.clear();
	m_computedOrb.resize(ListSize);
	m_computedHF.resize(ListSize);
	m_computedFC.resize(ListSize);
	m_computedPC.resize(ListSize);
	m_computedTotal.resize(ListSize);
	m_computedLineWidth.resize(ListSize);
}

void CSimulation::AddToList(std::wstring GroupName, std::vector<int> GroupIndices, std::wstring Element, double ExpShift, double Weight, bool Simulate)
{
	m_listedGroupName.push_back(GroupName);
	m_listedGroupIndices.push_back(GroupIndices);
	m_listedElement.push_back(Element);
	m_listedExpShift.push_back(ExpShift);
	m_listedWeight.push_back(Weight);
	m_listedSimulate.push_back(Simulate);
	m_FWHM.push_back(0.0);
	m_chemShift_Sim.push_back(0.0);
	ListSize++;
	UpdateList();
}

void CSimulation::ModifyList_GroupName(int Entry, std::wstring GroupName)
{
	if (Entry < static_cast<int>(ListSize))
	{
		m_listedGroupName[Entry] = GroupName;
	}
	UpdateList();
}

void CSimulation::ModifyList_ExpShift(int Entry, double ExpShift)
{
	if (Entry < static_cast<int>(ListSize))
	{
		m_listedExpShift[Entry] = ExpShift;
	}
	UpdateList();
}

void CSimulation::ModifyList_Weight(int Entry, double Weight)
{
	if (Entry < static_cast<int>(ListSize))
	{
		m_listedWeight[Entry] = Weight;
	}
	UpdateList();
}

void CSimulation::ModifyList_simulate(int Entry, bool simulate)
{
	if (Entry < static_cast<int>(ListSize))
	{
		m_listedSimulate[Entry] = simulate;
	}
	UpdateList();
}

void CSimulation::RemoveFromList(int Entry)
{
	if (Entry < static_cast<int>(ListSize))
	{
		m_listedGroupName.erase(m_listedGroupName.begin() + Entry);
		m_listedGroupIndices.erase(m_listedGroupIndices.begin() + Entry);
		m_listedElement.erase(m_listedElement.begin() + Entry);
		m_listedExpShift.erase(m_listedExpShift.begin() + Entry);
		m_listedWeight.erase(m_listedWeight.begin() + Entry);
		m_listedSimulate.erase(m_listedSimulate.begin() + Entry);
		m_FWHM.erase(m_FWHM.begin() + Entry);
		m_chemShift_Sim.erase(m_chemShift_Sim.begin() + Entry);
		ListSize--;
	}
	UpdateList();
}

bool CSimulation::IndexListed(int index)
{
	for (const auto& vec : m_listedGroupIndices)
	{
		for (const auto& i : vec)
		{
			if (i == index)
			{
				return true;
			}
		}
	}
	return false;
}

std::wstring CSimulation::GetListedIndicesW(int Entry)
{
	std::wstring return_value;
	if (Entry < static_cast<int>(ListSize))
	{
		bool first = true;
		for (auto i : m_listedGroupIndices[Entry])
		{
			auto str = std::to_wstring(i);
			if (!first)
			{
				return_value = return_value + L", ";
			}
			return_value = return_value + str;
			first = false;
		}
	}
	return return_value;
}

void CSimulation::InitList(CListCtrl* main_list)
{
	m_main_list = main_list;
}

void CSimulation::UpdateList()
{
	m_main_list->DeleteAllItems();
	for (auto slider : m_Slider)
	{
		if (slider->GetSafeHwnd())
		{
			slider->ShowWindow(SW_HIDE);
			slider->DestroyWindow();
			delete slider;
		}
	}
	m_Slider.clear();
	for (auto cBox : m_CheckBox)
	{
		if (cBox->GetSafeHwnd())
		{
			cBox->ShowWindow(SW_HIDE);
			cBox->DestroyWindow();
			delete cBox;
		}
	}
	m_CheckBox.clear();
	for (size_t i = 0; i < ListSize; i++)
	{
		m_main_list->InsertItem((int)i, m_listedGroupName[i].c_str());
		m_main_list->SetItemText((int)i, 1, GetListedIndicesW((int)i).c_str());
		m_main_list->SetItemText((int)i, 2, m_listedElement[i].c_str());
		m_main_list->SetItemText((int)i, 3, std::to_wstring(m_listedExpShift[i]).c_str());
		m_main_list->SetItemText((int)i, 4, std::to_wstring(m_listedWeight[i]).c_str());
		m_main_list->SetItemText((int)i, 5, L"");
		CListSliderCtrl* pSlider = new CListSliderCtrl(m_listedWeight, (int)i);
		CRect rect;
		m_main_list->GetSubItemRect((int)i, 4, LVIR_BOUNDS, rect);
		int columnWidth = m_main_list->GetColumnWidth(4);
		rect.right = rect.left + columnWidth;
		pSlider->Create(WS_CHILD | WS_VISIBLE | TBS_HORZ, rect, m_main_list, 3001 + (int)i);
		m_Slider.push_back(pSlider);
		m_Slider[i]->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
		m_Slider[i]->SetRange(0, 1000);
		m_Slider[i]->SetPos(static_cast<int>(m_listedWeight[i]*1000));
		m_Slider[i]->ShowWindow(SW_SHOW);
		CListButton* pCBox = new CListButton(m_listedSimulate, (int)i, 4001 + (int)i);
		m_main_list->GetSubItemRect((int)i, 5, LVIR_BOUNDS, rect);
		rect.DeflateRect(25, 0);
		columnWidth = m_main_list->GetColumnWidth(5) - 48;
		rect.right = rect.left + columnWidth;
		pCBox->Create(_T(""), WS_CHILD | WS_VISIBLE | BS_CHECKBOX, rect, m_main_list, 4001 + (int)i);
		m_CheckBox.push_back(pCBox);
		m_CheckBox[i]->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
		if (m_listedSimulate[i])
		{
			m_CheckBox[i]->SetCheck(BST_CHECKED);
		}
		else
		{
			m_CheckBox[i]->SetCheck(BST_UNCHECKED);
		}
		m_CheckBox[i]->ShowWindow(SW_SHOW);
	}
}

void CSimulation::SetEssentials_g(std::vector<std::vector<double>>& g_tensor, bool g_init)
{
	m_g_tensor.clear();
	m_g_tensor = g_tensor;
	m_g_tensor_inverse = inverse_Matrix(g_tensor);
	if (m_g_tensor_inverse.size() == 1)
	{
		AfxMessageBox(_T("Computing inverse g-tensor failed. Using 1/g_e instead.\n\nPlease reset g-Tensor."), MB_ICONERROR);
		m_g_tensor_inverse = inverse_Matrix({ {g_e, 0.0, 0.0}, {0.0, g_e, 0.0}, {0.0, 0.0, g_e} });
	}
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m_g_tensor_inverse[i][j] *= g_e;
		}
	}
	if (!g_init && m_hfc_tmp.size() > 0)
	{
		m_hfc.clear();
		for (size_t i = 0; i < m_hfc_tmp.size(); i++)
		{
			m_hfc.push_back(MatrixMultiplication(m_hfc_tmp[i], m_g_tensor_inverse));
		}
	}
}

void CSimulation::SetEssentials_dorb(std::vector<double>& d_orb)
{
	m_dorb.clear();
	m_dorb = d_orb;
}

void CSimulation::SetEssentials_HFC(std::vector<std::vector<std::vector<double>>>& HFCs, std::vector<std::vector<std::vector<double>>>& HFCs_inHz)
{
	m_hfc_tmp.clear();
	m_hfc_tmp = HFCs;
	m_hfc.clear();
	for (size_t i = 0; i < m_hfc_tmp.size(); i++)
	{
		m_hfc.push_back(MatrixMultiplication(m_hfc_tmp[i], m_g_tensor_inverse));
	}
	m_hfc_inHz.clear();
	m_hfc_inHz = HFCs_inHz;
}

void CSimulation::SetEssentials_FC(std::vector<double> FCs)
{
	m_fc.clear();
	m_fc = FCs;
}

void CSimulation::setEssentials_Shielding(std::vector<std::vector<std::vector<double>>>& shieldingTensors)
{
	m_shieldingTensors.clear();
	m_shieldingTensors = shieldingTensors;
}

void CSimulation::SetFitType(int type)
{
	m_FitType = type;
}

void CSimulation::SetFitTemperature(double temperature)
{
	m_FitTemperature = temperature;
}

std::vector<std::vector<std::vector<double>>> CSimulation::m_hfc;
std::vector<double> CSimulation::m_dorb;
std::vector<std::vector<int>> CSimulation::m_listedGroupIndices;
std::vector<double> CSimulation::m_listedExpShift;
std::vector<double> CSimulation::m_listedWeight;
std::vector<std::vector<int>> CSimulation::m_symmetry;
int CSimulation::m_FitType = 0;
double CSimulation::m_FitTemperature = 298.0;
std::vector<double> CSimulation::m_fc;
std::vector<std::vector<double>> CSimulation::m_R;
std::vector<double> CSimulation::m_yfit;

double CSimulation::RMSD(std::vector<double> chi)
{
	m_yfit.clear();
	std::vector<std::vector<double>> chi_tens(3, std::vector<double>(3, 0.0));
	int sym_val = -1;
	std::vector<std::vector<bool>> sym_checked(3, std::vector<bool>(3, false));
	for (size_t n = 0; n < chi.size(); n++)
	{
		sym_val = -1;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				if (m_symmetry[i][j] == 0)
				{
					chi_tens[i][j] = 0.0;
					sym_checked[i][j] = true;
				}
				if (sym_checked[i][j] == false)
				{
					if (sym_val == -1)
					{
						sym_val = m_symmetry[i][j];
					}
					if (sym_val == m_symmetry[i][j])
					{
						chi_tens[i][j] = chi[n];
						sym_checked[i][j] = true;
					}
				}
			}
		}
	}
	std::vector<double> w_i;
	std::vector<double> w_ii;
	for (size_t i = 0; i < m_listedWeight.size(); i++)
	{
		w_i.emplace_back(m_listedWeight[i]);
		w_ii.emplace_back(m_listedWeight[i]);
	}
	double inner_sum = 0.0;
	double outer_sum = 0.0;
	double trace = 0.0;
	int atom_index = -1;
	double tmp = 0.0;
	double CHI_iso = (1.0 / 3.0) * (chi_tens[0][0] + chi_tens[1][1] + chi_tens[2][2]);
	double A_iso = 0.0;
	for (size_t i = 0; i < m_listedGroupIndices.size(); i++)
	{
		tmp = 0.0;
		inner_sum = 0.0;
		for (size_t j = 0; j < m_listedGroupIndices[i].size(); j++)
		{
			atom_index = m_listedGroupIndices[i][j];
			trace = 0.0;
			if (m_FitType == 0)
			{
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						trace += m_hfc[atom_index][c][d] * chi_tens[d][c];
					}
				}
			}
			else if (m_FitType == 1)
			{
				double distance_length = sqrt(pow(m_R[atom_index][0], 2.0) + pow(m_R[atom_index][1], 2.0) + pow(m_R[atom_index][2], 2.0));
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						trace += 3.0 * chi_tens[c][d] * m_R[atom_index][c] * m_R[atom_index][d];
					}
				}
				trace -= ((pow(distance_length, 2.0) * chi_tens[0][0]) + (pow(distance_length, 2.0) * chi_tens[1][1]) + (pow(distance_length, 2.0) * chi_tens[2][2]));
				trace *= (3.0 * pow(10.0, 6.0)) / (12.0 * PI * pow(distance_length, 5.0));
				double fermi_contact = 0.0;
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						fermi_contact += m_hfc[atom_index][c][c] * chi_tens[d][d];
					}
				}
				fermi_contact /= 9.0;
				trace += 3.0 * fermi_contact;
			}
			else if (m_FitType == 2)
			{
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						if (c == d)
						{
							trace += 2.0 * m_hfc[atom_index][c][d] * chi_tens[d][c];
						}
						else
						{
							trace += 3.0 * m_hfc[atom_index][c][d] * chi_tens[d][c];
						}
					}
				}
				trace -= (chi_tens[0][0] * m_hfc[atom_index][1][1] + chi_tens[0][0] * m_hfc[atom_index][2][2] +
					chi_tens[1][1] * m_hfc[atom_index][0][0] + chi_tens[1][1] * m_hfc[atom_index][2][2] +
					chi_tens[2][2] * m_hfc[atom_index][0][0] + chi_tens[2][2] * m_hfc[atom_index][1][1]);
				trace /= 3.0;
				trace += 3.0 * m_fc[atom_index] / m_FitTemperature;
			}
			else if (m_FitType == 3)
			{
				double distance_length = sqrt(pow(m_R[atom_index][0], 2.0) + pow(m_R[atom_index][1], 2.0) + pow(m_R[atom_index][2], 2.0));
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						trace += 3.0 * chi_tens[c][d] * m_R[atom_index][c] * m_R[atom_index][d];
					}
				}
				trace -= ((pow(distance_length, 2.0) * chi_tens[0][0]) + (pow(distance_length, 2.0) * chi_tens[1][1]) + (pow(distance_length, 2.0) * chi_tens[2][2]));
				trace *= (3.0 * pow(10.0, 6.0)) / (12.0 * PI * pow(distance_length, 5.0));
				trace += 3.0 * m_fc[atom_index] / m_FitTemperature;
			}
			inner_sum += m_dorb[atom_index] + (1.0 / 3.0) * trace;
		}
		m_yfit.emplace_back(inner_sum / (double)m_listedGroupIndices[i].size());
		inner_sum *= (w_i[i] / (double)m_listedGroupIndices[i].size());
		tmp = inner_sum - (w_ii[i] * m_listedExpShift[i]);
		outer_sum += pow(tmp, 2.0);
	}
	return sqrt(outer_sum);
}

void CSimulation::SetSymmetry(std::vector<std::vector<int>>& symmetry)
{
	m_symmetry = symmetry;
}

bool CSimulation::WeightsOK()
{
	for (size_t i = 0; i < m_listedWeight.size(); i++)
	{
		if (m_listedWeight[i] > 0.0)
		{
			return true;
		}
	}
	return false;
}

void CSimulation::SetPDDistance(std::vector<std::vector<double>>& PD_distances)
{
	m_R.clear();
	m_R = PD_distances;
	m_R_SI.clear();
	m_R_SI = m_R;
	for (size_t i = 0; i < m_R.size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m_R_SI[i][j] *= pow(10.0, -10.0);
		}
	}
}


void CSimulation::chi_error(double temperature)
{
	std::vector<std::vector<double>> covariance;
	error_list.clear();
	std::vector<std::vector<double>> SigmaChi(9, std::vector<double>(9, 0.0));
	std::vector<std::vector<double>> SigmaSignal;
	std::vector<double> w_i;
	for (size_t i = 0; i < m_listedWeight.size(); i++)
	{
		w_i.emplace_back(pow(m_listedWeight[i],2.0));
	}
	std::vector<std::vector<double>> df(9, std::vector<double>(9, 0.0));
	std::vector<int> tmp;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			tmp.push_back(m_symmetry[i][j]);
		}
	}
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			if (tmp[i] == 0)
			{
				df[i][j] = 0.0;
			}
			else if (tmp[j] == tmp[i])
			{
				df[i][j] = 1.0;
			}
		}
	}
	std::vector<std::vector<double>> JacobiMaxi(m_listedGroupIndices.size(), std::vector<double>(9, 0.0));
	size_t atom_index;
	for (size_t group = 0; group < m_listedGroupIndices.size(); group++)
	{
		for (size_t param = 0; param < 9; param++)
		{
			for (size_t atoms = 0; atoms < m_listedGroupIndices[group].size(); atoms++)
			{
				atom_index = m_listedGroupIndices[group][atoms];
				for (size_t i = 0; i < 3; i++)
				{
					for (size_t j = 0; j < 3; j++)
					{
						int long_index = i * 3 + j * 1;
						if (m_FitType == 0)
						{
							JacobiMaxi[group][param] += m_hfc[atom_index][j][i] * df[param][long_index];
						}
						else if (m_FitType == 1)
						{
							double distance_length = sqrt(pow(m_R[atom_index][0], 2.0) + pow(m_R[atom_index][1], 2.0) + pow(m_R[atom_index][2], 2.0));
							if (i == j && df[param][long_index] != 0)
							{
								JacobiMaxi[group][param] += (1.0 / 9.0) * (m_hfc[atom_index][0][0] + m_hfc[atom_index][1][1] + m_hfc[atom_index][2][2]);
								JacobiMaxi[group][param] += (pow(10.0, 6.0) / (12.0 * PI * pow(distance_length, 5.0))) * (m_R[atom_index][i] * m_R[atom_index][j] * 3.0 - pow(distance_length, 2.0));
							}
							else if (i != j && df[param][long_index] != 0)
							{
								JacobiMaxi[group][param] += ((3.0 * pow(10.0, 6.0)) / (12.0 * PI * pow(distance_length, 5.0))) * m_R[atom_index][i] * m_R[atom_index][j];
							}
						}
						else if (m_FitType == 2)
						{
							if (i == j && df[param][long_index] != 0)
							{
								for (size_t o = 0; o < 3; o++)
								{
									if (o != i)
									{
										JacobiMaxi[group][param] -= (1.0 / 9.0) * m_hfc[atom_index][o][o];
									}
								}
								JacobiMaxi[group][param] += (2.0 / 9.0) * m_hfc[atom_index][i][i];
							}
							else if (i != j && df[param][long_index] != 0)
							{
								JacobiMaxi[group][param] += (3.0 / 9.0) * m_hfc[atom_index][j][i];
							}
						}
						else if (m_FitType == 3)
						{
							double distance_length = sqrt(pow(m_R[atom_index][0], 2.0) + pow(m_R[atom_index][1], 2.0) + pow(m_R[atom_index][2], 2.0));
							if (i == j && df[param][long_index] != 0)
							{
								JacobiMaxi[group][param] = (1.0 / pow(distance_length, 5.0)) * ((3.0 * m_R[atom_index][i] * m_R[atom_index][j]) - pow(distance_length, 2.0));
							}
							else if (i != j && df[param][long_index] != 0)
							{
								JacobiMaxi[group][param] = (3.0 / pow(distance_length, 5.0)) * m_R[atom_index][i] * m_R[atom_index][j];
							}
						}
					}
				}
			}
			if (m_FitType == 0)
			{
				JacobiMaxi[group][param] /= 3.0 * (double)m_listedGroupIndices[group].size();
			}
			else if (m_FitType == 1 || m_FitType == 2)
			{
				JacobiMaxi[group][param] /= (double)m_listedGroupIndices[group].size();
			}
			else if (m_FitType == 3)
			{
				JacobiMaxi[group][param] *= pow(10.0,6.0)/(12.0*PI*(double)m_listedGroupIndices[group].size());
			}
		}
	}
	std::vector<std::vector<double>> weight_mat(w_i.size(), std::vector<double>(w_i.size(), 0.0));
	for (size_t i = 0; i < w_i.size(); i++)
	{
		weight_mat[i][i] = w_i[i];
	}
	std::vector<std::vector<double>> fisher_information = MatrixMultiplication(transpose(JacobiMaxi), MatrixMultiplication(weight_mat, JacobiMaxi));
	std::vector<int> tmp_mod = tmp;
	for (size_t i = 0; i < 9; i++)
	{
		if (tmp_mod[i] != 0)
		{
			for (size_t j = 0; j < 9; j++)
			{
				if (i != j && tmp_mod[j] == tmp_mod[i])
				{
					tmp_mod[j] = 0;
				}
			}
		}
	}
	std::vector<std::vector<double>> fisher_information_edited;
	for (size_t i = 0; i < 9; i++)
	{
		if (tmp_mod[i] != 0)
		{
			fisher_information_edited.emplace_back(std::vector<double>());
			for (size_t j = 0; j < 9; j++)
			{
				if (tmp_mod[j] != 0)
				{
					fisher_information_edited[fisher_information_edited.size() - 1].push_back(fisher_information[i][j]);
				}
			}
		}
	}
	double det = determinant(fisher_information_edited, true);
	if (abs(det) < 1E-10)
	{ 
		error_list.push_back(1);
		for (size_t i = 0; i < fisher_information_edited.size(); i++)
		{
			fisher_information_edited[i][i] += 0.001;
		}
		det = determinant(fisher_information_edited, true);
	}
	double p = 0.0;
	double N = 0.0;
	double y_mean = 0.0;
	for (size_t i = 0; i < m_listedGroupIndices.size(); i++)
	{
		y_mean += w_i[i] * m_listedExpShift[i];
	}
	if (det != 0)
	{
		double DOF = 0;
		N = static_cast<double>(w_i.size());
		double sum_w = 0.0;
		for (size_t i = 0; i < w_i.size(); i++)
		{
			sum_w += w_i[i];
		}
		y_mean /= sum_w;
		std::vector<int> sym_tmp = tmp;
		for (size_t i = 0; i < 9; i++)
		{
			if (sym_tmp[i] != 0)
			{
				p++;
				for (size_t j = 0; j < 9; j++)
				{
					if (j != i && sym_tmp[j] == sym_tmp[i])
					{
						sym_tmp[j] = 0;
					}
				}
			}
		}
		DOF = N - p;
		double red_chi_quad = 0.0;
		m_SSreg = 0.0;
		m_SSres = 0.0;
		m_SStot = 0.0;
		for (size_t group = 0; group < m_listedGroupIndices.size(); group++)
		{
			m_SStot += w_i[group] * pow((m_listedExpShift[group] - y_mean), 2.0);
			m_SSreg += w_i[group] * pow((m_yfit[group] - y_mean), 2.0);
			m_SSres += w_i[group] * pow((m_listedExpShift[group] - m_yfit[group]), 2.0);
		}
		if (DOF > 0)
		{
			red_chi_quad = m_SSres/DOF;
		}
		else
		{
			error_list.push_back(3);
			red_chi_quad = 1.0;
		}
		m_chi2_quad = red_chi_quad;
		std::vector<std::vector<double>> fisher_information_inv = invert_nxnMatrix(fisher_information_edited);
		for (auto& fisher_row : fisher_information_inv)
		{
			for (auto& fisher_element : fisher_row)
			{
				fisher_element *= red_chi_quad;
			}
		}
		size_t ind_i = 0;
		size_t ind_j = 0;
		for (size_t i = 0; i < 9; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				if (tmp_mod[i] != 0 && tmp_mod[j] != 0)
				{
					SigmaChi[i][j] = fisher_information_inv[ind_i][ind_j];
					ind_j++;
				}
			}
			if (tmp_mod[i] != 0)
			{
				ind_i++;
				ind_j = 0;
			}
		}
		for (size_t i = 0; i < 9; i++)
		{
			if (tmp[i] != 0 && tmp_mod[i] != 0)
			{
				for (size_t j = 0; j < 9; j++)
				{
					if (tmp[i] == tmp[j])
					{
						SigmaChi[j][j] = SigmaChi[i][i];
						for (size_t n = 0; n < 9; n++)
						{
							if (n != j)
							{
								SigmaChi[j][n] = SigmaChi[i][n];
								SigmaChi[n][j] = SigmaChi[n][i];
							}
						}
					}
				}
			}
		}
		covariance = SigmaChi;
		for (size_t i = 0; i < 9; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				if (SigmaChi[i][j] >= 0.0)
				{
					SigmaChi[i][j] = sqrt(SigmaChi[i][j]);
				}
				else
				{
					if (i == j)
					{
						error_list.push_back(4);
					}
				}
			}
		}
		if (m_SStot == 0.0)
		{
			m_R2 = 0.0;
			error_list.push_back(6);
		}
		else
		{
			m_R2 = 1.0 - ((m_SSres) / (m_SStot));
		}
		if (N - p == 0.0 || N - 1.0 == 0.0 || m_SStot == 0.0)
		{
			m_R2corr = 0.0;
			error_list.push_back(7);
		}
		else
		{
			m_R2corr = 1.0 - ((m_SSres / (N - p)) / (m_SStot / (N - 1.0)));
		}
		SigmaSignal = MatrixMultiplication(JacobiMaxi, MatrixMultiplication(covariance, transpose(JacobiMaxi)));
		for (size_t i = 0; i < SigmaSignal.size(); i++)
		{
			if (SigmaSignal[i][i] >= 0)
			{
				SigmaSignal[i][i] = sqrt(SigmaSignal[i][i]);
			}
			else
			{
				error_list.push_back(5);
			}
		}
	}
	else
	{
		error_list.push_back(2);
		m_SStot = 0.0;
		m_SSreg = 0.0;
		m_SSres = 0.0;
		m_R2 = 0.0;
		m_R2corr = 0.0;
	}
	std::vector<std::vector<double>> SigmaChiCGI = SigmaChi;
	for (auto& sigma_row : SigmaChiCGI)
	{
		for (auto& sigma_element : sigma_row)
		{
			sigma_element = (sigma_element * temperature * N_avo) / (4.0 * PI * pow(10.0, 24.0));
		}
	}
	m_chi_error.clear();
	m_chi_error_cgi.clear();
	m_chi_error = std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0));
	m_chi_error_cgi = m_chi_error;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			m_chi_error[i][j] = SigmaChi[3 * i + 1 * j][3 * i + 1 * j];
			m_chi_error_cgi[i][j] = SigmaChiCGI[3 * i + 1 * j][3 * i + 1 * j];
		}
	}
	if (SigmaSignal.size() != 0)
	{
		m_signal_error.clear();
		for (size_t i = 0; i < SigmaSignal.size(); i++)
		{
			m_signal_error.push_back(SigmaSignal[i][i]);
		}
	}
}

std::vector<std::vector<double>> CSimulation::invert_nxnMatrix(std::vector<std::vector<double>> Matrix)
{
	size_t dim = Matrix.size();
	std::vector<std::vector<double>> left = Matrix;
	std::vector<std::vector<double>> right(dim, std::vector<double>(dim, 0.0));
	for (size_t i = 0; i < dim; i++)
	{
		for (size_t j = 0; j < dim; j++)
		{
			if (i == j)
			{
				right[i][j] = 1.0;
			}
		}
	}
	double tmp;
	bool failed = false;
	for (size_t x = 0; x < dim; x++)
	{
		for (size_t l = x; l < dim; l++)
		{
			if (left[l][x] == 0.0)
			{
				//
			}
			else if (l != x)
			{
				for (size_t t = 0; t < dim; t++)
				{
					tmp = left[x][t];
					left[x][t] = left[l][t];
					left[l][t] = tmp;

					tmp = right[x][t];
					right[x][t] = right[l][t];
					right[l][t] = tmp;
				}
				break;
			}
			else
			{
				break;
			}
		}
		if (left[x][x] != 0)
		{
			double norm = left[x][x];
			for (size_t j = 0; j < dim; j++)
			{
				left[x][j] /= norm;
				right[x][j] /= norm;
			}
			for (size_t i = 0; i < dim; i++)
			{
				if (i != x)
				{
					double factor = left[i][x];
					for (size_t j = 0; j < dim; j++)
					{
						left[i][j] -= left[x][j] * factor;
						right[i][j] -= right[x][j] * factor;
					}
				}
			}
		}
		else
		{
			failed = true;
			break;
		}
	}
	if (failed)
	{
		AfxMessageBox(_T("Inversion Failed"));
		return std::vector<std::vector<double>>(Matrix.size(), std::vector<double>(Matrix[0].size(), 0.0));
	}
	return right;
}

std::vector<std::vector<double>> CSimulation::get_chi_error()
{
	return m_chi_error_cgi;
}

std::vector<std::vector<double>> CSimulation::get_chi_errorSI()
{
	return m_chi_error;
}

double CSimulation::get_chi2red()
{
	return m_chi2_quad;
}

double CSimulation::get_r2()
{
	return m_R2;
}

double CSimulation::get_r2adj()
{
	return m_R2corr;
}

std::vector<int> CSimulation::get_error_list()
{
	return error_list;
}

std::vector<double> CSimulation::get_signal_error()
{
	return m_signal_error;
}

void CSimulation::SetSimChi(std::vector<std::vector<double>>& CHI)
{
	SimChi = CHI;
}

void CSimulation::SetRelaxationTimes(std::vector<double>& t)
{
	RelaxationTimes = t;
}

void CSimulation::SetTotalSpin(double multiplicity)
{
	Spin = (multiplicity - 1.0) / 2.0;
}

void CSimulation::SetMagneticField(double MagFieldMHz)
{
	MagFieldTesla = (MagFieldMHz * 1000000.0) / get_gyromagnetic_ratio("1H", -1, false);
}

void CSimulation::SetSimTemperature(double Temp)
{
	SimTemperature = Temp;
}

void CSimulation::ComputeFWHM(int type_contact, int type_dipolar, int type_curie, CString isotope, std::wstring element)
{
	std::vector<size_t> Groups;
	for (size_t i = 0; i < m_listedElement.size(); i++)
	{
		if (m_listedElement[i] == element)
		{
			Groups.push_back(i);
		}
	}
	m_SimGroupIndices.clear();
	m_SimGroupIndices = Groups;
	m_SimFWHM.clear();
	m_SimFWHM = std::vector<double>(Groups.size(), 0.0);
	m_print_shielding.clear();
	m_print_shielding = std::vector<bool>(m_shieldingTensors.size(), false);
	std::vector<std::vector<std::vector<double>>> TotalShieldings(m_shieldingTensors.size(), std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0)));
	for (size_t group = 0; group < Groups.size(); group++)
	{
		for (size_t atom = 0; atom < m_listedGroupIndices[Groups[group]].size(); atom++)
		{
			TotalShieldings[m_listedGroupIndices[Groups[group]][atom]] = m_shieldingTensors[m_listedGroupIndices[Groups[group]][atom]];
			m_print_shielding[m_listedGroupIndices[Groups[group]][atom]] = true;
			for (size_t i = 0; i < 3; i++)
			{
				for (size_t j = 0; j < 3; j++)
				{
					double matmul = 0.0;
					for (size_t a = 0; a < 3; a++)
					{
						matmul += m_hfc[m_listedGroupIndices[Groups[group]][atom]][i][a] * SimChi[a][j];
					}
					TotalShieldings[m_listedGroupIndices[Groups[group]][atom]][i][j] -= matmul;
				}
			}
		}
	}
	m_saveTotalShieldings.clear();
	m_saveTotalShieldings = TotalShieldings;
	std::wstring wstr = isotope.GetString();
	std::string NucIsotope(wstr.begin(), wstr.end());
	m_saveR2.clear();
	m_saveR2 = std::vector<std::vector<double>>(3, std::vector<double>(Groups.size(), 0.0));
	for (size_t g = 0; g < Groups.size(); g++)
	{
		double fwhm = 0.0;
		for (size_t i = 0; i < m_listedGroupIndices[Groups[g]].size(); i++)
		{
			int atom_index = m_listedGroupIndices[Groups[g]][i];
			double R2_total = 0.0;
			double distance = sqrt(pow(m_R_SI[atom_index][0], 2.0) + pow(m_R_SI[atom_index][1], 2.0) + pow(m_R_SI[atom_index][2], 2.0));
			double tmp_dip_relax = dipolar_relaxation(type_dipolar, NucIsotope, Spin, distance, MagFieldTesla, m_hfc_inHz[atom_index], RelaxationTimes[0], RelaxationTimes[1]);
			double tmp_cont_relax = contact_relaxation(type_contact, NucIsotope, Spin, MagFieldTesla, m_hfc_inHz[atom_index], RelaxationTimes[2], RelaxationTimes[3]);
			double tmp_curie_relax = curie_relaxation(type_curie, NucIsotope, Spin, distance, MagFieldTesla, SimTemperature, SimChi, TotalShieldings[atom_index], RelaxationTimes[4]);
			R2_total += tmp_dip_relax + tmp_cont_relax + tmp_curie_relax;
			m_saveR2[0][g] += tmp_dip_relax / m_listedGroupIndices[Groups[g]].size();
			m_saveR2[1][g] += tmp_cont_relax / m_listedGroupIndices[Groups[g]].size();
			m_saveR2[2][g] += tmp_curie_relax / m_listedGroupIndices[Groups[g]].size();
			fwhm += ((R2_total / PI) * ((2.0 * PI) / (get_gyromagnetic_ratio(NucIsotope, -1, true) * MagFieldTesla)) * pow(10.0, 6.0));
		}
		m_FWHM[Groups[g]] = abs(fwhm / static_cast<int>(m_listedGroupIndices[Groups[g]].size()));
		m_SimFWHM[g] = m_FWHM[Groups[g]];
	}
}

double CSimulation::j_function(double omega, double tau)
{
	return (tau / (1.0 + (pow(omega, 2.0) * pow(tau, 2.0))));
}

double CSimulation::second_rank_invariant(std::vector<std::vector<double>>& xyz_Matrix)
{
	return (pow(xyz_Matrix[0][0], 2.0) + pow(xyz_Matrix[1][1], 2.0) + pow(xyz_Matrix[2][2], 2.0) - (xyz_Matrix[0][0] * xyz_Matrix[1][1]) - (xyz_Matrix[0][0] * xyz_Matrix[2][2]) - (xyz_Matrix[1][1] * xyz_Matrix[2][2]) + ((3.0 / 4.0) * (pow((xyz_Matrix[0][1] + xyz_Matrix[1][0]), 2.0) + pow((xyz_Matrix[0][2] + xyz_Matrix[2][0]), 2.0) + pow((xyz_Matrix[1][2] + xyz_Matrix[2][1]), 2.0))));
}

double CSimulation::first_rank_invariant(std::vector<std::vector<double>>& xyz_Matrix)
{
	return (pow((xyz_Matrix[0][1] - xyz_Matrix[1][0]), 2.0) + pow((xyz_Matrix[0][2] - xyz_Matrix[2][0]), 2.0) + pow((xyz_Matrix[1][2] - xyz_Matrix[2][1]), 2.0));
}

double CSimulation::dipolar_relaxation(int type, std::string isotope, double S, double r, double B_0, std::vector<std::vector<double>>& HFC_hz, double tau_dip_1E, double tau_dip_2E)
{
	double R2;
	double omega_N;
	double omega_E;
	omega_N = -(get_gyromagnetic_ratio(isotope, -1, false) * B_0);
	omega_E = -(get_gyromagnetic_ratio("E", -1, false) * B_0);
	if (type == 1)
	{
		R2 = (1.0 / 15.0) * pow((mu0 / (4.0 * PI)), 2.0) * ((pow(get_gyromagnetic_ratio(isotope, -1, false), 2.0) * pow(g_e, 2.0) * pow(muB, 2.0) * S * (S + 1.0)) / pow(r, 6.0)) * (4.0 * j_function(0.0, tau_dip_1E) + 3.0 * j_function(omega_N, tau_dip_1E) + 6.0 * j_function(omega_E, tau_dip_2E) + 6.0 * j_function((omega_E + omega_N), tau_dip_2E) + j_function((omega_E - omega_N), tau_dip_2E));
	}
	else if (type == 0)
	{
		R2 = (1.0 / 180.0) * (second_rank_invariant(HFC_hz) * S * (S + 1.0)) * (4.0 * j_function(0.0, tau_dip_1E) + 3.0 * j_function(omega_N, tau_dip_1E) + 6.0 * j_function(omega_E, tau_dip_2E) + 6.0 * j_function((omega_E + omega_N), tau_dip_2E) + j_function((omega_E - omega_N), tau_dip_2E));
	}
	else if (type == 2)
	{
		R2 = 0.0;
	}
	else
	{
		R2 = 0.0;
	}
	return R2;
}

double CSimulation::contact_relaxation(int type, std::string isotope, double S, double B_0, std::vector<std::vector<double>>& HFC_hz, double tau_con_1E, double tau_con_2E)
{
	double R2;
	double omega_N;
	double omega_E;
	double A_iso;
	omega_N = -(get_gyromagnetic_ratio(isotope, -1, false) * B_0);
	omega_E = -(get_gyromagnetic_ratio("E", -1, false) * B_0);
	A_iso = ((1.0 / 3.0) * (HFC_hz[0][0] + HFC_hz[1][1] + HFC_hz[2][2]));
	if (type == 1)
	{
		R2 = 0.0;
	}
	else if (type == 0)
	{
		R2 = (1.0 / 3.0) * (S * (S + 1)) * pow((A_iso), 2.0) * (j_function(0.0, tau_con_1E) + j_function((omega_E - omega_N), tau_con_2E));
	}
	else
	{
		R2 = 0.0;
	}
	return R2;
}

double CSimulation::curie_relaxation(int type, std::string isotope, double S, double r, double B_0, double T, std::vector<std::vector<double>>& Chi, std::vector<std::vector<double>>& sigma, double tau_curie)
{
	double R2;
	double omega_N;
	double Chi_iso;
	double r_angstrom;
	omega_N = -(get_gyromagnetic_ratio(isotope, -1, false) * B_0);
	Chi_iso = (1.0/3.0) * (Chi[0][0] + Chi[1][1] + Chi[2][2]);
	r_angstrom = r * pow(10.0, 10.0);
	double sigma_ppm_to_no_unit = pow(pow(10.0, -6.0), 2.0);
	if (type == 1)
	{
		R2 = (1.0 / 5.0) * pow((mu0 / (4 * PI)), 2.0) * ((pow(get_gyromagnetic_ratio(isotope, -1, false), 2.0) * pow(B_0, 2.0) * pow(g_e, 4.0) * pow(muB, 4.0)) / (pow((3.0 * k_B * T), 2.0) * pow(r, 6.0))) * pow((S * (S + 1)), 2.0) * (4 * j_function(0, tau_curie) + 3 * j_function(omega_N, tau_curie));
	}
	else if (type == 2)
	{
		R2 = (1.0 / 5.0) * pow((1 / (4 * PI)), 2.0) * ((pow(get_gyromagnetic_ratio(isotope, -1, false), 2.0) * pow(B_0, 2.0)) / pow(r_angstrom, 6.0)) * pow(Chi_iso, 2.0) * (4 * j_function(0, tau_curie) + 3 * j_function(omega_N, tau_curie));
	}
	else if (type == 0)
	{
		R2 = ((1.0 / 4.0) * first_rank_invariant(sigma) * sigma_ppm_to_no_unit * pow(get_gyromagnetic_ratio(isotope, -1, false), 2.0) * pow(B_0, 2.0) * j_function((3 * omega_N), tau_curie)) + ((1.0 / 45.0) * second_rank_invariant(sigma) * sigma_ppm_to_no_unit * pow(get_gyromagnetic_ratio(isotope, -1, false), 2.0) * pow(B_0, 2.0) * (4 * j_function(0, tau_curie) + 3 * j_function(omega_N, tau_curie)));
	}
	else if (type == 3)
	{
		R2 = 0.0;
	}
	else
	{
		R2 = 0.0;
	}
	return R2;
}

void CSimulation::ComputeChemShift(int shift_type, std::wstring element)
{
	std::vector<size_t> Groups;
	for (size_t i = 0; i < m_listedElement.size(); i++)
	{
		if (m_listedElement[i] == element)
		{
			Groups.push_back(i);
		}
	}
	m_SimIsoOrb.clear();
	m_SimIsoOrb = std::vector<double>(Groups.size(), 0.0);
	m_SimIsoFc.clear();
	m_SimIsoFc = std::vector<double>(Groups.size(), 0.0);
	m_SimIsoPcs.clear();
	m_SimIsoPcs = std::vector<double>(Groups.size(), 0.0);
	for (size_t g = 0; g < Groups.size(); g++)
	{
		double shift = 0.0;
		for (size_t i = 0; i < m_listedGroupIndices[Groups[g]].size(); i++)
		{
			int atom_index = m_listedGroupIndices[Groups[g]][i];
			double trace = 0.0;
			if (shift_type == 0)
			{
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						trace += m_hfc[atom_index][c][d] * SimChi[d][c];
					}
				}
				m_SimIsoFc[g] += ((1.0 / 9.0) * (m_hfc[atom_index][0][0] + m_hfc[atom_index][1][1] + m_hfc[atom_index][2][2]) * (SimChi[0][0] + SimChi[1][1] + SimChi[2][2]));
			}
			else if (shift_type == 1)
			{
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						if (c == d)
						{
							trace += 2.0 * m_hfc[atom_index][c][d] * SimChi[d][c];
						}
						else
						{
							trace += 3.0 * m_hfc[atom_index][c][d] * SimChi[d][c];
						}
					}
				}
				trace -= (SimChi[0][0] * m_hfc[atom_index][1][1] + SimChi[0][0] * m_hfc[atom_index][2][2] +
					SimChi[1][1] * m_hfc[atom_index][0][0] + SimChi[1][1] * m_hfc[atom_index][2][2] +
					SimChi[2][2] * m_hfc[atom_index][0][0] + SimChi[2][2] * m_hfc[atom_index][1][1]);
				trace /= 3.0;
				trace += 3.0 * m_fc[atom_index] / SimTemperature;
				m_SimIsoFc[g] += m_fc[atom_index] / SimTemperature;
			}
			else if (shift_type == 2)
			{
				double distance_length = sqrt(pow(m_R[atom_index][0], 2.0) + pow(m_R[atom_index][1], 2.0) + pow(m_R[atom_index][2], 2.0));
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						trace += 3.0 * SimChi[c][d] * m_R[atom_index][c] * m_R[atom_index][d];
					}
				}
				trace -= ((pow(distance_length, 2.0) * SimChi[0][0]) + (pow(distance_length, 2.0) * SimChi[1][1]) + (pow(distance_length, 2.0) * SimChi[2][2]));
				trace *= (3.0 * pow(10.0, 6.0)) / (12.0 * PI * pow(distance_length, 5.0));
				double fermi_contact = 0.0;
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						fermi_contact += m_hfc[atom_index][c][c] * SimChi[d][d];
					}
				}
				fermi_contact /= 9.0;
				trace += 3.0 * fermi_contact;
				m_SimIsoFc[g] += fermi_contact;
			}
			else if (shift_type == 3)
			{
				double distance_length = sqrt(pow(m_R[atom_index][0], 2.0) + pow(m_R[atom_index][1], 2.0) + pow(m_R[atom_index][2], 2.0));
				for (size_t c = 0; c < 3; c++)
				{
					for (size_t d = 0; d < 3; d++)
					{
						trace += 3.0 * SimChi[c][d] * m_R[atom_index][c] * m_R[atom_index][d];
					}
				}
				trace -= ((pow(distance_length, 2.0) * SimChi[0][0]) + (pow(distance_length, 2.0) * SimChi[1][1]) + (pow(distance_length, 2.0) * SimChi[2][2])); 
				trace *= (3.0 * pow(10.0, 6.0)) / (12.0 * PI * pow(distance_length, 5.0));
				trace += 3.0 * m_fc[atom_index] / SimTemperature;
				m_SimIsoFc[g] += m_fc[atom_index] / SimTemperature;
			}
			shift += m_dorb[atom_index] + (1.0 / 3.0) * trace;
			m_SimIsoOrb[g] += m_dorb[atom_index];
			m_SimIsoPcs[g] += (1.0 / 3.0) * trace;
		}
		m_chemShift_Sim[Groups[g]] = (shift / static_cast<int>(m_listedGroupIndices[Groups[g]].size()));
		m_SimIsoOrb[g] /= static_cast<int>(m_listedGroupIndices[Groups[g]].size());
		m_SimIsoFc[g] /= static_cast<int>(m_listedGroupIndices[Groups[g]].size());
		m_SimIsoPcs[g] /= static_cast<int>(m_listedGroupIndices[Groups[g]].size());
		m_SimIsoPcs[g] -= m_SimIsoFc[g];
	}
}


bool CSimulation::ComputeSpectrum(std::wstring element, double min_ppm, double max_ppm, double precision, double windowMin, double windowMax, double windowStepSize, char type)
{
	double spc_start;
	double spc_end;
	double step_size;
	if (precision < 0)
	{
		spc_start = windowMin;
		spc_end = windowMax;
		step_size = windowStepSize;
	}
	else
	{
		spc_start = min_ppm;
		spc_end = max_ppm;
		if (precision == 0.0)
		{
			precision = 1.0;
		}
		step_size = precision;
	}
	std::vector<size_t> Groups;
	for (size_t i = 0; i < m_listedElement.size(); i++)
	{
		if (m_listedElement[i] == element && m_listedSimulate[i] == true)
		{
			Groups.push_back(i);
		}
	}
	std::vector<double> CS_signal_shift;
	std::vector<double> CS_signal_FWHM;
	std::vector<int> CS_num_eq_atoms;
	for (size_t g = 0; g < Groups.size(); g++)
	{
		CS_signal_shift.push_back(m_chemShift_Sim[Groups[g]]);
		CS_signal_FWHM.push_back(m_FWHM[Groups[g]]);
		CS_num_eq_atoms.push_back(static_cast<int>(m_listedGroupIndices[Groups[g]].size()));
	}
	m_SPECTRUM.clear();
	double shift;
	double intermediate = -INFINITY;
	double intermediate_tmp = 0.0;
	size_t spc_entry = 0;
	size_t add = 0;
	std::pair<double, double> pre_shift;
	bool pre_shift_found = false;
	std::pair<double, double> post_shift;
	bool post_shift_found = false;
	double tmp_prepost = 0.0;
	for (size_t n = 0; n < CS_signal_shift.size(); n++)
	{
		if (CS_signal_shift[n] < spc_start && pre_shift_found == false)
		{
			pre_shift.first = CS_signal_shift[n];
			pre_shift.second = 0.0;
			for (size_t t = 0; t < CS_signal_shift.size(); t++)
			{
				if (type == 'L')
				{
					pre_shift.second += Lorentzian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[t], CS_signal_shift[t], CS_signal_FWHM[0], CS_signal_FWHM[t]);
				}
				else
				{
					pre_shift.second += Gaussian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[t], CS_signal_shift[t], CS_signal_FWHM[0], CS_signal_FWHM[t]);
				}
			}
			pre_shift_found = true;
		}
		else if (CS_signal_shift[n] < spc_start && pre_shift_found == true)
		{
			tmp_prepost = 0.0;
			for (size_t t = 0; t < CS_signal_shift.size(); t++)
			{
				if (type == 'L')
				{
					tmp_prepost += Lorentzian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[t], CS_signal_shift[t], CS_signal_FWHM[0], CS_signal_FWHM[t]);
				}
				else
				{
					tmp_prepost += Gaussian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[t], CS_signal_shift[t], CS_signal_FWHM[0], CS_signal_FWHM[t]);
				}
			}
			if (tmp_prepost > pre_shift.second)
			{
				pre_shift.first = CS_signal_shift[n];
				pre_shift.second = tmp_prepost;
			}
		}
		else if (CS_signal_shift[n] > spc_end && post_shift_found == false)
		{
			post_shift.first = CS_signal_shift[n];
			post_shift.second = 0.0;
			for (size_t t = 0; t < CS_signal_shift.size(); t++)
			{
				if (type == 'L')
				{
					post_shift.second += Lorentzian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[t], CS_signal_shift[t], CS_signal_FWHM[0], CS_signal_FWHM[t]);
				}
				else
				{
					post_shift.second += Gaussian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[t], CS_signal_shift[t], CS_signal_FWHM[0], CS_signal_FWHM[t]);
				}
			}
			post_shift_found = true;
		}
		else if (CS_signal_shift[n] > spc_end && post_shift_found == true)
		{
			tmp_prepost = 0.0;
			for (size_t t = 0; t < CS_signal_shift.size(); t++)
			{
				if (type == 'L')
				{
					tmp_prepost += Lorentzian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[t], CS_signal_shift[t], CS_signal_FWHM[0], CS_signal_FWHM[t]);
				}
				else
				{
					tmp_prepost += Gaussian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[t], CS_signal_shift[t], CS_signal_FWHM[0], CS_signal_FWHM[t]);
				}
			}
			if (tmp_prepost > post_shift.second)
			{
				post_shift.first = CS_signal_shift[n];
				post_shift.second = tmp_prepost;
			}
		}
	}
	if (pre_shift_found)
	{
		m_SPECTRUM.push_back(pre_shift);
		spc_entry++;
	}
	for (double i = spc_start; i <= spc_end; i += step_size)
	{
		if (i >= min_ppm && i <= max_ppm)
		{
			m_SPECTRUM.push_back(std::make_pair<double, double>(0.0, 0.0));
			shift = i;
			m_SPECTRUM[spc_entry].first = shift;
			for (size_t n = 0; n < CS_signal_shift.size(); n++)
			{
				if (true)
				{
					if (type == 'L')
					{
						m_SPECTRUM[spc_entry].second += Lorentzian(shift, CS_num_eq_atoms[0], CS_num_eq_atoms[n], CS_signal_shift[n], CS_signal_FWHM[0], CS_signal_FWHM[n]);
					}
					else if (type == 'G')
					{
						m_SPECTRUM[spc_entry].second += Gaussian(shift, CS_num_eq_atoms[0], CS_num_eq_atoms[n], CS_signal_shift[n], CS_signal_FWHM[0], CS_signal_FWHM[n]);
					}
					else
					{
						return false;
					}
				}
				if (shift < CS_signal_shift[n] && shift + step_size > CS_signal_shift[n])
				{
					double mean_shift = shift + (step_size / 2.0);
					if (add == 0)
					{
						add++;
						m_SPECTRUM.push_back(std::make_pair<double, double>(0.0, 0.0));
						m_SPECTRUM[spc_entry + add].first = mean_shift;
					}
					intermediate_tmp = 0.0;
					for (size_t m = 0; m < CS_signal_shift.size(); m++)
					{
						if (true)
						{
							if (type == 'L')
							{
								intermediate_tmp += Lorentzian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[m], CS_signal_shift[m], CS_signal_FWHM[0], CS_signal_FWHM[m]);
							}
							else if (type == 'G')
							{
								intermediate_tmp += Gaussian(CS_signal_shift[n], CS_num_eq_atoms[0], CS_num_eq_atoms[m], CS_signal_shift[m], CS_signal_FWHM[0], CS_signal_FWHM[m]);
							}
							else
							{
								return false;
							}
						}
					}
					if (intermediate_tmp > intermediate)
					{
						intermediate = intermediate_tmp;
					}
				}
			}
			spc_entry++;
			if (add != 0)
			{
				m_SPECTRUM[spc_entry].second = intermediate;
				spc_entry ++;
			}
			add = 0;
			intermediate = -INFINITY;
		}
	}
	if (post_shift_found)
	{
		m_SPECTRUM.push_back(post_shift);
	}
	return true;
}

double CSimulation::Lorentzian(double shift, double A, double B, double shift_N, double FWHM_Na, double FWHM_Nb)
{
	double L = ((B / A) * (FWHM_Na / FWHM_Nb)) / (1.0 + pow(((shift - shift_N) / (FWHM_Nb / 2.0)), 2.0));
	return L;
}

double CSimulation::Gaussian(double shift, double A, double B, double shift_N, double FWHM_Na, double FWHM_Nb)
{
	double G = (B / A) * (FWHM_Na / FWHM_Nb) * exp(-log(2.0) * pow(((shift - shift_N) / (FWHM_Nb / 2.0)), 2.0));
	return G;
}

std::vector<std::pair<double, double>> CSimulation::GetSimSpec()
{
	return m_SPECTRUM;
}

double CSimulation::get_sim_min(std::wstring sim_element, char type, double P)
{
	double factor;
	if (type == 'L')
	{
		factor = sqrt((100.0 / P) - 1.0) / 2.0;
	}
	else
	{
		factor = sqrt(-log(P / 100.0) / log(2.0)) / 2.0;
	}
	double _MINIMUM = INFINITY;
	for (size_t i = 0; i < m_listedElement.size(); i++)
	{
		if (m_listedElement[i] == sim_element && m_listedSimulate[i] == true)
		{
			double lower_bound = m_chemShift_Sim[i] - factor * m_FWHM[i];
			if (lower_bound < _MINIMUM)
			{
				_MINIMUM = lower_bound;
			}
		}
	}
	if (_MINIMUM == INFINITY)
	{
		_MINIMUM = 0.0;
	}
	else
	{
		for (size_t i = 0; i < m_listedElement.size(); i++)
		{
			if (m_listedElement[i] == sim_element && m_listedSimulate[i] == true)
			{
				if ((m_chemShift_Sim[i] - _MINIMUM) < 10.0)
				{
					_MINIMUM = m_chemShift_Sim[i] - 10.0;
				}
			}
		}
	}
	return _MINIMUM;
}

double CSimulation::get_sim_max(std::wstring sim_element, char type, double P)
{
	double factor;
	if (type == 'L')
	{
		factor = sqrt((100.0 / P) - 1.0) / 2.0;
	}
	else
	{
		factor = sqrt(-log(P / 100.0) / log(2.0)) / 2.0;
	}
	double _MAXIMUM = -INFINITY;
	for (size_t i = 0; i < m_listedElement.size(); i++)
	{
		if (m_listedElement[i] == sim_element && m_listedSimulate[i] == true)
		{
			double upper_bound = m_chemShift_Sim[i] + factor * m_FWHM[i];
			if (upper_bound > _MAXIMUM)
			{
				_MAXIMUM = upper_bound;
			}
		}
	}
	if (_MAXIMUM == -INFINITY)
	{
		_MAXIMUM = 0.0;
	}
	else
	{
		for (size_t i = 0; i < m_listedElement.size(); i++)
		{
			if (m_listedElement[i] == sim_element && m_listedSimulate[i] == true)
			{
				if ((_MAXIMUM - m_chemShift_Sim[i]) < 10.0)
				{
					_MAXIMUM = m_chemShift_Sim[i] + 10.0;
				}
			}
		}	
	}
	return _MAXIMUM;
}

double CSimulation::get_sim_StepSize(std::wstring sim_element, char type, double P)
{
	double factor;
	if (type == 'L')
	{
		factor = sqrt((100.0 / P) - 1.0) / 2.0;
	}
	else
	{
		factor = sqrt(-log(P / 100.0) / log(2.0)) / 2.0;
	}
	double _MIN_WIDTH = INFINITY;
	for (size_t i = 0; i < m_listedElement.size(); i++)
	{
		if (m_listedElement[i] == sim_element && m_listedSimulate[i] == true)
		{
			double width = factor * m_FWHM[i];
			if (width < _MIN_WIDTH)
			{
				_MIN_WIDTH = width;
			}
		}
	}
	if (_MIN_WIDTH == INFINITY)
	{
		_MIN_WIDTH = 0.1;
	}
	else
	{
		_MIN_WIDTH /= 10.0;
	}
	return _MIN_WIDTH;
}

size_t CSimulation::get_num_groups()
{
	return ListSize;
}

CString CSimulation::get_GroupsName_byIndex(size_t index)
{
	if (index < ListSize)
	{
		return m_listedGroupName[index].c_str();
	}
	return _T("error");
}

std::vector<int> CSimulation::get_GroupsIndices_byIndex(size_t index)
{
	if (index < ListSize)
	{
		return m_listedGroupIndices[index];
	}
	return { -1 };
}

CString CSimulation::get_GroupsElement_byIndex(size_t index)
{
	if (index < ListSize)
	{
		return m_listedElement[index].c_str();
	}
	return _T("error");
}

std::vector<double> CSimulation::get_fwhm()
{
	return m_SimFWHM;
}

std::vector<double> CSimulation::get_SimIsoOrb()
{
	return m_SimIsoOrb;
}
std::vector<double> CSimulation::get_SimIsoFc()
{
	return m_SimIsoFc;
}
std::vector<double> CSimulation::get_SimIsoPcs()
{
	return m_SimIsoPcs;
}
std::vector<size_t> CSimulation::get_SimGroupIndices()
{
	return m_SimGroupIndices;
}

std::vector<std::wstring> CSimulation::get_GroupsName()
{
	return m_listedGroupName;
}

std::vector<std::vector<std::pair<double, double>>> CSimulation::save_CreateFullSpectrum(std::wstring element, double min_ppm, double max_ppm, double precision, char type)
{
	std::vector<std::pair<double, double>> return_empty(1);
	return_empty[0].first = 0.0;
	return_empty[0].second = 0.0;
	std::vector<std::pair<double, double>> return_SPECTRUM;
	std::vector<std::vector<std::pair<double, double>>> return_isolatedSPECTRUM;
	double spc_start = min_ppm;
	double spc_end = max_ppm;
	if (precision == 0.0)
	{
		precision = 1.0;
	}
	double step_size = precision;
	std::vector<size_t> Groups;
	for (size_t i = 0; i < m_listedElement.size(); i++)
	{
		if (m_listedElement[i] == element)
		{
			Groups.push_back(i);
		}
	}
	__CS_signal_shift.clear();
	__CS_signal_FWHM.clear();
	__CS_num_eq_atoms.clear();
	for (size_t g = 0; g < Groups.size(); g++)
	{
		__CS_signal_shift.push_back(m_chemShift_Sim[Groups[g]]);
		__CS_signal_FWHM.push_back(m_FWHM[Groups[g]]);
		__CS_num_eq_atoms.push_back(static_cast<int>(m_listedGroupIndices[Groups[g]].size()));
		if (m_FWHM[Groups[g]] < precision)
		{
			precision = m_FWHM[Groups[g]];
			step_size = precision;
		}
	}
	double intermediate = -INFINITY;
	double intermediate_tmp = 0.0;
	size_t spc_entry = 0;
	size_t add = 0;
	for (double i = spc_start; i <= spc_end; i += step_size)
	{
		return_SPECTRUM.push_back(std::make_pair<double, double>(0.0, 0.0));
		return_SPECTRUM[spc_entry].first = i;
		for (size_t n = 0; n < __CS_signal_shift.size(); n++)
		{
			if (true)
			{
				if (type == 'L')
				{
					return_SPECTRUM[spc_entry].second += Lorentzian(i, __CS_num_eq_atoms[0], __CS_num_eq_atoms[n], __CS_signal_shift[n], __CS_signal_FWHM[0], __CS_signal_FWHM[n]);
				}
				else if (type == 'G')
				{
					return_SPECTRUM[spc_entry].second += Gaussian(i, __CS_num_eq_atoms[0], __CS_num_eq_atoms[n], __CS_signal_shift[n], __CS_signal_FWHM[0], __CS_signal_FWHM[n]);
				}
				else
				{	
					return { return_empty };
				}
			}
			if (i < __CS_signal_shift[n] && i + step_size > __CS_signal_shift[n])
			{
				double mean_shift = i + (step_size / 2.0);
				if (add == 0)
				{
					add++;
					return_SPECTRUM.push_back(std::make_pair<double, double>(0.0, 0.0));
					return_SPECTRUM[spc_entry + add].first = mean_shift;
				}
				intermediate_tmp = 0.0;
				for (size_t m = 0; m < __CS_signal_shift.size(); m++)
				{
					if (true)
					{
						if (type == 'L')
						{
							intermediate_tmp += Lorentzian(__CS_signal_shift[n], __CS_num_eq_atoms[0], __CS_num_eq_atoms[m], __CS_signal_shift[m], __CS_signal_FWHM[0], __CS_signal_FWHM[m]);
						}
						else if (type == 'G')
						{
							intermediate_tmp += Gaussian(__CS_signal_shift[n], __CS_num_eq_atoms[0], __CS_num_eq_atoms[m], __CS_signal_shift[m], __CS_signal_FWHM[0], __CS_signal_FWHM[m]);
						}
						else
						{
							return { return_empty };
						}
					}
				}
				if (intermediate_tmp > intermediate)
				{
					intermediate = intermediate_tmp;
				}
			}
		}
		spc_entry++;
		if (add != 0)
		{
			return_SPECTRUM[spc_entry].second = intermediate;
			spc_entry++;
		}
		add = 0;
		intermediate = -INFINITY;
	}
	for (size_t n = 0; n < __CS_signal_shift.size(); n++)
	{
		return_isolatedSPECTRUM.emplace_back(std::vector<std::pair<double, double>>());
		double isolatedPrecision = __CS_signal_FWHM[n] / 4.0;
		double isolated_minPPM = get_signal_min(__CS_signal_shift[n], __CS_signal_FWHM[n], type, 0.01);
		double isolated_maxPPM = get_signal_max(__CS_signal_shift[n], __CS_signal_FWHM[n], type, 0.01);
		double isolated_spc_start = __CS_signal_shift[n] - (round((__CS_signal_shift[n] - isolated_minPPM) / isolatedPrecision) + 1.0) * isolatedPrecision;
		double isolated_spc_end = __CS_signal_shift[n] + (round((isolated_maxPPM - __CS_signal_shift[n]) / isolatedPrecision) + 1.0) * isolatedPrecision;
		spc_entry = 0;
		for (double i = isolated_spc_start; i <= isolated_spc_end; i += isolatedPrecision)
		{
			return_isolatedSPECTRUM[n].push_back(std::make_pair<double, double>(0.0, 0.0));
			return_isolatedSPECTRUM[n][spc_entry].first = i;
			if (type == 'L')
			{
				return_isolatedSPECTRUM[n][spc_entry].second += Lorentzian(i, __CS_num_eq_atoms[0], __CS_num_eq_atoms[n], __CS_signal_shift[n], __CS_signal_FWHM[0], __CS_signal_FWHM[n]);
			}
			else if (type == 'G')
			{
				return_isolatedSPECTRUM[n][spc_entry].second += Gaussian(i, __CS_num_eq_atoms[0], __CS_num_eq_atoms[n], __CS_signal_shift[n], __CS_signal_FWHM[0], __CS_signal_FWHM[n]);
			}
			else
			{
				return { return_empty };
			}
			spc_entry++;
		}

	}
	double max_value_spectrum = -INFINITY;
	for (size_t i = 0; i < return_SPECTRUM.size(); i++)
	{
		if (return_SPECTRUM[i].second > max_value_spectrum)
		{
			max_value_spectrum = return_SPECTRUM[i].second;
		}
	}
	std::vector<std::vector<std::pair<double, double>>> return_allSPECTRA;
	for (size_t i = 0; i < return_SPECTRUM.size(); i++)
	{
		return_SPECTRUM[i].second /= max_value_spectrum;
	}
	return_allSPECTRA.push_back(return_SPECTRUM);
	for (size_t n = 0; n < return_isolatedSPECTRUM.size(); n++)
	{
		for (size_t i = 0; i < return_isolatedSPECTRUM[n].size(); i++)
		{
			return_isolatedSPECTRUM[n][i].second /= max_value_spectrum;
		}
		return_allSPECTRA.push_back(return_isolatedSPECTRUM[n]);
	}
	return return_allSPECTRA;
}

double CSimulation::get_signal_min(double chemShift, double FWHM, char type, double P)
{
	double factor;
	if (type == 'L')
	{
		factor = sqrt((100.0 / P) - 1.0) / 2.0;
	}
	else
	{
		factor = sqrt(-log(P / 100.0) / log(2.0)) / 2.0;
	}
	double _MINIMUM = chemShift - factor * FWHM;
	if ((chemShift - _MINIMUM) < 10.0)
	{
		_MINIMUM = chemShift - 10.0;
	}
	return _MINIMUM;
}

double CSimulation::get_signal_max(double chemShift, double FWHM, char type, double P)
{
	double factor;
	if (type == 'L')
	{
		factor = sqrt((100.0 / P) - 1.0) / 2.0;
	}
	else
	{
		factor = sqrt(-log(P / 100.0) / log(2.0)) / 2.0;
	}
	double _MAXIMUM = chemShift + factor * FWHM;
	if ((_MAXIMUM - chemShift) < 10.0)
	{
		_MAXIMUM = chemShift + 10.0;
	}
	return _MAXIMUM;
}

void CSimulation::print_TotalShieldings(std::ofstream& file)
{
	file << "Note: The total shieldings were only computed for atoms considered in the simulation!" << std::endl;
	file << "atom index\tsigma_xx\tsigma_xy\tsigma_xz\tsigma_yx\tsigma_yy\tsigma_yz\tsigma_zx\tsigma_zy\tsigma_zz" << std::endl;
	file << std::fixed << std::scientific << std::setprecision(12);
	for (size_t i = 0; i < m_print_shielding.size(); i++)
	{
		if (m_print_shielding[i])
		{
			file << i << "\t";
			for (size_t n = 0; n < 3; n++)
			{
				for (size_t m = 0; m < 3; m++)
				{
					if (n == 2 && m == 2)
					{
						file << m_saveTotalShieldings[i][n][m] << std::endl;
					}
					else
					{
						file << m_saveTotalShieldings[i][n][m] << "\t";
					}
				}
			}
		}
	}
}

void CSimulation::print_R2(std::ofstream& file)
{
	file << "Group\tcontact relaxation rate (Hz)\tdipolar relaxation rate (Hz)\tcurie relaxation rate (Hz)" << std::endl;
	file << std::fixed << std::scientific << std::setprecision(12);
	for (size_t i = 0; i < m_saveR2[0].size(); i++)
	{
		file << i << "\t" << m_saveR2[1][i] << "\t" << m_saveR2[0][i] << "\t" << m_saveR2[2][i] << std::endl;
	}
}

void CSimulation::print_fitusage(std::ofstream& file)
{
	file << std::endl;
	file << "------------------" << std::endl;
	file << "data used for fit" << std::endl;
	file << "------------------" << std::endl;
	if (m_save_weight.size() != 0)
	{
		file << "number of data points used for fit: " << m_save_weight.size() << std::endl;
		file << std::endl;
		file << std::setw(20) << std::setfill(' ') << "Group Names";
		file << std::setw(12) << std::setfill(' ') << "Elements";
		file << std::setw(19) << std::setfill(' ') << "delta_exp (ppm)";
		file << std::setw(10) << std::setfill(' ') << "weight" << std::endl;
		for (size_t i = 0; i < m_save_weight.size(); i++)
		{
			file << std::setw(20) << std::setfill(' ') << m_save_groupNames[i];
			file << std::setw(12) << std::setfill(' ') << m_save_elements[i];
			file << std::setw(19) << std::setfill(' ') << m_save_exp_dorb[i];
			file << std::setw(10) << std::setfill(' ') << m_save_weight[i] << std::endl;
		}
	}
	else
	{
		file << "no information was found about the data points used in the fit (no fit performed yet?)" << std::endl;
	}
}

void CSimulation::print_group_definition(std::ofstream& file)
{
	file << "GroupNames\tAtomIndices" << std::endl;
	for (size_t i = 0; i < m_listedGroupName.size(); i++)
	{
		std::string tmp_name(m_listedGroupName[i].begin(), m_listedGroupName[i].end());
		file << tmp_name << "\t";
		for (size_t j = 0; j < m_listedGroupIndices[i].size(); j++)
		{
			if (j < m_listedGroupIndices[i].size() - 1)
			{
				file << m_listedGroupIndices[i][j] << ",";
			}
			else
			{
				file << m_listedGroupIndices[i][j] << std::endl;
			}
		}
	}
}

std::vector<double> CSimulation::get_mdorb()
{
	return m_dorb;
}