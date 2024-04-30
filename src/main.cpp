#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

LRESULT CALLBACK WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND fat_cat;
HWND status_bar;
HWND cat_counter;

int cat_count = 0;

int cat_per_click = 1;



bool playSounds = true;


HFONT CreateCustomFont(COLORREF textColor)
{
    LOGFONT lf;
    HFONT hFont;

    ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfHeight = -12; // Change the font size as needed
    lf.lfWeight = FW_NORMAL;
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    wcscpy_s(lf.lfFaceName, LF_FACESIZE, L"Arial"); // Change the font name as needed

    hFont = CreateFontIndirect(&lf);

    if (hFont)
    {
        // Set the text color of the font
        SetTextColor(GetDC(NULL), textColor);
    }

    return hFont;
}


void playmauu() {

    if(playSounds)
        PlaySound(TEXT("img/clickSound.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void create(HWND hwnd) {

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_WIN95_CLASSES; // Use any appropriate flags
    InitCommonControlsEx(&icex);



    fat_cat = CreateWindowEx(
        WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
        L"BUTTON",
        L"Fat Cat",
        BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | BS_BITMAP,
        50, 50,
        352, 252,
        hwnd,
        (HMENU)8888,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    HBITMAP cat_bmp = (HBITMAP)LoadImage(NULL, L"img/fat_cat.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    SendMessage(fat_cat, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)cat_bmp);


    //todo: put in credits for -
    // (va) - Dorentina

    status_bar = CreateWindowEx(
        WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
        STATUSCLASSNAME,
        L"(voice acting - Dorentina)",
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0, hwnd, NULL, NULL, NULL
    );


    if (status_bar == NULL)
    {
        MessageBox(NULL, L"Failed to create the status bar!", L"Error", MB_OK | MB_ICONERROR);
        
    }
    
    // Set the custom font with pink text color
    HFONT hFont = CreateCustomFont(RGB(255, 0, 255)); // RGB(255, 0, 255) is pink color
    if (hFont)
    {
        // Set the font of the status bar control
        SendMessage(status_bar, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
    else
    {
        // Font creation failed
        MessageBox(NULL, L"Failed to create font!", L"Error", MB_OK | MB_ICONERROR);
    }


    cat_counter = CreateWindowEx(
        0,
        L"Static",
        L"Cat Count: 0",
        SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        50, 0, 1000, 50,
        hwnd,
        NULL,
        NULL,
        NULL

    );

    DeleteObject(hFont);

}

void paint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    EndPaint(hwnd, &ps);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"WhatIsMeowDoing";

    WNDCLASS wc = {};
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowCallback;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"WhatIsMeowDoing <by. Jelly>", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 512, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL) { return 0; }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            int id = LOWORD(wParam);
            if (id == 8888)
            {
                playmauu();

                cat_count += cat_per_click;

                std::wstring countText = L"Cat Count: " + std::to_wstring(cat_count);

                SendMessage(cat_counter, WM_SETTEXT, NULL, (LPARAM)countText.c_str());


            }
        }
        break;

    case WM_CREATE:
        create(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        paint(hwnd);
        break;

    case WM_DRAWITEM:
        break;


    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}