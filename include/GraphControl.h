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
#include <afxwin.h>
#include <vector>
#include "DataBase.h"
#include "CMainTab1.h"

class CCuvetteDlg;
class CMainTab1;

struct AxisSettings
{
	double majorTickSpacing;
	double minorTickSpacing;
	double startValue;
	CString axisLabel;
	double TickScaling;
};

class CGraphControl : public CStatic
{
public:
	CGraphControl();
	virtual ~CGraphControl();
	void ResetGraph();
	void load_graph(std::vector<std::pair<double,double>>& graph, bool simulation);
	void Init();
	void InitializeTab1(CMainTab1* pTab1);
	double GetMinPPM();
	double GetMaxPPM();
	double GetWindowStepSize();
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	DECLARE_MESSAGE_MAP()
private:
	CMainTab1* m_pTab1;
	CRect panel;
	AxisSettings xAxisSettings;
	CPoint origin;
	void DrawXAxis(CDC* pDC);
	void DrawGraph(CDC* pDC, CPen& pen, int index);
	double m_scaleX;
	double m_scaleY1;
	double m_scaleY2;
	CPoint m_lastMousePos;
	bool m_isDragging = false;
	double m_offsetX;
	double m_mousePosXDataValue;
	double m_mousePosXDataValue2;
	CCuvetteDlg* pParentDialog;
	std::vector<std::pair<double, double>> graph1;
	std::vector<std::pair<double, double>> graph2;
};