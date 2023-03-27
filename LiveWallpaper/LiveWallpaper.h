#include <Windows.h>
#include <stdlib.h>
#include <string>
#include <filesystem>
#include <tchar.h>
#include <wrl.h>
#include <wil/com.h>
#include <Shellapi.h>
#include "WebView2.h"
#include "WebView2EnvironmentOptions.h"

using namespace Microsoft::WRL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM Lparam);
BOOL isStartupRegistered();

WCHAR szExePath[MAX_PATH];

static wil::com_ptr<ICoreWebView2Controller> webviewController;
static wil::com_ptr<ICoreWebView2> webview;

#define ID_H1 0x1201
#define ID_H2 0x1202
#define ID_H3 0x1203
