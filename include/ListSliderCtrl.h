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
#ifndef LISTSLIDERCTRL_H
#define LISTSLIDERCTRL_H
#include "resource.h"
#include "framework.h"
#include <vector>

class CListSliderCtrl : public CSliderCtrl
{
    DECLARE_DYNAMIC(CListSliderCtrl)
public:
    CListSliderCtrl(std::vector<double>& pValue, int index) : m_pValue(pValue), m_index(index) 
    {
    }
    virtual ~CListSliderCtrl() {}
protected:
    afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
    {
        if (static_cast<int>(m_pValue[m_index]*1000) != GetPos())
        {
            m_pValue[m_index] = GetPos()/1000.0;
        }
        *pResult = 0;
    }
    DECLARE_MESSAGE_MAP()
private:
        std::vector<double>& m_pValue;
        int m_index;
};

#endif