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
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <sstream>
#include "DataBase.h"
#include "CIsotopeDlg.h"

struct molecule_info
{
	std::vector<std::string> element;
	std::vector<std::vector<double>> xyz_coords;
	int multiplicity = 0;
	std::vector<double> d_orb_iso;
	std::vector<std::vector<std::vector<double>>> HFCs;
	std::vector<std::vector<std::vector<double>>> HFCs_inHz;
	std::vector<double> FCs;
	std::vector<std::vector<std::vector<double>>> CHI_CASSCF;
	std::vector<double> CHI_CASSCF_temp;
	std::vector<std::vector<std::vector<double>>> CHI_NEVPT2;
	std::vector<double> CHI_NEVPT2_temp;
	std::vector<std::vector<std::vector<double>>> ShieldingTensors;
};

class CImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportDlg)
public:
	CImportDlg(DataBase* p_DataBase, CWnd* pParent = nullptr);
	virtual ~CImportDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT_DIALOG };
#endif
public:
	BOOL m_bCheckBOXREFSPC_visState;
	BOOL m_bCheckBoxMOLREF_visState;
	BOOL m_bCheckBoxORCANMR_visState;
	BOOL m_bCheckBoxORCAHFC_visState;
	BOOL m_bCheckBoxORCAFC_visState;
	BOOL m_bCheckBoxORCACASSCF_visState;
	BOOL m_bCOMBO_CASSCF;
	BOOL m_bCOMBO_NEVPT2;
	BOOL m_bCheckBoxREFSPC_State;
	std::vector<std::pair<double, double>> spc;
	CString REFSPC_name;
	molecule_info molekel;
	BOOL MOL;
	BOOL REF;
	BOOL m_bCheckBoxMOL_State;
	BOOL m_bCheckBoxORCANMR_State;
	BOOL m_bCheckBoxORCAHFC_State;
	BOOL m_bCheckBoxORCAFC_State;
	BOOL m_bCheckBoxORCACASSCF_State;
	CString NMR_name;
	CString HFC_name;
	CString FC_name;
	CString CASSCF_name;
	void ImportFile(std::string path);
	void ResetDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	DECLARE_MESSAGE_MAP()
private:
	DataBase* m_data_base;
	CButton m_CheckBoxREFSPC;
	CEdit m_TextBoxREFSPC;
	CComboBox m_ComboBoxMOLREF;
	CButton m_CheckBoxMOLREF;
	CButton m_CheckBoxMOL;
	CButton m_CheckBoxORCANMR;
	CButton m_CheckBoxORCAHFC;
	CButton m_CheckBoxORCACASSCF;
	CComboBox m_ComboBoxCASSCF;
	CButton m_CheckBoxORCAFC;
	CEdit m_TextBoxFC;
	CEdit m_TextBoxHFC;
	CEdit m_TextBoxSHIFT;
	void define_isotopes();
	bool try_import_spectrum_csv(std::ifstream& file);
	bool try_import_spectrum_original(std::ifstream& file, std::string& path);
	bool try_import_ORCA_MoleculeInfo(std::ifstream& file);
	bool try_import_ORCA_HFC(std::ifstream& file);
	bool try_import_ORCA_FC(std::ifstream& file);
	bool try_import_ORCA_NMR(std::ifstream& file);
	bool try_import_ORCA_ShieldingTensor(std::ifstream& file);
	bool try_import_ORCA_CASSCF(std::ifstream& file);
	bool load_procs(int& SI, double& SW_p, double& SF, double& OFFSET, std::ifstream& file);
	std::vector<int32_t> read1r(const std::string& filename, int size);
	std::string get_block_in_line(std::string line, std::string seperator, int block, bool& error);
	double string_to_double(std::string line, bool& error);
	int string_to_int(std::string line, bool& error);
	int seperate_int(std::string line, bool& error);
	double convert_MHz_to_ppm(double value, size_t index);
	double convert_FC_to_ppmK(double value);
	std::string GetPLine(std::string str);
	std::vector<std::string> m_found_element;
	std::vector<CString> m_assigned_isotope;
public:
	afx_msg void OnBnClickedCheckImportMolref();
	afx_msg void OnCbnSelchangeComboImportMolref();
	afx_msg void OnBnClickedCheckImportHfc();
	afx_msg void OnBnClickedCheckImportFc();
	afx_msg void OnBnClickedCheckImportNmr();
};
