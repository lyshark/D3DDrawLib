#include "D3DDrawLib.h"

int 窗口宽度 = 0;
int 窗口高度 = 0;

static int 当前选中 = 1;

bool 是否显示 = true;
bool 功能项A = false;
bool 功能项B = false;
bool 功能项C = false;
bool 功能项D = false;
bool 功能项E = false;
bool 功能项F = false;
bool 功能项G = false;

// 输出菜单
void Menu()
{
	if (是否显示 == true)
	{
		int 间距位置 = 0;
		int 菜单最左 = 窗口宽度 / 30;
		int 菜单最高 = 窗口高度 / 3;

		间距位置 = 间距位置 + 10;

		// 选择条大小
		D3DDrawLib::Start()->DrawBorderRectangle(菜单最左 + 5, 菜单最高 + 9 + (当前选中 - 1) * 25, 110, 20, 1, D3DCOLOR_XRGB(148, 0, 211));

		// 外边框大小
		D3DDrawLib::Start()->DrawBorderRectangle(菜单最左, 菜单最高, 120, 190, 1, D3DCOLOR_XRGB(255, 69, 0));

		D3DDrawLib::Start()->DrawStringAndString("功能项A ", 功能项A == true ? "[开启]" : "[关闭]", 12, 65, 菜单最左 + 10, 菜单最高 + 间距位置, D3DCOLOR_XRGB(255, 52, 179), 功能项A == true ? D3DCOLOR_XRGB(0, 255, 0) : D3DCOLOR_XRGB(255, 250, 250));
		间距位置 = 间距位置 + 25;

		D3DDrawLib::Start()->DrawStringAndString("功能项B ", 功能项B == true ? "[开启]" : "[关闭]", 12, 65, 菜单最左 + 10, 菜单最高 + 间距位置, D3DCOLOR_XRGB(255, 52, 179), 功能项B == true ? D3DCOLOR_XRGB(0, 255, 0) : D3DCOLOR_XRGB(255, 250, 250));
		间距位置 = 间距位置 + 25;

		D3DDrawLib::Start()->DrawStringAndString("功能项C ", 功能项C == true ? "[开启]" : "[关闭]", 12, 65, 菜单最左 + 10, 菜单最高 + 间距位置, D3DCOLOR_XRGB(255, 52, 179), 功能项C == true ? D3DCOLOR_XRGB(0, 255, 0) : D3DCOLOR_XRGB(255, 250, 250));
		间距位置 = 间距位置 + 25;

		D3DDrawLib::Start()->DrawStringAndString("功能项D ", 功能项D == true ? "[开启]" : "[关闭]", 12, 65, 菜单最左 + 10, 菜单最高 + 间距位置, D3DCOLOR_XRGB(255, 52, 179), 功能项D == true ? D3DCOLOR_XRGB(0, 255, 0) : D3DCOLOR_XRGB(255, 250, 250));
		间距位置 = 间距位置 + 25;

		D3DDrawLib::Start()->DrawStringAndString("功能项E ", 功能项E == true ? "[开启]" : "[关闭]", 12, 65, 菜单最左 + 10, 菜单最高 + 间距位置, D3DCOLOR_XRGB(255, 52, 179), 功能项E == true ? D3DCOLOR_XRGB(0, 255, 0) : D3DCOLOR_XRGB(255, 250, 250));
		间距位置 = 间距位置 + 25;

		D3DDrawLib::Start()->DrawStringAndString("功能项F ", 功能项F == true ? "[开启]" : "[关闭]", 12, 65, 菜单最左 + 10, 菜单最高 + 间距位置, D3DCOLOR_XRGB(255, 52, 179), 功能项F == true ? D3DCOLOR_XRGB(0, 255, 0) : D3DCOLOR_XRGB(255, 250, 250));
		间距位置 = 间距位置 + 25;

		D3DDrawLib::Start()->DrawStringAndString("功能项G ", 功能项G == true ? "[开启]" : "[关闭]", 12, 65, 菜单最左 + 10, 菜单最高 + 间距位置, D3DCOLOR_XRGB(255, 52, 179), 功能项G == true ? D3DCOLOR_XRGB(0, 255, 0) : D3DCOLOR_XRGB(255, 250, 250));
		间距位置 = 间距位置 + 35;

		D3DDrawLib::Start()->DrawString("HOME 显示/隐藏", 15, 菜单最左 + 9, 菜单最高 + 间距位置, D3DCOLOR_RGBA(255, 0, 0, 255));
	}
}

// 控制器函数
void MyFunctionCallBack()
{
	MSG msg = { 0 };

	GetMessage(&msg, NULL, 0, 0);

	switch (msg.message)
	{
	case WM_HOTKEY:
	{
		// 上光标
		if (1 == msg.wParam)
		{
			// 当前选中是7 因为我们有7个功能
			当前选中 = 当前选中 - 1;
			if (当前选中 == 0)
			{
				当前选中 = 7;
			}
		}

		// 下光标
		else if (2 == msg.wParam)
		{
			当前选中 = 当前选中 + 1;
			if (当前选中 == 8)
			{
				当前选中 = 1;
			}
		}

		// 左光标
		else if (3 == msg.wParam)
		{
			if (当前选中 == 1)
			{
				功能项A = false;
			}
			else if (当前选中 == 2)
			{
				功能项B = false;
			}
			else if (当前选中 == 3)
			{
				功能项C = false;
			}
			else if (当前选中 == 4)
			{
				功能项D = false;
			}
			else if (当前选中 == 5)
			{
				功能项E = false;
			}
			else if (当前选中 == 6)
			{
				功能项F = false;
			}
			else if (当前选中 == 7)
			{
				功能项G = false;
			}
		}

		// 右光标
		else if (4 == msg.wParam)
		{
			if (当前选中 == 1)
			{
				功能项A = true;
			}
			else if (当前选中 == 2)
			{
				功能项B = true;
			}
			else if (当前选中 == 3)
			{
				功能项C = true;
			}
			else if (当前选中 == 4)
			{
				功能项D = true;
			}
			else if (当前选中 == 5)
			{
				功能项E = true;
			}
			else if (当前选中 == 6)
			{
				功能项F = true;
			}
			else if (当前选中 == 7)
			{
				功能项G = true;
			}
		}

		// home键
		else if (5 == msg.wParam)
		{
			if (是否显示 == true)
			{
				是否显示 = false;
			}
			else if (是否显示 == false)
			{
				是否显示 = true;
			}
		}
		break;
	}
	default:
		break;
	}
}

void Draw()
{
	Menu();
	MyFunctionCallBack();
}

int main(int argc, char *argv[])
{
	// 注册 [上下左右] 热键
	RegisterHotKey(NULL, 1, 0, VK_UP);
	RegisterHotKey(NULL, 2, 0, VK_DOWN);
	RegisterHotKey(NULL, 3, 0, VK_LEFT);
	RegisterHotKey(NULL, 4, 0, VK_RIGHT);
	RegisterHotKey(NULL, 5, 0, VK_HOME);

	// 运行线程
	const char* class_name = "DrawingBoard";

	窗口宽度 = D3DDrawLib::Start()->GetWindowWidth(class_name, class_name);
	窗口高度 = D3DDrawLib::Start()->GetWindowHeight(class_name, class_name);
	D3DDrawLib::Start()->createWindow(class_name, class_name, "黑体", Draw);
	return 0;
}