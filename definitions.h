#ifndef BMSD_H_INCLUDED
#define BMSD_H_INCLUDED

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define open_file             1
#define OnSerialRefresh		  2
#define OnConnectRequest	  3
#define OnExitSoftware		  4
#define emergency_stop_button 5
#define solve_but             6
#define params                7
#define send_commands         8

#define TextBufferSize		  256
#define ComSelectIndex		  120
#define ComPortAmount		  50

CONST DWORD ReadTimeout = 1;

int selectedPort = 1;
int targetBaudRate = 9600;

char Buffer[TextBufferSize];
int U_0;

OPENFILENAMEA ofn;
char filename[260];
volatile bool isConnected = false;
volatile bool isThreading = true;

HANDLE connectedPort;
HANDLE readThread;

HWND hStaticControl, text_enter;
HWND rho_t, kappa_t, sigma_t, epsilon_t, mu_t;
HWND rho_e, kappa_e, sigma_e, epsilon_e, mu_e;
HWND hEditControl;
HWND hNumberControl;
HWND open_file_but, send_but, emergency_stop_but, test_but;

HWND upload_parameters_but;

HWND hwnd_0, tt;
HINSTANCE hInst;

HMENU ComPortSubMenu;
HMENU ComPortListMenu;

double Omega_ = 28.;
double U_max = 120.;
double U_min = 16.;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK TTParamsProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS TTClass, SoftwareMainClass;

void MainWndAddMenus(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void SetOpenFileParams(HWND hWnd);
void SetWindowStatus(std::string status);

unsigned char getSeed(unsigned char* line);
unsigned char G_CMD_test[] = {0xE6, 0x00, 0xA3, 0x78};
unsigned char CRC = getSeed(G_CMD_test);

#endif // BMSD_H_INCLUDED
