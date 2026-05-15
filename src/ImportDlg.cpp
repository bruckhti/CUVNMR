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
#include "physical_constants.h"
#include "ImportDlg.h"
#include "resource.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CImportDlg, CDialogEx)

CImportDlg::CImportDlg(DataBase* p_DataBase, CWnd* pParent /*=nullptr*/) 
	: CDialogEx(IDD_IMPORT_DIALOG, pParent)
{
	m_data_base = p_DataBase;
}

CImportDlg::~CImportDlg()
{
}

BEGIN_MESSAGE_MAP(CImportDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CImportDlg::OnOK)
	ON_BN_CLICKED(IDC_CHECK_IMPORT_MOLREF, &CImportDlg::OnBnClickedCheckImportMolref)
	ON_CBN_SELCHANGE(IDC_COMBO_IMPORT_MOLREF, &CImportDlg::OnCbnSelchangeComboImportMolref)
	ON_BN_CLICKED(IDC_CHECK_IMPORT_HFC, &CImportDlg::OnBnClickedCheckImportHfc)
	ON_BN_CLICKED(IDC_CHECK_IMPORT_FC, &CImportDlg::OnBnClickedCheckImportFc)
	ON_BN_CLICKED(IDC_CHECK_IMPORT_NMR, &CImportDlg::OnBnClickedCheckImportNmr)
END_MESSAGE_MAP()

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CImportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_CheckBoxREFSPC.SubclassDlgItem(IDC_CHECK_IMPORT_REFSPC, this);
	m_TextBoxREFSPC.SubclassDlgItem(IDC_TEXTFIELD_IMPORT_REFSPC, this);
	if (m_bCheckBOXREFSPC_visState == TRUE)
	{
		m_CheckBoxREFSPC.EnableWindow();
		m_TextBoxREFSPC.EnableWindow();
	}
	m_CheckBoxMOLREF.SubclassDlgItem(IDC_CHECK_IMPORT_MOLREF, this);
	if (m_bCheckBoxMOLREF_visState == TRUE)
	{
		m_CheckBoxMOLREF.EnableWindow();
	}
	m_CheckBoxMOL.SubclassDlgItem(IDC_CHECK_IMPORT_MOL, this);
	m_ComboBoxMOLREF.SubclassDlgItem(IDC_COMBO_IMPORT_MOLREF, this);
	{
		CRect combo_rc;
		m_ComboBoxMOLREF.GetWindowRect(&combo_rc);
		m_ComboBoxMOLREF.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (2 * m_ComboBoxMOLREF.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		m_ComboBoxMOLREF.AddString(_T("molecule"));
		m_ComboBoxMOLREF.AddString(_T("reference (TMS)"));
		m_ComboBoxMOLREF.EnableWindow(FALSE);
	}
	m_CheckBoxORCANMR.SubclassDlgItem(IDC_CHECK_IMPORT_NMR, this);
	m_CheckBoxORCAHFC.SubclassDlgItem(IDC_CHECK_IMPORT_HFC, this);
	m_CheckBoxORCACASSCF.SubclassDlgItem(IDC_CHECK_IMPORT_CASSCF, this);
	m_ComboBoxCASSCF.SubclassDlgItem(IDC_COMBO_IMPORT_CASSCF, this);
	{
		CRect combo_rc;
		m_ComboBoxCASSCF.GetWindowRect(&combo_rc);
		if (m_bCOMBO_CASSCF != m_bCOMBO_NEVPT2)
		{
			m_ComboBoxCASSCF.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (1 * m_ComboBoxMOLREF.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		}
		else
		{
			m_ComboBoxCASSCF.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (2 * m_ComboBoxMOLREF.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
		}
		if (m_bCOMBO_CASSCF == TRUE)
		{
			m_ComboBoxCASSCF.AddString(_T("CASSCF"));
		}
		if (m_bCOMBO_NEVPT2 == TRUE)
		{
			m_ComboBoxCASSCF.AddString(_T("NEVPT2"));
		}
	}
	m_ComboBoxCASSCF.EnableWindow(FALSE);
	m_CheckBoxORCAFC.SubclassDlgItem(IDC_CHECK_IMPORT_FC, this);
	m_TextBoxHFC.SubclassDlgItem(IDC_TEXTFIELD_IMPORT_HFC, this);
	m_TextBoxFC.SubclassDlgItem(IDC_TEXTFIELD_IMPORT_FC, this);
	m_TextBoxSHIFT.SubclassDlgItem(IDC_TEXTFIELD_IMPORT_SHIFT, this);
	return TRUE;
}
void CImportDlg::OnOK()
{
	m_bCheckBoxREFSPC_State = (m_CheckBoxREFSPC.GetCheck() == BST_CHECKED);
	m_TextBoxREFSPC.GetWindowText(REFSPC_name);
	if (REFSPC_name == _T(""))
	{
		REFSPC_name = _T("unnamed spectrum");
	}
	int selectedIndex = m_ComboBoxMOLREF.GetCurSel();
	if (selectedIndex == 0 && selectedIndex != CB_ERR)
	{
		MOL = TRUE;
		REF = FALSE;
	}
	else if (selectedIndex == 1 && selectedIndex != CB_ERR)
	{
		MOL = FALSE;
		REF = TRUE;
	}
	m_bCheckBoxMOL_State = (m_CheckBoxMOL.GetCheck() == BST_CHECKED);
	m_bCheckBoxORCANMR_State = (m_CheckBoxORCANMR.GetCheck() == BST_CHECKED);
	m_bCheckBoxORCAHFC_State = (m_CheckBoxORCAHFC.GetCheck() == BST_CHECKED);
	m_bCheckBoxORCAFC_State = (m_CheckBoxORCAFC.GetCheck() == BST_CHECKED);
	m_bCheckBoxORCACASSCF_State = (m_CheckBoxORCACASSCF.GetCheck() == BST_CHECKED);
	m_TextBoxSHIFT.GetWindowText(NMR_name);
	if (NMR_name == _T(""))
	{
		NMR_name = _T("unnamed diam. shift");
	}
	m_TextBoxHFC.GetWindowText(HFC_name);
	if (HFC_name == _T(""))
	{
		HFC_name = _T("unnamed HFCs");
	}
	m_TextBoxFC.GetWindowText(FC_name);
	if (FC_name == _T(""))
	{
		FC_name = _T("unnamed FCs");
	}
	if (m_ComboBoxCASSCF.GetCurSel() != CB_ERR && m_ComboBoxCASSCF.GetCurSel() >= 0)
	{
		m_ComboBoxCASSCF.GetLBText(m_ComboBoxCASSCF.GetCurSel(), CASSCF_name);
	}
	else
	{
		CASSCF_name = _T("ERROR");
	}
	CDialogEx::OnOK();
}

void CImportDlg::ImportFile(std::string path)
{
	std::ifstream import_file(path);
	if (import_file.is_open())
	{
		if (try_import_spectrum_original(import_file, path))
		{
			m_bCheckBOXREFSPC_visState = TRUE;
		}
		else if (try_import_spectrum_csv(import_file))
		{
			m_bCheckBOXREFSPC_visState = TRUE;
		}
		else
		{
			m_bCheckBOXREFSPC_visState = FALSE;
			if (try_import_ORCA_MoleculeInfo(import_file))
			{
				m_bCheckBoxMOLREF_visState = TRUE;
				define_isotopes();
				if (try_import_ORCA_NMR(import_file) && try_import_ORCA_ShieldingTensor(import_file))
				{
					m_bCheckBoxORCANMR_visState = TRUE;
				}
				else
				{
					m_bCheckBoxORCANMR_visState = FALSE;
				}
				if (try_import_ORCA_HFC(import_file))
				{
					m_bCheckBoxORCAHFC_visState = TRUE;
					for (size_t i = 0; i < molekel.HFCs.size(); i++)
					{
						double pfac = Aprefac * get_gyromagnetic_ratio(m_data_base->CStoS(m_assigned_isotope[i]), -1, true) * get_gyromagnetic_ratio("E", -1, true) * pow(10.0, -6.0);
						for (size_t k = 0; k < 3; k++)
						{
							for (size_t l = 0; l < 3; l++)
							{
								molekel.HFCs_inHz[i][k][l] *= pfac * pow(10, 6);
								molekel.HFCs[i][k][l] *= pfac;
								molekel.HFCs[i][k][l] = convert_MHz_to_ppm(molekel.HFCs[i][k][l], i);
							}
						}
					}
					if (try_import_ORCA_FC(import_file))
					{
						m_bCheckBoxORCAFC_visState = TRUE;
					}
					else
					{
						m_bCheckBoxORCAFC_visState = FALSE;
					}
				}
				else
				{
					m_bCheckBoxORCAHFC_visState = FALSE;
				}
			}
			else
			{
				m_bCheckBoxMOLREF_visState = FALSE;
			}
			if (try_import_ORCA_CASSCF(import_file))
			{
				if (molekel.CHI_CASSCF_temp.size() != 0)
				{
					m_bCOMBO_CASSCF = TRUE;
					m_bCheckBoxORCACASSCF_visState = TRUE;
				}
				if (molekel.CHI_NEVPT2_temp.size() != 0)
				{
					m_bCOMBO_NEVPT2 = TRUE;
					m_bCheckBoxORCACASSCF_visState = TRUE;
				}
			}
			else
			{
				m_bCheckBoxORCACASSCF_visState = FALSE;
			}
		}
	}
	else
	{
		AfxMessageBox(_T("Unable to load file!"), MB_ICONERROR);
	}
	import_file.close();
}

void CImportDlg::ResetDlg()
{
	m_bCheckBoxREFSPC_State = FALSE;
	spc.clear();
	REFSPC_name = _T("");
	molekel.element.clear();
	molekel.xyz_coords.clear();
	molekel.multiplicity = 0;
	molekel.d_orb_iso.clear();
	molekel.HFCs.clear();
	molekel.FCs.clear();
	molekel.CHI_CASSCF.clear();
	molekel.CHI_CASSCF_temp.clear();
	molekel.CHI_NEVPT2.clear();
	molekel.CHI_NEVPT2_temp.clear();
	molekel.ShieldingTensors.clear();
	MOL = FALSE;
	REF = FALSE;
	m_bCheckBoxMOL_State = FALSE;
	m_bCheckBoxORCANMR_State = FALSE;
	m_bCheckBoxORCAHFC_State = FALSE;
	m_bCheckBoxORCAFC_State = FALSE;
	m_bCheckBoxORCACASSCF_State = FALSE;
	NMR_name = _T("");
	HFC_name = _T("");
	FC_name = _T("");
	CASSCF_name = _T("");
	m_found_element.clear();
	m_assigned_isotope.clear();
}

bool CImportDlg::try_import_spectrum_csv(std::ifstream& file)
{
	std::string line;
	bool error = false;
	std::vector<std::pair<double, double>> spectrum;
	while (std::getline(file, line))
	{
		spectrum.push_back(std::make_pair(string_to_double(get_block_in_line(line, "\t", 0, error), error), string_to_double(get_block_in_line(line, "\t", 1, error), error)));
		if (error)
		{
			break;
		}
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	if (error)
	{
		return false;
	}
	spc = spectrum;
	return true;
}

bool CImportDlg::try_import_spectrum_original(std::ifstream& file, std::string& path)
{
	std::vector<std::pair<double, double>> spectrum;
	bool error = false;
	int SI;
	double SW_p, SF, OFFSET;
	if (!load_procs(SI, SW_p, SF, OFFSET, file)) { error = true; }
	file.clear();
	file.seekg(0, std::ios::beg);
	if (error) { return false; }
	std::string file2 = std::filesystem::path(path).replace_filename("1r").string();
	std::vector<int32_t> intensities = read1r(file2, SI);
	if (intensities.size() != SI)
	{
		return false;
	}
	double ppm;
	for (int i = 0; i < SI; i++)
	{
		ppm = OFFSET - (static_cast<double>(i) / SI) * (SW_p / SF);
		spectrum.push_back(std::make_pair(ppm, intensities[i]));
	}
	spc = spectrum;
	return true;
}

bool CImportDlg::try_import_ORCA_MoleculeInfo(std::ifstream& file)
{
	std::string line;
	bool error = false;
	std::vector<int> found_index = { 0, 0 };
	bool found_coord = false;
	bool found_mult = false;
	while (std::getline(file, line))
	{
		if (found_index[0] == 0)
		{
			if (line.find("CARTESIAN COORDINATES (ANGSTROEM)") != std::string::npos)
			{
				found_index[0] = 1;
				found_index[1] = 0;
			}
			else if (line.find("Multiplicity") != std::string::npos)
			{
				found_index[0] = 2;
				found_index[1] = 0;
			}
		}
		if (found_index[0] == 1 && found_coord == false)
		{
			if (line == "")
			{
				found_index[0] = 0;
				found_index[1] = 0;
				molekel.d_orb_iso.resize(molekel.element.size());
				molekel.HFCs.resize(molekel.element.size());
				molekel.HFCs_inHz.resize(molekel.element.size());
				molekel.ShieldingTensors.resize(molekel.element.size());
				found_coord = true;
			}
			if (found_index[1] > 1)
			{
				std::string tmp_str;
				molekel.element.push_back(get_block_in_line(line, " ", 0, error));
				molekel.xyz_coords.push_back(std::vector<double>());
				molekel.xyz_coords[molekel.xyz_coords.size() - 1].push_back(string_to_double(get_block_in_line(line, " ", 1, error), error));
				molekel.xyz_coords[molekel.xyz_coords.size() - 1].push_back(string_to_double(get_block_in_line(line, " ", 2, error), error));
				molekel.xyz_coords[molekel.xyz_coords.size() - 1].push_back(string_to_double(get_block_in_line(line, " ", 3, error), error));
			}
		}
		else if (found_index[0] == 2 && found_mult == false)
		{
			if (molekel.multiplicity == 0)
			{
				molekel.multiplicity = string_to_int(get_block_in_line(line, " ", 3, error), error);
				found_mult = true;
			}
			found_index[0] = 0;
			found_index[1] = 0;
		}
		found_index[1]++;
		if ((found_coord == true && found_mult == true) || error == true)
		{
			break;
		}
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	if (error)
	{
		return false;
	}
	if (found_coord == true && found_mult == true)
	{
		m_found_element = molekel.element;
		return true;
	}
	return false;
}

bool CImportDlg::try_import_ORCA_NMR(std::ifstream& file)
{
	std::string line;
	bool error = false;
	std::vector<int> found_index = { 0, 0 };
	bool found_shielding = false;
	while (std::getline(file, line))
	{
		if (found_index[0] == 0)
		{
			if (line.find("CHEMICAL SHIELDING SUMMARY (ppm)") != std::string::npos)
			{
				found_index[0] = 1;
				found_index[1] = 0;
			}
		}
		if (found_index[0] == 1 && found_shielding == false)
		{
			if (line == "" && found_index[1] > 3)
			{
				found_index[0] = 0;
				found_index[1] = 0;
				found_shielding = true;
			}
			if (found_index[1] > 5)
			{
				int index = string_to_int(get_block_in_line(line, " ", 0, error), error);
				if (index >= 0 && index < molekel.d_orb_iso.size())
				{
					molekel.d_orb_iso[index] = string_to_double(get_block_in_line(line, " ", 2, error), error);
				}
				else
				{
					error = true;
				}
			}
		}
		found_index[1]++;
		if (error == true || found_shielding == true)
		{
			break;
		}
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	if (error)
	{
		return false;
	}
	if (found_shielding)
	{
		return true;
	}
	return false;
}

bool CImportDlg::try_import_ORCA_ShieldingTensor(std::ifstream& file)
{
	std::string line;
	bool error = false;
	bool shielding_init_found = false;
	int shielding_index = 0;
	std::vector<int> found_index = { 0, 0 };
	bool found_shielding = false;
	while (std::getline(file, line))
	{
		if (found_index[0] == 0)
		{
			if (line.find("CHEMICAL SHIELDINGS (ppm)") != std::string::npos)
			{
				found_index[0] = 1;
				found_index[1] = 0;
			}
		}
		else if (found_index[0] == 1)
		{
			if (!shielding_init_found)
			{
				if (line.find("Nucleus") != std::string::npos)
				{
					shielding_index = seperate_int(get_block_in_line(line, " ", 1, error), error);
					if (shielding_index < 0 || shielding_index >= molekel.ShieldingTensors.size())
					{
						error = true;
					}
					found_index[1] = 0;
					shielding_init_found = true;
				}
			}
			else
			{
				if (line.find("CHEMICAL SHIELDING SUMMARY") != std::string::npos)
				{
					found_index[0] = 0;
					found_index[1] = 0;
					found_shielding = true;
				}
				else if (line.find("Nucleus") != std::string::npos)
				{
					shielding_index = seperate_int(get_block_in_line(line, " ", 1, error), error);
					if (shielding_index < 0 || shielding_index >= molekel.HFCs.size())
					{
						error = true;
					}
					found_index[1] = 0;
				}
				else if (line.find("Total shielding tensor (ppm):") != std::string::npos)
				{
					found_index[0] = 2;
					found_index[1] = 0;
				}
			}
		}
		else if (found_index[0] == 2)
		{
			if (found_index[1] == 1)
			{
				molekel.ShieldingTensors[shielding_index].resize(3);
				molekel.ShieldingTensors[shielding_index][0].push_back(string_to_double(get_block_in_line(line, " ", 0, error), error));
				molekel.ShieldingTensors[shielding_index][0].push_back(string_to_double(get_block_in_line(line, " ", 1, error), error));
				molekel.ShieldingTensors[shielding_index][0].push_back(string_to_double(get_block_in_line(line, " ", 2, error), error));
			}
			else if (found_index[1] == 2)
			{
				molekel.ShieldingTensors[shielding_index][1].push_back(string_to_double(get_block_in_line(line, " ", 0, error), error));
				molekel.ShieldingTensors[shielding_index][1].push_back(string_to_double(get_block_in_line(line, " ", 1, error), error));
				molekel.ShieldingTensors[shielding_index][1].push_back(string_to_double(get_block_in_line(line, " ", 2, error), error));
			}
			else if (found_index[1] == 3)
			{
				molekel.ShieldingTensors[shielding_index][2].push_back(string_to_double(get_block_in_line(line, " ", 0, error), error));
				molekel.ShieldingTensors[shielding_index][2].push_back(string_to_double(get_block_in_line(line, " ", 1, error), error));
				molekel.ShieldingTensors[shielding_index][2].push_back(string_to_double(get_block_in_line(line, " ", 2, error), error));
				found_index[0] = 1;
				found_index[1] = 0;
			}
		}
		found_index[1]++;
		if (error == true || found_shielding == true)
		{
			break;
		}
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	if (error)
	{
		return false;
	}
	else if (found_shielding)
	{
		for (size_t i = 0; i < molekel.ShieldingTensors.size(); i++)
		{
			if (molekel.ShieldingTensors[i].empty())
			{
				molekel.ShieldingTensors[i].push_back(std::vector<double>(3, 0.0));
				molekel.ShieldingTensors[i].push_back(std::vector<double>(3, 0.0));
				molekel.ShieldingTensors[i].push_back(std::vector<double>(3, 0.0));
			}
		}
	}
	if (found_shielding)
	{
		return true;
	}
	return false;
}

bool CImportDlg::try_import_ORCA_HFC(std::ifstream& file)
{
	std::string line;
	bool error = false;
	bool hfc_init_found = false;
	int hfc_index = 0;
	std::vector<int> found_index = { 0, 0 };
	bool found_hfcs = false;
	double P;
	while (std::getline(file, line))
	{
		if (found_index[0] == 0)
		{
			if (line.find("ELECTRIC AND MAGNETIC HYPERFINE STRUCTURE") != std::string::npos)
			{
				found_index[0] = 1;
				found_index[1] = 0;
			}
		}

		if (found_index[0] == 1)
		{
			if (!hfc_init_found)
			{
				if (line.find("Nucleus") != std::string::npos)
				{
					hfc_index = seperate_int(get_block_in_line(line, " ", 1, error), error);
					if (hfc_index < 0 || hfc_index >= molekel.HFCs.size())
					{
						error = true;
					}
					std::string str = get_block_in_line(GetPLine(line), " ", 0, error);
					P = string_to_double(str, error);

					found_index[1] = 0;
					hfc_init_found = true;
				}
			}
			else
			{
				if (line.find("Hyperfine and quadrupole coupling calculation done") != std::string::npos)
				{
					found_index[0] = 0;
					found_index[1] = 0;
					found_hfcs = true;
				}
				else if (found_index[1] > 10 && line.find("Nucleus") != std::string::npos)
				{
					hfc_index = seperate_int(get_block_in_line(line, " ", 1, error), error);
					if (hfc_index < 0 || hfc_index >= molekel.HFCs.size())
					{
						error = true;
					}
					std::string str = get_block_in_line(GetPLine(line), " ", 0, error);
					P = string_to_double(str, error);
					found_index[1] = 0;
				}
				else if (found_index[1] == 8)
				{
					molekel.HFCs[hfc_index].resize(3);
					molekel.HFCs_inHz[hfc_index].resize(3);
					molekel.HFCs[hfc_index][0].emplace_back(string_to_double(get_block_in_line(line, " ", 0, error), error) / P);
					molekel.HFCs[hfc_index][0].emplace_back(string_to_double(get_block_in_line(line, " ", 1, error), error) / P);
					molekel.HFCs[hfc_index][0].emplace_back(string_to_double(get_block_in_line(line, " ", 2, error), error) / P);

					molekel.HFCs_inHz[hfc_index][0] = molekel.HFCs[hfc_index][0];
				}
				else if (found_index[1] == 9)
				{
					molekel.HFCs[hfc_index][1].emplace_back(string_to_double(get_block_in_line(line, " ", 0, error), error) / P);
					molekel.HFCs[hfc_index][1].emplace_back(string_to_double(get_block_in_line(line, " ", 1, error), error) / P);
					molekel.HFCs[hfc_index][1].emplace_back(string_to_double(get_block_in_line(line, " ", 2, error), error) / P);
					molekel.HFCs_inHz[hfc_index][1] = molekel.HFCs[hfc_index][1];
				}
				else if (found_index[1] == 10)
				{
					molekel.HFCs[hfc_index][2].emplace_back(string_to_double(get_block_in_line(line, " ", 0, error), error) / P);
					molekel.HFCs[hfc_index][2].emplace_back(string_to_double(get_block_in_line(line, " ", 1, error), error) / P);
					molekel.HFCs[hfc_index][2].emplace_back(string_to_double(get_block_in_line(line, " ", 2, error), error) / P);
					molekel.HFCs_inHz[hfc_index][2] = molekel.HFCs[hfc_index][2];
				}
			}
		}
		found_index[1]++;
		if (error == true || found_hfcs == true)
		{
			break;
		}
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	if (error)
	{
		return false;
	}
	else if (found_hfcs)
	{
		for (size_t i = 0; i < molekel.HFCs.size(); i++)
		{
			if (molekel.HFCs[i].empty())
			{
				molekel.HFCs[i].push_back(std::vector<double>(3, 0.0));
				molekel.HFCs[i].push_back(std::vector<double>(3, 0.0));
				molekel.HFCs[i].push_back(std::vector<double>(3, 0.0));
				molekel.HFCs_inHz[i] = molekel.HFCs[i];
			}
		}
	}
	if (found_hfcs)
	{
		return true;
	}
	return false;
}

bool CImportDlg::try_import_ORCA_FC(std::ifstream& file)
{
	molekel.FCs.resize(molekel.HFCs.size());
	std::string line;
	bool error = false;
	bool hfc_init_found = false;
	int fc_index = 0;
	std::vector<int> found_index = { 0, 0 };
	bool found_fcs = false;
	double P = 0.0;
	while (std::getline(file, line))
	{
		if (found_index[0] == 0)
		{
			if (line.find("ELECTRIC AND MAGNETIC HYPERFINE STRUCTURE") != std::string::npos)
			{
				found_index[0] = 1;
				found_index[1] = 0;
			}
		}
		if (found_index[0] == 1)
		{
			if (!hfc_init_found)
			{
				if (line.find("Nucleus") != std::string::npos)
				{
					fc_index = seperate_int(get_block_in_line(line, " ", 1, error), error);
					if (fc_index < 0 || fc_index >= molekel.FCs.size())
					{
						error = true;
					}
					std::string str = get_block_in_line(GetPLine(line), " ", 0, error);
					P = string_to_double(str, error);
					found_index[1] = 0;
					hfc_init_found = true;
				}
			}
			else
			{
				if (line.find("Hyperfine and quadrupole coupling calculation done") != std::string::npos)
				{
					found_index[0] = 0;
					found_index[1] = 0;
					found_fcs = true;
				}
				else if (found_index[1] > 12 && line.find("Nucleus") != std::string::npos)
				{
					fc_index = seperate_int(get_block_in_line(line, " ", 1, error), error);
					if (fc_index < 0 || fc_index >= molekel.FCs.size())
					{
						error = true;
					}
					std::string str = get_block_in_line(GetPLine(line), " ", 0, error);
					P = string_to_double(str, error);
					found_index[1] = 0;
				}
				else if (line.find("A(Tot)") != std::string::npos)
				{
					molekel.FCs[fc_index] = 0.0;
					molekel.FCs[fc_index] += string_to_double(get_block_in_line(line, " ", 1, error), error);
					molekel.FCs[fc_index] += string_to_double(get_block_in_line(line, " ", 2, error), error);
					molekel.FCs[fc_index] += string_to_double(get_block_in_line(line, " ", 3, error), error);
					molekel.FCs[fc_index] /= (3.0 * P);
				}
			}
		}
		found_index[1]++;
		if (error == true || found_fcs == true)
		{
			break;
		}
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	if (error)
	{
		return false;
	}
	else if (found_fcs)
	{
		for (size_t n = 0; n < molekel.FCs.size(); n++)
		{
			molekel.FCs[n] = convert_FC_to_ppmK(molekel.FCs[n]);
		}
	}
	if (found_fcs)
	{
		return true;
	}
	return false;
}

bool CImportDlg::try_import_ORCA_CASSCF(std::ifstream& file)
{
	std::string line;
	bool error = false;
	bool found_susceptibilityCASSCF = false;
	bool found_susceptibilityNEVPT2 = false;
	bool new_chi_found = false;
	std::vector<int> found_index = { 0,0 };
	while (std::getline(file, line))
	{
		if (found_index[0] == 0)
		{
			if (line.find("QDPT WITH CASSCF DIAGONAL ENERGIES") != std::string::npos)
			{
				found_index[0] = 1;
				found_index[1] = 0;
			}
			else if (line.find("QDPT WITH NEVPT2 DIAGONAL ENERGIES") != std::string::npos)
			{
				found_index[0] = 3;
				found_index[1] = 0;
			}
		}
		else if (found_index[0] == 1)
		{
			if (line.find("TEMPERATURE DEPENDENT MOLAR MAGNETIC SUSCEPTIBILITY TENSOR") != std::string::npos)
			{
				found_index[0] = 2;
				found_index[1] = 0;
			}
		}
		else if (found_index[0] == 3)
		{
			if (line.find("TEMPERATURE DEPENDENT MOLAR MAGNETIC SUSCEPTIBILITY TENSOR") != std::string::npos)
			{
				found_index[0] = 4;
				found_index[1] = 0;
			}
		}

		if (found_index[0] == 2)
		{
			if (found_index[1] > 1 && line.find("-----") != std::string::npos)
			{
				found_index[0] = 0;
				found_index[1] = 0;
				found_susceptibilityCASSCF = true;
			}
			else if (found_index[1] > 5 && line.find("TEMPERATURE/K") != std::string::npos)
			{
				molekel.CHI_CASSCF_temp.emplace_back(string_to_double(get_block_in_line(line, " ", 1, error), error));
				found_index[1] = 0;
				new_chi_found = true;
			}
			else if (found_index[1] == 2 && new_chi_found == true)
			{
				molekel.CHI_CASSCF.emplace_back(std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0)));
				size_t CHI_index = molekel.CHI_CASSCF.size() - 1;
				molekel.CHI_CASSCF[CHI_index][0][0] = string_to_double(get_block_in_line(line, " ", 0, error), error);
				molekel.CHI_CASSCF[CHI_index][0][1] = string_to_double(get_block_in_line(line, " ", 1, error), error);
				molekel.CHI_CASSCF[CHI_index][0][2] = string_to_double(get_block_in_line(line, " ", 2, error), error);
			}
			else if (found_index[1] == 3 && new_chi_found == true)
			{
				size_t CHI_index = molekel.CHI_CASSCF.size() - 1;
				molekel.CHI_CASSCF[CHI_index][1][0] = string_to_double(get_block_in_line(line, " ", 0, error), error);
				molekel.CHI_CASSCF[CHI_index][1][1] = string_to_double(get_block_in_line(line, " ", 1, error), error);
				molekel.CHI_CASSCF[CHI_index][1][2] = string_to_double(get_block_in_line(line, " ", 2, error), error);
			}
			else if (found_index[1] == 4 && new_chi_found == true)
			{
				size_t CHI_index = molekel.CHI_CASSCF.size() - 1;
				molekel.CHI_CASSCF[CHI_index][2][0] = string_to_double(get_block_in_line(line, " ", 0, error), error);
				molekel.CHI_CASSCF[CHI_index][2][1] = string_to_double(get_block_in_line(line, " ", 1, error), error);
				molekel.CHI_CASSCF[CHI_index][2][2] = string_to_double(get_block_in_line(line, " ", 2, error), error);
				new_chi_found = false;
			}
		}
		else if (found_index[0] == 4)
		{
			if (found_index[1] > 1 && line.find("-----") != std::string::npos)
			{
				found_index[0] = 0;
				found_index[1] = 0;
				found_susceptibilityNEVPT2 = true;
			}
			else if (found_index[1] > 5 && line.find("TEMPERATURE/K") != std::string::npos)
			{
				molekel.CHI_NEVPT2_temp.emplace_back(string_to_double(get_block_in_line(line, " ", 1, error), error));
				found_index[1] = 0;
				new_chi_found = true;
			}
			else if (found_index[1] == 2 && new_chi_found == true)
			{
				molekel.CHI_NEVPT2.emplace_back(std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0)));
				size_t CHI_index = molekel.CHI_NEVPT2.size() - 1;
				molekel.CHI_NEVPT2[CHI_index][0][0] = string_to_double(get_block_in_line(line, " ", 0, error), error);
				molekel.CHI_NEVPT2[CHI_index][0][1] = string_to_double(get_block_in_line(line, " ", 1, error), error);
				molekel.CHI_NEVPT2[CHI_index][0][2] = string_to_double(get_block_in_line(line, " ", 2, error), error);
			}
			else if (found_index[1] == 3 && new_chi_found == true)
			{
				size_t CHI_index = molekel.CHI_NEVPT2.size() - 1;
				molekel.CHI_NEVPT2[CHI_index][1][0] = string_to_double(get_block_in_line(line, " ", 0, error), error);
				molekel.CHI_NEVPT2[CHI_index][1][1] = string_to_double(get_block_in_line(line, " ", 1, error), error);
				molekel.CHI_NEVPT2[CHI_index][1][2] = string_to_double(get_block_in_line(line, " ", 2, error), error);
			}
			else if (found_index[1] == 4 && new_chi_found == true)
			{
				size_t CHI_index = molekel.CHI_NEVPT2.size() - 1;
				molekel.CHI_NEVPT2[CHI_index][2][0] = string_to_double(get_block_in_line(line, " ", 0, error), error);
				molekel.CHI_NEVPT2[CHI_index][2][1] = string_to_double(get_block_in_line(line, " ", 1, error), error);
				molekel.CHI_NEVPT2[CHI_index][2][2] = string_to_double(get_block_in_line(line, " ", 2, error), error);
				new_chi_found = false;
			}
		}
		found_index[1]++;
		if (error == true || (found_susceptibilityCASSCF == true && found_susceptibilityNEVPT2 == true))
		{
			break;
		}
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	if (error || new_chi_found)
	{
		return false;
	}
	if (found_susceptibilityCASSCF || found_susceptibilityNEVPT2)
	{
		return true;
	}
	return false;
}

std::string CImportDlg::get_block_in_line(std::string line, std::string seperator, int block, bool& error)
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
double CImportDlg::string_to_double(std::string line, bool& error)
{
	bool last_error = error;
	std::istringstream iss(line);
	double d;
	char c;
	if (!(iss >> d))
	{
		error = true;
		return 0.0;
	}
	if (iss >> c) {
		error = true;
		return 0.0;
	}
	error = last_error;
	return d;
}

int CImportDlg::string_to_int(std::string line, bool& error)
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

double CImportDlg::convert_MHz_to_ppm(double value, size_t index)
{
	double factor = (pow(10.0, 6.0) * h * pow(10.0, -24.0)) / (mu0 * muB * g_e * hquer);	
	factor /= get_gyromagnetic_ratio(m_data_base->CStoS(m_assigned_isotope[index]), -1, true);
	return value * factor;
}

void CImportDlg::define_isotopes()
{
	std::vector<CString> isotope(m_found_element.size(), _T("empty"));
	for (size_t i = 0; i < m_found_element.size(); i++)
	{
		CString current_isotope;
		std::string current_element;
		if (isotope[i] == "empty")
		{
			current_element = m_found_element[i];
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
				if (m_found_element[j] == current_element)
				{
					isotope[j] = current_isotope;
				}
			}
		}
	}
	m_assigned_isotope.clear();
	m_assigned_isotope = isotope;
}

int CImportDlg::seperate_int(std::string line, bool& error)
{
	bool last_error = error;
	int number;
	std::istringstream iss(line);
	std::string numPart;
	for (size_t i = 0; i < line.size(); ++i)
	{
		if (std::isdigit(line[i]))
		{
			numPart += line[i];
		}
		else
		{
			break;
		}
	}
	if (numPart.empty())
	{
		error = true;
		return 0;
	}
	std::istringstream numStream(numPart);
	if (!(numStream >> number))
	{
		error = true;
		return 0;
	}
	error = last_error;
	return number;
}

double CImportDlg::convert_FC_to_ppmK(double value)
{
	double m = (mu0 * pow(muB, 2.0) * pow(g_e, 2.0)) / (9 * k_B);
	m /= pow(a0, 3.0);
	m *= pow(10.0, 6.0);
	double S = (molekel.multiplicity - 1.0) / 2.0;
	double factor = (3.0 * m * S * (S + 1.0)) / (4.0 * PI);
	return value * factor;
}

std::string CImportDlg::GetPLine(std::string str)
{
	return str.substr(str.find("P=") + 2, str.length());
}

bool CImportDlg::load_procs(int& SI, double& SW_p, double& SF, double& OFFSET, std::ifstream& file)
{
	std::string line;
	int count = 0;
	while (std::getline(file, line))
	{
		if (line.find("##$SI=") != std::string::npos || line.find("##$SW_p=") != std::string::npos || line.find("##$SF=") != std::string::npos || line.find("##$OFFSET=") != std::string::npos)
		{
			size_t pos = line.find("=");
			std::string value = line.substr(pos + 1);
			if (line.find("##$SI=") != std::string::npos)
			{
				SI = stoi(value);
				count++;
			}
			else if (line.find("##$SW_p=") != std::string::npos)
			{
				SW_p = stod(value);
				count++;
			}
			else if (line.find("##$SF=") != std::string::npos)
			{
				SF = stod(value);
				count++;
			}
			else if (line.find("##$OFFSET=") != std::string::npos)
			{
				OFFSET = stod(value);
				count++;
			}
		}
	}
	if (count != 4) { return false; }
	return true;
}

std::vector<int32_t> CImportDlg::read1r(const std::string& filename, int size)
{
	std::vector<int32_t> data(size);
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		return { 0 };
	}
	file.read(reinterpret_cast<char*>(data.data()), size * sizeof(int32_t));
	file.close();
	return data;
}

void CImportDlg::OnBnClickedCheckImportMolref()
{
	if (m_CheckBoxMOLREF.GetCheck() == BST_CHECKED)
	{
		m_ComboBoxMOLREF.EnableWindow();
	}
	else
	{
		m_ComboBoxMOLREF.EnableWindow(FALSE);
		m_ComboBoxMOLREF.SetCurSel(-1);
		m_CheckBoxMOL.EnableWindow(FALSE);
		m_CheckBoxMOL.SetCheck(!BST_CHECKED);
		m_CheckBoxORCANMR.EnableWindow(FALSE);
		m_CheckBoxORCANMR.SetCheck(!BST_CHECKED);
		m_CheckBoxORCAHFC.EnableWindow(FALSE);
		m_CheckBoxORCAHFC.SetCheck(!BST_CHECKED);
		m_CheckBoxORCACASSCF.EnableWindow(FALSE);
		m_CheckBoxORCACASSCF.SetCheck(!BST_CHECKED);
		m_ComboBoxCASSCF.EnableWindow(FALSE);
		m_ComboBoxCASSCF.SetCurSel(-1);
		m_CheckBoxORCAFC.EnableWindow(FALSE);
		m_CheckBoxORCAFC.SetCheck(!BST_CHECKED);
		m_TextBoxFC.EnableWindow(FALSE);
		m_TextBoxFC.SetWindowText(_T(""));
		m_TextBoxHFC.EnableWindow(FALSE);
		m_TextBoxHFC.SetWindowText(_T(""));
		m_TextBoxSHIFT.EnableWindow(FALSE);
		m_TextBoxSHIFT.SetWindowText(_T(""));
	}
}

void CImportDlg::OnCbnSelchangeComboImportMolref()
{
	int selectedIndex = m_ComboBoxMOLREF.GetCurSel();
	if (selectedIndex == 0 && selectedIndex != CB_ERR)
	{
		m_CheckBoxMOL.EnableWindow();
		if (m_bCheckBoxORCANMR_visState == TRUE)
		{
			m_CheckBoxORCANMR.EnableWindow();
		}
		if (m_bCheckBoxORCAHFC_visState == TRUE)
		{
			m_CheckBoxORCAHFC.EnableWindow();
		}
		if (m_bCheckBoxORCACASSCF_visState == TRUE)
		{
			m_CheckBoxORCACASSCF.EnableWindow();
			m_ComboBoxCASSCF.EnableWindow();
		}
		if (m_bCheckBoxORCAFC_visState == TRUE)
		{
			m_CheckBoxORCAFC.EnableWindow();
		}
	}
	else if (selectedIndex != CB_ERR)
	{
		m_CheckBoxMOL.EnableWindow(FALSE);
		m_CheckBoxMOL.SetCheck(!BST_CHECKED);
		m_CheckBoxORCANMR.EnableWindow(FALSE);
		m_CheckBoxORCANMR.SetCheck(!BST_CHECKED);
		m_CheckBoxORCAHFC.EnableWindow(FALSE);
		m_CheckBoxORCAHFC.SetCheck(!BST_CHECKED);
		m_CheckBoxORCACASSCF.EnableWindow(FALSE);
		m_CheckBoxORCACASSCF.SetCheck(!BST_CHECKED);
		m_ComboBoxCASSCF.EnableWindow(FALSE);
		m_ComboBoxCASSCF.SetCurSel(-1);
		m_CheckBoxORCAFC.EnableWindow(FALSE);
		m_CheckBoxORCAFC.SetCheck(!BST_CHECKED);
		m_TextBoxHFC.EnableWindow(FALSE);
		m_TextBoxFC.EnableWindow(FALSE);
		m_TextBoxHFC.SetWindowTextW(_T(""));
		m_TextBoxFC.SetWindowTextW(_T(""));
		m_TextBoxSHIFT.EnableWindow(FALSE);
		m_TextBoxSHIFT.SetWindowText(_T(""));
	}
}

void CImportDlg::OnBnClickedCheckImportHfc()
{
	if (m_CheckBoxORCAHFC.GetCheck() == BST_CHECKED)
	{
		m_TextBoxHFC.EnableWindow();
	}
	else
	{
		m_TextBoxHFC.EnableWindow(FALSE);
		m_TextBoxHFC.SetWindowTextW(_T(""));
	}
}

void CImportDlg::OnBnClickedCheckImportFc()
{
	if (m_CheckBoxORCAFC.GetCheck() == BST_CHECKED)
	{
		m_TextBoxFC.EnableWindow();
	}
	else
	{
		m_TextBoxFC.EnableWindow(FALSE);
		m_TextBoxFC.SetWindowTextW(_T(""));
	}
}

void CImportDlg::OnBnClickedCheckImportNmr()
{
	if (m_CheckBoxORCANMR.GetCheck() == BST_CHECKED)
	{
		m_TextBoxSHIFT.EnableWindow();
	}
	else
	{
		m_TextBoxSHIFT.EnableWindow(FALSE);
		m_TextBoxSHIFT.SetWindowText(_T(""));
	}
}














