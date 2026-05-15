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
class CMainTab1;
class CMainTab3;
#include "CMainTab1.h"

class CMainTab3 : public CDialogEx
{
	DECLARE_DYNAMIC(CMainTab3)
public:
	CMainTab3(CWnd* pParent = nullptr);
	virtual ~CMainTab3();
	void ResetTab();
	void Initialize_TAB1(CMainTab1* pTab1);
	double GetT1E();
	double GetT2E();
	double GetTM();
	double GetTR();
	void SetT1ESlider(double value, bool updateSim);
	void SetT2ESlider(double value, bool updateSim);
	void SetTMSlider(double value, bool updateSim);
	void SetTRSlider(double value, bool updateSim);
	void OnB0Change();
	void OnTempChange();
	void SetMoleculeSphereRadius(double radius);
	void SetMoleculeCoordinates(std::vector<std::vector<double>> xyz);
	void print_info(std::ofstream& file);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_TAB3 };
#endif
protected:
	CMainTab1* m_pTab1;
	CSliderCtrl m_SliderT1E;
	CSliderCtrl m_SliderT2E;
	CSliderCtrl m_SliderTM;
	CSliderCtrl m_SliderTR;
	CEdit m_EditValT1E;
	CEdit m_EditValT2E;
	CEdit m_EditValTM;
	CEdit m_EditValTR;
	CEdit m_EditExponentT1E;
	CEdit m_EditExponentT2E;
	CEdit m_EditExponentTM;
	CEdit m_EditExponentTR;
	CStatic m_CubicAngstrom;
	double T1E;
	double T2E;
	double TM;
	double TR;
	double m_C;
	double m_D;
	double m_omegaE;
	int ScientificNumber(double& value);
	int GetSliderPosByValue(double value, int slider_min, int slider_max, double min_E);
	std::vector<CString> m_solvent;
	std::vector<double> m_eta0;
	std::vector<double> m_eta1;
	double m_temperature;
	double m_viscosity;
	double m_sphere_radius;
	double m_mol_sphere_radius;
	double print_vol;
	CComboBox m_ComboBoxRelSolvent;
	CEdit m_EditSphereRadius;
	static double tmp_volume;
	static std::vector<std::vector<double>> m_coordinates_for_fit;
	static std::vector<std::vector<double>> m_positioned_coordinates;
	static int m_mode;
	static double ComputeSmallestSphere(std::vector<double> parameters);
	static double GroundMolecule(std::vector<double> parameters);
	static double GroundOnMolecule(std::vector<double> parameters);
	std::vector<double> ComputePlane(std::vector<double> Pj, std::vector<double> Pi, std::vector<double> Pk, double theta, double phi, double tau);
	double ComputeMissingPlaneCoordinate(int xyz_index, double x, double y, double z, std::vector<double> Plane);
	double ComputeVolumeTetraeder(std::vector<double>& Pi, std::vector<double>& Pj, std::vector<double>& Pk, std::vector<double>& Pl);
	double calculateT1E();
	double calculateT2E();
	double calculateD();
	double calculateC();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnT1EEditKillfocus();
	afx_msg void OnT2EEditKillfocus();
	afx_msg void OnTMEditKillfocus();
	afx_msg void OnTREditKillfocus();
	afx_msg void OnExpT1EEditKillfocus();
	afx_msg void OnExpT2EEditKillfocus();
	afx_msg void OnExpTMEditKillfocus();
	afx_msg void OnExpTREditKillfocus();
	afx_msg void OnCbnSelchangeSolvent();
	afx_msg void OnBnClickedSphereVol();
	afx_msg void OnBnClickedEllipsoidVol();
	DECLARE_MESSAGE_MAP()
};
