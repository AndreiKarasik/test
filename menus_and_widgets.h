#ifndef MENUS_AND_WIDGETS_H_INCLUDED
#define MENUS_AND_WIDGETS_H_INCLUDED


void MainWndAddMenus(HWND hWnd) // main window menus
{
    HMENU RootMenu = CreateMenu();
    HMENU SubMenu = CreateMenu();

    ComPortSubMenu = CreateMenu();
    ComPortListMenu = CreateMenu();

    AppendMenu(ComPortSubMenu, MF_STRING, OnConnectRequest, L"Connect"); // connect to port (submenu)
    AppendMenu(ComPortSubMenu, MF_SEPARATOR, 0, NULL); // separator
    AppendMenu(ComPortSubMenu, MF_STRING, OnSerialRefresh, L"Refresh ports"); // update list of ports (submenu)
    AppendMenu(ComPortSubMenu, MF_POPUP, (UINT_PTR)ComPortListMenu, L"Selected port"); // select port (submenu)

    AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)ComPortSubMenu, L"Connection"); // main menu
    AppendMenu(RootMenu, MF_STRING, params, L"Turntable parameters"); // main menu
    AppendMenu(RootMenu, MF_STRING, NULL, L"Graphics"); // main menu
    AppendMenu(RootMenu, MF_STRING, NULL, L"Help"); // main menu

    SetMenu(hWnd, RootMenu); // setting
}


void MainWndAddWidgets(HWND hWnd) // main window widgets
{
    RECT rc{};
    GetClientRect(hWnd, &rc);

    hStaticControl = CreateWindowA("static", "Welcome to the BMSD control app", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 0+10, rc.right, 20, hWnd, NULL, NULL, NULL); // welcome_text or COM port number
    hEditControl = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER | ES_MULTILINE | WS_VSCROLL, 40, 70+10, 420, 150, hWnd, NULL, NULL, NULL); // text from file (time & speed)

    open_file_but =  CreateWindow(L"button", L"Open file", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER | BS_PUSHBUTTON, 20, 25+10, 200, 40, hWnd, (HMENU)open_file, NULL, NULL); // open file
    send_but =  CreateWindow(L"button", L"Send commands", WS_VISIBLE | ES_CENTER | WS_CHILD | WS_BORDER | BS_PUSHBUTTON, 260, 25+10, 200, 40, hWnd, (HMENU)send_commands, NULL, NULL); // send commands from file
    emergency_stop_but = CreateWindow(L"button", L"STOP turntable", WS_VISIBLE | WS_BORDER | WS_CHILD | ES_CENTER | BS_PUSHBUTTON, 0, 225+10, rc.right, 50, hWnd, (HMENU)emergency_stop_button, NULL, NULL); // stop button
}


void TTParamsWidgets(HWND hWnd) // turntable params widgets
{
    rho_t = CreateWindowA("static", "rho", WS_VISIBLE | WS_CHILD | ES_RIGHT, 10, 10, 80, 20, hWnd, NULL, NULL, NULL); // names
    kappa_t = CreateWindowA("static", "kappa", WS_VISIBLE | WS_CHILD | ES_RIGHT, 10, 31, 80, 20, hWnd, NULL, NULL, NULL);
    epsilon_t = CreateWindowA("static", "epsilon", WS_VISIBLE | WS_CHILD | ES_RIGHT, 10, 52, 80, 20, hWnd, NULL, NULL, NULL);
    mu_t = CreateWindowA("static", "mu", WS_VISIBLE | WS_CHILD | ES_RIGHT, 10, 73, 80, 20, hWnd, NULL, NULL, NULL);
    sigma_t = CreateWindowA("static", "sigma", WS_VISIBLE | WS_CHILD | ES_RIGHT, 10, 94, 80, 20, hWnd, NULL, NULL, NULL);

    text_enter = CreateWindowA("static", "enter parameters or upload them", WS_VISIBLE | WS_CHILD | ES_RIGHT, 10, 120, 230, 20, hWnd, NULL, NULL, NULL); // upload params text

    rho_e = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER | ES_MULTILINE, 100, 10, 80, 20, hWnd, NULL, NULL, NULL); // edit values
    kappa_e = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER | ES_MULTILINE, 100, 31, 80, 20, hWnd, NULL, NULL, NULL);
    epsilon_e = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER | ES_MULTILINE, 100, 52, 80, 20, hWnd, NULL, NULL, NULL);
    mu_e = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER | ES_MULTILINE, 100, 73, 80, 20, hWnd, NULL, NULL, NULL);
    sigma_e = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER | ES_MULTILINE, 100, 94, 80, 20, hWnd, NULL, NULL, NULL);

    upload_parameters_but = CreateWindow(L"button", L"upload", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 245, 115, 55, 30, hWnd, NULL, NULL, NULL); // upload params text
}


#endif // MENUS_AND_WIDGETS_H_INCLUDED
