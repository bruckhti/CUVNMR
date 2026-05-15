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
#include "GraphControl.h"
#include "CuvetteDlg.h"
#include <fstream>
#include <sstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CGraphControl, CStatic)
	ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CGraphControl::CGraphControl()
{
}

CGraphControl::~CGraphControl()
{
}

void CGraphControl::Init()
{
    GetClientRect(&panel);
    xAxisSettings = { 80.0, 20.0, 0.0 ,_T("[ppm]"), 1.0 };
    m_scaleX = 1.0*(panel.right/100);
    m_scaleY1 = 1.0;
    m_scaleY2 = 1.0;
    m_offsetX = 55;
    m_mousePosXDataValue = 0.0;
    m_mousePosXDataValue2 = 0.0;
}

void CGraphControl::InitializeTab1(CMainTab1* pTab1)
{
    m_pTab1 = pTab1;
}

void CGraphControl::OnPaint()
{
    GetClientRect(&panel);
    CPaintDC dc(this);
    dc.IntersectClipRect(&panel);
    dc.FillSolidRect(panel, RGB(255, 255, 255));
    DrawXAxis(&dc);
    CPen penGraph1(PS_SOLID, 2, RGB(0, 0, 0));
    CPen penGraph2(PS_SOLID, 2, RGB(255, 0, 0));
    dc.SelectObject(&penGraph1);
    DrawGraph(&dc, penGraph1, 1);
    dc.SelectObject(&penGraph2);
    DrawGraph(&dc, penGraph2, 2);
}

void CGraphControl::DrawXAxis(CDC* pDC)
{  
    origin = CPoint(0, panel.bottom - 50);
    double axisLength = panel.Width();
    pDC->MoveTo(origin);
    pDC->LineTo(origin.x + static_cast<int>(axisLength), origin.y);
    if (xAxisSettings.majorTickSpacing / (m_scaleX / xAxisSettings.TickScaling) < xAxisSettings.majorTickSpacing / 2)
    {
        xAxisSettings.majorTickSpacing /= 2.0;
        xAxisSettings.minorTickSpacing /= 2.0;
        xAxisSettings.TickScaling *= 2.0;
    }
    if (xAxisSettings.majorTickSpacing / (m_scaleX / xAxisSettings.TickScaling) > xAxisSettings.majorTickSpacing * 2)
    {
        xAxisSettings.majorTickSpacing *= 2.0;
        xAxisSettings.minorTickSpacing *= 2.0;
        xAxisSettings.TickScaling /= 2.0;
    }
    double _start = m_offsetX - round(m_offsetX / xAxisSettings.majorTickSpacing)*xAxisSettings.majorTickSpacing;
    for (double i = _start; i <= axisLength / m_scaleX; i += xAxisSettings.majorTickSpacing)
    {
        pDC->MoveTo(origin.x + static_cast<int>(i * m_scaleX), origin.y);
        pDC->LineTo(origin.x + static_cast<int>(i * m_scaleX), origin.y + 10);
        CString label;
        double labelValue = ((xAxisSettings.startValue + i) - m_offsetX)*-1.0;
        label.Format(_T("%.2f"), labelValue);
        pDC->TextOutW(origin.x + static_cast<int>(i * m_scaleX) - 20, origin.y + 15, label);
    }
    for (double i = _start; i <= axisLength / m_scaleX; i += xAxisSettings.minorTickSpacing)
    {
        {
            pDC->MoveTo(origin.x + static_cast<int>(i * m_scaleX), origin.y);
            pDC->LineTo(origin.x + static_cast<int>(i * m_scaleX), origin.y + 5);
        }
    }
    pDC->TextOutW(origin.x + static_cast<int>(axisLength / 2) - 20, origin.y + 30, xAxisSettings.axisLabel);
}

void CGraphControl::DrawGraph(CDC* pDC, CPen& pen, int index)
{
    pDC->SelectObject(&pen);
    if (index == 1 && graph1.size() > 2)
    {
        int min = INT_MAX;
        int max = INT_MIN;
        for (size_t i = 0; i < graph1.size() - 1; i++)
        {
            int x1 = static_cast<int>((-graph1[i].first + m_offsetX) * m_scaleX);
            int x2 = static_cast<int>((-graph1[i + 1].first + m_offsetX) * m_scaleX);
            if (x1 != x2 && (x1 > 0 || x2 > 0) && (x1 < panel.right || x2 < panel.right))
            {
                if (min != INT_MAX && max != INT_MIN)
                {
                    if ((min > 0 || max > 0) && (min < panel.bottom || max < panel.bottom))
                    {
                        pDC->MoveTo(x1, min);
                        pDC->LineTo(x1, max);
                    }
                    min = INT_MAX;
                    max = INT_MIN;
                }
                int y1 = (origin.y + static_cast<int>(-graph1[i].second * m_scaleY1)) - 10;
                int y2 = (origin.y + static_cast<int>(-graph1[i + 1].second * m_scaleY1)) - 10;
                if ((y1 > 0 || y2 > 0) && (y1 < panel.bottom || y2 < panel.bottom))
                {
                    pDC->MoveTo(x1, y1);
                    pDC->LineTo(x2, y2);
                }
            }
            else if (x1 == x2 && x1 > 0 && x1 < panel.right)
            {
                int y1 = (origin.y + static_cast<int>(-graph1[i].second * m_scaleY1)) - 10;
                int y2 = (origin.y + static_cast<int>(-graph1[i + 1].second * m_scaleY1)) - 10;
                if (y1 > max)
                {
                    max = y1;
                }
                if (y2 > max)
                {
                    max = y2;
                }
                if (y1 < min)
                {
                    min = y1;
                }
                if (y2 < min)
                {
                    min = y2;
                }
            }
        }
    }
    else if (index == 2 && graph2.size() > 2)
    {
        int min = INT_MAX;
        int max = INT_MIN;
        for (size_t i = 0; i < graph2.size() - 1; i++)
        {
            int x1 = static_cast<int>((-graph2[i].first + m_offsetX) * m_scaleX);
            int x2 = static_cast<int>((-graph2[i + 1].first + m_offsetX) * m_scaleX);
            if (x1 != x2 && (x1 > 0 || x2 > 0) && (x1 < panel.right || x2 < panel.right))
            {
                if (min != INT_MAX && max != INT_MIN)
                {
                    if ((min > 0 || max > 0) && (min < panel.bottom || max < panel.bottom))
                    {
                        pDC->MoveTo(x1, min);
                        pDC->LineTo(x1, max);
                    }
                    min = INT_MAX;
                    max = INT_MIN;
                }
                int y1 = (origin.y + static_cast<int>(-graph2[i].second * m_scaleY2)) - 10;
                int y2 = (origin.y + static_cast<int>(-graph2[i + 1].second * m_scaleY2)) - 10;
                if ((y1 > 0 || y2 > 0) && (y1 < panel.bottom || y2 < panel.bottom))
                {
                    pDC->MoveTo(x1, y1);
                    pDC->LineTo(x2, y2);
                }
            }
            else if (x1 == x2 && x1 > 0 && x1 < panel.right)
            {
                int y1 = (origin.y + static_cast<int>(-graph2[i].second * m_scaleY2)) - 10;
                int y2 = (origin.y + static_cast<int>(-graph2[i + 1].second * m_scaleY2)) - 10;
                if (y1 > max)
                {
                    max = y1;
                }
                if (y2 > max)
                {
                    max = y2;
                }
                if (y1 < min)
                {
                    min = y1;
                }
                if (y2 < min)
                {
                    min = y2;
                }
            }
        }
    }
}

BOOL CGraphControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    ScreenToClient(&pt);
    if (pt.y > 0 && pt.y < panel.bottom && pt.x > 0 && pt.x < panel.right)
    {
        if (pt.y < panel.bottom - 50)
        {
            CCuvetteDlg* pParentDialog = static_cast<CCuvetteDlg*>(GetParent());
            if (pParentDialog->m_CheckBoxGraph1.GetCheck() == BST_CHECKED)
            {
                if (zDelta > 0)
                    m_scaleY1 *= 1.2;
                else
                    m_scaleY1 /= 1.2;
            }
            if (pParentDialog->m_CheckBoxGraph2.GetCheck() == BST_CHECKED)
            {
                if (zDelta > 0)
                    m_scaleY2 *= 1.2;
                else
                    m_scaleY2 /= 1.2;
            }
        }
        else
        {
            m_mousePosXDataValue = (pt.x / m_scaleX - m_offsetX);
            if (zDelta > 0)
            {
                if (m_scaleX < 4000000)
                {
                    m_scaleX *= 1.2;
                }
            }
            else
            {
                if (m_scaleX > 0.001)
                {
                    m_scaleX /= 1.2;
                }
            }
            m_mousePosXDataValue2 = (pt.x / m_scaleX - m_offsetX);
            double newXOffset = m_mousePosXDataValue - m_mousePosXDataValue2;
            m_offsetX -= newXOffset;
        }
        m_pTab1->Graph_sim_start();
        Invalidate();
    }
    return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}

void CGraphControl::OnLButtonDown(UINT nFlags, CPoint pt)
{
    ScreenToClient(&pt);
    if (pt.y > 0 && pt.y < panel.bottom && pt.x > 0 && pt.x < panel.right)
    {
        if (pt.y > panel.bottom - 50)
        {
            m_isDragging = true;
            m_lastMousePos = pt.x;
            SetCapture();
        }
    }
    CStatic::OnLButtonDown(nFlags, pt);
}

void CGraphControl::OnLButtonUp(UINT nFlags, CPoint pt)
{
    if (m_isDragging)
    {
        m_isDragging = false;
        ReleaseCapture();
    }
    CStatic::OnLButtonUp(nFlags, pt);
}

void CGraphControl::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_isDragging)
    {
        int deltaX = point.x - m_lastMousePos.x;
        m_offsetX += (deltaX) / m_scaleX;
        m_lastMousePos = point;
        m_pTab1->Graph_sim_start();
        Invalidate();
    }
    CStatic::OnMouseMove(nFlags, point);
}

void CGraphControl::load_graph(std::vector<std::pair<double, double>>& graph, bool simulation)
{
    if (!simulation)
    {
        graph1.clear();
        graph1 = graph;
        double max = -INFINITY;
        for (auto& entry : graph1)
        {
            if (entry.second > max)
            {
                max = entry.second;
            }
        }
        for (auto& entry : graph1)
        {
            entry.second /= max / (panel.bottom - 60);
        }
    }
    else
    {
        graph2.clear();
        graph2 = graph;
        double max = -INFINITY;
        for (auto& entry : graph2)
        {
            if (entry.second > max)
            {
                max = entry.second;
            }
        }
        for (auto& entry : graph2)
        {
            entry.second /= max / (panel.bottom - 60);
        }
    }
    Invalidate();
}


double CGraphControl::GetMinPPM()
{
    return (m_offsetX - ((double)panel.right / m_scaleX));

}

double CGraphControl::GetMaxPPM()
{
    return (m_offsetX - (0.0 / m_scaleX));
}

double CGraphControl::GetWindowStepSize()
{
    double max = (m_offsetX - (0.0 / m_scaleX));
    double min = (m_offsetX - ((double)panel.right / m_scaleX));
    return (max-min)/(double)panel.Width();
}

void CGraphControl::ResetGraph()
{
    xAxisSettings = { 80.0, 20.0, 0.0 ,_T("[ppm]"), 1.0 };
    m_scaleX = 1.0 * (panel.right / 100);
    m_scaleY1 = 1.0;
    m_scaleY2 = 1.0;
    m_offsetX = 55;
    m_mousePosXDataValue = 0.0;
    m_mousePosXDataValue2 = 0.0;
    graph1.clear();
    graph2.clear();
    Invalidate();
}