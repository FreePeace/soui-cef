#include "StdAfx.h"
#include "CefClientImpl.h"
#include <sstream>
#include <string>
#include "resource.h"

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace {

	CefClientImpl* g_instance = NULL;

}  // namespace
// Handle messages in the browser process.
class Handler : public CefMessageRouterBrowserSide::Handler {
public:
	Handler() {}

	// Called due to cefQuery execution in binding.html.
	virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback) OVERRIDE{
			//// Only handle messages from the test URL.
			//const std::string& url = frame->GetURL();
			//if (url.find(kTestUrl) != 0)
			//    return false;

			//const std::string& message_name = request;
			//if (message_name.find(kTestMessageName) == 0) {
			//    // Reverse the string and return.
			//    std::string result = message_name.substr(sizeof(kTestMessageName));
			//    std::reverse(result.begin(), result.end());
			//    callback->Success(result);
			//    return true;
			//}

			return false;
	}
};

extern HWND hMessageWnd ;
int CefClientImpl::browser_count_ = 0;

CefClientImpl::CefClientImpl()
: is_closing_(false)
, browser_id_(0){
	DCHECK(!g_instance);
	g_instance = this;
}

CefClientImpl::~CefClientImpl() {
	g_instance = NULL;
}

CefClientImpl* CefClientImpl::GetInstance() {
	return g_instance;
}

void CefClientImpl::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	if (!message_router_) {
		// Create the browser-side router for query handling.
		CefMessageRouterConfig config;
		message_router_ = CefMessageRouterBrowserSide::Create(config);
		// Register handlers with the router.
		message_router_->AddHandler(new Handler(), false);
	}

 
	if(!GetBrowser())
	{
		base::AutoLock lock_scope(lock_);
		browser_child = browser;
		browser_id_ = browser->GetIdentifier();
	}else if (browser->IsPopup()) {
		// Add to the list of popup browsers.
		popup_browsers_.push_back(browser);

		// Give focus to the popup browser. Perform asynchronously because the
		// parent window may attempt to keep focus after launching the popup.
		CefPostTask(TID_UI,
			base::Bind(&CefBrowserHost::SetFocus, browser->GetHost().get(), true));
	}

	browser_count_++;

	//std::stringstream ss;
	//ss << "<html><body bgcolor=\"white\"><h1>Chatting with xxx</h1>Text:<pre>" << "XXXYY" <<
	//    "</pre></body></html>";
	//browser->GetMainFrame()->LoadString(ss.str(), "http://tests/gettext");
}

bool CefClientImpl::DoClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	if (GetBrowserId() == browser->GetIdentifier()) {
		base::AutoLock lock_scope(lock_);
		// Set a flag to indicate that the window close should be allowed.
		is_closing_ = true;
	}

	return false;
}

int CefClientImpl::GetBrowserId() const
{
   return browser_id_;
}

void CefClientImpl::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	if (GetBrowserId() == browser->GetIdentifier())
	{
		base::AutoLock lock_scope(lock_);
			// Free the browser pointer so that the browser can be destroyed
		browser_child = NULL;
	}
	else if(browser->IsPopup())
	{
		// Remove from the browser popup list.
		BrowserList::iterator bit = popup_browsers_.begin();
		for (; bit != popup_browsers_.end(); ++bit) {
			if ((*bit)->IsSame(browser)) {
				popup_browsers_.erase(bit);
				break;
			}
		}
	}

	if (--browser_count_ == 0) {
		// All browser windows have closed.
		// Remove and delete message router handlers.
		message_router_ = NULL;
		// Quit the application message loop.
		PostMessage(hMessageWnd,WM_COMMAND,ID_QUIT,0);
	}
}

void CefClientImpl::OnLoadError(CefRefPtr<CefBrowser> browser,
								CefRefPtr<CefFrame> frame,
								ErrorCode errorCode,
								const CefString& errorText,
								const CefString& failedUrl) {
									CEF_REQUIRE_UI_THREAD();


}

void CefClientImpl::OnTitleChange(CefRefPtr<CefBrowser> browser,
								  const CefString& title) {
									  CEF_REQUIRE_UI_THREAD();
}

void CefClientImpl::OnBeforeContextMenu(
										CefRefPtr<CefBrowser> browser,
										CefRefPtr<CefFrame> frame,
										CefRefPtr<CefContextMenuParams> params,
										CefRefPtr<CefMenuModel> model) {
											CEF_REQUIRE_UI_THREAD();
											//http://blog.sina.com.cn/s/blog_dad2c54101019cwb.html
											model->Clear();//Çå³ýÓÒ¼ü²Ëµ¥
}

bool CefClientImpl::OnJSDialog(CefRefPtr<CefBrowser> browser, const CefString& origin_url, const CefString& accept_lang, JSDialogType dialog_type, const CefString& message_text, const CefString& default_prompt_text, CefRefPtr<CefJSDialogCallback> callback, bool& suppress_message)
{
	CEF_REQUIRE_UI_THREAD();
	return false;
}

bool CefClientImpl::OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser, const CefString& message_text, bool is_reload, CefRefPtr<CefJSDialogCallback> callback)
{
	CEF_REQUIRE_UI_THREAD();
	return false;
}

void CefClientImpl::OnResetDialogState(CefRefPtr<CefBrowser> browser)
{
   CEF_REQUIRE_UI_THREAD();
}
