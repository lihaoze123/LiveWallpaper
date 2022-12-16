#include "LiveWallpaper.h"

INT CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	const wchar_t CLASS_NAME[] = L"LiveWallpaper";

	WNDCLASS wc = { };

	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

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

	RegisterHotKey(hWnd, NULL, MOD_WIN, VK_ESCAPE);

	if (hWnd == NULL)
		return 0;

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	CreateCoreWebView2Environment(Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
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
	).Get());

	SetParent(hWnd, hProgm);
	EnumWindows(EnumWindowsProc, 0);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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

	case WM_HOTKEY: // fallthrough
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