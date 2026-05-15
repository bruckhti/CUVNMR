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
#include <sstream>
#include "DataBase.h"
#include "CIsotopeDlg.h"

class CEnsembleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnsembleDlg)
public:
	CEnsembleDlg(DataBase* p_dataBase, CWnd* pParent = nullptr);
	virtual ~CEnsembleDlg();
	bool IsHFC();
	bool IsShielding();
	std::vector<std::string> get_molecule();
	std::vector<std::vector<std::vector<double>>> get_hfc();
	std::vector<std::vector<std::vector<double>>> get_hfc_hz();
	std::vector<std::vector<std::vector<double>>> get_shielding();
	std::vector<double> get_simga_iso();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT_ENSEMBLE };
#endif
protected:
	DataBase* m_data_base;
	HICON m_hIcon;
	CListBox m_ListBoxInfo;
	CEdit m_EditTemp;
	double m_temperature;
	bool do_hfc;
	bool do_shielding;
	std::vector<std::vector<std::vector<double>>> m_hfc;
	std::vector<std::vector<std::vector<double>>> m_hfc_hz;
	std::vector<std::vector<std::vector<double>>> m_shielding;
	std::vector<double> m_sigma_iso;
	std::vector<std::string> m_molecule;
	int find_file_part(std::ifstream& file);
	std::vector<std::vector<double>> import_g_orientation(std::ifstream& file);
	std::vector<std::vector<std::vector<double>>> import_shielding_tensors(std::ifstream& file);
	std::vector<std::vector<std::vector<double>>> import_hfc_tensors(std::ifstream& file);
	double import_scf_energy(std::ifstream& file);
	std::vector<std::string> import_molecule(std::ifstream& file);
	std::string get_block_in_line(std::string line, std::string seperator, int block, bool& error);
	double string_to_double(std::string line, bool& error);
	int string_to_int(std::string line, bool& error);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnCreateEnsemble();
	afx_msg void OnEditTempKillfocus();
	DECLARE_MESSAGE_MAP()
};
