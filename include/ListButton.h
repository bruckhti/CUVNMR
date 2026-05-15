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
#ifndef LISTBUTTON_H
#define LISTBUTTON_H

#include "resource.h"
#include "framework.h"
#include <vector>

class CListButton : public CButton
{
    DECLARE_DYNAMIC(CListButton)
public:
    CListButton(std::vector<bool>& pValue, int index, UINT nID) : m_pValue(pValue), m_index(index), m_nButtonID(nID)
    { }
    virtual ~CListButton() {}
protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point)
    {
        if (m_pValue[m_index])
        {
            SetCheck(BST_UNCHECKED);
            m_pValue[m_index] = false;
        }
        else
        {
            SetCheck(BST_CHECKED);
            m_pValue[m_index] = true;
        }
    }
    DECLARE_MESSAGE_MAP()
private:
    std::vector<bool>& m_pValue;
    int m_index;
    UINT m_nButtonID;
};

#endif