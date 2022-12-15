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

	HWND hwnd = CreateWindowEx(0, CLASS_NAME, NULL, WS_POPUP, 0, 0, 3440, 1440, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
		return 0;

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	CreateCoreWebView2Environment(Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
		[hwnd](HRESULT result, ICoreWebView2Environment* env) {
			env->CreateCoreWebView2Controller(hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
				[hwnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
					if (controller != nullptr) {
						webviewController = controller;
						webviewController->get_CoreWebView2(&webview);
					}

					BOOL containsFullScreenElement = TRUE;

					RECT bounds;
					GetClientRect(hwnd, &bounds);
					webviewController->put_Bounds(bounds);

					std::wstring uri = std::wstring(L"file://") + std::filesystem::current_path().generic_wstring() + std::wstring(L"/Wallpaper/demo.html");

					webview->Navigate(uri.c_str());

					return 0;
				}).Get());
			return 0;
		}
	).Get());

	SetParent(hwnd, hProgm);
	EnumWindows(EnumWindowsProc, 0);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);

		return 0;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam) {
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, 0);

		return FALSE;
	}
	return TRUE;
}