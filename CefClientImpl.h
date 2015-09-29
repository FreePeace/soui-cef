// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef GALLERY_08_CEFCLIENTIMPL_HPP
#define GALLERY_08_CEFCLIENTIMPL_HPP
#include "include/base/cef_lock.h"
#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"

#include <list>
#include <map>
class CefClientImpl : public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefContextMenuHandler,
	public CefLoadHandler,
	public CefJSDialogHandler
{
public:
	CefClientImpl();
	~CefClientImpl();

	// Provide access to the single global instance of this object.
	static CefClientImpl* GetInstance();

	// CefClient methods:
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE{
		return this;
	}
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE{
		return this;
	}
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE{
		return this;
	}

	virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE {
		return this;
	}

	// CefDisplayHandler methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefLoadHandler methods:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;

 // CefJSDialogHandler methods
	virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
		const CefString& origin_url,
		const CefString& accept_lang,
		JSDialogType dialog_type,
		const CefString& message_text,
		const CefString& default_prompt_text,
		CefRefPtr<CefJSDialogCallback> callback,
		bool& suppress_message) OVERRIDE;

	virtual bool OnBeforeUnloadDialog(
		CefRefPtr<CefBrowser> browser,
		const CefString& message_text,
		bool is_reload,
		CefRefPtr<CefJSDialogCallback> callback) OVERRIDE;

	virtual void OnResetDialogState(CefRefPtr<CefBrowser> browser) OVERRIDE;


	bool IsClosing() const { return is_closing_; }

	CefRefPtr<CefBrowser> GetBrowser() {
         return browser_child;
	}
    
	int GetBrowserId() const;

	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE {
		return this;
	}

	// CefContextMenuHandler methods
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model) OVERRIDE;

private:

	CefRefPtr<CefMessageRouterBrowserSide> message_router_;

	typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
	BrowserList popup_browsers_;

	CefRefPtr<CefBrowser> browser_child;
	int browser_id_;

	bool is_closing_;
	static int browser_count_;

	mutable base::Lock lock_;
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CefClientImpl);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
