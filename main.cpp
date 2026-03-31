#include "MainWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MainWindow window;
    
    if (!window.Create())
    {
        return -1;
    }
    
    window.Show();
    
    return window.MessageLoop();
}