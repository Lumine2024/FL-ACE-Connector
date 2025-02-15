#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include<windows.h>
#include"D:/MyHeader/myheader.h"
PAINTSTRUCT ps;
HDC hdc;
HWND hbutton1, hbutton2, FL = nullptr, ACE = nullptr, Connector, hdelay, hbutton3;
constexpr UINT nbutton1 = 1, nbutton2 = 2, ndelay = 3, nbutton3 = 4;
std::fstream file;
long long delaytime = 0;
bool isplaying = false, spacedown = false, enterdown = false;
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

BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lp) {
	TCHAR WndTitle[256];
	GetWindowText(hwnd, WndTitle, 256);
	std::vector<std::_tstring> *pTitles = reinterpret_cast<std::vector<std::_tstring>*>(lp);
	pTitles->emplace_back(WndTitle);
	return TRUE;
}

HWND FindWndByName(const std::_tstring &Title, const std::_tstring &ClassName) {
	std::vector<std::_tstring> WndTitles;
	EnumWindows(EnumProc, reinterpret_cast<LPARAM>(&WndTitles));
	for(const auto &WndName : WndTitles) {
		if(WndName.find(Title) != std::_tstring::npos) {
			HWND ret = FindWindow(ClassName.c_str(), WndName.c_str());
			if(ret) return ret;
		}
	}
	return nullptr;
}

LRESULT CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	UINT buttonid = LOWORD(wp), code = HIWORD(wp);
	switch (msg) {
		case WM_CREATE: {
			hbutton1 = CreateWindow(_T("button"), _T("Play / Stop"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 80, 60, 200, 30, hwnd, (HMENU)nbutton1, 0, 0);
			hbutton2 = CreateWindow(_T("button"), _T("Set Window"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 80, 140, 200, 30, hwnd, (HMENU)nbutton2, 0, 0);
			hbutton3 = CreateWindow(_T("button"), _T("Save"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 330, 200, 50, 30, hwnd, (HMENU)nbutton3, 0, 0);
			hdelay = CreateWindow(_T("edit"), MY::ToW(delaytime), WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 200, 180, 30, hwnd, (HMENU)ndelay, 0, 0);
		}
			return 0;
		case WM_DESTROY: PostQuitMessage(0); return 0;
		case WM_PAINT: {
			hdc = BeginPaint(hwnd, &ps);
			TextOut(hdc, 20, 20, _T("FL_ACE Connector"), _tcslen(_T("FL_ACE Connector")));
			TextOut(hdc, 20, 205, _T("Delaytime(ms):"), _tcslen(_T("Delaytime(ms):")));
			TextOut(hdc, 20, 240, _T("Copyright (c) Oxyane OH2. All rights reserved."), _tcslen(_T("Copyright (c) Oxyane OH2. All rights reserved.")));
			TextOut(hdc, 20, 270, _T("Bilibili: https://space.bilibili.com/3494380801362515"), _tcslen(_T("Bilibili: https://space.bilibili.com/3494380801362515")));
			TextOut(hdc, 20, 300, _T("Github: https://github.com/Lumine2024"), _tcslen(_T("Github: https://github.com/Lumine2024")));
			EndPaint(hwnd, &ps);
			return 0;
		}
		case WM_COMMAND: {
			if (code == BN_CLICKED) {
				if (buttonid == nbutton1) {
					if (!FL || !ACE) {
						MessageBox(Connector, _T("Error: please set FL and ACE windows first."),
							_T("FL_ACE Connector"), MB_OK);
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
						MessageBox(Connector, _T("Please stop playing before setting windows."),
							_T("FL_ACE Connector"), MB_OK);
						return 0;
					}
					MessageBox(Connector, _T("Please set FL Studio foreground.\nIt shouldn't be maximized."),
						_T("FL_ACE Connector"), MB_OK);
					while (GetForegroundWindow() == Connector);
					Sleep(300);
					FL = GetForegroundWindow();
					MessageBox(FL, _T("Please set ACE Studio foreground."), _T("FL_ACE Connector"), MB_OK);
					while (GetForegroundWindow() == FL);
					Sleep(300);
					ACE = GetForegroundWindow();
					MessageBox(ACE, _T("Success!"), _T("FL_ACE Connector"), MB_OK);
				}
				else if (buttonid == nbutton3) {
					TCHAR chartime[128];
					GetWindowText(hdelay, chartime, 128);
					delaytime = MY::ToLong(chartime);
					save();
					MessageBox(Connector, _T("Success!"), _T("FL_ACE Connector"), MB_OK);
				}
			}
		}
		return 0;
		case WM_KEYDOWN: {
			int key = wp;
			if(key == VK_SPACE) {
				if(spacedown) return 0;
				spacedown = true;
			}
			else if(key == VK_RETURN) {
				if(enterdown) return 0;
				enterdown = true;
				TCHAR chartime[128];
				GetWindowText(hdelay, chartime, 128);
				delaytime = MY::ToLong(chartime);
				save();
				MessageBox(Connector, _T("Success!"), _T("FL_ACE Connector"), MB_OK);
			}
			return 0;
		}
		case WM_KEYUP: {
			int key = wp;
			if(key == VK_SPACE) {
				spacedown = false;
				if(!FL || !ACE) {
					MessageBox(Connector, _T("Error: please set FL and ACE windows first."),
						_T("FL_ACE Connector"), MB_OK);
					return 0;
				}
				else if(isplaying) {
					SetForegroundWindow(FL);
					kd(VK_SPACE);
					SetForegroundWindow(ACE);
					kd(VK_SPACE);
					SetForegroundWindow(Connector);
					isplaying = !isplaying;
				}
				else {
					if(delaytime >= 0) {
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
			else if(key == VK_RETURN) {
				enterdown = false;
			}
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}
int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR Cmd, _In_ int nCmd) {
	load();
	WNDCLASSEX wnd = { 0 };
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpszClassName = _T("C");
	wnd.lpfnWndProc = MainProc;
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	if (!RegisterClassEx(&wnd)) {
		MessageBox(0, _T("Registry failed. Please retry."), _T("FL_ACE Connector"), MB_OK);
		return 1;
	}
	HWND hwnd = CreateWindow(_T("C"), _T("FL_ACE Connector"), WS_OVERLAPPEDWINDOW,
		200, 60, 500, 500, 0, 0, hInst, 0);
	if(!hwnd) {
		MessageBox(0, _T("Unable to create window. Please retry."), _T("FL_ACE Connector"), MB_OK);
		return 1;
	}
	Connector = hwnd;
	FL = FindWndByName(_T("FL"), _T("TFruityLoopsMainForm"));
	ACE = FindWndByName(_T("ACE Studio"), _T("Qt663QWindowIcon"));
	if(FL && ACE) {
		MessageBox(Connector, _T("Auto match complete."),
			_T("FL_ACE Connector"), MB_OK);
	}
	else {
		MessageBox(Connector, _T("Auto match failed, please match manually."),
			_T("FL_ACE Connector"), MB_OK);
	}
	ShowWindow(hwnd, nCmd);
	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(_T("C"), hInst);
	save();
	return 0;
}
