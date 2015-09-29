#include "StdAfx.h"
#include "CefRealWndHandler.h"
#include "SCefBrowser.h"

CCefRealWndHandler::CCefRealWndHandler(void)
{
}

CCefRealWndHandler::~CCefRealWndHandler(void)
{
}


HWND CCefRealWndHandler::OnRealWndCreate(SRealWnd *pRealWnd)
{
       const SRealWndParam&  param = pRealWnd->GetRealWndParam();
	   if(param.m_strClassName == _T("cef_cls"))
	   {
		   HWND HostWnd = pRealWnd->GetContainer()->GetHostHwnd();
		   SCefBrowser* pSCefBrowser = new SCefBrowser;    
		   HWND hCefWnd = pSCefBrowser->Create(L"cef", WS_CHILD |WS_VISIBLE,NULL,0,0,0,0,HostWnd,NULL);
		   return pSCefBrowser->m_hWnd;
	   }
	   return 0;
}

void CCefRealWndHandler::OnRealWndDestroy(SRealWnd *pRealWnd)
{
	const SRealWndParam &param=pRealWnd->GetRealWndParam();
	if(param.m_strClassName==_T("cef_cls"))
	{
		SCefBrowser *pSCefBrowser=(SCefBrowser*) pRealWnd->GetData();
		if(pSCefBrowser)
		{
			pSCefBrowser->DestroyWindow();
			delete pSCefBrowser;
		}
	}
}

BOOL CCefRealWndHandler::OnRealWndInit(SRealWnd *pRealWnd)
{
     return FALSE;
}

BOOL CCefRealWndHandler::OnRealWndSize(SRealWnd *pRealWnd)
{
	if(!pRealWnd)
		return FALSE;
    
// return TRUE 由自己来移动窗口位置
	SWindow* pParent = pRealWnd->GetParent();
	CRect rcParent = pParent->GetClientRect();
	::SetWindowPos(pRealWnd->GetRealHwnd(FALSE),0,rcParent.left,rcParent.top,rcParent.Width(),rcParent.Height(),SWP_NOZORDER);
    return TRUE;
}
