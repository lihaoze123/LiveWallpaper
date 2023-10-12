#include "LiveWallpaper.h"

INT CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	const wchar_t CLASS_NAME[] = L"LiveWallpaper";

	WNDCLASS wc = { };

	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	GetModuleFileName(NULL, szExePath, MAX_PATH);

	HWND hProgm = FindWindow(L"Progman", 0);
	SendMessageTimeout(hProgm, 0x52C, 0, 0, 0, 100, 0);

	HDC hdc = GetDC(NULL);
	int iXLength = GetDeviceCaps(hdc, DESKTOPHORZRES), iYLength = GetDeviceCaps(hdc, DESKTOPVERTRES);

	HWND hWnd = CreateWindowEx(
		0, 
		CLASS_NAME, 
		NULL, 
		WS_POPUP, 
		0, 
		0, 
		iXLength,
		iYLength, 
		NULL, 
		NULL, 
		hInstance, 
		NULL
	);

	ReleaseDC(NULL, hdc);

	RegisterHotKey(hWnd, ID_H1, MOD_WIN, VK_ESCAPE); // Press Win + Esc to exit
	// Use alt + e to register and alt + u to unregister
	RegisterHotKey(hWnd, ID_H2, MOD_ALT, 0x45);
	RegisterHotKey(hWnd, ID_H3, MOD_ALT, 0x55);

	if (hWnd == NULL)
		return 0;

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	auto environmentCreatedHandlerCallback = Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
		[&](HRESULT result, ICoreWebView2Environment* env) {
			env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
				[&](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
					if (controller != nullptr) {
						webviewController = controller;
						webviewController->get_CoreWebView2(&webview);
					}

					RECT bounds = { 0, 0, iXLength, iYLength };
					webviewController->put_Bounds(bounds);

					std::wstring uri = std::wstring(L"file://") + std::filesystem::current_path().generic_wstring() + std::wstring(L"/resource/wallpaper.html");

					webview->Navigate(uri.c_str());

					return 0;
				}).Get());
			return 0;
		}
	);

	auto options = Make<CoreWebView2EnvironmentOptions>();
	options->put_AdditionalBrowserArguments(L"--disable-web-security");

	CreateCoreWebView2EnvironmentWithOptions(NULL, NULL, options.Get(), environmentCreatedHandlerCallback.Get());

	SetParent(hWnd, hProgm);
	EnumWindows(EnumWindowsProc, 0);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (GetAncestor(hWnd, GA_PARENT) != hProgm) {
			SetParent(hWnd, hProgm);
			EnumWindows(EnumWindowsProc, 0);
		}
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hWnd, &ps);

        return 0;
    }

    case WM_HOTKEY: {
        switch (wParam) {
        case ID_H1: {
            PostQuitMessage(0);
            return 0;
        }

		case ID_H2: {
			HKEY hKey;
			RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
			RegSetValueEx(hKey, L"LiveWallpaper", 0, REG_SZ, (LPBYTE)szExePath, (wcslen(szExePath) + 1) * sizeof(WCHAR));
			RegCloseKey(hKey);

			if (isStartupRegistered()) {
				MessageBox(hWnd, L"LiveWallpaper已成功注册为开机启动项！", L"成功", MB_OK);
			} else {
				MessageBox(hWnd, L"注册开机启动项失败，请重试！", L"错误", MB_OK);
			}
			return 0;
		}

		case ID_H3: {
			HKEY hKey;
			RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
			RegDeleteValue(hKey, L"LiveWallpaper");
			RegCloseKey(hKey);

			if (!isStartupRegistered()) {
				MessageBox(hWnd, L"已成功取消LiveWallpaper的开机启动！", L"成功", MB_OK);
			}
			else {
				MessageBox(hWnd, L"取消开机启动项失败，请重试！", L"错误", MB_OK);
			}
			return 0;
		}


        default:
            break;
        }
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProc(_In_ HWND hWnd, _In_ LPARAM Lparam) {
	HWND hDefView = FindWindowEx(hWnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		HWND hWorkerw = FindWindowEx(0, hWnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, 0);

		return FALSE;
	}
	return TRUE;
}

BOOL isStartupRegistered() {
	HKEY hKey;
	TCHAR szValue[MAX_PATH];
	DWORD dwBufferSize = sizeof(szValue);
	DWORD dwType = REG_SZ;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
		return false;
	}

	if (RegQueryValueEx(hKey, L"LiveWallpaper", NULL, &dwType, (LPBYTE)&szValue, &dwBufferSize) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return false;
	}

	RegCloseKey(hKey);
	return true;
}

