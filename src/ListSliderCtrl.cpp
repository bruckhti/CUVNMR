#include "pch.h"
#include "ListSliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CListSliderCtrl, CSliderCtrl)

BEGIN_MESSAGE_MAP(CListSliderCtrl, CSliderCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListSliderCtrl::OnNMCustomdraw)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()