#define UNICODE

#include <Windows.h>
#include <string>
#include <iostream>
#include <cmath>
#include <fstream>
#include <time.h>

using namespace std;

#include "definitions.h"
#include "resource.h"
#include "Connection.h"
#include "menus_and_widgets.h"


unsigned char OWI_COMPUTECRC8(unsigned char* inData, unsigned char seed) // CRC computing function
{
    unsigned char bitsLeft;
    unsigned char temp;

    for (bitsLeft = 8; bitsLeft > 0; bitsLeft--)
    {
        temp = ((seed ^ *inData) & 0x01);
        if (temp == 0)
        {
            seed >>= 1;
        }
        else
        {
            seed ^= 0x18;
            seed >>= 1;
            seed |= 0x80;
        }
        *inData >>= 1;
    }
    return seed;
}

unsigned char getSeed(unsigned char* line)
{
    int i;
    unsigned char seed = 0;
    int size = sizeof(G_CMD_test) / sizeof(G_CMD_test[0]);
    for (i = 1; i < size; i++)
    {
        seed = OWI_COMPUTECRC8((G_CMD_test+i), seed);
    }

    return seed;
}

double file()
{
    double Omega;
    double T;

    double Omega_ = 28.;
    double U_max = 120.;
    double U_min = 16.;

    ifstream file ("test.txt");
    do
    {
        file >> Omega >> T;
    }
    while (!file.eof());

    cout << Omega << " " << T;

    int U_0 = round(U_min + (Omega / Omega_) * (U_max - U_min));
    unsigned char U = (unsigned char) U_0;
    cout << U_0 << endl;
    return U_0;
    file.close();
}

void CenterWindow(HWND hwnd) // MainWindow centering function
{
    RECT rc = {0};

    GetWindowRect(hwnd, &rc);
    int win_w = rc.right - rc.left;
    int win_h = rc.bottom - rc.top;

    int screen_w = GetSystemMetrics(SM_CXSCREEN);
    int screen_h = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(hwnd, HWND_TOP, (screen_w - win_w)/2,
                 (screen_h - win_h)/2, 0, 0, SWP_NOSIZE);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    SoftwareMainClass.style		    = CS_HREDRAW | CS_VREDRAW;
    SoftwareMainClass.lpfnWndProc   = SoftwareMainProcedure;
    SoftwareMainClass.hInstance	    = hInst;
    SoftwareMainClass.hIcon		    = LoadIcon (hInst, MAKEINTRESOURCE(IDI_ICON2));
    SoftwareMainClass.hCursor	    = LoadCursor (NULL, IDC_ARROW);
    SoftwareMainClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    SoftwareMainClass.lpszMenuName  = NULL;
    SoftwareMainClass.lpszClassName = L"Main";

    if (!RegisterClassW(&SoftwareMainClass))
    {
        return -1;
    }
    MSG SoftwareMainMessage = { 0 };

    hwnd_0 = CreateWindow(L"Main", L"NavLab", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 380, NULL, NULL, hInst, NULL);

    ShowWindow(hwnd_0,ncmdshow);
    UpdateWindow(hwnd_0);

    while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL))
    {
        TranslateMessage(&SoftwareMainMessage);
        DispatchMessage(&SoftwareMainMessage);
    }

    TerminateThread(readThread, 0);
    return 0;
}

void ExitSoftware(void)
{
    isConnected = false;
    isThreading = false;
    CloseHandle(connectedPort);
    CloseHandle(readThread);
    ExitThread(0);
    PostQuitMessage(0);
}

void LoadData(LPCSTR path)
{
    HANDLE FileToLoad = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    DWORD bytesIterated;
    ReadFile(FileToLoad, Buffer, TextBufferSize, &bytesIterated, NULL);

    SetWindowTextA(hEditControl, Buffer);

    CloseHandle(FileToLoad);
}

void SetOpenFileParams(HWND hwnd_0)
{
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd_0;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = NULL;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_COMMAND:

        if ((wp >= ComSelectIndex) && (wp < ComSelectIndex + ComPortAmount))
        {
            selectedPort = wp - ComSelectIndex;
            SetWindowStatus("PORT: " + std::to_string(selectedPort));
            SerialUpdate();
            break;
        }
        switch (wp)
        {
        case OnConnectRequest:
            ConnectRequest();
            break;
        case emergency_stop_button:
            SerialWriteSTOP();
            break;
        case send_commands:
            SerialWriteTEST();
            break;
        case open_file:
            if (GetOpenFileNameA(&ofn))
            {
                LoadData(filename);
            }
            break;
        case params:
            TTClass.style		  = CS_HREDRAW | CS_VREDRAW;
            TTClass.lpfnWndProc   = TTParamsProcedure;
            TTClass.hInstance	  = hInst;
            TTClass.hIcon		  = LoadIcon (NULL, IDI_APPLICATION);
            TTClass.hCursor	      = LoadCursor (NULL, IDC_ARROW);
            TTClass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
            TTClass.lpszMenuName  = NULL;
            TTClass.lpszClassName = L"tt";

            if (!RegisterClassW(&TTClass))
            {
                return -1;
            }

            tt = CreateWindow(L"tt", L"Turntable parameters", WS_VISIBLE | WS_BORDER | WS_OVERLAPPEDWINDOW, 100, 100, 330, 200, hwnd_0, NULL, hInst, NULL);
            break;
        case OnSerialRefresh:
            SerialUpdate();
            break;
        case OnExitSoftware:
            PostQuitMessage(0);
            break;
        default:
            break;
        }
        break;

    case WM_SIZE:
    {
        RECT rc{};
        GetClientRect(hWnd, &rc);
        SetWindowPos(hStaticControl, NULL, 0, 0, rc.right, 20, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
    }
    break;
    case WM_CREATE:
        MainWndAddMenus(hWnd);
        MainWndAddWidgets(hWnd);
        SerialUpdate();
        SetOpenFileParams(hWnd);
        readThread = CreateThread(NULL, 0, SerialRead, NULL, 0, NULL);
        CenterWindow(hWnd);
        break;
    case WM_DESTROY: // close mainwindow
        ExitSoftware();
        break;
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
}

LRESULT CALLBACK TTParamsProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        TTParamsWidgets(hwnd);
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

void SetWindowStatus(std::string status)
{
    SetWindowTextA(hStaticControl, status.c_str());
}
