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
#include "CEnsembleDlg.h"
#include "afxdialogex.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <regex>
#include "physical_constants.h"

IMPLEMENT_DYNAMIC(CEnsembleDlg, CDialogEx)

CEnsembleDlg::CEnsembleDlg(DataBase* p_dataBase, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMPORT_ENSEMBLE, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_data_base = p_dataBase;
}

CEnsembleDlg::~CEnsembleDlg()
{
}

void CEnsembleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEnsembleDlg, CDialogEx)
	ON_BN_CLICKED(IDCREATE, &CEnsembleDlg::OnCreateEnsemble)
	ON_EN_KILLFOCUS(IDC_EDIT_ENSEMBLE_TEMPERATURE, &CEnsembleDlg::OnEditTempKillfocus)
END_MESSAGE_MAP()

BOOL CEnsembleDlg::OnInitDialog()
{
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_ListBoxInfo.SubclassDlgItem(IDC_LISTBOX_INFO, this);
	m_ListBoxInfo.AddString(_T("INFO"));
	m_ListBoxInfo.AddString(_T(""));
	m_ListBoxInfo.AddString(_T(""));
	m_EditTemp.SubclassDlgItem(IDC_EDIT_ENSEMBLE_TEMPERATURE, this);
	m_EditTemp.SetWindowTextW(_T("298"));
	do_hfc = false;;
	do_shielding = false;;
	return TRUE;
}

void CEnsembleDlg::OnCreateEnsemble()
{
	CString temperature_string;
	m_EditTemp.GetWindowTextW(temperature_string);
	m_temperature = _tstoi(temperature_string);
	m_ListBoxInfo.ResetContent();
	m_ListBoxInfo.AddString(_T("Searching for files ..."));
	UpdateWindow();
	std::string root_path = m_data_base->get_UserRootPath();
	std::string folder_path = root_path + "./ensemble";
	std::vector<std::vector<std::vector<double>>> O_n;
	std::vector<double> tmp_energies;
	std::vector<std::vector<std::vector<std::vector<double>>>> tmp_shielding_tensors;
	std::vector<std::vector<std::vector<std::vector<double>>>> tmp_hfc_tensors;
	std::vector<std::vector<std::string>> tmp_molecule_compositions;
	size_t entries = 0;
	try
	{
		if (!std::filesystem::exists(folder_path) || !std::filesystem::is_directory(folder_path))
		{
			AfxMessageBox(_T("The folder /ensemble is missing."), MB_ICONERROR);
			m_ListBoxInfo.AddString(_T("--- ensemble creation failed ---"));
			UpdateWindow();
			return;
		}
		std::ifstream main_file(folder_path + "/main.txt");
		if (main_file.is_open())
		{
			int search_result = 0;
			do
			{
				search_result = find_file_part(main_file);
				if (search_result == 1)
				{
					tmp_molecule_compositions.emplace_back(import_molecule(main_file));
				}
				else if (search_result == 2)
				{
					tmp_energies.emplace_back(import_scf_energy(main_file));
				}
				else if (search_result == 3)
				{
					O_n.emplace_back(import_g_orientation(main_file));
				}
				else if (search_result == 4)
				{
					tmp_hfc_tensors.emplace_back(import_hfc_tensors(main_file));
				}
				else if (search_result == 5)
				{
					tmp_shielding_tensors.emplace_back(import_shielding_tensors(main_file));
				}
				
			} while (search_result != 0);
			entries++;
			m_ListBoxInfo.AddString(_T("   - main file processed"));
			UpdateWindow();
			for (const auto& entry : std::filesystem::directory_iterator(folder_path))
			{
				if (entry.is_regular_file() && entry.path().extension() == ".txt" && entry.path().string() != folder_path + "\\main.txt")
				{
					std::string file_name = entry.path().string();
					std::ifstream file(file_name);
					if (file.is_open())
					{
						entries++;
						search_result = 0;
						do
						{
							search_result = find_file_part(file);
							if (search_result == 1)
							{
								tmp_molecule_compositions.emplace_back(import_molecule(file));
							}
							else if (search_result == 2)
							{
								tmp_energies.emplace_back(import_scf_energy(file));
							}
							else if (search_result == 3)
							{
								O_n.emplace_back(import_g_orientation(file));
							}
							else if (search_result == 4)
							{
								tmp_hfc_tensors.emplace_back(import_hfc_tensors(file));
							}
							else if (search_result == 5)
							{
								tmp_shielding_tensors.emplace_back(import_shielding_tensors(file));
							}
							
						} while (search_result != 0);
					}
					CString CSfileName(file_name.c_str());
					m_ListBoxInfo.AddString(_T("   - ") + CSfileName + _T(" file processed"));
					UpdateWindow();
					file.close();
				}
			}
		}
		else
		{
			AfxMessageBox(_T("main.txt is missing."), MB_ICONERROR);
			m_ListBoxInfo.AddString(_T("--- ensemble creation failed ---"));
			UpdateWindow();
			main_file.close();
			return;
		}
		main_file.close();
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		CString errorMsg(e.what());
		AfxMessageBox(errorMsg, MB_ICONERROR);
		m_ListBoxInfo.AddString(_T("--- ensemble creation failed ---"));
		UpdateWindow();
		return;
	}
	m_ListBoxInfo.AddString(_T("file import finished"));
	m_ListBoxInfo.AddString(_T(""));
	m_ListBoxInfo.AddString(_T(""));
	m_ListBoxInfo.AddString(_T("Starting data verification ..."));
	UpdateWindow();
	bool failed = false;
	do_shielding = true;
	do_hfc = true;
	if (tmp_molecule_compositions.size() != entries)
	{
		m_ListBoxInfo.AddString(_T("ERROR: One or more molecule compositions are missing."));
		UpdateWindow();
		failed = true;
	}
	if (O_n.size() != entries)
	{
		m_ListBoxInfo.AddString(_T("ERROR: One or more g-tensor orientations are missing."));
		UpdateWindow();
		failed = true;
	}
	if (tmp_energies.size() != entries)
	{
		m_ListBoxInfo.AddString(_T("ERROR: One or more single point energies are missing."));
		UpdateWindow();
		failed = true;
	}
	if (tmp_shielding_tensors.size() != entries)
	{
		do_shielding = false;
	}
	if (tmp_hfc_tensors.size() != entries)
	{
		do_hfc = false;
	}
	if (do_shielding == false && do_hfc == false)
	{
		m_ListBoxInfo.AddString(_T("ERROR: One or more shielding and hfc tensors are missing."));
		UpdateWindow();
		failed = true;
	}
	else if (failed == false)
	{
		if (do_shielding == false && do_hfc == true)
		{
			m_ListBoxInfo.AddString(_T("WARNING: One or more shielding tensors are missing."));
			m_ListBoxInfo.AddString(_T("WARNING: The run is only continued with hfc tensors."));
			UpdateWindow();
		}
		if (do_shielding == true && do_hfc == false)
		{
			m_ListBoxInfo.AddString(_T("WARNING: One or more hfc tensors are missing."));
			m_ListBoxInfo.AddString(_T("WARNING: The run is only continued with shielding tensors."));
			UpdateWindow();
		}
	}
	if (failed)
	{
		m_ListBoxInfo.AddString(_T("--- ensemble creation failed ---"));
		UpdateWindow();
		return;
	}
	for (size_t molecule = 1; molecule < entries; molecule++)
	{
		for (size_t element = 0; element < tmp_molecule_compositions[molecule].size(); element++)
		{
			if (tmp_molecule_compositions[molecule][element] != tmp_molecule_compositions[0][element])
			{
				failed = true;
				break;
			}
		}
		if (failed)
		{
			break;
		}
		if (tmp_molecule_compositions[molecule].size() != tmp_molecule_compositions[0].size())
		{
			failed = true;
			break;
		}
	}
	if (failed)
	{
		m_ListBoxInfo.AddString(_T("ERROR: different molecule compositions detected."));
		m_ListBoxInfo.AddString(_T("--- ensemble creation failed ---"));
		UpdateWindow();
		return;
	}
	for (size_t energy = 0; energy < tmp_energies.size(); energy++)
	{
		if (tmp_energies[energy] == 0.0)
		{
			m_ListBoxInfo.AddString(_T("ERROR: An unphysical energy of 0.0 J/mol was found."));
			m_ListBoxInfo.AddString(_T("--- ensemble creation failed ---"));
			UpdateWindow();
		}
	}
	for (size_t ori = 0; ori < O_n.size(); ori++)
	{
		if (O_n[ori].size() != 3)
		{
			failed = true;
		}
		else if (O_n[ori][0].size() != 3 || O_n[ori][1].size() != 3 || O_n[ori][2].size() != 3)
		{
			failed = true;
		}
	}
	if (failed)
	{
		m_ListBoxInfo.AddString(_T("ERROR: g-tensor orientations unreadable."));
		m_ListBoxInfo.AddString(_T("--- ensemble creation failed ---"));
		UpdateWindow();
		return;
	}
	if (do_hfc)
	{
		for (size_t molecule = 0; molecule < tmp_hfc_tensors.size(); molecule++)
		{
			if (tmp_hfc_tensors[molecule].size() == 1)
			{
				m_ListBoxInfo.AddString(_T("ERROR: hfc tensors unreadable."));
				m_ListBoxInfo.AddString(_T("--- ensemble creation failed ---"));
				UpdateWindow();
				return;
			}
			if (tmp_hfc_tensors[molecule].size() < tmp_molecule_compositions[0].size())
			{
				int conv_size = static_cast<int>(tmp_hfc_tensors[molecule].size());
				CString formattedString;
				formattedString.Format(_T("%d"), conv_size);
				m_ListBoxInfo.AddString(formattedString + _T(" tensors were found -> appending empty tensors"));
				UpdateWindow();
				for (size_t i = tmp_hfc_tensors[molecule].size(); i <= tmp_molecule_compositions[0].size(); i++)
				{
					tmp_hfc_tensors[molecule].emplace_back(std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0)));
				}
			}
		}
	}
	if (do_shielding)
	{
		for (size_t molecule = 0; molecule < tmp_shielding_tensors.size(); molecule++)
		{
			if (tmp_shielding_tensors[molecule].size() < tmp_molecule_compositions[0].size())
			{
				m_ListBoxInfo.AddString(_T("ERROR: shielding tensors unreadable."));
				m_ListBoxInfo.AddString(_T("--- ensemble creation failed ---"));
				UpdateWindow();
				return;
			}
		}
	}
	m_ListBoxInfo.AddString(_T("... SUCCESS"));
	m_ListBoxInfo.AddString(_T(""));
	m_ListBoxInfo.AddString(_T(""));
	m_ListBoxInfo.AddString(_T("Comparing and matching ensemble entries ..."));
	UpdateWindow();
	double Z = 0.0;
	double min_energy = INFINITY;
	for (size_t i = 0; i < tmp_energies.size(); i++)
	{
		if (min_energy > tmp_energies[i])
		{
			min_energy = tmp_energies[i];
		}
	}
	for (size_t i = 0; i < tmp_energies.size(); i++)
	{
		double rel_energy = (tmp_energies[i] - min_energy)/N_avo;
		tmp_energies[i] = rel_energy;
		Z += exp(-(tmp_energies[i]) / (k_B * m_temperature));
	}
	std::vector<std::vector<double>> R;
	std::vector<std::vector<double>> RT;
	for (size_t i = 0; i < tmp_energies.size(); i++)
	{
		double prob = exp(-(tmp_energies[i]) / (k_B * m_temperature)) / Z;
		CString formattedValue;
		formattedValue.Format(_T("%.2f"), (prob * 100.0));
		if (i > 0)
		{
			R.clear(); 
			RT.clear();
			std::vector<std::vector<double>> R = MatrixMultiplication(O_n[0], transpose(O_n[i]));
			std::vector<std::vector<double>> RT = transpose(R);
			std::vector<std::vector<double>> rotated_tensor;
			for (size_t atom = 0; atom < tmp_molecule_compositions[0].size(); atom++)
			{
				if (do_hfc)
				{
					rotated_tensor.clear();
					rotated_tensor = MatrixMultiplication(R, MatrixMultiplication(tmp_hfc_tensors[i][atom], RT));
					for (size_t row = 0; row < 3; row++)
					{
						for (size_t col = 0; col < 3; col++)
						{
							m_hfc[atom][row][col] += (rotated_tensor[row][col] * prob);
						}
					}
				}
				if (do_shielding)
				{
					rotated_tensor.clear();
					rotated_tensor = MatrixMultiplication(R, MatrixMultiplication(tmp_shielding_tensors[i][atom], RT));
					for (size_t row = 0; row < 3; row++)
					{
						for (size_t col = 0; col < 3; col++)
						{
							m_shielding[atom][row][col] += (rotated_tensor[row][col] * prob);
						}
					}
				}
			}
		}
		else
		{
			if (do_hfc)
			{
				m_hfc = tmp_hfc_tensors[i];
			}
			if (do_shielding)
			{
				m_shielding = tmp_shielding_tensors[i];
			}
			for (size_t atom = 0; atom < tmp_molecule_compositions[0].size(); atom++)
			{
				for (size_t row = 0; row < 3; row++)
				{
					for (size_t col = 0; col < 3; col++)
					{
						if (do_hfc)
						{
							m_hfc[atom][row][col] *= prob;
						}
						if (do_shielding)
						{
							m_shielding[atom][row][col] *= prob;
						}
					}
				}
			}
		}
		m_ListBoxInfo.AddString(_T("   - ") + formattedValue + _T("% contribution computed"));
		UpdateWindow();
	}
	m_ListBoxInfo.AddString(_T("... completed"));
	m_ListBoxInfo.AddString(_T(""));
	m_ListBoxInfo.AddString(_T(""));
	m_ListBoxInfo.AddString(_T("correcting for isotopes"));
	UpdateWindow();
	m_hfc_hz = m_hfc;
	std::vector<CString> isotope(tmp_molecule_compositions[0].size(), _T("empty"));
	for (size_t i = 0; i < tmp_molecule_compositions[0].size(); i++)
	{
		CString current_isotope;
		std::string current_element;
		if (isotope[i] == "empty")
		{
			current_element = tmp_molecule_compositions[0][i];
			int index = m_data_base->IsNMRactiveIsotope(m_data_base->StoCS(current_element));
			if (index == -1)
			{
				CIsotopeDlg IsotopeSelectionDlg(m_data_base->GetIsotopesOfType(m_data_base->StoCS(current_element)));
				IsotopeSelectionDlg.DoModal();
				current_isotope = IsotopeSelectionDlg.m_ChosenIsotope;
			}
			else if (index == -2)
			{
				current_isotope = _T("error");
			}
			else
			{
				current_isotope = m_data_base->GetNMRnuclei(index);
			}
			for (size_t j = 0; j < isotope.size(); j++)
			{
				if (tmp_molecule_compositions[0][j] == current_element)
				{
					isotope[j] = current_isotope;
				}
			}
		}
		std::string isotope_string = m_data_base->CStoS(isotope[i]);
		double pfac = Aprefac * get_gyromagnetic_ratio(isotope_string, -1, true) * get_gyromagnetic_ratio("E", -1, true) * pow(10.0, -6.0);
		for (size_t k = 0; k < 3; k++)
		{
			for (size_t l = 0; l < 3; l++)
			{
				m_hfc_hz[i][k][l] *= pfac * pow(10, 6);
				m_hfc[i][k][l] *= pfac;
				m_hfc[i][k][l] *= (pow(10.0, 6.0) * h * pow(10.0, -24.0)) / (mu0 * muB * g_e * hquer *get_gyromagnetic_ratio(isotope_string, -1, true));
			}
		}
	}
	m_molecule = tmp_molecule_compositions[0];
	for (size_t atom = 0; atom < m_shielding.size(); atom++)
	{
		m_sigma_iso.emplace_back(((m_shielding[atom][0][0] + m_shielding[atom][1][1] + m_shielding[atom][2][2]) / 3.0));
	}
}


int CEnsembleDlg::find_file_part(std::ifstream& file)
{
	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("$Geometry") != std::string::npos)
		{
			return 1;
		}
		else if (line.find("$Calculation_Info") != std::string::npos)
		{
			return 2;
		}
		else if (line.find("$SCF_G_Tensor") != std::string::npos)
		{
			return 3;
		}
		else if (line.find("$SCF_A_Tensor") != std::string::npos)
		{
			return 4;
		}
		else if (line.find("$SCF_Chemical_Shift") != std::string::npos)
		{
			return 5;
		}
	}
	return 0;
}

std::vector<std::vector<double>> CEnsembleDlg::import_g_orientation(std::ifstream& file)
{
	std::string line;
	bool error = false;
	std::vector<std::vector<double>> orientation;
	int counter = 0;
	bool pos_found = false;
	while (std::getline(file, line))
	{
		if (pos_found && line.find("$End") == std::string::npos)
		{
			counter++;
			if (counter > 5)
			{
				pos_found = false;
			}
			else if (counter > 2)
			{
				orientation.emplace_back(std::vector<double>(3, 0.0));
				orientation[counter - 3][0] = (string_to_double(get_block_in_line(line, " ", 1, error), error));
				orientation[counter - 3][1] = (string_to_double(get_block_in_line(line, " ", 2, error), error));
				orientation[counter - 3][2] = (string_to_double(get_block_in_line(line, " ", 3, error), error));
			}
		}
		else if (line.find("&ORIENTATION") != std::string::npos)
		{
			pos_found = true;
			
		}
		if (error || line.find("$End") != std::string::npos)
		{
			break;
		}
	}
	if (error || orientation.size() != 3)
	{
		return {{0.0}};
	}
	return orientation;
}

std::vector<std::vector<std::vector<double>>> CEnsembleDlg::import_shielding_tensors(std::ifstream& file)
{
	std::string line;
	bool error = false;
	bool pos_found = false;
	std::vector<std::vector<std::vector<double>>> shieldings;
	int counter = 0;
	while (std::getline(file, line))
	{
		if (pos_found && line.find("$End") == std::string::npos)
		{
			counter++;
			if (counter > 5)
			{
				counter = 0;
				pos_found = false;
			}
			else if (counter > 2)
			{
				shieldings[shieldings.size() - 1][counter - 3][0] = (string_to_double(get_block_in_line(line, " ", 1, error), error));
				shieldings[shieldings.size() - 1][counter - 3][1] = (string_to_double(get_block_in_line(line, " ", 2, error), error));
				shieldings[shieldings.size() - 1][counter - 3][2] = (string_to_double(get_block_in_line(line, " ", 3, error), error));
			}
		}
		else if (line.find("&STOT") != std::string::npos && line.find("&STOTEIGEN") == std::string::npos)
		{
			pos_found = true;
			shieldings.emplace_back(std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0)));
		}
		if (error || line.find("$End") != std::string::npos)
		{
			break;
		}
	}
	if (error)
	{
		return { {{0.0}} };
	}
	return shieldings;
}

std::vector<std::vector<std::vector<double>>> CEnsembleDlg::import_hfc_tensors(std::ifstream& file)
{
	std::string line;
	bool error = false;
	bool pos_found = false;
	std::vector<std::vector<std::vector<double>>> hfc;
	std::vector<std::vector<std::vector<double>>> unsorted_hfc;
	std::vector<int> index_of_nuclei;
	std::vector<double> pfac;
	int counter = 0;
	while (std::getline(file, line))
	{
		if (pos_found && line.find("$End") == std::string::npos)
		{
			counter++;
			if (counter > 5)
			{
				counter = 0;
				pos_found = false;
			}
			else if (counter > 2)
			{
				unsorted_hfc[unsorted_hfc.size() - 1][counter - 3][0] = (string_to_double(get_block_in_line(line, " ", 1, error), error));
				unsorted_hfc[unsorted_hfc.size() - 1][counter - 3][1] = (string_to_double(get_block_in_line(line, " ", 2, error), error));
				unsorted_hfc[unsorted_hfc.size() - 1][counter - 3][2] = (string_to_double(get_block_in_line(line, " ", 3, error), error));
			}
		}
		else if (line.find("&NUC") != std::string::npos)
		{
			index_of_nuclei.emplace_back(string_to_int(get_block_in_line(line, " ", 3, error), error));
			pos_found = false;
		}
		else if (line.find("&PFAC") != std::string::npos)
		{
			pfac.emplace_back(string_to_double(get_block_in_line(line, " ", 3, error), error));
			pos_found = false;
		}
		else if (line.find("&ARAW") != std::string::npos)
		{
			pos_found = true;
			unsorted_hfc.emplace_back(std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0)));
		}
		if (error || line.find("$End") != std::string::npos)
		{
			if (pos_found)
			{
				error = true;
			}
			break;
		}
	}
	if (index_of_nuclei.size() != unsorted_hfc.size() || index_of_nuclei.size() != pfac.size() || pfac.size() != unsorted_hfc.size())
	{
		error = true;
	}
	if (error)
	{
		return { {{0.0}} };
	}
	for (size_t i = 0; i < unsorted_hfc.size(); i++)
	{
		for (size_t k = 0; k < 3; k++)
		{
			for (size_t l = 0; l < 3; l++)
			{
				unsorted_hfc[i][k][l] /= pfac[i];
			}
		}
	}
	int largest_index = -1;
	for (size_t i = 0; i < index_of_nuclei.size(); i++)
	{
		if (index_of_nuclei[i] > largest_index)
		{
			largest_index = index_of_nuclei[i];
		}
	}
	bool index_found = false;
	size_t search_to_index = index_of_nuclei.size();
	for (size_t i = 0; i < largest_index; i++)
	{
		index_found = false;
		for (size_t j = 0; j < search_to_index; j++)
		{
			if (index_of_nuclei[j] == i)
			{
				index_found = true;
				break;
			}
		}
		if (!index_found)
		{
			index_of_nuclei.emplace_back(i);
			unsorted_hfc.emplace_back(std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0)));
		}
	}
	hfc = std::vector<std::vector<std::vector<double>>>(index_of_nuclei.size());
	for (size_t i = 0; i < index_of_nuclei.size(); i++)
	{
		if (index_of_nuclei[i] >= index_of_nuclei.size())
		{
			error = true;
			break;
		}
		hfc[index_of_nuclei[i]] = unsorted_hfc[i];
	}
	if (error)
	{
		return std::vector<std::vector<std::vector<double>>>(1, std::vector<std::vector<double>>(3, std::vector<double>(3,0.0)));
	}
	return hfc;
}

double CEnsembleDlg::import_scf_energy(std::ifstream& file)
{
	std::string line;
	bool error = false;
	double energy = 0.0;
	while (std::getline(file, line))
	{
		if (line.find("&TOTALENERGY") != std::string::npos)
		{
			energy = string_to_double(get_block_in_line(line, " ", 3, error), error);
		}

		if (line.find("$End") != std::string::npos)
		{
			break;
		}
	}
	energy *= 2625.499748;
	energy *= 1000.0;
	return energy;
}

std::vector<std::string> CEnsembleDlg::import_molecule(std::ifstream& file)
{
	std::string line;
	bool error = false;
	bool pos_found = false;
	std::vector<std::string> elements;
	while (std::getline(file, line))
	{
		if (pos_found && line.find("$End") == std::string::npos)
		{
			elements.emplace_back(get_block_in_line(line, " ", 0, error));
		}
		else if (line.find("&CartesianCoordinates") != std::string::npos)
		{
			pos_found = true;
		}

		if (error || line.find("$End") != std::string::npos)
		{
			break;
		}
	}
	if (error)
	{
		return { "error" };
	}
	return elements;
}

std::string CEnsembleDlg::get_block_in_line(std::string line, std::string seperator, int block, bool& error)
{
	bool last_error = error;
	std::vector<std::string> found_block;
	std::string tmp_line = line;
	size_t space_position = line.find(seperator);
	int counter = 0;
	while (tmp_line.size() != 0)
	{
		while (space_position != std::string::npos && space_position == 0)
		{
			tmp_line = tmp_line.substr(1, tmp_line.size());
			space_position = tmp_line.find(seperator);
		}
		if (space_position != std::string::npos)
		{
			found_block.push_back(tmp_line.substr(0, space_position));
			tmp_line = tmp_line.substr(space_position, tmp_line.size());
			space_position = tmp_line.find(seperator);
		}
		else if (tmp_line.size() != 0)
		{
			found_block.push_back(tmp_line);
			tmp_line = "";
		}
		else
		{
			//
		}
	}
	if (block < static_cast<int>(found_block.size()))
	{
		error = last_error;
		return found_block[block];
	}
	error = true;
	return "";
}

double CEnsembleDlg::string_to_double(std::string line, bool& error)
{
	bool last_error = error;
	std::regex doubleRegex(R"(([+-]?\d*\.?\d+([eE][+-]?\d+)?))");
	std::smatch match;
	if (std::regex_search(line, match, doubleRegex))
	{
		std::string number = match.str();
		std::istringstream iss(number);
		double value;
		if (iss >> value)
		{
			return value;
		}
	}
	error = true;
	return 0.0;
}

int CEnsembleDlg::string_to_int(std::string line, bool& error)
{
	bool last_error = error;
	std::istringstream iss(line);
	int i;
	char c;
	if (!(iss >> i))
	{
		error = true;
		return 0;
	}
	if (iss >> c)
	{
		error = true;
		return 0;
	}
	error = last_error;
	return i;
}

bool CEnsembleDlg::IsHFC()
{
	return do_hfc;
}

bool CEnsembleDlg::IsShielding()
{
	return do_shielding;
}

std::vector<std::string> CEnsembleDlg::get_molecule()
{
	return m_molecule;
}

std::vector<std::vector<std::vector<double>>> CEnsembleDlg::get_hfc()
{
	return m_hfc;
}

std::vector<std::vector<std::vector<double>>> CEnsembleDlg::get_hfc_hz()
{
	return m_hfc_hz;
}

std::vector<std::vector<std::vector<double>>> CEnsembleDlg::get_shielding()
{
	return m_shielding;
}

std::vector<double> CEnsembleDlg::get_simga_iso()
{
	return m_sigma_iso;
}

void CEnsembleDlg::OnEditTempKillfocus()
{
	CString EditText;
	m_EditTemp.GetWindowTextW(EditText);
	int value = _tstoi(EditText);
	if (value == 0 || value < 0)
	{
		value = 298;
	}
	EditText.Format(_T("%d"), value);
	m_EditTemp.SetWindowTextW(EditText);
	UpdateWindow();
}