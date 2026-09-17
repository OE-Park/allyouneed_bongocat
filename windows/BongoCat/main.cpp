// BongoCat Native - Win32 shell entry point (skeleton).
// Step 03 replaces this plain window with the layered cat window.
#include <windows.h>
#include <string>
#include <wil/resource.h>
#include <wil/result.h>

#include "BongoCore.h"
#include "MessageLoop.h"
#include "resource.h"

namespace
{
    constexpr wchar_t kWindowClass[] = L"BongoCatNativeWindow";

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_PAINT:
        {
            PAINTSTRUCT ps{};
            HDC hdc = BeginPaint(hwnd, &ps);
            const std::string_view ver = bongo::Version();
            std::wstring text = L"BongoCat Native ";
            for (char c : ver)
            {
                text.push_back(static_cast<wchar_t>(c));
            }
            RECT rc{};
            GetClientRect(hwnd, &rc);
            DrawTextW(hdc, text.c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
        }
    }
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_APP));
    wc.hIconSm = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_SMALL));
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    wc.lpszClassName = kWindowClass;
    if (!RegisterClassExW(&wc))
    {
        LOG_LAST_ERROR();
        return 1;
    }

    HWND hwnd = CreateWindowExW(0, kWindowClass, L"BongoCat Native", WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, 420, 200,
                                nullptr, nullptr, hInstance, nullptr);
    if (!hwnd)
    {
        LOG_LAST_ERROR();
        return 1;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg{};
    while (true)
    {
        switch (bongo::win32::ClassifyGetMessageResult(GetMessageW(&msg, nullptr, 0, 0)))
        {
        case bongo::win32::MessageResult::error:
            LOG_LAST_ERROR();
            return 1;
        case bongo::win32::MessageResult::quit:
            return static_cast<int>(msg.wParam);
        case bongo::win32::MessageResult::dispatch:
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            break;
        }
    }
}
