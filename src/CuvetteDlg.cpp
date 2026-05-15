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
#include "framework.h"
#include "CUVNMR.h"
#include "CuvetteDlg.h"
#include "afxdialogex.h"
#include <string>
#include "ImportDlg.h"
#include "DataBase.h"
#include "CEditAddOrb.h"
#include "iomanip"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_EDIT_CONTROL_LIST 1

CCuvetteDlg::CCuvetteDlg(DataBase* dataBase, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CUVETTE_DIALOG, pParent)
{
	m_data_base = dataBase;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bOpenGLWindowsExists = false;
	m_SimDataDlg.Create(IDD_DIALOG_DISPLAY_SIM_DATA, this);
	m_StatErrorDlg.Create(IDD_DIALOG_STATERROR, this);
}

void CCuvetteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEWSMALL, m_PictCtrl);
	DDX_Control(pDX, IDC_LIST, g_main_list);
	DDX_Control(pDX, IDC_LIST1, g_header_list);
	m_data_base->InitSimulation(&g_main_list);
}

BEGIN_MESSAGE_MAP(CCuvetteDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_IMPORT, &CCuvetteDlg::OnFileImport)
	ON_COMMAND(ID_EDIT_EDITREFERENCESHIFTS, OnEditReferenceShifts)
	ON_COMMAND(ID_EDIT_EDITORBITALSHIFTS, &CCuvetteDlg::OnEditdOrb)
	ON_COMMAND(ID_EDIT_DISPLAYSIMULATIONDATA, &CCuvetteDlg::ShowSimDataDlg)
	ON_COMMAND(ID_TOOLS_DISPLAYFITSTATISTICS, &CCuvetteDlg::ShowStatErrorDlg)
	ON_COMMAND(ID_EDIT_CREATEENSEMBLE, &CCuvetteDlg::OnCreateEnsemble)
	ON_COMMAND(ID_TOOLS_CREATEEMPTYHFCDATA, &CCuvetteDlg::OnCreateEmptyHFC)
	ON_COMMAND(ID_TOOLS_EDITGTENSOR, &CCuvetteDlg::OnEditGTensor)
	ON_CBN_SELCHANGE(IDC_COMBO_REFERENCE, &CCuvetteDlg::OnCbnSelchangeComboReference)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAINTAB, &CCuvetteDlg::OnTcnSelchangeMaintab)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CCuvetteDlg::OnNMDblclkList)
	ON_NOTIFY(HDN_BEGINTRACK, 0, &CCuvetteDlg::OnHdnBegintrack)
	ON_BN_CLICKED(IDC_BUTTON_EXPSPC_REMOVE, &CCuvetteDlg::OnBnClickedRemoveExpSpc)
	ON_BN_CLICKED(IDC_BUTTON_EXPSPC_HIDE, &CCuvetteDlg::OnBnClickedHideExpSpc)
	ON_COMMAND(ID_FILE_NEW, &CCuvetteDlg::OnFileNew)
	ON_COMMAND(ID_FILE_EXIT, &CCuvetteDlg::OnFileExit)
	ON_COMMAND(ID_VERSION_ABOUT, &CCuvetteDlg::OnVersionAbout)
	ON_COMMAND(ID_FILE_EXPORT, &CCuvetteDlg::OnExport)
END_MESSAGE_MAP()

BOOL CCuvetteDlg::OnInitDialog()
{
	if (m_data_base->EmergencyShutdown()) { EndDialog(IDOK); }
	CDialogEx::OnInitDialog();
	ModifyStyle(0, WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_TabCtrl_Main.SubclassDlgItem(IDC_MAINTAB, this);
	CTabCtrl* pTabCtrl1 = (CTabCtrl*)GetDlgItem(IDC_MAINTAB);
	m_tab1.Create(IDD_MAIN_TAB1, pTabCtrl1);
	m_tab1.Initialize(m_data_base);
	CTabCtrl* pTabCtrl2 = (CTabCtrl*)GetDlgItem(IDC_MAINTAB);
	m_tab2.Create(IDD_MAIN_TAB2, pTabCtrl2);
	m_tab2.Initialize(m_data_base);
	m_tab2.Initialize_Tabs(&m_tab1);
	m_tab1.Initialize_Tabs2(&m_tab2);
	CTabCtrl* pTabCtrl3 = (CTabCtrl*)GetDlgItem(IDC_MAINTAB);
	m_tab3.Create(IDD_MAIN_TAB3, pTabCtrl3);
	m_tab3.Initialize_TAB1(&m_tab1);
	m_tab1.Initialize_Tabs3(&m_tab3);
	TCITEM item1, item2, item3;
	item1.mask = TCIF_TEXT | TCIF_PARAM;
	item1.lParam = (LPARAM)&m_tab1;
	item1.pszText = _T("Home");
	pTabCtrl1->InsertItem(0, &item1);
	item2.mask = TCIF_TEXT | TCIF_PARAM;
	item2.lParam = (LPARAM)&m_tab2;
	item2.pszText = _T("Susceptibility");
	pTabCtrl2->InsertItem(1, &item2);
	item3.mask = TCIF_TEXT | TCIF_PARAM;
	item3.lParam = (LPARAM)&m_tab3;
	item3.pszText = _T("Relaxation");
	pTabCtrl3->InsertItem(2, &item3);
	CRect rcItem1;
	pTabCtrl1->GetItemRect(0, &rcItem1);
	m_tab1.SetWindowPos(NULL, rcItem1.left, rcItem1.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	CRect rcItem2;
	pTabCtrl2->GetItemRect(0, &rcItem2);
	m_tab2.SetWindowPos(NULL, rcItem2.left, rcItem2.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	CRect rcItem3;
	pTabCtrl3->GetItemRect(0, &rcItem3);
	m_tab3.SetWindowPos(NULL, rcItem3.left, rcItem3.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_tab1.ShowWindow(SW_SHOW);
	m_tab2.ShowWindow(SW_HIDE);
	m_tab3.ShowWindow(SW_HIDE);
	m_GraphControl.SubclassDlgItem(IDC_GRAPH_PANEL, this);
	m_GraphControl.Init();
	m_tab1.Initialize_Graph(&m_GraphControl);
	m_tab2.Initialize_Graph(&m_GraphControl);
	m_GraphControl.InitializeTab1(&m_tab1);
	m_CheckBoxGraph1.SubclassDlgItem(IDC_CHECK_GRAPH1, this);
	m_CheckBoxGraph2.SubclassDlgItem(IDC_CHECK_GRAPH2, this);
	m_ComboBoxReferenceSpectrum.SubclassDlgItem(IDC_COMBO_REFERENCE, this);
	{
		CRect combo_rc;
		m_ComboBoxReferenceSpectrum.GetWindowRect(&combo_rc);
		m_ComboBoxReferenceSpectrum.SetWindowPos(NULL, 0, 0, combo_rc.Width(), combo_rc.Height() + (5 * m_ComboBoxReferenceSpectrum.GetItemHeight(-1)), SWP_NOMOVE | SWP_NOZORDER);
	}
	StartOpenGL();
	g_main_list.SetExtendedStyle(g_main_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | HDS_NOSIZING);
	g_main_list.InsertColumn(0, L"Group Name", LVCFMT_LEFT, 80);
	g_main_list.InsertColumn(1, L"Group Indices", LVCFMT_LEFT, 100);
	g_main_list.InsertColumn(2, L"Element", LVCFMT_LEFT, 55);
	g_main_list.InsertColumn(3, L"exp. Shift (ppm)", LVCFMT_LEFT, 100);
	g_main_list.InsertColumn(4, L"Weight (0-1)", LVCFMT_LEFT, 100);
	g_main_list.InsertColumn(5, L"Simulate", LVCFMT_LEFT, 60);
	g_main_list.ModifyStyle(0, LVS_NOCOLUMNHEADER);
	g_header_list.InsertColumn(0, L"Group Name", LVCFMT_LEFT, 81);
	g_header_list.InsertColumn(1, L"Group Indices", LVCFMT_LEFT, 100);
	g_header_list.InsertColumn(2, L"Element", LVCFMT_LEFT, 55);
	g_header_list.InsertColumn(3, L"exp. Shift (ppm)", LVCFMT_LEFT, 100);
	g_header_list.InsertColumn(4, L"Weight (0-1)", LVCFMT_LEFT, 100);
	g_header_list.InsertColumn(5, L"Simulate", LVCFMT_LEFT, 60);
	m_tab1.Initialize_SimDataDlg(&m_SimDataDlg);
	m_tab2.Initialize_StatErrorDlg(&m_StatErrorDlg);
	return TRUE;
}

void CCuvetteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CCuvetteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CCuvetteDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		CPoint pt(pMsg->pt.x, pMsg->pt.y);
		if (m_GraphControl.GetSafeHwnd() != NULL)
		{
			m_GraphControl.SendMessage(WM_MOUSEWHEEL, pMsg->wParam, MAKELPARAM(pt.x, pt.y));
		}
	}
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		CPoint pt(pMsg->pt.x, pMsg->pt.y);
		if (m_GraphControl.GetSafeHwnd() != NULL)
		{
			m_GraphControl.SendMessage(WM_LBUTTONDOWN, pMsg->wParam, MAKELPARAM(pt.x, pt.y));
		}
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (g_edit_list.GetSafeHwnd())
			{
				if (g_edit_list.IsWindowVisible())
				{
					KillEditControl();
					g_edit_list.DestroyWindow();
				}	
			}
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
		{
			int nSelectedItem = g_main_list.GetNextItem(-1, LVNI_SELECTED);
			if (nSelectedItem != -1)
			{
				m_data_base->m_simulation.RemoveFromList(nSelectedItem);
			}
			m_tab1.OnListItemRemoveSimUpdate();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CCuvetteDlg::OnFileImport()
{
	bool stop_loading = false;
	const TCHAR szFilter[] = _T("ORCA Files (.out)|*.out|Bruker NMR dataset|procs|CSV Files (.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("csv"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, this);
	CString PathForImport = m_data_base->GetUserDataPath();
	dlg.m_ofn.lpstrInitialDir = PathForImport;
	if (dlg.DoModal() == IDOK)
	{
		CString csFilePath = dlg.GetPathName();  
		std::wstring wstrFilePath(csFilePath);
		std::string strFilePath(wstrFilePath.begin(), wstrFilePath.end());
		CImportDlg ImportDlg(m_data_base);
		ImportDlg.ImportFile(strFilePath);
		INT_PTR nResponse = ImportDlg.DoModal();
		if (nResponse == IDOK)
		{
			if (ImportDlg.m_bCheckBoxREFSPC_State == TRUE)
			{
				m_data_base->set_spectrum(ImportDlg.spc, ImportDlg.REFSPC_name);
				UpdateReferenceComboBox();
			}
			if(ImportDlg.MOL == TRUE)
			{
				if (ImportDlg.m_bCheckBoxMOL_State == TRUE)
				{
					if (m_data_base->compare_molecules(ImportDlg.molekel.element))
					{
						m_tab1.resetPD();
						m_data_base->set_molecule(ImportDlg.molekel.element, ImportDlg.molekel.xyz_coords, ImportDlg.molekel.multiplicity);
						m_tab3.SetMoleculeSphereRadius(m_data_base->get_mol_radius());
						m_tab3.SetMoleculeCoordinates(ImportDlg.molekel.xyz_coords);
					}
					else
					{
						AfxMessageBox(_T("The imported molecule composition does not match the already loaded molecule"), MB_ICONWARNING);
						stop_loading = true;
						int response = AfxMessageBox(_T("Do you want to reset all molecule informations? This will delete all diamagn. shifts, HFCs, FCs and overwrites the molecule topology."), MB_YESNO);
						if (response == IDYES)
						{
							stop_loading = false;
							m_tab1.resetPD();
							m_data_base->reset_molecule();
							m_data_base->set_molecule(ImportDlg.molekel.element, ImportDlg.molekel.xyz_coords, ImportDlg.molekel.multiplicity);
							m_tab3.SetMoleculeSphereRadius(m_data_base->get_mol_radius());
							m_tab3.SetMoleculeCoordinates(ImportDlg.molekel.xyz_coords);
						}
					}
				}
				if (ImportDlg.m_bCheckBoxORCANMR_State == TRUE || ImportDlg.m_bCheckBoxORCAHFC_State == TRUE || ImportDlg.m_bCheckBoxORCAFC_State == TRUE)
				{
					if (!m_data_base->compare_molecules(ImportDlg.molekel.element))
					{
						AfxMessageBox(_T("The imported molecule composition does not match the already loaded molecule"), MB_ICONWARNING);
						stop_loading = true;
						int response = AfxMessageBox(_T("Do you want to reset all molecule informations before importing? This will delete all diamagn. shifts, HFCs, FCs."), MB_YESNO);
						if (response == IDYES)
						{
							stop_loading = false;
							m_tab1.resetPD();
							m_data_base->reset_molecule();
							m_data_base->set_molecule(ImportDlg.molekel.element, ImportDlg.molekel.xyz_coords, ImportDlg.molekel.multiplicity);
							m_tab3.SetMoleculeSphereRadius(m_data_base->get_mol_radius());
							m_tab3.SetMoleculeCoordinates(ImportDlg.molekel.xyz_coords);
						}
					}
					else if (!m_data_base->is_molecule_set())
					{
						m_tab1.resetPD();
						AfxMessageBox(_T("A molecule composition is missing but needed and will be imported from the current file."), MB_ICONWARNING);
						m_data_base->set_molecule(ImportDlg.molekel.element, ImportDlg.molekel.xyz_coords, ImportDlg.molekel.multiplicity);
						m_tab3.SetMoleculeSphereRadius(m_data_base->get_mol_radius());
						m_tab3.SetMoleculeCoordinates(ImportDlg.molekel.xyz_coords);
					}

					if (ImportDlg.m_bCheckBoxORCANMR_State == TRUE && !stop_loading)
					{
						m_data_base->set_mol_d_orb(ImportDlg.molekel.d_orb_iso, ImportDlg.NMR_name);
						m_data_base->set_mol_shielding(ImportDlg.molekel.ShieldingTensors);
					}
					if (ImportDlg.m_bCheckBoxORCAHFC_State == TRUE && !stop_loading)
					{
						m_data_base->set_mol_HFCs(ImportDlg.molekel.HFCs, ImportDlg.molekel.HFCs_inHz, ImportDlg.HFC_name);
					}
					if (ImportDlg.m_bCheckBoxORCAFC_State == TRUE && !stop_loading)
					{
						m_data_base->set_mol_FC(ImportDlg.molekel.FCs, ImportDlg.FC_name);
					}
				}
				if (ImportDlg.m_bCheckBoxORCACASSCF_State == TRUE)
				{
					if (ImportDlg.CASSCF_name == _T("") || ImportDlg.CASSCF_name == _T("ERROR"))
					{
						AfxMessageBox(_T("Computed susceptibility type was not set (CASSCF or NEVPT2) and will not be imported."), MB_ICONWARNING);
					}
					else if (ImportDlg.CASSCF_name == _T("CASSCF"))
					{
						m_data_base->set_CHI(ImportDlg.molekel.CHI_CASSCF, ImportDlg.molekel.CHI_CASSCF_temp);
						m_tab2.RefreshCompChi();
					}
					else if (ImportDlg.CASSCF_name == _T("NEVPT2"))
					{
						m_data_base->set_CHI(ImportDlg.molekel.CHI_NEVPT2, ImportDlg.molekel.CHI_NEVPT2_temp);
						m_tab2.RefreshCompChi();
					}
				}
			}
			else if (ImportDlg.REF == TRUE)
			{
				int num_atoms = static_cast<int>(ImportDlg.molekel.element.size());
				int num_Si = 0;
				int num_C = 0;
				int num_H = 0;
				for (auto& element : ImportDlg.molekel.element)
				{
					if (element == "Si")
					{
						num_Si++;
					}
					else if (element == "C")
					{
						num_C++;
					}
					else if (element == "H")
					{
						num_H++;
					}
				}
				if (num_atoms == 17 && num_Si == 1 && num_C == 4 && num_H == 12)
				{
					m_data_base->set_reference(ImportDlg.molekel.element, ImportDlg.molekel.xyz_coords, ImportDlg.molekel.d_orb_iso);
					m_tab1.UnselectComboBoxData();
				}
				else
				{
					AfxMessageBox(_T("Unable to load reference (TMS) shieldings!\nMolecule composition differs from TMS composition!"), MB_ICONWARNING);
				}
			}
		}
		else if (nResponse == IDCANCEL)
		{
		}
		m_tab1.UpdateLoadedData(true, true, true);
		m_tab1.tab2_sim(false, false);
		Invalidate();
	}
}

void CCuvetteDlg::UpdateReferenceComboBox()
{
	m_ComboBoxReferenceSpectrum.ResetContent();
	for (size_t index = 0; index < m_data_base->GetSpectraCount(); index++)
	{
		m_ComboBoxReferenceSpectrum.AddString(m_data_base->GetSpectrumName(index));
	}
}

void CCuvetteDlg::OnCbnSelchangeComboReference()
{
	int selectedIndex = m_ComboBoxReferenceSpectrum.GetCurSel();
	if (selectedIndex != CB_ERR)
	{
		m_GraphControl.load_graph(m_data_base->GetSpectrum(selectedIndex), false);
	}
}

void CCuvetteDlg::OnTcnSelchangeMaintab(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	int iSel = m_TabCtrl_Main.GetCurSel();
	if (iSel == 0)
	{
		m_tab1.ShowWindow(SW_SHOW);
		m_tab2.ShowWindow(SW_HIDE);
		m_tab3.ShowWindow(SW_HIDE);
	}
	else if (iSel == 1)
	{
		m_tab1.ShowWindow(SW_HIDE);
		m_tab2.ShowWindow(SW_SHOW);
		m_tab3.ShowWindow(SW_HIDE);
	}
	else if (iSel == 2)
	{
		m_tab1.ShowWindow(SW_HIDE);
		m_tab2.ShowWindow(SW_HIDE);
		m_tab3.ShowWindow(SW_SHOW);
	}
	else
	{
		m_tab1.ShowWindow(SW_SHOW);
		m_tab2.ShowWindow(SW_HIDE);
		m_tab3.ShowWindow(SW_HIDE);
	}
}


void CCuvetteDlg::OnHdnBegintrack(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = TRUE;
}

void CCuvetteDlg::OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pItem = (LPNMITEMACTIVATE)pNMHDR;
	if (g_edit_list.GetSafeHwnd())
	{
		if (g_edit_list.IsWindowVisible())
		{
			KillEditControl();
			g_edit_list.DestroyWindow();
		}
	}
	if (pItem->iSubItem == 0 && g_main_list.GetItemCount() != 0)
	{
		CRect rect;
		g_main_list.GetSubItemRect(pItem->iItem, pItem->iSubItem, LVIR_LABEL, rect);
		int columnWidth = g_main_list.GetColumnWidth(pItem->iSubItem);
		rect.right = rect.left + columnWidth;
		CString currentValue = g_main_list.GetItemText(pItem->iItem, 0);
		if (!g_edit_list.GetSafeHwnd())
		{
			g_edit_list.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER, rect, &g_main_list, IDC_EDIT_CONTROL_LIST);
		}
		g_edit_list.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
		g_edit_list.SetWindowTextW(currentValue);
		g_edit_list.SetFocus();
		g_edit_list.SetSel(0, -1);
		g_edit_list.SetDlgCtrlID(1);
		g_edit_list.ShowWindow(SW_SHOW);
		g_lastRow_list = pItem->iItem;
		g_lastCol_list = pItem->iSubItem;
	}
	else if (pItem->iSubItem == 3 && g_main_list.GetItemCount() != 0)
	{
		CRect rect;
		g_main_list.GetSubItemRect(pItem->iItem, pItem->iSubItem, LVIR_LABEL, rect);
		int columnWidth = g_main_list.GetColumnWidth(pItem->iSubItem);
		rect.right = rect.left + columnWidth;
		CString currentValue = g_main_list.GetItemText(pItem->iItem, 3);
		if (!g_edit_list.GetSafeHwnd())
		{
			g_edit_list.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER, rect, &g_main_list, IDC_EDIT_CONTROL_LIST);
		}
		g_edit_list.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
		g_edit_list.SetWindowTextW(currentValue);
		g_edit_list.SetFocus();
		g_edit_list.SetSel(0, -1);
		g_edit_list.SetDlgCtrlID(1);
		g_edit_list.ShowWindow(SW_SHOW);
		g_lastRow_list = pItem->iItem;
		g_lastCol_list = pItem->iSubItem;
	}
	*pResult = 0;
}

void CCuvetteDlg::KillEditControl()
{
	if (g_lastCol_list == 0)
	{
		CString newValue;
		g_edit_list.GetWindowText(newValue);
		int nRow = g_lastRow_list;
		if (nRow != -1)
		{
			g_main_list.SetItemText(nRow, 0, newValue);
			m_data_base->m_simulation.ModifyList_GroupName(nRow, newValue.GetString());
		}
		g_edit_list.ShowWindow(SW_HIDE);
	}
	else if (g_lastCol_list == 3)
	{
		CString newValue;
		g_edit_list.GetWindowText(newValue);
		double doubleValue = _tstof(newValue);
		int nRow = g_lastRow_list;
		CString formattedValue;
		formattedValue.Format(_T("%.2f"), doubleValue);
		if (nRow != -1)
		{
			g_main_list.SetItemText(nRow, 3, formattedValue);
			m_data_base->m_simulation.ModifyList_ExpShift(nRow, doubleValue);
		}
		g_edit_list.ShowWindow(SW_HIDE);
	}
}

void CCuvetteDlg::StartOpenGL()
{
	CRect rect(19, 42, 1218 / 2, 668 / 2);
	m_PictCtrl.GetWindowRect(rect);
	ScreenToClient(rect);
	if (bOpenGLWindowsExists)
	{
		m_OGL_Window.DestroyScene();
		bOpenGLWindowsExists = false;
	}
	if (!bOpenGLWindowsExists)
	{
		m_OGL_Window.attach_data_base(m_data_base);
		m_OGL_Window.CreateGLContext(rect, (LPCWSTR)"coco1", this);
		m_OGL_Window.PrepareScene();
		m_OGL_Window.setPrimary();
		m_OGL_Window.setList(&g_main_list);
		bOpenGLWindowsExists = true;
	}
}

void CCuvetteDlg::OnBnClickedHideExpSpc()
{
	if (m_ComboBoxReferenceSpectrum.GetCurSel() != -1)
	{
		m_GraphControl.load_graph(std::vector<std::pair<double, double>>(), false);
		m_ComboBoxReferenceSpectrum.SetCurSel(-1);
	}
}
void CCuvetteDlg::OnBnClickedRemoveExpSpc()
{
	size_t selection = m_ComboBoxReferenceSpectrum.GetCurSel();
	if (selection != -1)
	{
		m_GraphControl.load_graph(std::vector<std::pair<double, double>>(), false);
		m_data_base->remove_spectrum(selection);
		m_ComboBoxReferenceSpectrum.DeleteString(selection);
		m_ComboBoxReferenceSpectrum.SetCurSel(-1);
	}
}

void CCuvetteDlg::OnFileNew()
{
	int result = AfxMessageBox(_T("Are you sure you want to create a new window? Any non-exported progress will be lost."), MB_YESNO | MB_ICONQUESTION);
	if (result == IDYES)
	{
		m_CheckBoxGraph1.SetCheck(FALSE);
		m_CheckBoxGraph2.SetCheck(FALSE);
		m_ComboBoxReferenceSpectrum.SetCurSel(-1);
		m_ComboBoxReferenceSpectrum.ResetContent();
		m_data_base->remove_spectra();
		m_tab1.main_sim_stop();
		m_data_base->ResetProgram();
		m_tab1.ResetTab();
		m_tab2.ResetTab();
		m_tab3.ResetTab();
		m_GraphControl.ResetGraph();
		m_StatErrorDlg.UpdateErrors({}, { {0.0, 0.0, 0.0}, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 } }, {}, 0.0, 0.0, 0.0, {}, 0.0, {}, 0.0);
		m_StatErrorDlg.UpdateErrorsSI({ {0.0, 0.0, 0.0}, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 } }, {}, 0.0, 0.0);
		Invalidate();
		UpdateWindow();
	}
	else if (result == IDNO)
	{
		
	}
}

void CCuvetteDlg::OnFileExit()
{
	EndDialog(IDOK);
}

void CCuvetteDlg::OnVersionAbout()
{
	CAbout AboutDlg;
	AboutDlg.DoModal();
}

void CCuvetteDlg::OnEditReferenceShifts()
{
	CEditRefDlg EditRefDlg(m_data_base);
	EditRefDlg.DoModal();
	m_tab1.UpdateLoadedData(true, false, false);
	m_tab1.tab2_sim(false, false);
	Invalidate();
}

void CCuvetteDlg::OnEditGTensor()
{
	CGTensor EditGTensor(m_data_base, &m_tab1);
	EditGTensor.DoModal();
}

void CCuvetteDlg::OnEditdOrb()
{
	INT_PTR nResponse;
	CEditAddOrb EditAddOrbDlg(m_data_base);
	nResponse = EditAddOrbDlg.DoModal();
	if (nResponse == IDOK)
	{
		size_t SizeOfOldData = m_data_base->get_mol_d_orb_DataSize();
		m_data_base->delete_Shieldings();
		for (size_t i = 0; i < EditAddOrbDlg.sizet(); i++)
		{
			m_data_base->set_mol_d_orb(EditAddOrbDlg.get_new_sigma_iso(i), EditAddOrbDlg.get_new_Name(i));
			m_data_base->set_mol_shielding(EditAddOrbDlg.get_new_shielding_tensors(i));
			if (i >= SizeOfOldData)
			{
				std::string filename = "exports/" + m_data_base->CStoS(m_data_base->get_mol_d_orb_DataName(i)) + ".shieldings.txt";
				std::ofstream shielding_export(filename);
				if (shielding_export.is_open())
				{
					shielding_export << std::fixed << std::setprecision(12);
					shielding_export << "$Geometry" << std::endl;
					shielding_export << "   &CartesianCoordinates [&Type \"Coordinates\", &Dim(" << m_data_base->get_mol_atom_count() << ",4), & Units \"Angstrom\"]" << std::endl;
					for (size_t atom = 0; atom < m_data_base->get_mol_atom_count(); atom++)
					{
						shielding_export << std::setw(16) << std::setfill(' ') << m_data_base->CStoS(m_data_base->get_mol_element_by_index(atom));
						shielding_export << std::setw(20) << std::setfill(' ') << m_data_base->get_mol_coordinate(atom, 0);
						shielding_export << std::setw(20) << std::setfill(' ') << m_data_base->get_mol_coordinate(atom, 1);
						shielding_export << std::setw(20) << std::setfill(' ') << m_data_base->get_mol_coordinate(atom, 2) << std::endl;
					}
					shielding_export << "$End" << std::endl;
					shielding_export << "$Calculation_Info" << std::endl;
					shielding_export << "   &MULT [&Type \"Integer\"] " << m_data_base->get_mol_multiplicity() << std::endl;
					shielding_export << "$End" << std::endl;
					shielding_export << "$created_Chemical_Shift" << std::endl;
					shielding_export << std::scientific << std::setprecision(16);
					std::vector<std::vector<std::vector<double>>> shielding_for_print = EditAddOrbDlg.get_new_shielding_tensors(i);
					for (size_t nuc = 0; nuc < shielding_for_print.size(); nuc++)
					{
						shielding_export << "   &NUC [&Type \"Integer\"] " << nuc << " \"Index of the nuclei\"" << std::endl;
						shielding_export << "   &STOT [&Type \"ArrayOfDoubles\", &Dim (3,3)] \"Total tensor\"" << std::endl;
						shielding_export << std::endl << std::endl;
						for (size_t tensor_row = 0; tensor_row < 3; tensor_row++)
						{
							shielding_export << tensor_row << "          ";
							for (size_t tensor_col = 0; tensor_col < 3; tensor_col++)
							{
								shielding_export << std::setw(32) << std::setfill(' ') << shielding_for_print[nuc][tensor_row][tensor_col];
							}
							shielding_export << std::endl;
						}
					}
					shielding_export << "$End" << std::endl;
				}
				shielding_export.close();
			}
		}
		m_tab1.UpdateLoadedData(true, false, false);
		m_tab1.tab2_sim(false, false);
		Invalidate();
	}
}

void CCuvetteDlg::ShowSimDataDlg()
{
	m_SimDataDlg.ShowWindow(SW_SHOW);
}

void CCuvetteDlg::ShowStatErrorDlg()
{
	m_StatErrorDlg.ShowWindow(SW_SHOW);
}

void CCuvetteDlg::OnCreateEnsemble()
{
	CEnsembleDlg CreateEnsembleDlg(m_data_base);
	CreateEnsembleDlg.DoModal();
	if (m_data_base->is_molecule_set())
	{
		if (CreateEnsembleDlg.IsHFC() || CreateEnsembleDlg.IsShielding())
		{
			if (m_data_base->compare_molecules(CreateEnsembleDlg.get_molecule()))
			{
				if (CreateEnsembleDlg.IsHFC())
				{
					m_data_base->set_mol_HFCs(CreateEnsembleDlg.get_hfc(), CreateEnsembleDlg.get_hfc_hz(), _T("ensemble"));
				}
				if (CreateEnsembleDlg.IsShielding())
				{
					m_data_base->set_mol_d_orb(CreateEnsembleDlg.get_simga_iso(), _T("ensemble"));
					m_data_base->set_mol_shielding(CreateEnsembleDlg.get_shielding());
				}
				m_tab1.UpdateLoadedData(CreateEnsembleDlg.IsShielding(), false, CreateEnsembleDlg.IsHFC());
				m_tab1.tab2_sim(false, false);
				Invalidate();
			}
			else
			{
				AfxMessageBox(_T("molecule composition incompatible"), MB_ICONERROR);
			}
		}
	}
	else
	{
		AfxMessageBox(_T("A molecule composition needs to be imported first."), MB_ICONWARNING);
	}
}


void CCuvetteDlg::OnCreateEmptyHFC()
{
	if (m_data_base->is_molecule_set())
	{
		m_data_base->create_empty_HFC();
		m_tab1.UpdateLoadedData(false, false, true);
		m_tab1.tab2_sim(false, false);
		Invalidate();
	}
	else
	{
		AfxMessageBox(_T("A molecule composition needs to be imported first."), MB_ICONWARNING);
	}
}



void CCuvetteDlg::OnExport()
{
	const TCHAR szFilter[] = _T("cuvette output (.cout)|*.cout||");
	CFileDialog dlg(FALSE, _T("cout"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString PathForImport = m_data_base->GetUserExportPath();
	dlg.m_ofn.lpstrInitialDir = PathForImport;
	if (dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		if (sFilePath.Right(5).CompareNoCase(_T(".cout")) == 0)
		{
			sFilePath = sFilePath.Left(sFilePath.GetLength() - 5);
		}
		std::ofstream xport_info(sFilePath + _T(".info.cout"));
		CString sPath_indiSus = sFilePath + _T(".indSus.cout");
		CString sPath_compSus = sFilePath + _T(".compSus.cout");
		CString sPath_fitSus = sFilePath + _T(".fitSus.cout");
		CString sPath_xyz = sFilePath + _T(".coordinates.xyz");
		CString sPath_groupDefine = sFilePath + _T(".groupDefinition.cout");
		CString sPath_error = sFilePath + _T(".errors.cout");
		if (xport_info.is_open())
		{
			char c_CT;
			std::wstring w_E;
			CString cs_NI;
			bool sim_exported = false;
			if (m_tab1.save_isSimRunning())
			{
				c_CT = m_tab1.save_getCurveType();
				w_E = m_tab1.save_getElement();
				cs_NI = m_tab1.save_getNucIsotope();
				CString sPath_FullSpectrum = sFilePath + _T(".") + cs_NI + _T("_fullspc.cout");
				std::ofstream xport_spc(sPath_FullSpectrum);
				if (xport_spc.is_open())
				{
					double spc_min = m_data_base->m_simulation.get_sim_min(w_E, c_CT, 1.0);
					double spc_max = m_data_base->m_simulation.get_sim_max(w_E, c_CT, 1.0);
					std::vector<std::vector<std::pair<double, double>>> spc_to_save = m_data_base->m_simulation.save_CreateFullSpectrum(w_E, spc_min, spc_max, 1.0, c_CT);
					for (size_t i = 0; i < spc_to_save[0].size(); i++)
					{
						xport_spc << spc_to_save[0][i].first << "\t" << spc_to_save[0][i].second << std::endl;
					}
					xport_info << "Export of active simulation successful. Exported full spectrum: " << m_data_base->CStoS(cs_NI) << std::endl;
					for (size_t i = 1; i < spc_to_save.size(); i++)
					{
						CString sPath_isolatedSpectrum = sFilePath + _T(".") + cs_NI + _T("_group") + std::to_string(i).c_str() + _T("_spc.cout");
						std::ofstream xport_isolspc(sPath_isolatedSpectrum);
						if (xport_isolspc.is_open())
						{
							for (size_t j = 0; j < spc_to_save[i].size(); j++)
							{
								xport_isolspc << spc_to_save[i][j].first << "\t" << spc_to_save[i][j].second << std::endl;
							}
						}
						else
						{
							AfxMessageBox(_T("Export failed - unable to write isolated spc-file ") + sFilePath, MB_ICONERROR);
							xport_info << "Export of active simulation failed. Unable to write isolated spc-file (group " << std::to_string(i) << ")." << std::endl;
						}
						xport_isolspc.close();
					}
					sim_exported = true;
				}
				else
				{
					AfxMessageBox(_T("Export failed - unable to write spc-file ") + sFilePath, MB_ICONERROR);
					xport_info << "Export of active simulation failed. Unable to write spc-file." << std::endl;
				}
				xport_spc.close();
			}
			else
			{
				xport_info << "Export of active simulation failed. No simulation running." << std::endl;
			}
			m_data_base->m_simulation.print_fitusage(xport_info);
			if (sim_exported)
			{
				m_tab1.print_info(xport_info);
				m_tab3.print_info(xport_info);
				xport_info << std::fixed << std::scientific << std::setprecision(10);
				xport_info << "--------" << std::endl;
				xport_info << "g-Tensor" << std::endl;
				xport_info << "--------" << std::endl;
				std::vector<std::vector<double>> gtensorprint = m_data_base->get_g_tensor();
				for (size_t i = 0; i < 3; i++)
				{
					xport_info << gtensorprint[i][0] << "\t" << gtensorprint[i][1] << "\t" << gtensorprint[i][2] << std::endl;
				}
				xport_info << std::endl << std::endl;
				CString sPath_SimData = sFilePath + _T(".") + cs_NI + _T(".simData.cout");
				CString sPath_fullSigma = sFilePath + _T(".") + cs_NI + _T(".FullShieldings.cout");
				CString sPath_relax = sFilePath + _T(".") + cs_NI + _T(".relaxationRates.cout");
				CString sPath_dorb = sFilePath + _T(".") + cs_NI + _T(".diamshift.cout");
				CString sPath_dfc = sFilePath + _T(".") + cs_NI + _T(".fermicontact.cout");
				CString sPath_dhfc = sFilePath + _T(".") + cs_NI + _T(".hfc.cout");
				std::ofstream xport_simData(sPath_SimData);
				if (xport_simData.is_open())
				{
					m_SimDataDlg.print_info(xport_simData);
					xport_info << "- simulated data exported to " << m_data_base->CStoS(sPath_SimData) << std::endl;
				}
				else
				{
					xport_info << "- export of simulated data failed" << std::endl;
				}
				xport_simData.close();
				std::ofstream xport_fullSigma(sPath_fullSigma);
				if(xport_fullSigma.is_open())
				{
					m_data_base->m_simulation.print_TotalShieldings(xport_fullSigma);
					xport_info << "- computed total shieldings (sigma_total = sigma_dia + sigma_hf; sigma_hf = -chi*A) exported to " << m_data_base->CStoS(sPath_fullSigma) << std::endl;
				}
				else
				{
					xport_info << "- export of full shielding tensors failed" << std::endl;
				}
				xport_fullSigma.close();
				std::ofstream xport_relax(sPath_relax);
				if(xport_relax.is_open())
				{
					m_data_base->m_simulation.print_R2(xport_relax);
					xport_info << "- computed relaxation rates exported to " << m_data_base->CStoS(sPath_relax) << std::endl;
				}
				else
				{
					xport_info << "- export of relaxation rates failed" << std::endl;
				}
				xport_relax.close();
				std::ofstream xport_groupDefine(sPath_groupDefine);
				if (xport_groupDefine.is_open())
				{
					m_data_base->m_simulation.print_group_definition(xport_groupDefine);
					xport_info << "- definition of all groups exported to " << m_data_base->CStoS(sPath_groupDefine) << std::endl;
				}
				else
				{
					xport_info << "- export of definition of groups failed" << std::endl;
				}
				xport_groupDefine.close();
				std::ofstream xport_dorb(sPath_dorb);
				if (xport_dorb.is_open())
				{
					m_tab1.print_dorb(xport_dorb, w_E);
					xport_info << "- orbital contributions to the chemical shielding and shift exported to " << m_data_base->CStoS(sPath_dorb) << std::endl;
				}
				else
				{
					xport_info << "- export of orbital contributions to the chemical shielding and shift failed" << std::endl;
				}
				xport_dorb.close();
				std::ofstream xport_dfc(sPath_dfc);
				if (xport_dfc.is_open())
				{
					m_tab1.print_dfc(xport_dfc, w_E);
					xport_info << "- imported fermi contact shifts exported to " << m_data_base->CStoS(sPath_dfc) << std::endl;
				}
				else
				{
					xport_info << "- export of imported fermi contact shifts failed" << std::endl;
				}
				xport_dfc.close();
				std::ofstream xport_dhfc(sPath_dhfc);
				if (xport_dhfc.is_open())
				{
					m_tab1.print_dhfc(xport_dhfc, w_E);
					xport_info << "- hyperfine coupling tensors (in ppm/Angstroem^3) exported to " << m_data_base->CStoS(sPath_dhfc) << std::endl;
				}
				else
				{
					xport_info << "- export of hyperfine coupling tensors failed" << std::endl;
				}
				xport_dhfc.close();
			}
			std::ofstream xport_indiSus(sPath_indiSus);
			if (xport_indiSus.is_open())
			{
				m_tab2.print_info(xport_indiSus, 0);
				xport_info << "- individual susceptibility tensor exported to " << m_data_base->CStoS(sPath_indiSus) << std::endl;
			}
			else
			{
				xport_info << "- export of individual susceptibility tensor failed" << std::endl;
			}
			xport_indiSus.close();
			std::ofstream xport_compSus(sPath_compSus);
			if (xport_compSus.is_open())
			{
				m_tab2.print_info(xport_compSus, 1);
				xport_info << "- imported \"computed\" susceptibility tensor (CASSCF/NEVPT2) exported to " << m_data_base->CStoS(sPath_compSus) << std::endl;
			}
			else
			{
				xport_info << "- export of imported \"computed\" susceptibility tensor failed" << std::endl;
			}
			xport_compSus.close();
			std::ofstream xport_fitSus(sPath_fitSus);
			if (xport_fitSus.is_open())
			{
				m_tab2.print_info(xport_fitSus, 2);
				xport_info << "- fitted susceptibility tensor exported to " << m_data_base->CStoS(sPath_fitSus) << std::endl;
			}
			else
			{
				xport_info << "- export of fitted susceptibility tensor failed" << std::endl;
			}
			xport_fitSus.close();
			std::ofstream xport_xyz(sPath_xyz);
			if (xport_xyz.is_open())
			{
				xport_xyz << m_data_base->get_mol_atom_count() << std::endl;
				xport_xyz << "molecule coordinates used for simulation (Angstroem)" << std::endl;
				xport_xyz << std::fixed << std::setprecision(12);
				for (size_t i = 0; i < m_data_base->get_mol_atom_count(); i++)
				{
					xport_xyz << std::setw(2) << std::setfill(' ') << m_data_base->CStoS(m_data_base->get_mol_element_by_index(i));
					xport_xyz << std::setw(20) << std::setfill(' ') << m_data_base->get_mol_coordinate(i, 0);
					xport_xyz << std::setw(20) << std::setfill(' ') << m_data_base->get_mol_coordinate(i, 1);
					xport_xyz << std::setw(20) << std::setfill(' ') << m_data_base->get_mol_coordinate(i, 2) << std::endl;
				}
				xport_info << "- molecule coordinates exported to " << m_data_base->CStoS(sPath_xyz) << std::endl;
			}
			else
			{
				xport_info << "- export of molecule coordinates failed" << std::endl;
			}
			xport_xyz.close();
			m_data_base->get_mol_coordinate(0, 0);
			std::ofstream xport_errors(sPath_error);
			if (xport_errors.is_open())
			{
				m_StatErrorDlg.print_errors(xport_errors);
				xport_info << "- standard errors exported to " << m_data_base->CStoS(sPath_error) << std::endl;
			}
			else
			{
				xport_info << "- export standard errors failed" << std::endl;
			}
			xport_errors.close();			
		}
		else
		{
			AfxMessageBox(_T("Export failed - unable to write info-file"));
		}
		xport_info.close();
		AfxMessageBox(_T("export done"), MB_ICONINFORMATION);
	}
}



