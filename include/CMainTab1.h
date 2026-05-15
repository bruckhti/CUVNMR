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
#ifndef CMAINTAB1_H
#define CMAINTAB1_H
#include "DataBase.h"
#include "CMainTab2.h"
#include "CMainTab3.h"
#include "GraphControl.h"
#include "CDisplaySimDataDlg.h"
#include <fstream>

class CGraphControl;
class CMainTab2;

class CMainTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CMainTab1)
public:
	CMainTab1(CWnd* pParent = nullptr);
	virtual ~CMainTab1();
	void ResetTab();
	void Initialize(DataBase* dataBase);
	void Initialize_Tabs2(CMainTab2* TAB2);
	void Initialize_Tabs3(CMainTab3* TAB3);
	void Initialize_Graph(CGraphControl* Graph);
	void Initialize_SimDataDlg(CDisplaySimDataDlg* dlg);
	void UpdateLoadedData(bool shielding, bool FC, bool HFC);
	int GetFittingState();
	void UnselectComboBoxData();
	double get_temperature();
	double get_MagField();
	void resetPD();
	void tab3_sim_start();
	void GTensor_dlg_sim_start();
	void Graph_sim_start();
	void tab2_sim(bool start, bool fit);
	void OnListItemRemoveSimUpdate();
	void main_sim_stop();
	CString save_getNucIsotope();
	std::wstring save_getElement();
	char save_getCurveType();
	bool save_isSimRunning();
	void print_info(std::ofstream& file);
	void print_dorb(std::ofstream& file, std::wstring element);
	void print_dfc(std::ofstream& file, std::wstring element);
	void print_dhfc(std::ofstream& file, std::wstring element);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_TAB1 };
#endif
protected:
	DataBase* m_data_base;
	CDisplaySimDataDlg* m_SimDataDlg;
	CGraphControl* m_GraphControl;
	CMainTab2* m_pTab2;
	CMainTab3* m_pTab3;
	virtual void DoDataExchange(CDataExchange* pDX);
	CComboBox m_ComboBoxNucleus;
	CEdit m_EditTemperature;
	CEdit m_EditMagField;
	CComboBox m_ComboBoxShieldingData;
	CComboBox m_ComboBoxFCData;
	CComboBox m_ComboBoxHFCData;
	CComboBox m_ComboBoxSimFC;
	CComboBox m_ComboBoxSimPC;
	CComboBox m_ComboBoxSimSusceptibility;
	CComboBox m_ComboBoxSimCurvertype;
	CEdit m_Edit_PDAx;
	CEdit m_Edit_PDAy;
	CEdit m_Edit_PDAz;
	CComboBox m_ComboBoxRelaxationContact;
	CComboBox m_ComboBoxRelaxationDipolar;
	CComboBox m_ComboBoxRelaxationCurie;
	CButton m_SimulateButton;
	bool m_bSimulation_On;
	bool m_bActivatedByButton;
	void TurnSimOn(bool FWHMUpdate, bool ShiftUpdate, bool SimDatDlgReset, bool SimDatDlgUpdate);
	void TurnSimOff();
	void UpdateSim();
	char CurveType;
	std::wstring w_element;
	CString NucleusIsotope;
	virtual BOOL OnInitDialog();
	afx_msg void OnTemperatureEditKillfocus();
	afx_msg void OnMagfieldEditKillfocus();
	afx_msg void OnPDAxEditKillfocus();
	afx_msg void OnPDAyEditKillfocus();
	afx_msg void OnPDAzEditKillfocus();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboShieldingData();
	afx_msg void OnCbnSelchangeComboFCData();
	afx_msg void OnCbnSelchangeComboHFCData();
	afx_msg void OnCbnSelchangeComboNucleus();
	afx_msg void OnBnClickedSimulate();
	afx_msg void OnCbnSelchangeSHIFTComboBoxes();
	afx_msg void OnCbnSelchangeFWHMComboBoxes();
	afx_msg void OnBnClickedRemoveDia();
	afx_msg void OnBnClickedRemoveFermi();
	afx_msg void OnBnClickedRemoveHFC();
	DECLARE_MESSAGE_MAP()
};
#endif