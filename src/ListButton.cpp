#include "pch.h"
#include "ListButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CListButton, CSliderCtrl)

BEGIN_MESSAGE_MAP(CListButton, CSliderCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()