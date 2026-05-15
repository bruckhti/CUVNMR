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
#include "CGTensor.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CGTensor, CDialogEx)

CGTensor::CGTensor(DataBase* p_DataBase, CMainTab1* pTab1, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_GVALUE, pParent)
{
	m_data_base = p_DataBase;
	m_Tab1 = pTab1;
}

CGTensor::~CGTensor()
{
}

void CGTensor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGTensor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_G_RESET, &CGTensor::OnBnClickedButtonGReset)
	ON_BN_CLICKED(IDOK, &CGTensor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGTensor::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_EDIT_G_XX, &CGTensor::OnGTensorEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_G_XY, &CGTensor::OnGTensorEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_G_XZ, &CGTensor::OnGTensorEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_G_YX, &CGTensor::OnGTensorEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_G_YY, &CGTensor::OnGTensorEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_G_YZ, &CGTensor::OnGTensorEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_G_ZX, &CGTensor::OnGTensorEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_G_ZY, &CGTensor::OnGTensorEditKillfocus)
	ON_EN_KILLFOCUS(IDC_EDIT_G_ZZ, &CGTensor::OnGTensorEditKillfocus)
END_MESSAGE_MAP()

BOOL CGTensor::OnInitDialog()
{
	CString formattedValue;
	m_g_tensor = m_data_base->get_g_tensor();
	m_EditGValXX.SubclassDlgItem(IDC_EDIT_G_XX, this);
	formattedValue.Format(_T("%.10f"), m_g_tensor[0][0]);
	m_EditGValXX.SetWindowTextW(formattedValue);
	m_EditGValXY.SubclassDlgItem(IDC_EDIT_G_XY, this);
	formattedValue.Format(_T("%.10f"), m_g_tensor[0][1]);
	m_EditGValXY.SetWindowTextW(formattedValue);
	m_EditGValXZ.SubclassDlgItem(IDC_EDIT_G_XZ, this);
	formattedValue.Format(_T("%.10f"), m_g_tensor[0][2]);
	m_EditGValXZ.SetWindowTextW(formattedValue);
	m_EditGValYX.SubclassDlgItem(IDC_EDIT_G_YX, this);
	formattedValue.Format(_T("%.10f"), m_g_tensor[1][0]);
	m_EditGValYX.SetWindowTextW(formattedValue);
	m_EditGValYY.SubclassDlgItem(IDC_EDIT_G_YY, this);
	formattedValue.Format(_T("%.10f"), m_g_tensor[1][1]);
	m_EditGValYY.SetWindowTextW(formattedValue);
	m_EditGValYZ.SubclassDlgItem(IDC_EDIT_G_YZ, this);
	formattedValue.Format(_T("%.10f"), m_g_tensor[1][2]);
	m_EditGValYZ.SetWindowTextW(formattedValue);
	m_EditGValZX.SubclassDlgItem(IDC_EDIT_G_ZX, this);
	formattedValue.Format(_T("%.10f"), m_g_tensor[2][0]);
	m_EditGValZX.SetWindowTextW(formattedValue);
	m_EditGValZY.SubclassDlgItem(IDC_EDIT_G_ZY, this);
	formattedValue.Format(_T("%.10f"), m_g_tensor[2][1]);
	m_EditGValZY.SetWindowTextW(formattedValue);
	m_EditGValZZ.SubclassDlgItem(IDC_EDIT_G_ZZ, this);
	formattedValue.Format(_T("%.10f"), m_g_tensor[2][2]);
	m_EditGValZZ.SetWindowTextW(formattedValue);
	return TRUE;
}

void CGTensor::OnBnClickedButtonGReset()
{
	CString formattedValue;
	formattedValue.Format(_T("%.10f"), g_e);
	m_EditGValXX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.10f"), 0.0);
	m_EditGValXY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.10f"), 0.0);
	m_EditGValXZ.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.10f"), 0.0);
	m_EditGValYX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.10f"), g_e);
	m_EditGValYY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.10f"), 0.0);
	m_EditGValYZ.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.10f"), 0.0);
	m_EditGValZX.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.10f"), 0.0);
	m_EditGValZY.SetWindowTextW(formattedValue);
	formattedValue.Format(_T("%.10f"), g_e);
	m_EditGValZZ.SetWindowTextW(formattedValue);
	m_g_tensor = std::vector<std::vector<double>>(3, std::vector<double>(3, 0.0));
	m_g_tensor[0][0] = g_e;
	m_g_tensor[1][1] = g_e;
	m_g_tensor[2][2] = g_e;
}

void CGTensor::OnBnClickedOk()
{
	m_data_base->set_g_tensor(m_g_tensor);
	m_Tab1->GTensor_dlg_sim_start();
	CDialogEx::OnOK();
}

void CGTensor::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CGTensor::OnGTensorEditKillfocus()
{
	CString edittext;
	CString formattedValue;
	m_EditGValXX.GetWindowTextW(edittext);
	m_g_tensor[0][0] = _tstof(edittext);
	formattedValue.Format(_T("%.10f"), m_g_tensor[0][0]);
	m_EditGValXX.SetWindowTextW(formattedValue);
	m_EditGValXY.GetWindowTextW(edittext);
	m_g_tensor[0][1] = _tstof(edittext);
	formattedValue.Format(_T("%.10f"), m_g_tensor[0][1]);
	m_EditGValXY.SetWindowTextW(formattedValue);
	m_EditGValXZ.GetWindowTextW(edittext);
	m_g_tensor[0][2] = _tstof(edittext);
	formattedValue.Format(_T("%.10f"), m_g_tensor[0][2]);
	m_EditGValXZ.SetWindowTextW(formattedValue);
	m_EditGValYX.GetWindowTextW(edittext);
	m_g_tensor[1][0] = _tstof(edittext);
	formattedValue.Format(_T("%.10f"), m_g_tensor[1][0]);
	m_EditGValYX.SetWindowTextW(formattedValue);
	m_EditGValYY.GetWindowTextW(edittext);
	m_g_tensor[1][1] = _tstof(edittext);
	formattedValue.Format(_T("%.10f"), m_g_tensor[1][1]);
	m_EditGValYY.SetWindowTextW(formattedValue);
	m_EditGValYZ.GetWindowTextW(edittext);
	m_g_tensor[1][2] = _tstof(edittext);
	formattedValue.Format(_T("%.10f"), m_g_tensor[1][2]);
	m_EditGValYZ.SetWindowTextW(formattedValue);
	m_EditGValZX.GetWindowTextW(edittext);
	m_g_tensor[2][0] = _tstof(edittext);
	formattedValue.Format(_T("%.10f"), m_g_tensor[2][0]);
	m_EditGValZX.SetWindowTextW(formattedValue);
	m_EditGValZY.GetWindowTextW(edittext);
	m_g_tensor[2][1] = _tstof(edittext);
	formattedValue.Format(_T("%.10f"), m_g_tensor[2][1]);
	m_EditGValZY.SetWindowTextW(formattedValue);
	m_EditGValZZ.GetWindowTextW(edittext);
	m_g_tensor[2][2] = _tstof(edittext);
	formattedValue.Format(_T("%.10f"), m_g_tensor[2][2]);
	m_EditGValZZ.SetWindowTextW(formattedValue);
}

