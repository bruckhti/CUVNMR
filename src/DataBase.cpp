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
#include "DataBase.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DataBase::DataBase()
{
	if (GetUserPath())
	{
		b_shutdown = false;
		Init();
		m_default_conn_threshold = 2.1f;
		m_background_color = glm::vec3(1.0f);
		m_cylinder_radius = 0.08f;
	}
	else
	{
		b_shutdown = true;
	}	
}

DataBase::~DataBase() {

	for (int i = 0; i < m_connections.size(); i++) 
	{
		delete m_connections[i];
	}
	for (int i = 0; i < m_point_names.size(); i++) 
	{
		delete m_point_names[i];
	}
}

bool DataBase::EmergencyShutdown()
{
	return b_shutdown;
}

bool DataBase::GetUserPath()
{
	char* buffer = nullptr;
	size_t size = 0;
	if (_dupenv_s(&buffer, &size, "USERPROFILE") != 0)
	{
		AfxMessageBox(_T("Critical Error: Unable to access user environment variables. CUVNMR cannot determine the workspace directory."), MB_ICONERROR);
		free(buffer);
		return false;
	}
	std::filesystem::path userDir(buffer);
	free(buffer);
	std::filesystem::path cuvnmrDir = userDir / "cuvnmr";
	if (!std::filesystem::exists(cuvnmrDir))
	{
		try
		{
			std::filesystem::create_directory(cuvnmrDir);
			CString infoMsg = _T("Welcome to CUVNMR!\n\nIt appears to be your first time running the software on this system. A dedicated workspace folder has been created for your settings and data exports:\n\n");
			infoMsg += cuvnmrDir.c_str();
			infoMsg += _T("\n\nYou can customize viewport settings that will be saved as 'ViewerProperties.cuv' located in the '/essentials' subfolder.");

			AfxMessageBox(infoMsg, MB_ICONINFORMATION);
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			CString errorMsg = _T("Failed to initialize workspace directory.\n\nSystem Error: ");
			errorMsg += e.what();
			errorMsg += _T("\n\nPlease check write permissions for your user profile.");
			AfxMessageBox(errorMsg, MB_ICONERROR); 
			return false;
		}
	}
	UserProfilePath = cuvnmrDir / "essentials";
	UserDataPath = cuvnmrDir / "data";
	UserExportPath = cuvnmrDir / "exports";
	UserRootPath = cuvnmrDir;
	if (!std::filesystem::exists(UserProfilePath)) { std::filesystem::create_directory(UserProfilePath); }
	if (!std::filesystem::exists(UserDataPath)) { std::filesystem::create_directory(UserDataPath); }
	if (!std::filesystem::exists(cuvnmrDir / "ensemble")) { std::filesystem::create_directory(cuvnmrDir / "ensemble"); }
	if (!std::filesystem::exists(UserExportPath)) { std::filesystem::create_directory(UserExportPath); }
	return true;
}

std::string DataBase::get_UserRootPath()
{
	return UserRootPath.string();
}

void DataBase::InitSimulation(CListCtrl* main_list)
{
	m_simulation.InitList(main_list);
}

void DataBase::Init()
{
	mol_CHI_fitted = std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0));
	ref_element = { "Si", "C" ,"C"	,"C","C","H","H","H","H","H","H","H","H","H","H","H","H" };
	ref_xyz_coords = { {5.0000000000000004e-06, 3.0000000000000001e-06, -7.9999999999999996e-06},
					{0.56036699999999995, -1.5429260000000000, 0.92924899999999999},
					{-0.5646350000000000,1.3081440000000000,1.2361540000000000},
					{-1.4316759999999999,-0.44151299999999999,-1.1461180000000000},
					{1.4359390000000001,0.67629300000000003,-1.0192749999999999},
					{-2.2819620000000000,-0.83780900000000003,-0.58305600000000002},
					{-1.1347739999999999,-1.1999109999999999,-1.8767060000000000},
					{-1.7800300000000000,0.43510100000000002,-1.7004349999999999},
					{2.2839190000000000,0.94222099999999998,-0.38108700000000001},
					{1.1415580000000001,1.5738410000000000,-1.5713729999999999},
					{1.7867480000000000,-0.061233999999999997,-1.7473179999999999},
					{-0.25257900000000000,-1.9597500000000001,1.5312910000000000},
					{1.3919539999999999,-1.3187089999999999,1.6039829999999999},
					{0.89465499999999998,-2.3221029999999998,0.23772699999999999},
					{-1.3987289999999999,0.94482600000000005,1.8438969999999999},
					{-0.89660200000000001,2.2175799999999999,0.72645099999999996},
					{0.24579999999999999,1.5859070000000000,1.9166780000000001} };
	ref_d_orb_iso = { 338.970, 188.682, 188.671, 188.708, 188.708, 31.929, 31.928, 31.928, 31.927,
		            31.927, 31.927, 31.928, 31.927, 31.927, 31.928, 31.927, 31.927 };
	update_reference();
	NMR_active_nucleus = { L"1H",L"2H",L"3H",L"3He",L"6Li",L"7Li",L"9Be",L"10B",L"11B",L"13C",L"14N",L"15N",L"17O",L"19F",L"21Ne",L"23Na",L"25Mg",L"27Al",
		L"29Si",L"31P",L"33S",L"35Cl",L"37Cl",L"39K",L"41K",L"43Ca",L"45Sc",L"47Ti",L"49Ti",L"50V",L"51V",L"53Cr",L"55Mn",L"57Fe",L"59Co",L"61Ni",L"63Cu",
		L"65Cu",L"67Zn",L"69Ga",L"71Ga",L"73Ge",L"75As",L"77Se",L"79Se",L"79Br",L"81Br",L"83Kr",L"85Rb",L"87Rb",L"87Sr",L"89Y",L"91Zr",L"93Nb",L"95Mo",
		L"97Mo",L"99Tc",L"99Ru",L"101Ru",L"103Rh",L"105Pd",L"107Ag",L"109Ag",L"111Cd",L"113Cd",L"113In",L"115In",L"115Sn",L"117Sn",L"119Sn",L"121Sb",
		L"123Sb",L"123Te",L"125Te",L"127I",L"129Xe",L"131Xe",L"133Cs",L"135Ba",L"137Ba",L"138La",L"139La",L"141Pr",L"143Nd",L"145Nd",L"147Sm",L"149Sm",
		L"151Eu",L"153Eu",L"155Gd",L"157Gd",L"159Tb",L"161Dy",L"163Dy",L"165Ho",L"167Er",L"169Tm",L"171Yb",L"173Yb",L"175Lu",L"176Lu",L"177Hf",L"179Hf",
		L"181Ta",L"183W",L"185Re",L"187Re",L"187Os",L"189Os",L"191Ir",L"193Ir",L"195Pt",L"197Au",L"199Hg",L"201Hg",L"203Tl",L"205Tl",L"207Pb",L"209Bi",L"235U" };
	NMR_nucleus_spin = { 0.50,1.00,0.50,0.50,1.00,1.50,1.50,3.00,1.50,0.50,1.00,0.50,2.50,0.50,1.50,1.50,2.50,2.50,0.50,0.50,1.50,1.50,1.50,1.50,1.50,3.50,
		3.50,2.50,3.50,6.00,3.50,1.50,2.50,0.50,3.50,1.50,1.50,1.50,2.50,1.50,1.50,4.50,1.50,0.50,3.50,1.50,1.50,4.50,2.50,1.50,4.50,0.50,2.50,4.50,2.50,
		2.50,4.50,2.50,2.50,0.50,2.50,0.50,0.50,0.50,0.50,4.50,4.50,0.50,0.50,0.50,2.50,3.50,0.50,0.50,2.50,0.50,1.50,3.50,1.50,1.50,5.00,3.50,2.50,3.50,
		3.50,3.50,3.50,2.50,2.50,1.50,1.50,1.50,2.50,2.50,3.50,3.50,0.50,0.50,2.50,3.50,7.00,3.50,4.50,3.50,0.50,2.50,2.50,0.50,1.50,1.50,1.50,0.50,1.50,
		0.50,1.50,0.50,0.50,0.50,4.50,3.50 };
	additional_ref_elements = { L"H",L"H",L"H",L"He",L"Li",L"Li",L"Be",L"B",L"B",L"C",L"N",L"N",L"O",L"F",L"Ne",L"Na",L"Mg",L"Al",
		L"Si",L"P",L"S",L"Cl",L"Cl",L"K",L"K",L"Ca",L"Sc",L"Ti",L"Ti",L"V",L"V",L"Cr",L"Mn",L"Fe",L"Co",L"Ni",L"Cu",
		L"Cu",L"Zn",L"Ga",L"Ga",L"Ge",L"As",L"Se",L"Se",L"Br",L"Br",L"Kr",L"Rb",L"Rb",L"Sr",L"Y",L"Zr",L"Nb",L"Mo",
		L"Mo",L"Tc",L"Ru",L"Ru",L"Rh",L"Pd",L"Ag",L"Ag",L"Cd",L"Cd",L"In",L"In",L"Sn",L"Sn",L"Sn",L"Sb",
		L"Sb",L"Te",L"Te",L"I",L"Xe",L"Xe",L"Cs",L"Ba",L"Ba",L"La",L"La",L"Pr",L"Nd",L"Nd",L"Sm",L"Sm",
		L"Eu",L"Eu",L"Gd",L"Gd",L"Tb",L"Dy",L"Dy",L"Ho",L"Er",L"Tm",L"Yb",L"Yb",L"Lu",L"Lu",L"Hf",L"Hf",
		L"Ta",L"W",L"Re",L"Re",L"Os",L"Os",L"Ir",L"Ir",L"Pt",L"Au",L"Hg",L"Hg",L"Tl",L"Tl",L"Pb",L"Bi",L"U" };
	additional_ref_shifts = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
		0.0,0.0,0.0,0.0,0.0,0.0,0.0 };
	mol_g_tensor = std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0));
	mol_g_tensor[0][0] = g_e;
	mol_g_tensor[1][1] = g_e;
	mol_g_tensor[2][2] = g_e;
	set_sim_g_tensor(true);
	SetDefaultViewProperties();
	if (!loadCustomAdditionalReferences())
	{
		saveCustomAdditionalReferences();
	}
}

CString DataBase::GetAdditionalRefElements(size_t index)
{
	if (index >= 0 && index < additional_ref_elements.size())
	{
		return additional_ref_elements[index];
	}
	return _T("ERROR");
}

void DataBase::SetDefaultViewProperties()
{
	CreateNewPoint(L"H", { 255, 251, 240 }, 0.2);
	CreateNewPoint(L"He", { 217, 255, 255 }, 0.2);
	CreateNewPoint(L"Li", { 204, 128, 255 }, 0.3);
	CreateNewPoint(L"Be", { 194, 255, 0 }, 0.3);
	CreateNewPoint(L"B", { 255, 181, 181 }, 0.3);
	CreateNewPoint(L"C", { 155, 155, 155 }, 0.3);
	CreateNewPoint(L"N", { 38, 0, 255 }, 0.3);
	CreateNewPoint(L"O", { 203, 0, 0 }, 0.3);
	CreateNewPoint(L"F", { 198, 255, 18 }, 0.3);
	CreateNewPoint(L"Ne", { 173, 227, 242 }, 0.3);
	CreateNewPoint(L"Na", { 171, 92, 242 }, 0.3);
	CreateNewPoint(L"Mg", { 135, 255, 0 }, 0.3);
	CreateNewPoint(L"Al", { 207, 166, 166 }, 0.3);
	CreateNewPoint(L"Si", { 209, 214, 140 }, 0.3);
	CreateNewPoint(L"P", { 255, 128, 0 }, 0.3);
	CreateNewPoint(L"S", { 255, 203, 53 }, 0.3);
	CreateNewPoint(L"Cl", { 68, 255, 58 }, 0.3);
	CreateNewPoint(L"Ar", { 128, 209, 230 }, 0.3);
	CreateNewPoint(L"K", { 140, 64, 212 }, 0.3);
	CreateNewPoint(L"Ca", { 0, 255, 144 }, 0.3);
	CreateNewPoint(L"Sc", { 230, 230, 230 }, 0.3);
	CreateNewPoint(L"Ti", { 191, 194, 199 }, 0.3);
	CreateNewPoint(L"V", { 166, 166, 171 }, 0.3);
	CreateNewPoint(L"Cr", { 5, 105, 40 }, 0.3);
	CreateNewPoint(L"Mn", { 156, 122, 199 }, 0.3);
	CreateNewPoint(L"Fe", { 230, 77, 0 }, 0.3);
	CreateNewPoint(L"Co", { 77, 51, 204 }, 0.3);
	CreateNewPoint(L"Ni", { 0, 204, 102 }, 0.3);
	CreateNewPoint(L"Cu", { 204, 77, 26 }, 0.3);
	CreateNewPoint(L"Zn", { 125, 128, 176 }, 0.3);
	CreateNewPoint(L"Ga", { 194, 143, 143 }, 0.3);
	CreateNewPoint(L"Ge", { 102, 143, 143 }, 0.3);
	CreateNewPoint(L"As", { 189, 128, 227 }, 0.3);
	CreateNewPoint(L"Se", { 255, 161, 0 }, 0.3);
	CreateNewPoint(L"Br", { 190, 130, 60 }, 0.3);
	CreateNewPoint(L"Kr", { 92, 184, 209 }, 0.3);
	CreateNewPoint(L"Rb", { 112, 46, 176 }, 0.3);
	CreateNewPoint(L"Sr", { 0, 255, 0 }, 0.3);
	CreateNewPoint(L"Y", { 148, 255, 255 }, 0.3);
	CreateNewPoint(L"Zr", { 148, 224, 224 }, 0.3);
	CreateNewPoint(L"Nb", { 115, 194, 201 }, 0.3);
	CreateNewPoint(L"Mo", { 84, 181, 181 }, 0.3);
	CreateNewPoint(L"Tc", { 59, 158, 158 }, 0.3);
	CreateNewPoint(L"Ru", { 36, 143, 143 }, 0.3);
	CreateNewPoint(L"Rh", { 10, 125, 140 }, 0.3);
	CreateNewPoint(L"Pd", { 0, 122, 153 }, 0.3);
	CreateNewPoint(L"Ag", { 192, 192, 192 }, 0.3);
	CreateNewPoint(L"Cd", { 255, 217, 143 }, 0.3);
	CreateNewPoint(L"In", { 166, 117, 115 }, 0.3);
	CreateNewPoint(L"Sn", { 102, 128, 128 }, 0.3);
	CreateNewPoint(L"Sb", { 158, 99, 181 }, 0.3);
	CreateNewPoint(L"Te", { 212, 122, 0 }, 0.3);
	CreateNewPoint(L"I", { 153, 0, 153 }, 0.3);
	CreateNewPoint(L"Xe", { 66, 158, 176 }, 0.3);
	CreateNewPoint(L"Cs", { 87, 23, 143 }, 0.3);
	CreateNewPoint(L"Ba", { 0, 201, 0 }, 0.3);
	CreateNewPoint(L"La", { 112, 212, 255 }, 0.3);
	CreateNewPoint(L"Ce", { 255, 255, 199 }, 0.3);
	CreateNewPoint(L"Pr", { 217, 255, 199 }, 0.3);
	CreateNewPoint(L"Nd", { 199, 255, 199 }, 0.3);
	CreateNewPoint(L"Pm", { 163, 255, 199 }, 0.3);
	CreateNewPoint(L"Sm", { 143, 255, 199 }, 0.3);
	CreateNewPoint(L"Eu", { 97, 255, 199 }, 0.3);
	CreateNewPoint(L"Gd", { 69, 255, 199 }, 0.3);
	CreateNewPoint(L"Tb", { 48, 255, 199 }, 0.3);
	CreateNewPoint(L"Dy", { 31, 255, 199 }, 0.3);
	CreateNewPoint(L"Ho", { 0, 255, 156 }, 0.3);
	CreateNewPoint(L"Er", { 0, 230, 117 }, 0.3);
	CreateNewPoint(L"Tm", { 0, 212, 82 }, 0.3);
	CreateNewPoint(L"Yb", { 0, 191, 56 }, 0.3);
	CreateNewPoint(L"Lu", { 0, 171, 36 }, 0.3);
	CreateNewPoint(L"Hf", { 77, 194, 255 }, 0.3);
	CreateNewPoint(L"Ta", { 77, 166, 255 }, 0.3);
	CreateNewPoint(L"W", { 33, 148, 214 }, 0.3);
	CreateNewPoint(L"Re", { 38, 125, 171 }, 0.3);
	CreateNewPoint(L"Os", { 38, 102, 150 }, 0.3);
	CreateNewPoint(L"Ir", { 23, 84, 135 }, 0.3);
	CreateNewPoint(L"Pt", { 208, 208, 224 }, 0.3);
	CreateNewPoint(L"Au", { 255, 209, 35 }, 0.3);
	CreateNewPoint(L"Hg", { 184, 184, 208 }, 0.3);
	CreateNewPoint(L"Tl", { 166, 84, 77 }, 0.3);
	CreateNewPoint(L"Pb", { 87, 89, 97 }, 0.3);
	CreateNewPoint(L"Bi", { 158, 79, 181 }, 0.3);
	CreateNewPoint(L"Po", { 171, 92, 0 }, 0.3);
	CreateNewPoint(L"At", { 117, 79, 69 }, 0.3);
	CreateNewPoint(L"Rn", { 66, 130, 150 }, 0.3);
	CreateNewPoint(L"Fr", { 66, 0, 102 }, 0.3);
	CreateNewPoint(L"Ra", { 0, 125, 0 }, 0.3);
	CreateNewPoint(L"Ac", { 112, 171, 250 }, 0.3);
	CreateNewPoint(L"Th", { 0, 186, 255 }, 0.3);
	CreateNewPoint(L"Pa", { 0, 161, 255 }, 0.3);
	CreateNewPoint(L"U", { 0, 143, 255 }, 0.3);
	CreateNewPoint(L"Np", { 0, 128, 255 }, 0.3);
	CreateNewPoint(L"Pu", { 0, 107, 255 }, 0.3);
	CreateNewPoint(L"Am", { 84, 92, 242 }, 0.3);
	CreateNewPoint(L"Cm", { 120, 92, 227 }, 0.3);
	CreateNewPoint(L"Bk", { 138, 79, 227 }, 0.3);
	CreateNewPoint(L"Cf", { 161, 54, 212 }, 0.3);
	CreateNewPoint(L"Es", { 179, 31, 212 }, 0.3);
	CreateNewPoint(L"Fm", { 179, 31, 186 }, 0.3);
	CreateNewPoint(L"Md", { 179, 13, 166 }, 0.3);
	CreateNewPoint(L"No", { 189, 13, 135 }, 0.3);
	CreateNewPoint(L"Lr", { 199, 0, 102 }, 0.3);
	CreateNewPoint(L"Rf", { 204, 0, 89 }, 0.3);
	CreateNewPoint(L"Db", { 209, 0, 79 }, 0.3);
	CreateNewPoint(L"Sg", { 217, 0, 69 }, 0.3);
	CreateNewPoint(L"Bh", { 224, 0, 56 }, 0.3);
	CreateNewPoint(L"Hs", { 230, 0, 46 }, 0.3);
	CreateNewPoint(L"Mt", { 235, 0, 38 }, 0.3);
	CreateNewPoint(L"Ds", { 255, 0, 255 }, 0.3);
	CreateNewPoint(L"Rg", { 255, 0, 255 }, 0.3);
	CreateNewPoint(L"Cn", { 255, 0, 255 }, 0.3);
	CreateNewPoint(L"Nh", { 255, 0, 255 }, 0.3);
	CreateNewPoint(L"Fl", { 255, 0, 255 }, 0.3);
	CreateNewPoint(L"Mc", { 255, 0, 255 }, 0.3);
	CreateNewPoint(L"Lv", { 255, 0, 255 }, 0.3);
	CreateNewPoint(L"Ts", { 255, 0, 255 }, 0.3);
	CreateNewPoint(L"Og", { 255, 0, 255 }, 0.3);
	if (!loadViewerProperties())
	{
		saveViewerProperties();
	}
}

void DataBase::update_reference()
{
	int sumH = 0;
	int sumC = 0;
	ref_Si_shift = 0.0;
	ref_H_shift = 0.0;
	ref_C_shift = 0.0;
	for (size_t i = 0; i < ref_element.size(); i++)
	{
		if (ref_element[i] == "Si")
		{
			ref_Si_shift += ref_d_orb_iso[i];
		}
		else if (ref_element[i] == "H")
		{
			ref_H_shift += ref_d_orb_iso[i];
			sumH++;
		}
		else if (ref_element[i] == "C")
		{
			ref_C_shift += ref_d_orb_iso[i];
			sumC++;
		}
	}
	ref_H_shift /= sumH;
	ref_C_shift /= sumC;
}

void DataBase::set_spectrum(std::vector<std::pair<double, double>>& spectrum, CString name)
{
	spectra.push_back(spectrum);
	spectra_names.push_back(name);
}

void DataBase::remove_spectrum(size_t Entry)
{
	if (Entry < spectra.size() && Entry >= 0)
	{
		spectra.erase(spectra.begin() + Entry);
		spectra_names.erase(spectra_names.begin() + Entry);
	}
}

CString DataBase::GetSpectrumName(size_t index)
{
	if (index < spectra_names.size())
	{
		return spectra_names[index];
	}
	return _T("");
}

size_t DataBase::GetSpectraCount()
{
	return spectra_names.size();
}

std::vector<std::pair<double, double>> DataBase::GetSpectrum(size_t index)
{
	if (index < spectra.size())
	{
		return spectra[index];
	}
	return { { 0,0 } };
}

void DataBase::set_reference(std::vector<std::string>& elements, std::vector<std::vector<double>>& xyz, std::vector<double>& d_orb)
{
	ref_element.clear();
	ref_element = elements;
	ref_xyz_coords.clear();
	ref_xyz_coords = xyz;
	ref_d_orb_iso.clear();
	ref_d_orb_iso = d_orb;
	update_reference();
}

void DataBase::set_additional_reference(CString element, double d_orb)
{
	for (size_t i = 0; i < NMR_active_nucleus.size(); i++)
	{
		if (element == NMR_active_nucleus[i])
		{
			additional_ref_shifts[i] = d_orb;
			break;
		}
	}
	saveCustomAdditionalReferences();
}

bool DataBase::compare_molecules(std::vector<std::string>& elements)
{
	if (mol_element.size() != 0)
	{
		if (mol_element.size() != elements.size())
		{
			return false;
		}
		else
		{
			for (size_t i = 0; i < mol_element.size(); i++)
			{
				if (mol_element[i] != elements[i])
				{
					return false;
				}
			}
		}
	}
	return true;
}

void DataBase::reset_molecule()
{
	mol_element.clear();
	mol_xyz_coords.clear();
	mol_multiplicity = 1;
	mol_d_orb_iso.clear();
	mol_d_orb_names.clear();
	mol_HFCs.clear();
	mol_HFCs_names.clear();
	mol_FCs.clear();
	mol_FCs_names.clear();
	mol_ShieldingTensors.clear();
	mol_HFCs_inHz.clear();
}

void DataBase::set_molecule(std::vector<std::string>& elements, std::vector<std::vector<double>>& xyz, int multiplicity)
{
	mol_element.clear();
	mol_element = elements;
	mol_xyz_coords.clear();
	mol_xyz_coords = xyz;
	mol_multiplicity = multiplicity;
	m_simulation.SetTotalSpin((double)multiplicity);
	m_simulation.SetPDDistance(CalculatePDDistances());
	UpdatePoints(false);
}

void DataBase::set_mol_HFCs(std::vector<std::vector<std::vector<double>>>& HFC, std::vector<std::vector<std::vector<double>>>& HFC_inHz, CString name)
{
	mol_HFCs.push_back(HFC);
	mol_HFCs_inHz.push_back(HFC_inHz);
	mol_HFCs_names.push_back(name);
}

void DataBase::set_mol_FC(std::vector<double>& FC, CString name)
{
	mol_FCs.push_back(FC);
	mol_FCs_names.push_back(name);
}

void DataBase::set_mol_d_orb(std::vector<double>& d_orb, CString name)
{
	mol_d_orb_iso.push_back(d_orb);
	mol_d_orb_names.push_back(name);
}

void DataBase::set_mol_shielding(std::vector<std::vector<std::vector<double>>>& shieldings)
{
	mol_ShieldingTensors.push_back(shieldings);
}

bool DataBase::is_molecule_set()
{
	if (mol_element.size() == 0)
	{
		return false;
	}
	if (mol_xyz_coords.size() == 0)
	{
		return false;
	}
	return true;
}

void DataBase::set_CHI(std::vector<std::vector<std::vector<double>>>& CHI, std::vector<double>& CHI_temp)
{
	mol_CHI.clear();
	mol_CHI = CHI;
	mol_CHI_temp.clear();
	mol_CHI_temp = CHI_temp;
}

double DataBase::get_mol_radius()
{
	double max_distance = -INFINITY;
	double distance;
	for (size_t i = 0; i < mol_xyz_coords.size(); i++)
	{
		for (size_t j = 0; j < mol_xyz_coords.size(); j++)
		{
			distance = 0.0;
			distance += pow((mol_xyz_coords[j][0] - mol_xyz_coords[i][0]), 2.0);
			distance += pow((mol_xyz_coords[j][1] - mol_xyz_coords[i][1]), 2.0);
			distance += pow((mol_xyz_coords[j][2] - mol_xyz_coords[i][2]), 2.0);
			distance = sqrt(distance);
			if (distance > max_distance)
			{
				max_distance = distance;
			}
		}
	}
	return max_distance / 2.0;
}

void DataBase::UpdatePoints(bool reset)
{
	if (reset)
	{
		m_simulation.Reset_listed();
		m_simulation.Reset_computed();
		m_points.clear();
	}
	else
	{
		m_simulation.Reset_listed();
		m_simulation.Reset_computed();
		m_points.clear();
		for (size_t i = 0; i < mol_element.size(); i++)
		{
			std::wstring wStr(mol_element[i].begin(), mol_element[i].end());
			PointName* pntName = getPointNameCreate(wStr);
			float x, y, z;
			x = static_cast<float>(mol_xyz_coords[i][0]);
			y = static_cast<float>(mol_xyz_coords[i][1]);
			z = static_cast<float>(mol_xyz_coords[i][2]);
			m_points.emplace_back(Point{ pntName, glm::vec3(x,y,z) });
		}
	}
}

void DataBase::RotatePoints(float alpha, float beta)
{
	alpha /= 10.0;
	beta /= 10.0;
	float x, y, z;
	glm::vec3 position;
	for (size_t i = 0; i < m_points.size(); i++)
	{
		position = m_points[i].pos;
		x = cos(beta) * position.x + sin(beta) * position.z;
		y = (-sin(alpha)) * (-sin(beta)) * position.x + cos(alpha) * position.y - sin(alpha) * cos(beta) * position.z;
		z = -cos(alpha) * sin(beta) * position.x + sin(alpha) * position.y + cos(alpha) * cos(beta) * position.z;
		m_points[i].pos = glm::vec3(x, y, z);
	}
}

void DataBase::CreateNewPoint(std::wstring name, std::vector<int> RGB, float radius)
{
	float r = RGB[0] / 255.0f;
	float g = RGB[1] / 255.0f;
	float b = RGB[2] / 255.0f;
	PointName* pnt_name = new PointName(m_point_names.size(), name, glm::vec3(r, g, b), radius);
	std::vector<PointConnection*> newConnectionRow;
	for (size_t i = 0; i < m_point_names.size(); i++)
	{
		PointConnection* connection = new PointConnection(m_connections.size(), pnt_name, m_point_names[i], 2.1f);
		m_connections.emplace_back(connection);
		m_conn_matrix[i].emplace_back(connection);
		newConnectionRow.emplace_back(connection);
	}
	PointConnection* connection = new PointConnection(m_connections.size(), pnt_name, pnt_name, 2.1f);
	m_connections.emplace_back(connection);
	newConnectionRow.emplace_back(connection);
	m_conn_matrix.emplace_back(newConnectionRow);
	m_point_names.emplace_back(pnt_name);
}

PointName* DataBase::getPointNameCreate(std::wstring name) 
{
	for (int i = 0; i < m_point_names.size(); i++) 
	{
		if (m_point_names[i]->getName() == name) 
		{
			return m_point_names[i];
		}
	}
	float r = rand() / (RAND_MAX * 1.5f);
	float g = rand() / (RAND_MAX * 1.5f);
	float b = rand() / (RAND_MAX * 1.5f);
	PointName* pnt_name = new PointName(m_point_names.size(), name, glm::vec3(r, g, b), 0.3f);
	std::vector<PointConnection*> newRow;
	for (int i = 0; i < m_point_names.size(); i++) 
	{
		PointConnection* conn = new PointConnection(m_connections.size(), pnt_name, m_point_names[i], m_default_conn_threshold);
		m_connections.emplace_back(conn);
		m_conn_matrix[i].emplace_back(conn);
		newRow.emplace_back(conn);
	}
	PointConnection* conn = new PointConnection(m_connections.size(), pnt_name, pnt_name, m_default_conn_threshold);
	m_connections.emplace_back(conn);
	newRow.emplace_back(conn);
	m_conn_matrix.emplace_back(newRow);
	m_point_names.emplace_back(pnt_name);
	return m_point_names[m_point_names.size() - 1];
}

size_t DataBase::getNumPoints() 
{
	return m_points.size();
}

Point* DataBase::getPoint(int index) 
{
	if (index >= m_points.size()) 
	{
		return nullptr;
	}
	return &m_points[index];
}

size_t DataBase::getNumPointNames() 
{
	return m_point_names.size();
}

PointName* DataBase::getPointName(int index)
{
	if (index >= m_point_names.size()) 
	{
		return nullptr;
	}
	return m_point_names[index];
}

PointName* DataBase::getPointName(std::wstring str) 
{
	for (int i = 0; i < m_point_names.size(); i++) 
	{

		if (m_point_names[i]->getName() == str) 
		{
			return m_point_names[i];
		}
	}
	return nullptr;
}

PointConnection* DataBase::getConnection(size_t indexA, size_t indexB) 
{
	return m_conn_matrix[indexA][indexB];
}

PointConnection* DataBase::getConnection(std::wstring strA, std::wstring strB) 
{
	size_t indexA = getPointName(strA)->getIndex();
	size_t indexB = getPointName(strB)->getIndex();
	return getConnection(indexA, indexB);
}

void  DataBase::setBackgroundColor(glm::vec3 col) 
{
	m_background_color = col;
}

glm::vec3 DataBase::getBackgroundColor()
{
	return m_background_color;
}

float DataBase::getCylinderRadius() 
{
	return m_cylinder_radius;
}

void DataBase::setCylinderRadius(float val) 
{
	m_cylinder_radius = val;
}

Camera* DataBase::getCamera() 
{
	return &m_camera;
}



double DataBase::get_mol_coordinate(size_t index, size_t xyz)
{
	if (index < mol_xyz_coords.size() && xyz < 3 && xyz >= 0)
	{
		return mol_xyz_coords[index][xyz];
	}
	return 0.0;
}

int DataBase::get_mol_multiplicity()
{
	return mol_multiplicity;
}

void DataBase::delete_Shieldings()
{
	mol_d_orb_iso.clear();
	mol_d_orb_names.clear();
	mol_ShieldingTensors.clear();
}

size_t DataBase::GetNMRnucleiSize()
{
	return NMR_active_nucleus.size();
}

CString DataBase::GetNMRnuclei(size_t index)
{
	return NMR_active_nucleus[index];
}

size_t DataBase::get_mol_d_orb_DataSize()
{
	return mol_d_orb_names.size();
}

size_t DataBase::get_mol_FC_DataSize()
{
	return mol_FCs_names.size();
}

size_t DataBase::get_mol_HFC_DataSize()
{
	return mol_HFCs_names.size();
}

CString DataBase::get_mol_d_orb_DataName(size_t index)
{
	return mol_d_orb_names[index];
}

CString DataBase::get_mol_HFC_DataName(size_t index)
{
	return mol_HFCs_names[index];
}

CString DataBase::get_mol_FC_DataName(size_t index)
{
	return mol_FCs_names[index];
}

size_t DataBase::get_mol_chi_size()
{
	return mol_CHI_temp.size();
}

double DataBase::get_mol_chi_Temp(size_t index)
{
	return mol_CHI_temp[index];
}

std::vector<std::vector<double>> DataBase::get_mol_CHI(size_t index)
{
	return mol_CHI[index];
}

std::vector<std::vector<double>> DataBase::get_g_tensor()
{
	return mol_g_tensor;
}

void DataBase::set_g_tensor(std::vector<std::vector<double>> new_g_tensor)
{
	mol_g_tensor = new_g_tensor;
	set_sim_g_tensor(false);
}

void DataBase::set_sim_g_tensor(bool g_init)
{
	m_simulation.SetEssentials_g(mol_g_tensor, g_init);
}

void DataBase::set_fitted_Chi(std::vector<std::vector<double>>& CHI)
{
	mol_CHI_fitted = CHI;
}

std::vector<std::vector<double>> DataBase::get_fitted_CHI()
{
	return mol_CHI_fitted;
}

int DataBase::IsNMRactiveIsotope(CString element)
{
	int counter = 0;
	size_t index;
	for (size_t i = 0; i < additional_ref_elements.size(); i++)
	{
		if (additional_ref_elements[i] == element)
		{
			counter++;
			index = i;
		}
	}
	if (counter > 1)
	{
		return -1;
	}
	else if (counter == 1)
	{
		return static_cast<int>(index);
	}
	return -2;
}

CString DataBase::StoCS(std::string str)
{
	CString return_value(str.c_str());
	return return_value;
}

std::string DataBase::CStoS(CString str)
{
	std::wstring wstr = str.GetString();
	std::string return_value(wstr.begin(), wstr.end());
	return return_value;
}

std::vector<CString> DataBase::GetIsotopesOfType(CString element)
{
	std::vector<CString> return_vector;
	for (size_t i = 0; i < additional_ref_elements.size(); i++)
	{
		if (additional_ref_elements[i] == element)
		{
			return_vector.push_back(NMR_active_nucleus[i]);
		}
	}
	return return_vector;
}

std::vector<double> DataBase::get_mol_d_orb_raw(size_t index)
{
	return mol_d_orb_iso[index];
}

std::vector<double> DataBase::get_mol_d_orb(size_t index)
{
	if (index < mol_d_orb_iso.size())
	{
		std::vector<double> dorb_referenced;
		std::string element;
		std::string isotope = "";
		std::vector<std::string> found_element;
		std::vector<std::string> assigned_isotope;
		for (size_t i = 0; i < mol_d_orb_iso[index].size(); i++)
		{
			isotope = "";
			element = mol_element[i];
			for (size_t j = 0; j < found_element.size(); j++)
			{
				if (element == found_element[j])
				{
					isotope = assigned_isotope[j];
				}
			}
			if (isotope == "")
			{
				int index = IsNMRactiveIsotope(StoCS(element));
				if (index == -1)
				{
					CIsotopeDlg IsotopeSelectionDlg(GetIsotopesOfType(StoCS(element)));
					IsotopeSelectionDlg.DoModal();
					isotope = CStoS(IsotopeSelectionDlg.m_ChosenIsotope);
				}
				else if (index == -2)
				{
					isotope = "error";
				}
				else
				{
					isotope = CStoS(GetNMRnuclei(index));
				}
				found_element.push_back(element);
				assigned_isotope.push_back(isotope);
			}
			double delta;
			double sigma_ref;
			double sigma_calc;
			sigma_ref = get_reference_by_isotope(StoCS(isotope)) * 1E-6;
			sigma_calc = mol_d_orb_iso[index][i] * 1E-6;
			delta = (sigma_ref - sigma_calc) / (1.0 - sigma_ref);
			delta /= 1E-6;
			dorb_referenced.emplace_back(delta);
		}
		return dorb_referenced;
	}
	return { 0.0 };
}

double DataBase::get_reference_by_isotope(CString isotope)
{
	double sigma_ref = 0.0;
	if (isotope == _T("1H"))
	{
		sigma_ref = ref_H_shift;
	}
	else if (isotope == "13C")
	{
		sigma_ref = ref_C_shift;
	}
	else if (isotope == "29Si")
	{
		sigma_ref = ref_Si_shift;
	}
	else if (isotope == "error")
	{
		sigma_ref = 0.0;
	}
	else
	{
		for (size_t n = 0; n < additional_ref_elements.size(); n++)
		{
			if (NMR_active_nucleus[n] == isotope)
			{
				sigma_ref = additional_ref_shifts[n];
				break;
			}
		}
	}
	return sigma_ref;
}

std::vector<double> DataBase::get_mol_fc(size_t index)
{
	if (index < mol_FCs.size())
	{
		return mol_FCs[index];
	}
	return { 0.0 };
}

std::vector<std::vector<std::vector<double>>> DataBase::get_mol_hfc(size_t index)
{
	if (index < mol_HFCs.size())
	{
		return mol_HFCs[index];
	}
	return { {{0.0}} };
}

std::vector<std::vector<std::vector<double>>> DataBase::get_mol_hfc_inHz(size_t index)
{
	if (index < mol_HFCs_inHz.size())
	{
		return mol_HFCs_inHz[index];
	}
	return { {{0.0}} };
}

std::vector<std::vector<std::vector<double>>> DataBase::get_mol_shieldingTensors(size_t index)
{
	if (index < mol_ShieldingTensors.size())
	{
		return mol_ShieldingTensors[index];
	}
	return { {{0.0}} };
}

double DataBase::convertCHItoA3(double CHI_CGI, double temp)
{
	return (CHI_CGI * 4.0 * PI * pow(10.0, 24.0)) / (temp * N_avo);
}

double DataBase::convertCHItoCGI(double CHI_A3, double temp)
{
	return (CHI_A3 * temp * N_avo) / (4.0 * PI * pow(10.0, 24.0));
}

std::vector<std::vector<double>> DataBase::CalculatePDDistances()
{
	std::vector<std::vector<double>> return_list;
	double SI_Distance_x;
	double SI_Distance_y;
	double SI_Distance_z;
	for (size_t i = 0; i < mol_xyz_coords.size(); i++)
	{
		SI_Distance_x = mol_xyz_coords[i][0];
		SI_Distance_y = mol_xyz_coords[i][1];
		SI_Distance_z = mol_xyz_coords[i][2];
		return_list.push_back({ SI_Distance_x - PD_positions[0], SI_Distance_y - PD_positions[1], SI_Distance_z - PD_positions[2] });
	}
	return return_list;
}

void DataBase::setPDPositions(double x, double y, double z)
{
	PD_positions.clear();
	PD_positions.push_back(x);
	PD_positions.push_back(y);
	PD_positions.push_back(z);
}



void DataBase::loadDefaultViewerProperties()
{
	glm::vec3 colVec = glm::vec3(1.0f, 1.0f, 1.0f);
	setBackgroundColor(colVec);
	overridePoint(L"H", { 255, 251, 240 }, 0.2);
	overridePoint(L"He", { 217, 255, 255 }, 0.2);
	overridePoint(L"Li", { 204, 128, 255 }, 0.3);
	overridePoint(L"Be", { 194, 255, 0 }, 0.3);
	overridePoint(L"B", { 255, 181, 181 }, 0.3);
	overridePoint(L"C", { 155, 155, 155 }, 0.3);
	overridePoint(L"N", { 38, 0, 255 }, 0.3);
	overridePoint(L"O", { 203, 0, 0 }, 0.3);
	overridePoint(L"F", { 198, 255, 18 }, 0.3);
	overridePoint(L"Ne", { 173, 227, 242 }, 0.3);
	overridePoint(L"Na", { 171, 92, 242 }, 0.3);
	overridePoint(L"Mg", { 135, 255, 0 }, 0.3);
	overridePoint(L"Al", { 207, 166, 166 }, 0.3);
	overridePoint(L"Si", { 209, 214, 140 }, 0.3);
	overridePoint(L"P", { 255, 128, 0 }, 0.3);
	overridePoint(L"S", { 255, 203, 53 }, 0.3);
	overridePoint(L"Cl", { 68, 255, 58 }, 0.3);
	overridePoint(L"Ar", { 128, 209, 230 }, 0.3);
	overridePoint(L"K", { 140, 64, 212 }, 0.3);
	overridePoint(L"Ca", { 0, 255, 144 }, 0.3);
	overridePoint(L"Sc", { 230, 230, 230 }, 0.3);
	overridePoint(L"Ti", { 191, 194, 199 }, 0.3);
	overridePoint(L"V", { 166, 166, 171 }, 0.3);
	overridePoint(L"Cr", { 5, 105, 40 }, 0.3);
	overridePoint(L"Mn", { 156, 122, 199 }, 0.3);
	overridePoint(L"Fe", { 230, 77, 0 }, 0.3);
	overridePoint(L"Co", { 77, 51, 204 }, 0.3);
	overridePoint(L"Ni", { 0, 204, 102 }, 0.3);
	overridePoint(L"Cu", { 204, 77, 26 }, 0.3);
	overridePoint(L"Zn", { 125, 128, 176 }, 0.3);
	overridePoint(L"Ga", { 194, 143, 143 }, 0.3);
	overridePoint(L"Ge", { 102, 143, 143 }, 0.3);
	overridePoint(L"As", { 189, 128, 227 }, 0.3);
	overridePoint(L"Se", { 255, 161, 0 }, 0.3);
	overridePoint(L"Br", { 190, 130, 60 }, 0.3);
	overridePoint(L"Kr", { 92, 184, 209 }, 0.3);
	overridePoint(L"Rb", { 112, 46, 176 }, 0.3);
	overridePoint(L"Sr", { 0, 255, 0 }, 0.3);
	overridePoint(L"Y", { 148, 255, 255 }, 0.3);
	overridePoint(L"Zr", { 148, 224, 224 }, 0.3);
	overridePoint(L"Nb", { 115, 194, 201 }, 0.3);
	overridePoint(L"Mo", { 84, 181, 181 }, 0.3);
	overridePoint(L"Tc", { 59, 158, 158 }, 0.3);
	overridePoint(L"Ru", { 36, 143, 143 }, 0.3);
	overridePoint(L"Rh", { 10, 125, 140 }, 0.3);
	overridePoint(L"Pd", { 0, 122, 153 }, 0.3);
	overridePoint(L"Ag", { 192, 192, 192 }, 0.3);
	overridePoint(L"Cd", { 255, 217, 143 }, 0.3);
	overridePoint(L"In", { 166, 117, 115 }, 0.3);
	overridePoint(L"Sn", { 102, 128, 128 }, 0.3);
	overridePoint(L"Sb", { 158, 99, 181 }, 0.3);
	overridePoint(L"Te", { 212, 122, 0 }, 0.3);
	overridePoint(L"I", { 153, 0, 153 }, 0.3);
	overridePoint(L"Xe", { 66, 158, 176 }, 0.3);
	overridePoint(L"Cs", { 87, 23, 143 }, 0.3);
	overridePoint(L"Ba", { 0, 201, 0 }, 0.3);
	overridePoint(L"La", { 112, 212, 255 }, 0.3);
	overridePoint(L"Ce", { 255, 255, 199 }, 0.3);
	overridePoint(L"Pr", { 217, 255, 199 }, 0.3);
	overridePoint(L"Nd", { 199, 255, 199 }, 0.3);
	overridePoint(L"Pm", { 163, 255, 199 }, 0.3);
	overridePoint(L"Sm", { 143, 255, 199 }, 0.3);
	overridePoint(L"Eu", { 97, 255, 199 }, 0.3);
	overridePoint(L"Gd", { 69, 255, 199 }, 0.3);
	overridePoint(L"Tb", { 48, 255, 199 }, 0.3);
	overridePoint(L"Dy", { 31, 255, 199 }, 0.3);
	overridePoint(L"Ho", { 0, 255, 156 }, 0.3);
	overridePoint(L"Er", { 0, 230, 117 }, 0.3);
	overridePoint(L"Tm", { 0, 212, 82 }, 0.3);
	overridePoint(L"Yb", { 0, 191, 56 }, 0.3);
	overridePoint(L"Lu", { 0, 171, 36 }, 0.3);
	overridePoint(L"Hf", { 77, 194, 255 }, 0.3);
	overridePoint(L"Ta", { 77, 166, 255 }, 0.3);
	overridePoint(L"W", { 33, 148, 214 }, 0.3);
	overridePoint(L"Re", { 38, 125, 171 }, 0.3);
	overridePoint(L"Os", { 38, 102, 150 }, 0.3);
	overridePoint(L"Ir", { 23, 84, 135 }, 0.3);
	overridePoint(L"Pt", { 208, 208, 224 }, 0.3);
	overridePoint(L"Au", { 255, 209, 35 }, 0.3);
	overridePoint(L"Hg", { 184, 184, 208 }, 0.3);
	overridePoint(L"Tl", { 166, 84, 77 }, 0.3);
	overridePoint(L"Pb", { 87, 89, 97 }, 0.3);
	overridePoint(L"Bi", { 158, 79, 181 }, 0.3);
	overridePoint(L"Po", { 171, 92, 0 }, 0.3);
	overridePoint(L"At", { 117, 79, 69 }, 0.3);
	overridePoint(L"Rn", { 66, 130, 150 }, 0.3);
	overridePoint(L"Fr", { 66, 0, 102 }, 0.3);
	overridePoint(L"Ra", { 0, 125, 0 }, 0.3);
	overridePoint(L"Ac", { 112, 171, 250 }, 0.3);
	overridePoint(L"Th", { 0, 186, 255 }, 0.3);
	overridePoint(L"Pa", { 0, 161, 255 }, 0.3);
	overridePoint(L"U", { 0, 143, 255 }, 0.3);
	overridePoint(L"Np", { 0, 128, 255 }, 0.3);
	overridePoint(L"Pu", { 0, 107, 255 }, 0.3);
	overridePoint(L"Am", { 84, 92, 242 }, 0.3);
	overridePoint(L"Cm", { 120, 92, 227 }, 0.3);
	overridePoint(L"Bk", { 138, 79, 227 }, 0.3);
	overridePoint(L"Cf", { 161, 54, 212 }, 0.3);
	overridePoint(L"Es", { 179, 31, 212 }, 0.3);
	overridePoint(L"Fm", { 179, 31, 186 }, 0.3);
	overridePoint(L"Md", { 179, 13, 166 }, 0.3);
	overridePoint(L"No", { 189, 13, 135 }, 0.3);
	overridePoint(L"Lr", { 199, 0, 102 }, 0.3);
	overridePoint(L"Rf", { 204, 0, 89 }, 0.3);
	overridePoint(L"Db", { 209, 0, 79 }, 0.3);
	overridePoint(L"Sg", { 217, 0, 69 }, 0.3);
	overridePoint(L"Bh", { 224, 0, 56 }, 0.3);
	overridePoint(L"Hs", { 230, 0, 46 }, 0.3);
	overridePoint(L"Mt", { 235, 0, 38 }, 0.3);
	overridePoint(L"Ds", { 255, 0, 255 }, 0.3);
	overridePoint(L"Rg", { 255, 0, 255 }, 0.3);
	overridePoint(L"Cn", { 255, 0, 255 }, 0.3);
	overridePoint(L"Nh", { 255, 0, 255 }, 0.3);
	overridePoint(L"Fl", { 255, 0, 255 }, 0.3);
	overridePoint(L"Mc", { 255, 0, 255 }, 0.3);
	overridePoint(L"Lv", { 255, 0, 255 }, 0.3);
	overridePoint(L"Ts", { 255, 0, 255 }, 0.3);
	overridePoint(L"Og", { 255, 0, 255 }, 0.3);
	for (int i = 0; i < m_connections.size(); i++)
	{
		m_connections[i]->setThreshold(2.1f);
	}
	setCylinderRadius(0.08f);
}

void DataBase::overridePoint(std::wstring name, std::vector<int> RGB, float radius)
{
	PointName* pntName = getPointName(name);
	pntName->setRadius(radius);
	float r = RGB[0] / 255.0f;
	float g = RGB[1] / 255.0f;
	float b = RGB[2] / 255.0f;
	glm::vec3 colVec = glm::vec3(r, g, b);
	pntName->setColor(colVec);
}

bool DataBase::loadCustomAdditionalReferences()
{
	bool return_value = true;
	std::ifstream ReferenceLoadFile(UserProfilePath / "adnlReferences.cuv", std::ios::binary);
	if (ReferenceLoadFile.is_open())
	{
		size_t size = 0;
		ReferenceLoadFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		std::vector<double> ref_shifts_loaded(size);
		ReferenceLoadFile.read(reinterpret_cast<char*>(ref_shifts_loaded.data()), size * sizeof(double));
		additional_ref_shifts.clear();
		additional_ref_shifts = ref_shifts_loaded;
	}
	else
	{
		return_value = false;
	}
	ReferenceLoadFile.close();
	return return_value;
}

void DataBase::saveCustomAdditionalReferences()
{
	std::ofstream ReferenceSaveFile(UserProfilePath / "adnlReferences.cuv", std::ios::binary);
	if (ReferenceSaveFile.is_open())
	{
		size_t size = additional_ref_shifts.size();
		ReferenceSaveFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		ReferenceSaveFile.write(reinterpret_cast<const char*>(additional_ref_shifts.data()), size * sizeof(double));
	}
	ReferenceSaveFile.close();
}

bool DataBase::loadViewerProperties()
{
	bool return_value = true;
	std::ifstream ViewerLoadFile(UserProfilePath / "ViewerProperties.cuv", std::ios::binary);
	if (ViewerLoadFile.is_open())
	{
		glm::vec3 col(1.0f, 1.0f, 1.0f);
		ViewerLoadFile.read(reinterpret_cast<char*>(&col.r), sizeof(col.r));
		ViewerLoadFile.read(reinterpret_cast<char*>(&col.g), sizeof(col.g));
		ViewerLoadFile.read(reinterpret_cast<char*>(&col.b), sizeof(col.b));
		setBackgroundColor(col);
		std::vector<std::wstring> pntNamesStr(118);
		float radius;
		for (size_t i = 0; i < 118; i++)
		{
			size_t wstr_length;
			ViewerLoadFile.read(reinterpret_cast<char*>(&wstr_length), sizeof(wstr_length));
			pntNamesStr[i].resize(wstr_length);
			ViewerLoadFile.read(reinterpret_cast<char*>(&pntNamesStr[i][0]), wstr_length * sizeof(wchar_t));
			ViewerLoadFile.read(reinterpret_cast<char*>(&col.r), sizeof(col.r));
			ViewerLoadFile.read(reinterpret_cast<char*>(&col.g), sizeof(col.g));
			ViewerLoadFile.read(reinterpret_cast<char*>(&col.b), sizeof(col.b));
			ViewerLoadFile.read(reinterpret_cast<char*>(&radius), sizeof(radius));
			int r = col.r * 255;
			int g = col.g * 255;
			int b = col.b * 255;
			overridePoint(pntNamesStr[i], { r, g, b }, radius);
		}
		CString strA, strB;
		for (size_t i = 0; i < 118; i++)
		{
			for (size_t j = 0; j < 118; j++)
			{
				strA = pntNamesStr[i].c_str();
				strB = pntNamesStr[j].c_str();
				PointConnection* conn = getConnection(strA.GetString(), strB.GetString());
				float threshold;
				ViewerLoadFile.read(reinterpret_cast<char*>(&threshold), sizeof(threshold));
				conn->setThreshold(threshold);
			}
		}
		float radiusCylinder;
		ViewerLoadFile.read(reinterpret_cast<char*>(&radiusCylinder), sizeof(radiusCylinder));
		setCylinderRadius(radiusCylinder);
	}
	else
	{
		AfxMessageBox(_T("Failed to load view properties. Restoring default values."), MB_ICONWARNING);
		return_value = false;
	}
	ViewerLoadFile.close();
	return return_value;
}

void DataBase::saveViewerProperties()
{
	std::ofstream ViewerSaveFile(UserProfilePath / "ViewerProperties.cuv", std::ios::binary);
	if (ViewerSaveFile.is_open())
	{
		glm::vec3 col = getBackgroundColor();
		ViewerSaveFile.write(reinterpret_cast<const char*>(&col.r), sizeof(col.r));
		ViewerSaveFile.write(reinterpret_cast<const char*>(&col.g), sizeof(col.g));
		ViewerSaveFile.write(reinterpret_cast<const char*>(&col.b), sizeof(col.b));
		std::vector<std::wstring> pntNamesStr(118);
		float radius;
		for (size_t i = 0; i < 118; i++)
		{
			PointName* pntName = getPointName(i);
			pntNamesStr[i] = pntName->getName();
			col = pntName->getColor();
			radius = pntName->getRadius();
			size_t wstr_length = pntNamesStr[i].size();
			ViewerSaveFile.write(reinterpret_cast<const char*>(&wstr_length), sizeof(wstr_length));
			ViewerSaveFile.write(reinterpret_cast<const char*>(pntNamesStr[i].c_str()), wstr_length * sizeof(wchar_t));
			ViewerSaveFile.write(reinterpret_cast<const char*>(&col.r), sizeof(col.r));
			ViewerSaveFile.write(reinterpret_cast<const char*>(&col.g), sizeof(col.g));
			ViewerSaveFile.write(reinterpret_cast<const char*>(&col.b), sizeof(col.b));
			ViewerSaveFile.write(reinterpret_cast<const char*>(&radius), sizeof(radius));
		}
		CString strA, strB;
		for (size_t i = 0; i < 118; i++)
		{
			for (size_t j = 0; j < 118; j++)
			{
				strA = pntNamesStr[i].c_str();
				strB = pntNamesStr[j].c_str();
				PointConnection* conn = getConnection(strA.GetString(), strB.GetString());
				float threshold = conn->getThreshold();
				ViewerSaveFile.write(reinterpret_cast<const char*>(&threshold), sizeof(threshold));
			}
		}
		float radiusCylinder = getCylinderRadius();
		ViewerSaveFile.write(reinterpret_cast<const char*>(&radiusCylinder), sizeof(radiusCylinder));
		AfxMessageBox(_T("View properties saved."), MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox(_T("Failed to save view properties."), MB_ICONERROR);
		b_shutdown = true;
	}
	ViewerSaveFile.close();
}

size_t DataBase::get_mol_atom_count()
{
	return mol_element.size();
}

CString DataBase::get_mol_element_by_index(size_t index)
{
	return StoCS(mol_element[index]);
}

void DataBase::remove_diamag(size_t index)
{
	mol_d_orb_iso.erase(mol_d_orb_iso.begin() + index);
	mol_d_orb_names.erase(mol_d_orb_names.begin() + index);
	mol_ShieldingTensors.erase(mol_ShieldingTensors.begin() + index);
}

void DataBase::remove_hfc(size_t index)
{
	mol_HFCs.erase(mol_HFCs.begin() + index);
	mol_HFCs_inHz.erase(mol_HFCs_inHz.begin() + index);
	mol_HFCs_names.erase(mol_HFCs_names.begin() + index);
}

void DataBase::remove_fc(size_t index)
{
	mol_FCs.erase(mol_FCs.begin() + index);
	mol_FCs_names.erase(mol_FCs_names.begin() + index);
}

void DataBase::ResetProgram()
{
	mol_element.clear();
	mol_xyz_coords.clear();
	mol_multiplicity = 1;
	mol_d_orb_iso.clear();
	mol_d_orb_names.clear();
	mol_HFCs.clear();
	mol_HFCs_names.clear();
	mol_FCs.clear();
	mol_FCs_names.clear();
	mol_ShieldingTensors.clear();
	mol_HFCs_inHz.clear();
	mol_CHI.clear();
	mol_CHI_temp.clear();
	m_simulation.reset_error();
	m_simulation.set_fitusage(true);
	UpdatePoints(true);
}

void DataBase::remove_spectra()
{
	spectra.clear();
	spectra_names.clear();
}

void DataBase::create_empty_HFC()
{
	std::vector<std::vector<std::vector<double>>> empty_HFC(mol_element.size(), std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0)));
	mol_HFCs.push_back(empty_HFC);
	mol_HFCs_inHz.push_back(empty_HFC);
	mol_HFCs_names.push_back(_T("empty hfc"));
}

CString DataBase::GetUserDataPath()
{
	return UserDataPath.c_str();
}

CString DataBase::GetUserExportPath()
{
	return UserExportPath.c_str();
}