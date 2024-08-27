#define _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include"MyStringAlgorithm.hpp"//Go find my previous file
PAINTSTRUCT ps;
HDC hdc;
static HWND hbutton1{}, hbutton2{}, FL = 0, ACE = 0, Connector, hdelay{}, hbutton3;
const UINT nbutton1 = 1, nbutton2 = 2, ndelay = 3, nbutton3 = 4;
std::fstream file;
static long long delaytime = 0;
static bool isplaying = false;
void kd(BYTE vk) {
	keybd_event(vk, 0, 0, 0);
	Sleep(1);
	keybd_event(vk, 0, 2, 0);
	Sleep(1);
}
void load() {
	file.open("FL_ACE Connector.txt", std::ios::in);
	file >> delaytime;
	file.close();
}
void save() {
	file.open("FL_ACE Connector.txt", std::ios::out);
	file << delaytime;
	file.close();
}

LRESULT CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	UINT buttonid = LOWORD(wp), code = HIWORD(wp);
	switch (msg) {
	case WM_CREATE: {
		hbutton1=CreateWindow(L"button", L"Play/Stop", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 80, 60, 200, 30, hwnd, (HMENU)nbutton1, 0, 0);
		hbutton2 = CreateWindow(L"button", L"Set Window", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 80, 140, 200, 30, hwnd, (HMENU)nbutton2, 0, 0);
		hbutton3 = CreateWindow(L"button", L"Save", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 330, 200, 50, 30, hwnd, (HMENU)nbutton3, 0, 0);
		hdelay = CreateWindow(L"edit", ToW(delaytime), WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 200, 180, 30, hwnd, (HMENU)ndelay, 0, 0);
	}
		return 0;
	case WM_DESTROY: PostQuitMessage(0); return 0;
	case WM_PAINT:{
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 20, 20, L"FL_ACE Connector", _tcslen(L"FL_ACE Connector"));
		TextOut(hdc, 20, 205, L"Delaytime(ms):", _tcslen(L"Delaytime(ms):"));
		TextOut(hdc, 20, 240, L"Copyright (c) Oxyane OH2. All rights reserved.", _tcslen(L"Copyright (c) Oxyane OH2. All rights reserved."));
		TextOut(hdc, 20, 270, L"Bilibili: https://space.bilibili.com/3494380801362515", _tcslen(L"Bilibili: https://space.bilibili.com/3494380801362515"));
		TextOut(hdc, 20, 300, L"Github: https://github.com/Lumine2024", _tcslen(L"Github: https://github.com/Lumine2024"));
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_COMMAND: {
		if (code == BN_CLICKED) {
			if (buttonid == nbutton1) {
				if (FL == 0 || ACE == 0) {
					MessageBox(Connector, L"Error: please set FL and ACE windows first.", L"FL_ACE Connector", MB_OK);
					return 0;
				}
				else if (isplaying) {
					SetForegroundWindow(FL);
					kd(VK_SPACE);
					SetForegroundWindow(ACE);
					kd(VK_SPACE);
					SetForegroundWindow(Connector);
					isplaying = !isplaying;
				}
				else {
					if (delaytime >= 0) {
						SetForegroundWindow(FL);
						kd(VK_SPACE);
						SetForegroundWindow(Connector);
						Sleep(delaytime);
						SetForegroundWindow(ACE);
						kd(VK_SPACE);
						SetForegroundWindow(Connector);
					}
					else {
						SetForegroundWindow(ACE);
						kd(VK_SPACE);
						SetForegroundWindow(Connector);
						Sleep(-1 * delaytime);
						SetForegroundWindow(FL);
						kd(VK_SPACE);
						SetForegroundWindow(Connector);
					}
					isplaying = !isplaying;
				}
			}
			else if (buttonid == nbutton2) {
				if (isplaying) {
					MessageBox(Connector, L"Please stop playing before setting windows.", L"FL_ACE Connector", MB_OK);
					return 0;
				}
				MessageBox(Connector, L"Please set FL Studio foreground.\nIt shouldn't be maximized.", L"FL_ACE Connector", MB_OK);
				while (GetForegroundWindow() == Connector);
				Sleep(300);
				FL = GetForegroundWindow();
				MessageBox(FL, L"Please set ACE Studio foreground.", L"FL_ACE Connector", MB_OK);
				while (GetForegroundWindow() == FL);
				Sleep(300);
				ACE = GetForegroundWindow();
				MessageBox(ACE, L"Success!", L"FL_ACE Connector", MB_OK);
			}
			else if (buttonid == nbutton3) {
				wchar_t chartime[128];
				GetWindowText(hdelay, chartime, 128);
				delaytime = ToLong(chartime);
				save();
				MessageBox(Connector, L"Success!", L"FL_ACE Connector", MB_OK);
			}
			}
		}
			return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}
int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR nCmdLine, _In_ int nCmdShow) {
	load();
	WNDCLASSEX wnd = { 0 };
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpszClassName = L"C";
	wnd.lpfnWndProc = MainProc;
	if (!RegisterClassEx(&wnd)) {
		MessageBox(0, L"注册失败", L"错误", 0);
		return 1;
	}
	HWND hwnd = CreateWindow(L"C", L"FL_ACE Connector", WS_OVERLAPPEDWINDOW, 200, 60, 500, 500, 0, 0, hInst, 0);
	Connector = hwnd;
	ShowWindow(hwnd, nCmdShow);
	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(L"C", hInst);
	save();
	return 0;
}
