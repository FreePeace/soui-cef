// dui-demo.cpp : main source file
//

#include "stdafx.h"
#include "MainDlg.h"
#include "include/internal/cef_win.h"
#include "include/internal/cef_ptr.h"
#include "CefAppImpl.h"
#include "CefRealWndHandler.h"
#include "SCefChatEdit.h"

//从PE文件加载，注意从文件加载路径位置
#define RES_TYPE 1
//#define RES_TYPE 0   //从文件中加载资源
// #define RES_TYPE 1  //从PE资源中加载UI资源

#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif
    
extern HWND hMessageWnd;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{

    HRESULT hRes = OleInitialize(NULL);
    SASSERT(SUCCEEDED(hRes));

    int nRet = 0;
    
    SComMgr *pComMgr = new SComMgr;

    //将程序的运行路径修改到项目所在目录所在的目录
    TCHAR szCurrentDir[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));
    LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
    _tcscpy(lpInsertPos + 1, _T(".."));
    SetCurrentDirectory(szCurrentDir);
    {
        BOOL bLoaded=FALSE;
        CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
        CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
        bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
        SASSERT_FMT(bLoaded,_T("load interface [render] failed!"));
        bLoaded=pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
        SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("imgdecoder"));

        pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
        SApplication *theApp = new SApplication(pRenderFactory, hInstance);
        //从DLL加载系统资源
        HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
        if (hModSysResource)
        {
            CAutoRefPtr<IResProvider> sysResProvider;
            CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
            sysResProvider->Init((WPARAM)hModSysResource, 0);
            theApp->LoadSystemNamedResource(sysResProvider);
            FreeLibrary(hModSysResource);
        }else
        {
            SASSERT(0);
        }

        CAutoRefPtr<IResProvider>   pResProvider;
#if (RES_TYPE == 0)
        CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
        if (!pResProvider->Init((LPARAM)_T("uires"), 0))
        {
            SASSERT(0);
            return 1;
        }
#else 
        CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
        pResProvider->Init((WPARAM)hInstance, 0);
#endif

        theApp->AddResProvider(pResProvider);
        theApp->Init(_T("XML_INIT"));



   // provide CEF with command-line arguments.
		CefMainArgs main_args(hInstance);

  // CefChatApp implements application-level callbacks ,it will create the first browser  
  // instance in OnContextInitialized() after CEF has initialized.

		CefRefPtr<CefAppImpl> app(new CefAppImpl);

		int exit_code = CefExecuteProcess(main_args,app.get(),NULL);
		if(exit_code >0){
			return exit_code;
		}

        // Specify CEF global settings here
		CefSettings settings;

#if !CEF_ENABLE_SANDBOX
		settings.no_sandbox = true;

#endif

        settings.multi_threaded_message_loop = true;
		CefInitialize(main_args,settings,app,NULL);

		// 设置真窗口处理接口

		CCefRealWndHandler*  pRealWndHandler = new CCefRealWndHandler();
		theApp->SetRealWndHandler(pRealWndHandler);
		pRealWndHandler->Release();
       theApp->RegisterWndFactory(TplSWindowFactory<SCefChatEdit>());

        // BLOCK: Run application
        {
            CMainDlg dlgMain;
            dlgMain.Create(GetActiveWindow());
            dlgMain.SendMessage(WM_INITDIALOG);
            dlgMain.CenterWindow(dlgMain.m_hWnd);
            dlgMain.ShowWindow(SW_SHOWNORMAL);
            nRet = theApp->Run(dlgMain.m_hWnd);
        }

        delete theApp;
    }
    
    delete pComMgr;
	::DestroyWindow(hMessageWnd);
	hMessageWnd = NULL;
	CefShutdown();
    OleUninitialize();
    
    return nRet;
}
