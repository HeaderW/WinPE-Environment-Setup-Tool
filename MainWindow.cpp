#include "MainWindow.h"
#include "logo.h"
#include <thread>
#include <chrono>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

#define ID_INSTALL_BTN 1001
#define ID_UNINSTALL_BTN 1002
#define ID_PROGRESS_BAR 1003
#define ID_LOG_EDIT 1004
#define ID_CHECK_TERMS 1005

MainWindow::MainWindow()
    : m_hwnd(nullptr)
    , m_btnInstall(nullptr)
    , m_btnUninstall(nullptr)
    , m_progressBar(nullptr)
    , m_logEdit(nullptr)
    , m_checkTerms(nullptr)
    , m_lblStatus(nullptr)
    , m_isInstalling(false)
    , m_isUninstalling(false)
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

MainWindow::~MainWindow()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
    }
    GdiplusShutdown(m_gdiplusToken);
}

bool MainWindow::Create()
{
    const wchar_t CLASS_NAME[] = L"BiosInjectorWindow";
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(31, 34, 42));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    
    RegisterClass(&wc);
    
    m_hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Bios İnjector",
        WS_POPUP,
        (GetSystemMetrics(SM_CXSCREEN) - 900) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - 600) / 2,
        900, 600,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        this
    );
    
    if (m_hwnd == nullptr)
    {
        return false;
    }
    
    CreateControls();
    return true;
}

void MainWindow::Show()
{
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
}

int MainWindow::MessageLoop()
{
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MainWindow* pThis = nullptr;
    
    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (MainWindow*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->m_hwnd = hwnd;
    }
    else
    {
        pThis = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }
    
    if (pThis)
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hwnd, &ps);
            
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 900, 600);
            SelectObject(memDC, memBitmap);
            
            Graphics graphics(memDC);
            graphics.SetSmoothingMode(SmoothingModeAntiAlias);
            
            SolidBrush mainBrush(Color(255, 31, 34, 42));
            graphics.FillRectangle(&mainBrush, 0, 0, 900, 600);
            
            DrawLeftPanel(&graphics);
            
            DrawStatusPanel(&graphics);
            
            SolidBrush logBrush(Color(255, 20, 24, 29));
            graphics.FillRectangle(&logBrush, 343, 302, 545, 190);
            
            BitBlt(hdc, 0, 0, 900, 600, memDC, 0, 0, SRCCOPY);
            
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            
            EndPaint(m_hwnd, &ps);
            return 0;
        }
        
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_INSTALL_BTN:
            OnInstallClick();
            break;
        case ID_UNINSTALL_BTN:
            OnUninstallClick();
            break;
        }
        return 0;
        
    case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT* pDIS = (DRAWITEMSTRUCT*)lParam;
            if (pDIS->CtlID == ID_INSTALL_BTN)
            {
                HBRUSH hBrush = CreateSolidBrush(RGB(207, 24, 34));
                FillRect(pDIS->hDC, &pDIS->rcItem, hBrush);
                SetTextColor(pDIS->hDC, RGB(255, 255, 255));
                SetBkMode(pDIS->hDC, TRANSPARENT);
                
                HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Century Gothic");
                SelectObject(pDIS->hDC, hFont);
                
                DrawText(pDIS->hDC, L"Install", -1, &pDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                DeleteObject(hBrush);
                DeleteObject(hFont);
                return TRUE;
            }
            else if (pDIS->CtlID == ID_UNINSTALL_BTN)
            {
                HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
                FillRect(pDIS->hDC, &pDIS->rcItem, hBrush);
                SetTextColor(pDIS->hDC, RGB(255, 255, 255));
                SetBkMode(pDIS->hDC, TRANSPARENT);
                
                HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Century Gothic");
                SelectObject(pDIS->hDC, hFont);
                
                DrawText(pDIS->hDC, L"Uninstall", -1, &pDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                DeleteObject(hBrush);
                DeleteObject(hFont);
                return TRUE;
            }
        }
        
    case WM_CTLCOLOREDIT:
        {
            HDC hdc = (HDC)wParam;
            HWND hwndCtrl = (HWND)lParam;
            
            if (hwndCtrl == m_logEdit)
            {
                SetTextColor(hdc, RGB(134, 135, 135));
                SetBkColor(hdc, RGB(20, 24, 29));
                return (LRESULT)CreateSolidBrush(RGB(20, 24, 29));
            }
            return (LRESULT)CreateSolidBrush(RGB(31, 34, 42));
        }
        
    case WM_CTLCOLORSTATIC:
        {
            HDC hdc = (HDC)wParam;
            HWND hwndCtrl = (HWND)lParam;
            
            if (hwndCtrl == m_lblStatus)
            {
                SetTextColor(hdc, RGB(238, 26, 74));
                SetBkColor(hdc, RGB(31, 34, 42));
                return (LRESULT)CreateSolidBrush(RGB(31, 34, 42));
            }
            else
            {
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkMode(hdc, TRANSPARENT);
                return (LRESULT)GetStockObject(NULL_BRUSH);
            }
        }
        
    case WM_LBUTTONDOWN:
        SendMessage(m_hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        return 0;
        
    case WM_USER + 1:
        {
            m_isInstalling = false;
            EnableWindow(m_btnInstall, TRUE);
            EnableWindow(m_btnUninstall, TRUE);
            SetWindowText(m_lblStatus, L"Installation Complete");
            AddLog("Installation finished successfully!");
            m_progressValue = 100;
            InvalidateRect(m_hwnd, NULL, FALSE);
            return 0;
        }
        
    case WM_USER + 2:
        {
            m_isUninstalling = false;
            EnableWindow(m_btnInstall, TRUE);
            EnableWindow(m_btnUninstall, TRUE);
            SetWindowText(m_lblStatus, L"Uninstallation Complete");
            AddLog("Uninstallation finished successfully!");
            m_progressValue = 100;
            InvalidateRect(m_hwnd, NULL, FALSE);
            return 0;
        }
        
    case WM_USER + 3:
        {
            m_progressValue = (int)wParam;
            InvalidateRect(m_hwnd, NULL, FALSE);
            return 0;
        }
    }
    
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainWindow::DrawLeftPanel(Graphics* graphics)
{
    SolidBrush leftBrush(Color(255, 207, 24, 34));
    graphics->FillRectangle(&leftBrush, 0, 0, 321, 600);
    
    IStream* pStream = nullptr;
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, sizeof(logo));
    if (hGlobal)
    {
        void* pData = GlobalLock(hGlobal);
        memcpy(pData, logo, sizeof(logo));
        GlobalUnlock(hGlobal);
        
        if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
        {
            Image* logoImage = Image::FromStream(pStream);
            if (logoImage)
            {
                graphics->DrawImage(logoImage, 66, 103, 190, 196);
                delete logoImage;
            }
            pStream->Release();
        }
    }
    
    FontFamily fontFamily(L"Century Gothic");
    Font titleFont(&fontFamily, 24, FontStyleBold, UnitPixel);
    SolidBrush textBrush(Color(255, 255, 255, 255));
    
    RectF textRect(66, 320, 190, 40);
    StringFormat textFormat;
    textFormat.SetAlignment(StringAlignmentCenter);
    textFormat.SetLineAlignment(StringAlignmentCenter);
    
    graphics->DrawString(L"Bios İnjector", -1, &titleFont, textRect, &textFormat, &textBrush);
    
    Font subtitleFont(&fontFamily, 12, FontStyleBold, UnitPixel);
    graphics->DrawString(L"Powered By Header", -1, &subtitleFont, PointF(79, 339), &textBrush);
}

void MainWindow::DrawStatusPanel(Graphics* graphics)
{
    SolidBrush statusBrush(Color(255, 31, 34, 42));
    graphics->FillRectangle(&statusBrush, 321, 0, 579, 224);
    
    int centerX = 321 + 197 + 96;
    int centerY = 20 + 96;
    int radius = 96;
    
    Pen backgroundPen(Color(255, 220, 220, 220), 8);
    graphics->DrawEllipse(&backgroundPen, centerX - radius, centerY - radius, radius * 2, radius * 2);
    
    if (m_progressValue > 0)
    {
        Pen progressPen(Color(255, 207, 24, 34), 8);
        progressPen.SetStartCap(LineCapRound);
        progressPen.SetEndCap(LineCapRound);
        
        float sweepAngle = (float)(m_progressValue * 360.0 / 100.0);
        graphics->DrawArc(&progressPen, centerX - radius, centerY - radius, radius * 2, radius * 2, -90, sweepAngle);
    }
    
    FontFamily fontFamily(L"Microsoft Sans Serif");
    Font progressFont(&fontFamily, 26, FontStyleRegular, UnitPixel);
    SolidBrush textBrush(Color(255, 255, 255, 255));
    
    wchar_t progressText[10];
    swprintf_s(progressText, L"%d%%", m_progressValue);
    
    RectF textRect((float)(centerX - 50), (float)(centerY - 15), 100, 30);
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);
    
    graphics->DrawString(progressText, -1, &progressFont, textRect, &format, &textBrush);
}

void MainWindow::CreateControls()
{
    m_lblStatus = CreateWindow(
        L"STATIC", L"Waiting for Instruction",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        321, 194 + 40, 579, 30,
        m_hwnd, nullptr, GetModuleHandle(nullptr), nullptr
    );
    
    CreateWindow(
        L"STATIC", L"Logs:",
        WS_VISIBLE | WS_CHILD,
        339, 278, 48, 21,
        m_hwnd, nullptr, GetModuleHandle(nullptr), nullptr
    );
    
    m_logEdit = CreateWindow(
        L"EDIT", L"",
        WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
        343, 302, 545, 190,
        m_hwnd, (HMENU)ID_LOG_EDIT, GetModuleHandle(nullptr), nullptr
    );
    
    m_checkTerms = CreateWindow(
        L"BUTTON", L"",
        WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        343, 508, 20, 20,
        m_hwnd, (HMENU)ID_CHECK_TERMS, GetModuleHandle(nullptr), nullptr
    );
    SendMessage(m_checkTerms, BM_SETCHECK, BST_CHECKED, 0);
    
    CreateWindow(
        L"STATIC", L"I Agree Terms and Conditions",
        WS_VISIBLE | WS_CHILD,
        362, 506, 239, 21,
        m_hwnd, nullptr, GetModuleHandle(nullptr), nullptr
    );
    
    m_btnInstall = CreateWindow(
        L"BUTTON", L"Install",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        342, 539, 297, 34,
        m_hwnd, (HMENU)ID_INSTALL_BTN, GetModuleHandle(nullptr), nullptr
    );
    
    m_btnUninstall = CreateWindow(
        L"BUTTON", L"Uninstall",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        645, 539, 243, 34,
        m_hwnd, (HMENU)ID_UNINSTALL_BTN, GetModuleHandle(nullptr), nullptr
    );
    
    m_progressValue = 0;
}

void MainWindow::OnInstallClick()
{
    if (m_isInstalling || m_isUninstalling)
        return;
        
    m_isInstalling = true;
    EnableWindow(m_btnInstall, FALSE);
    EnableWindow(m_btnUninstall, FALSE);
    
    SetWindowText(m_lblStatus, L"Installing...");
    AddLog("Starting installation process...");
    
    std::thread([this]() {
        AddLog("Preparing installation...");
        PostMessage(m_hwnd, WM_USER + 3, 10, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        AddLog("Configuring application...");
        PostMessage(m_hwnd, WM_USER + 3, 60, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        PostMessage(m_hwnd, WM_USER + 3, 100, 0);
        PostMessage(m_hwnd, WM_USER + 1, 0, 0);
    }).detach();
}

void MainWindow::OnUninstallClick()
{
    if (m_isInstalling || m_isUninstalling)
        return;
        
    m_isUninstalling = true;
    EnableWindow(m_btnInstall, FALSE);
    EnableWindow(m_btnUninstall, FALSE);
    
    SetWindowText(m_lblStatus, L"Uninstalling...");
    AddLog("Starting uninstallation process...");
    
    std::thread([this]() {
        AddLog("Scanning for installed files...");
        PostMessage(m_hwnd, WM_USER + 3, 20, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        AddLog("No installation found");
        PostMessage(m_hwnd, WM_USER + 3, 100, 0);
        PostMessage(m_hwnd, WM_USER + 2, 0, 0);
    }).detach();
}

void MainWindow::AddLog(const std::string& message)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, nullptr, 0);
    std::wstring wstr(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, &wstr[0], len);
    
    int textLen = GetWindowTextLength(m_logEdit);
    
    SendMessage(m_logEdit, EM_SETSEL, textLen, textLen);
    
    if (textLen > 0)
    {
        SendMessage(m_logEdit, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
    }
    
    SendMessage(m_logEdit, EM_REPLACESEL, FALSE, (LPARAM)wstr.c_str());
    
    SendMessage(m_logEdit, EM_SCROLLCARET, 0, 0);
}
