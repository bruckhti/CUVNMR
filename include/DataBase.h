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
#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include <filesystem>
#include "Point.h"
#include "Camera.h"
#include "Simulation.h"
#include "CIsotopeDlg.h"
#include "physical_constants.h"

class DataBase
{
private:
	std::filesystem::path UserProfilePath;
	std::filesystem::path UserDataPath;
	std::filesystem::path UserExportPath;
	std::filesystem::path UserRootPath;
	bool b_shutdown;
	std::vector<std::vector<std::pair<double, double>>> spectra;
	std::vector<CString> spectra_names;
	std::vector<std::string> ref_element;
	std::vector<std::vector<double>> ref_xyz_coords;
	std::vector<double> ref_d_orb_iso;
	double ref_Si_shift;
	double ref_C_shift;
	double ref_H_shift;
	std::vector<CString> additional_ref_elements;
	std::vector<double> additional_ref_shifts;
	std::vector<std::string> mol_element;
	std::vector<std::vector<double>> mol_xyz_coords;
	int mol_multiplicity;
	std::vector<std::vector<double>> mol_d_orb_iso;
	std::vector<CString> mol_d_orb_names;
	std::vector<std::vector<std::vector<std::vector<double>>>> mol_ShieldingTensors;
	std::vector<std::vector<std::vector<std::vector<double>>>> mol_HFCs;
	std::vector<std::vector<std::vector<std::vector<double>>>> mol_HFCs_inHz;
	std::vector<CString> mol_HFCs_names;
	std::vector<std::vector<double>> mol_FCs;
	std::vector<CString> mol_FCs_names;
	std::vector<std::vector<std::vector<double>>> mol_CHI;
	std::vector<double> mol_CHI_temp;
	std::vector<std::vector<double>> mol_g_tensor;
	std::vector<CString> NMR_active_nucleus;
	std::vector<double> NMR_nucleus_spin;
	std::vector<std::vector<double>> mol_CHI_fitted; // in A^3
	std::vector<double> PD_positions;
	std::vector<Point> m_points;
	std::vector<PointName*> m_point_names;
	PointName* getPointNameCreate(std::wstring name);
	std::vector<PointConnection*> m_connections;
	std::vector<std::vector<PointConnection*>> m_conn_matrix;
	float m_default_conn_threshold;
	float m_cylinder_radius;
	Camera m_camera;
	glm::vec3 m_background_color;
	bool GetUserPath();
	void set_sim_g_tensor(bool g_init);
public:
	DataBase();
	~DataBase();
	void Init();
	bool EmergencyShutdown();
	void update_reference();
	void ResetProgram();
	std::string get_UserRootPath();
	void remove_spectra();
	void set_spectrum(std::vector<std::pair<double, double>>& spectrum, CString name);
	void remove_spectrum(size_t Entry);
	size_t GetSpectraCount();
	CString GetSpectrumName(size_t index);
	std::vector<std::pair<double, double>> GetSpectrum(size_t index);
	void set_reference(std::vector<std::string>& elements, std::vector<std::vector<double>>& xyz, std::vector<double>& d_orb);
	void set_additional_reference(CString element, double d_orb);
	double get_reference_by_isotope(CString isotope);
	void reset_molecule();
	bool compare_molecules(std::vector<std::string>& elements);
	void set_molecule(std::vector<std::string>& elements, std::vector<std::vector<double>>& xyz, int multiplicity);
	void set_mol_HFCs(std::vector<std::vector<std::vector<double>>>& HFC, std::vector<std::vector<std::vector<double>>>& HFC_inHz, CString name);
	void set_mol_FC(std::vector<double>& FC, CString name);
	void set_mol_d_orb(std::vector<double>& d_orb, CString name);
	void set_mol_shielding(std::vector<std::vector<std::vector<double>>>& shieldings);
	bool is_molecule_set();
	void set_CHI(std::vector<std::vector<std::vector<double>>>& CHI, std::vector<double>& CHI_temp);
	size_t get_mol_atom_count();
	CString get_mol_element_by_index(size_t index);
	int get_mol_multiplicity();
	double get_mol_coordinate(size_t index, size_t xyz);
	size_t get_mol_d_orb_DataSize();
	size_t get_mol_FC_DataSize();
	size_t get_mol_HFC_DataSize();
	CString get_mol_d_orb_DataName(size_t index);
	CString get_mol_HFC_DataName(size_t index);
	CString get_mol_FC_DataName(size_t index);
	size_t get_mol_chi_size();
	double get_mol_chi_Temp(size_t index);
	std::vector<std::vector<double>> get_mol_CHI(size_t index);
	std::vector<std::vector<double>> get_g_tensor();
	void set_g_tensor(std::vector<std::vector<double>> new_g_tensor);
	std::vector<double> get_mol_d_orb(size_t index);
	std::vector<double> get_mol_d_orb_raw(size_t index);
	std::vector<double> get_mol_fc(size_t index);
	std::vector<std::vector<std::vector<double>>> get_mol_hfc(size_t index);
	std::vector<std::vector<std::vector<double>>> get_mol_hfc_inHz(size_t index);
	std::vector<std::vector<std::vector<double>>> get_mol_shieldingTensors(size_t index);
	void remove_diamag(size_t index);
	void remove_hfc(size_t index);
	void remove_fc(size_t index);
	void delete_Shieldings();
	double get_mol_radius();
	void create_empty_HFC();
	void InitSimulation(CListCtrl* main_list);
	void SetDefaultViewProperties();
	void CreateNewPoint(std::wstring name, std::vector<int> RGB, float size);
	size_t GetNMRnucleiSize();
	CString GetNMRnuclei(size_t index);
	int IsNMRactiveIsotope(CString element);
	std::vector<CString> GetIsotopesOfType(CString element);
	CString GetAdditionalRefElements(size_t index);
	void set_fitted_Chi(std::vector<std::vector<double>>& CHI);
	std::vector<std::vector<double>> get_fitted_CHI();
	std::vector<std::vector<double>> CalculatePDDistances();
	void setPDPositions(double x, double y, double z);
	CString StoCS(std::string str);
	std::string CStoS(CString str);
	double convertCHItoA3(double CHI_CGI, double temp);
	double convertCHItoCGI(double CHI_A3, double temp);
	size_t getNumPoints();
	Point* getPoint(int index);
	void UpdatePoints(bool reset);
	size_t getNumPointNames();
	PointName* getPointName(int index);
	PointName* getPointName(std::wstring str);
	PointConnection* getConnection(size_t indexA, size_t indexB);
	PointConnection* getConnection(std::wstring strA, std::wstring strB);
	void setBackgroundColor(glm::vec3 col);
	glm::vec3 getBackgroundColor();
	float getCylinderRadius();
	void setCylinderRadius(float val);
	Camera* getCamera();
	void saveViewerProperties();
	void loadDefaultViewerProperties();
	bool loadViewerProperties();
	bool loadCustomAdditionalReferences();
	void saveCustomAdditionalReferences();
	void overridePoint(std::wstring name, std::vector<int> RGB, float radius);
	CSimulation m_simulation;
	void RotatePoints(float alpha, float beta);
	CString GetUserDataPath();
	CString GetUserExportPath();
};

#endif