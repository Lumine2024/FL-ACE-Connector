#include <iostream>
#include <windows.h>
#include <graphics.h>
using namespace std;
void kd(BYTE vk) {
	keybd_event(vk, 0, 0, 0);
	Sleep(1);
	keybd_event(vk, 0, 2, 0);
	Sleep(1);
}
//我不知道为什么，但这个东西只能在DEBUG模式中运行，所以生成的文件异常的大
int main() {
	HWND fl;
	HWND ACE;
	initgraph(900, 600, 0);
	settextcolor(RGB(102, 204, 255));
	settextstyle(30, 0, "华文楷体");
	outtextxy(50, 50, "请打开FL工程文件后将FL Studio前置，以便程序记录窗口");
	outtextxy(50, 90, "请不要前置其他窗口，否则程序可能会出故障");
	outtextxy(50, 130, "请不要直接前置最大化的FL Studio");
	HWND Bridge = FindWindow("EasyXWnd", "FL_ACE Connector");
	while (Bridge == GetForegroundWindow());
	Sleep(150);
	fl = GetForegroundWindow();
	outtextxy(50, 170, "已记录FL窗口，现在请前置ACE Studio");
	outtextxy(50, 210, "请不要前置其他窗口，否则程序可能会出故障");
	while (fl == GetForegroundWindow());
	Sleep(150);
	ACE = GetForegroundWindow();
	outtextxy(100, 260, "完成！");
	outtextxy(50, 300, "再次进入本界面开始播放/暂停");
	outtextxy(100, 340, "在任务栏中右键程序以退出");
	while (1) {
		if (Bridge == GetForegroundWindow()) {
			SetForegroundWindow(ACE);
			kd(VK_SPACE);
			SetForegroundWindow(fl);
			kd(VK_SPACE);
			SetForegroundWindow(ACE);
		}
	}
	return 0;
}
