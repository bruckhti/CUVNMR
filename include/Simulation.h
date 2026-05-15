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
#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include <string> 
#include "ListSliderCtrl.h"
#include "ListButton.h"
#include "physical_constants.h"

class CSimulation : public CWnd
{
private:
	size_t ListSize;
	std::vector<std::wstring> m_listedGroupName;
	static std::vector<std::vector<int>> m_listedGroupIndices;
	std::vector<std::wstring> m_listedElement;
	static std::vector<double> m_listedExpShift;
	static std::vector<double> m_listedWeight;
	std::vector<bool> m_listedSimulate;
	std::vector<double> m_computedOrb;
	std::vector<double> m_computedHF;
	std::vector<double> m_computedFC;
	std::vector<double> m_computedPC;
	std::vector<double> m_computedTotal;
	std::vector<double> m_computedLineWidth;
	CListCtrl* m_main_list;
	std::vector<CListSliderCtrl*> m_Slider;
	std::vector<CListButton*> m_CheckBox;
	std::wstring GetListedIndicesW(int Entry);
	std::vector<std::vector<double>> m_g_tensor;
	std::vector<std::vector<double>> m_g_tensor_inverse;
	std::vector<std::vector<std::vector<double>>> m_hfc_tmp;
	static std::vector<double> m_yfit;
	static std::vector<double> m_dorb;
	static std::vector<double> m_fc;
	static std::vector<std::vector<std::vector<double>>> m_hfc;
	std::vector<std::vector<std::vector<double>>> m_hfc_inHz;
	static std::vector<std::vector<int>> m_symmetry;
	static int m_FitType;
	static double m_FitTemperature;
	static std::vector<std::vector<double>> m_R;
	double Spin;
	double MagFieldTesla;
	double SimTemperature;
	std::vector<std::vector<double>> SimChi;
	std::vector<double> RelaxationTimes;
	std::vector<std::vector<std::vector<double>>> m_shieldingTensors;
	std::vector<std::vector<double>> m_R_SI;
	std::vector<double> m_FWHM;
	std::vector<double> m_chemShift_Sim;
	std::vector<std::pair<double, double>> m_SPECTRUM;
	double j_function(double omega, double tau);
	double second_rank_invariant(std::vector<std::vector<double>>& xyz_Matrix);
	double first_rank_invariant(std::vector<std::vector<double>>& xyz_Matrix);
	double dipolar_relaxation(int type, std::string isotope, double SpinTotal, double atom_distance, double B_0, std::vector<std::vector<double>>& HFC_hz, double tau_dip_1E, double tau_dip_2E);
	double contact_relaxation(int type, std::string isotope, double SpinTotal, double B_0, std::vector<std::vector<double>>& HFC_hz, double tau_con_1E, double tau_con_2E);
	double curie_relaxation(int type, std::string isotope, double SpinTotal, double atom_distance, double B_0, double T, std::vector<std::vector<double>>& Chi, std::vector<std::vector<double>>& sigma, double tau_curie);
	double Lorentzian(double shift, double A, double B, double shift_N, double FWHM_Na, double FWHM_Nb);
	double Gaussian(double shift, double A, double B, double shift_N, double FWHM_Na, double FWHM_Nb);
	std::vector<std::vector<double>> m_chi_error;
	std::vector<std::vector<double>> m_chi_error_cgi;
	double m_chi2_quad;
	double m_SStot;
	double m_SSreg;
	double m_SSres;
	double m_R2;
	double m_R2corr;
	std::vector<int> error_list;
	std::vector<double> m_signal_error;
	double get_signal_min(double chemShift, double FWHM, char type, double P);
	double get_signal_max(double chemShift, double FWHM, char type, double P);
	std::vector<double> __CS_signal_shift;
	std::vector<double> __CS_signal_FWHM;
	std::vector<int> __CS_num_eq_atoms;
	std::vector<std::vector<std::vector<double>>> m_saveTotalShieldings;
	std::vector<std::vector<double>> m_saveR2;
	std::vector<bool> m_print_shielding;
	std::vector<double> m_save_exp_dorb;
	std::vector<double> m_save_weight;
	std::vector<std::string> m_save_elements;
	std::vector<std::string> m_save_groupNames;
public:
	void InitList(CListCtrl* main_list);
	void Reset_listed();
	void Reset_computed();
	void UpdateList();
	void AddToList(std::wstring GroupName, std::vector<int> GroupIndices, std::wstring Element, double ExpShift, double Weight, bool Simulate);
	void ModifyList_GroupName(int Entry, std::wstring GroupName);
	void ModifyList_ExpShift(int Entry, double ExpShift);
	void ModifyList_Weight(int Entry, double Weight);
	void ModifyList_simulate(int Entry, bool simulate);
	void RemoveFromList(int Entry);
	bool IndexListed(int index);
	void SetEssentials_g(std::vector<std::vector<double>>& g_tensor, bool g_init);
	void SetEssentials_dorb(std::vector<double>& d_orb);
	void SetEssentials_HFC(std::vector<std::vector<std::vector<double>>>& HFCs, std::vector<std::vector<std::vector<double>>>& HFCs_inHz);
	void SetEssentials_FC(std::vector<double> FCs);
	void setEssentials_Shielding(std::vector<std::vector<std::vector<double>>>& shieldingTensors);
	bool WeightsOK();
	void SetSymmetry(std::vector<std::vector<int>>& symmetry);
	void SetFitType(int type);
	void SetFitTemperature(double temperature);
	void SetPDDistance(std::vector<std::vector<double>>& PD_distances);
	static double RMSD(std::vector<double> chi);
	void SetSimChi(std::vector<std::vector<double>>& CHI);
	void SetRelaxationTimes(std::vector<double>& t); 
	void SetTotalSpin(double multiplicity);
	void SetMagneticField(double MagFieldMHz);
	void SetSimTemperature(double Temp);
	void ComputeFWHM(int type_contact, int type_dipolar, int type_curie, CString isotope, std::wstring element);
	void ComputeChemShift(int shift_type, std::wstring element);
	bool ComputeSpectrum(std::wstring element, double min_ppm, double max_ppm, double precision, double windowMin, double windowMax, double windowStepSize, char type);
	std::vector<std::pair<double, double>> GetSimSpec();
	double get_sim_min(std::wstring sim_element, char type, double P);
	double get_sim_max(std::wstring sim_element, char type, double P);
	double get_sim_StepSize(std::wstring element, char type, double P);
	size_t get_num_groups();
	CString get_GroupsName_byIndex(size_t index);
	std::vector<int> get_GroupsIndices_byIndex(size_t index);
	CString get_GroupsElement_byIndex(size_t index);
	std::vector<size_t> m_SimGroupIndices;
	std::vector<double> m_SimIsoOrb;
	std::vector<double> m_SimIsoFc;
	std::vector<double> m_SimIsoPcs;
	std::vector<double> m_SimFWHM;
	std::vector<double> get_fwhm();
	std::vector<double> get_SimIsoOrb();
	std::vector<double> get_SimIsoFc();
	std::vector<double> get_SimIsoPcs();
	std::vector<size_t> get_SimGroupIndices();
	std::vector<std::wstring> get_GroupsName();
	std::vector<std::vector<std::pair<double, double>>> save_CreateFullSpectrum(std::wstring element, double min_ppm, double max_ppm, double precision, char type);
	void print_TotalShieldings(std::ofstream& file);
	void print_R2(std::ofstream& file);
	void print_fitusage(std::ofstream& file);
	void set_fitusage(bool reset);
	void print_group_definition(std::ofstream& file);
	std::vector<double> get_mdorb();
	void chi_error(double temperature);
	void reset_error();
	std::vector<std::vector<double>> invert_nxnMatrix(std::vector<std::vector<double>> Matrix);
	std::vector<std::vector<double>> get_chi_error();
	std::vector<std::vector<double>> get_chi_errorSI();
	double get_chi2red();
	double get_r2();
	double get_r2adj();
	std::vector<int> get_error_list();
	std::vector<double> get_signal_error();
protected:
	DECLARE_MESSAGE_MAP()
};

#endif