#pragma once

#include "core\SimpleWnd.h"



class CefBrowserWrapper;

class SCefBrowser :
	public CSimpleWnd
{
public:
	SCefBrowser(void);
	~SCefBrowser(void);

public:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus,LPARAM lInitParam);
    BOOL OnEraseBkgnd(HDC dc);
	void OnClose();
	void OnSize(UINT nType, CSize size);
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(SCefBrowser)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		//MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
	END_MSG_MAP()

private:
	CefBrowserWrapper *m_pCefBrowserWrapper;
};
