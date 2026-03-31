#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <gdiplus.h>

class MainWindow
{
public:
    MainWindow();
    ~MainWindow();
    
    bool Create();
    void Show();
    int MessageLoop();
    
private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    void CreateControls();
    void OnInstallClick();
    void OnUninstallClick();
    void AddLog(const std::string& message);
    
    void DrawLeftPanel(Gdiplus::Graphics* graphics);
    void DrawStatusPanel(Gdiplus::Graphics* graphics);
    
    HWND m_hwnd;
    HWND m_btnInstall;
    HWND m_btnUninstall;
    HWND m_progressBar;
    HWND m_logEdit;
    HWND m_checkTerms;
    HWND m_lblStatus;
    
    bool m_isInstalling;
    bool m_isUninstalling;
    int m_progressValue;
    
    ULONG_PTR m_gdiplusToken;
};

#endif
