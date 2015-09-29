#pragma once
#include "include\cef_app.h"

class CefAppImpl :
	public CefApp,public CefBrowserProcessHandler
{
public:
	CefAppImpl(void);
	virtual ~CefAppImpl(void);

public:
	//OVERRIDE CefApp
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() {
		return this;
	}

private:
	IMPLEMENT_REFCOUNTING(CefAppImpl);

};
