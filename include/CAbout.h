#pragma once

class CAbout : public CDialogEx
{
	DECLARE_DYNAMIC(CAbout)
public:
	CAbout(CWnd* pParent = nullptr);
	virtual ~CAbout();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ABOUT };
#endif
protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
