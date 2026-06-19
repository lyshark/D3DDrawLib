#include "LyDrawLib.h"

LyDrawLib* LyDrawLib::m_Instance = NULL;

// 回调函数
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		LyDrawLib::Start()->Render();
		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, LyDrawLib::Start()->getMARGINS());
		break;

	case WM_DESTROY:
		exit(1);
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

// 开始绘制
LyDrawLib* LyDrawLib::Start()
{
	if (NULL == m_Instance)
	{
		m_Instance = new LyDrawLib();
		m_Instance->m_nWidth = GetSystemMetrics(SM_CXSCREEN);
		m_Instance->m_nHeight = GetSystemMetrics(SM_CYSCREEN);
		m_Instance->m_Margin = new MARGINS{ 0, 0, m_Instance->m_nWidth, m_Instance->m_nHeight };
	}
	return m_Instance;
}

// 获取窗口句柄
DWORD LyDrawLib::FindWindowHwnd(const char* title, const char* className)
{
	return (DWORD)FindWindowA(className, title);
}

// 得到窗体宽度
DWORD LyDrawLib::GetWindowWidth(const char* title, const char* className)
{
	RECT rect;

	DWORD hwnd = (DWORD)FindWindowA(className, title);

	if (hwnd != NULL)
	{
		GetWindowRect((HWND)hwnd, &rect);
		DWORD width = rect.right - rect.left;
		return width;
	}
	return 0;
}

// 得到窗体高度
DWORD LyDrawLib::GetWindowHeight(const char* title, const char* className)
{
	RECT rect;

	DWORD hwnd = (DWORD)FindWindowA(className, title);

	if (hwnd != NULL)
	{
		GetWindowRect((HWND)hwnd, &rect);
		DWORD height = rect.bottom - rect.top;
		return height;
	}
	return 0;
}

// 获取进程ID通过窗口句柄
DWORD LyDrawLib::GetProcessIdByHwnd(DWORD hWnd)
{
	DWORD pid;
	GetWindowThreadProcessId((HWND)hWnd, &pid);
	return pid;
}

// 获取进程ID通过进程名
DWORD LyDrawLib::GetProcessIdByName(LPCTSTR  name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
		if (lstrcmpi(pe.szExeFile, name) == 0) {
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}

// 取指定进程PID里模块句柄
DWORD LyDrawLib::GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName)
{
	MODULEENTRY32 moduleEntry;
	HANDLE handle = NULL;
	handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (!handle) {
		CloseHandle(handle);
		return NULL;
	}
	ZeroMemory(&moduleEntry, sizeof(MODULEENTRY32));
	moduleEntry.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(handle, &moduleEntry)) {
		CloseHandle(handle);
		return NULL;
	}
	do {
		if (lstrcmpi(moduleEntry.szModule, moduleName) == 0) {
			return (DWORD)moduleEntry.hModule;
		}
	} while (Module32Next(handle, &moduleEntry));
	CloseHandle(handle);
	return 0;
}

// 获取进程句柄
HANDLE LyDrawLib::GetProcessHandle(DWORD pid)
{
	return OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
}

// 创建透明窗体
bool LyDrawLib::createWindow(std::string GameTitle, std::string GameClassName, std::string fontPath, void(*DrawThread)())
{

	m_strGameTitle = GameTitle;
	m_strGameClassName = GameClassName;
	m_strFontName = fontPath;
	pDrawThread = DrawThread;

	// 设置窗口属性
	WNDCLASSEX wClass;
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = NULL;
	wClass.lpfnWndProc = WinProc;
	wClass.lpszClassName = L" ";
	wClass.lpszMenuName = L" ";
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wClass))
		exit(1);

	m_hGameHwnd = FindWindowA(m_strGameClassName.c_str(), m_strGameTitle.c_str());
	if (m_hGameHwnd)
	{
		GetWindowRect(m_hGameHwnd, &m_tGameSize);
		m_nWidth = m_tGameSize.right - m_tGameSize.left;
		m_nHeight = m_tGameSize.bottom - m_tGameSize.top;
		m_hWindow = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW, L" ", L" ", WS_POPUP, 1, 1, m_nWidth, m_nHeight, 0, 0, wClass.hInstance, 0);
		SetLayeredWindowAttributes(m_hWindow, 0, 1.0f, LWA_ALPHA);
		SetLayeredWindowAttributes(m_hWindow, 0, RGB(0, 0, 0), LWA_COLORKEY);
		ShowWindow(m_hWindow, SW_SHOW);

		m_nWindowHandle = m_hWindow;
	}

	D3dInit(m_hWindow, m_nWidth, m_nHeight, m_strFontName.c_str());
	while (true)
	{
		m_hGameHwnd = FindWindowA(m_strGameClassName.c_str(), m_strGameTitle.c_str());
		DWORD Pid;
		GetWindowThreadProcessId(m_hGameHwnd, &Pid);
		if (m_hGameHwnd)
		{
			GetWindowRect(m_hGameHwnd, &m_tGameSize);
			m_nWidth = m_tGameSize.right - m_tGameSize.left;
			m_nHeight = m_tGameSize.bottom - m_tGameSize.top;
			DWORD dwStyle = GetWindowLong(m_hGameHwnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				m_tGameSize.top += 23;
				m_nHeight -= 23;
			}
			MoveWindow(m_hWindow, m_tGameSize.left, m_tGameSize.top, m_nWidth, m_nHeight, true);
		}
		else
		{
			m_pObject->Release();
			m_pDevice->Release();
			exit(1);
		}

		if (PeekMessage(&m_Message, m_hWindow, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&m_Message);
			TranslateMessage(&m_Message);
		}
		Sleep(1);
	}
	return true;
}

// 刷新窗体
void LyDrawLib::Render()
{
	D3DRender(m_hGameHwnd, pDrawThread);
}

// 绘制立体框
void LyDrawLib::Box3D(int HeadX, int HeadY, int bottomX, int bottomY, D3DCOLOR Color)
{
	DrawLine(bottomX - 5, bottomY, bottomX + 5, bottomY, 1.5, Color);
	DrawLine(HeadX - 5, HeadY, HeadX + 5, HeadY, 1.5, Color);
	DrawLine(HeadX - 5, HeadY, bottomX - 5, bottomY, 1.5, Color);
	DrawLine(HeadX + 5, HeadY, bottomX + 5, bottomY, 1.5, Color);
	DrawLine(HeadX - 7, HeadY - 10, bottomX - 7, bottomY - 10, 1.5, Color);
	DrawLine(bottomX - 5, bottomY, bottomX - 7, bottomY - 10, 1.5, Color);
	DrawLine(HeadX - 5, HeadY, HeadX - 7, HeadY - 10, 1.5, Color);
	DrawLine((HeadX + 5) + 7, HeadY - 10, (bottomX + 5) + 7, bottomY - 10, 1.5, Color);
	DrawLine(bottomX + 5, bottomY, (bottomX + 5) + 7, bottomY - 10, 1.5, Color);
	DrawLine(HeadX + 5, HeadY, (HeadX + 5) + 7, HeadY - 10, 1.5, Color);
	DrawLine(HeadX - 7, HeadY - 10, (HeadX + 5) + 7, HeadY - 10, 1.5, Color);
	DrawLine(bottomX - 7, bottomY - 10, (bottomX + 5) + 7, bottomY - 10, 1.5, Color);
}

// 绘制线条
void LyDrawLib::DrawLine(float x, float y, float x2, float y2, float width, D3DCOLOR color)
{
	D3DXVECTOR2 dLine[2];

	m_pLine->SetWidth(width);

	dLine[0].x = x;
	dLine[0].y = y;

	dLine[1].x = x2;
	dLine[1].y = y2;

	m_pLine->Draw(dLine, 2, color);
}

// 平面横线
void LyDrawLib::PlaneHorizontalLine(float left, float top, float size, float width, D3DCOLOR color)
{
	float v_left = left;
	float v_top = top;
	float v_left_size = left + size;
	float v_t = top;

	DrawLine(v_left, v_top, v_left_size, v_t, width, color);
}

// 平面竖线
void LyDrawLib::PlaneVerticalLine(float left, float top, float size, float height, D3DCOLOR color)
{
	float v_left = left;
	float v_top = top;
	float v_t = top + size;

	DrawLine(v_left, v_top, v_left, v_t, height, color);
}

// 绘制文本
void LyDrawLib::DrawString(const char* fmt, int fontsize, int x, int y, D3DCOLOR color)
{
	//string 转 LPCWSTR
	size_t size = m_strFontName.length();
	wchar_t fontName[100];
	MultiByteToWideChar(CP_ACP, 0, m_strFontName.c_str(), size, fontName, size * sizeof(wchar_t));
	D3DXCreateFont(m_pDevice, fontsize, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName, &m_pFontSmall);
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;

	CHAR buf[124] = { '\0' };
	va_list va_alist;
	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);

	m_pFontSmall->DrawTextA(0, buf, -1, &FontPos, DT_NOCLIP, color);
	m_pFontSmall->Release();
}

// 描边文本
void LyDrawLib::DrawColourString(const char* fmt, int fontsize, int x, int y, D3DCOLOR colour, D3DCOLOR color)
{
	DrawString(fmt, fontsize, x + 1, y, colour);
	DrawString(fmt, fontsize, x - 1, y, colour);

	DrawString(fmt, fontsize, x, y + 1, colour);
	DrawString(fmt, fontsize, x, y - 1, colour);

	DrawString(fmt, fontsize, x, y, color);
}

// 绘制双重文字
void LyDrawLib::DrawStringAndString(const char* str, const char* fmt, int fontsize, int offset, int x, int y, D3DCOLOR colorA, D3DCOLOR colorB)
{
	RECT FontPos, fmtPos;
	FontPos.left = x;
	FontPos.top = y;

	ID3DXFont* g_font = NULL;

	CHAR format[128] = { '\0' };
	CHAR string[128] = { '\0' };

	D3DXCreateFont(m_pDevice, fontsize, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 0, &m_pFontSmall);

	// 文字提示
	va_list va_alist;
	va_start(va_alist, str);
	vsprintf_s(format, str, va_alist);
	va_end(va_alist);

	m_pFontSmall->DrawTextA(0, format, -1, &FontPos, DT_NOCLIP, colorA);

	// 命令选择
	fmtPos.left = x + offset;
	fmtPos.top = y;

	va_list vb_alist;
	va_start(vb_alist, fmt);
	vsprintf_s(string, fmt, vb_alist);
	va_end(vb_alist);

	m_pFontSmall->DrawTextA(0, string, -1, &fmtPos, DT_NOCLIP, colorB);
}

// 绘制实心矩形1
void LyDrawLib::DrawFilledRectangle(float x, float y, float w, float h, D3DCOLOR color)
{
	D3DRECT rect = { x, y, x + w, y + h };
	m_pDevice->Clear(1, &rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

// 绘制实心矩形2
void LyDrawLib::DrawFilledRectangleB(int x, int y, int w, int h, D3DCOLOR Color)
{
	struct Vertex
	{
		float x, y, z, ht;
		DWORD Color;
	};

	Vertex V[8] = { 0 };

	V[0].Color = V[1].Color = V[2].Color = V[3].Color = Color;
	V[0].z = V[1].z = V[2].z = V[3].z = 0.0f;
	V[0].ht = V[1].ht = V[2].ht = V[3].ht = 0.0f;
	V[0].x = V[1].x = (float)x;
	V[0].y = V[2].y = (float)(y + h);
	V[1].y = V[3].y = (float)y;
	V[2].x = V[3].x = (float)(x + w);

	m_pDevice->SetTexture(0, NULL);

	// 设置缓冲区格式
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	// 画两个三角形组成实体矩形
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));
}

// 绘制空心矩形
void LyDrawLib::DrawBorderRectangle(float x, float y, float w, float h, float width, D3DCOLOR color)
{
	DrawLine(x, y - 1, x + w, y - 1, width, color);
	DrawLine(x - 1, y, x - 1, y + h, width, color);
	DrawLine(x + w, y, x + w, y + h, width, color);
	DrawLine(x, y + h - 1, x + w, y + h - 1, width, color);
}

// 画圆
void LyDrawLib::DrawCircle(float x, float y, float r, float px, D3DCOLOR color)
{
	if (m_pLine)
	{
		float d = 1.25f - r;
		float tx = 0, ty = r, fx = r / 1.4f;
		while ((int)tx != (int)fx)
		{

			if (d < 0)
				d += 2 * tx + 3;
			else
			{
				d += 2 * (tx - ty) + 5;
				--ty;
			}

			// 八分画法
			DrawPoint(x + tx, y + ty, px, color);
			DrawPoint(x + tx, y - ty, px, color);
			DrawPoint(x - tx, y + ty, px, color);
			DrawPoint(x - tx, y - ty, px, color);
			DrawPoint(x + ty, y - tx, px, color);
			DrawPoint(x + ty, y + tx, px, color);
			DrawPoint(x - ty, y + tx, px, color);
			DrawPoint(x - ty, y - tx, px, color);
			++tx;
		}
	}
}

// 画点
void LyDrawLib::DrawPoint(float x, float y, float px, D3DCOLOR color)
{
	DrawLine(x, y, x + 2, y + 2, px, color);
}

// 画十字准星
void LyDrawLib::DrawCollimation(float left, float top, D3DCOLOR color, int type)
{
	if (type == 1)
	{
		PlaneHorizontalLine(left - 10, top - 3, 7, 7, color);
		PlaneVerticalLine(left - 7, top - 6, 7, 7, color);
	}
	if (type == 2)
	{
		PlaneHorizontalLine(left - 8, top, 6, 1, color);
		PlaneHorizontalLine(left + 4, top, 6, 1, color);
		PlaneVerticalLine(left, top - 8, 6, 1, color);
		PlaneVerticalLine(left, top + 4, 6, 1, color);
	}
	if (type == 3)
	{
		DrawFilledRectangle(left, top, 2, 2, color);
		PlaneHorizontalLine(left - 12, top, 8, 1, color);
		PlaneHorizontalLine(left + 6, top, 8, 1, color);
		PlaneVerticalLine(left, top - 12, 8, 1, color);
		PlaneVerticalLine(left, top + 6, 8, 1, color);
	}
}

// 绘制血框
void LyDrawLib::DrawBloodFrame(float left, float top, float height, float width, int px, D3DCOLOR color)
{
	// 绘制竖向线条
	PlaneVerticalLine(left, top, height, px, color);
	PlaneVerticalLine(left + width, top, height, px, color);

	// 绘制横向线条
	PlaneHorizontalLine(left, top, width, px, color);
	PlaneHorizontalLine(left, top + height, width, px, color);
}

// 竖向血条(基础)
void LyDrawLib::DrawBlood_Vertical(float Hp, float x, float y, int width, int height, D3DCOLOR color)
{
	if (Hp < 0.0f) return;
	float temp1 = width / 100 * Hp;
	// 从上往下减血
	DrawLine(x, y, x + temp1, y, height, color);
}

// 竖向血条(封装)
void LyDrawLib::DrawBlood_Horizontal(float Hp, float x, float y, int width, int height, D3DCOLOR color, int style)
{
	// style = 0 从下往上减血  style = 1 从上往下减血
	if (Hp < 0.0f)
	{
		return;
	}
	float temp1 = height / 100 * Hp;
	float temp2 = (height / 100 * 100) - temp1;
	if (style == BLOOD_UPTODOWN)
	{
		// 从上往下减血
		DrawLine(x, y + height, x, y + temp2, width, color);
	}
	else if (style == BLOOD_DOWNTOUP)
	{
		// 从下往上减血
		DrawLine(x, y, x, y + temp1, width, color);
	}
}

// 封装版透视方框
void LyDrawLib::DrawPerspectiveRect(float Hp, float left, float top, int width, int height)
{
	if (Hp >= 85)
	{
		// 竖向血条 [血量 / 坐标x / 坐标y / 宽度 / 高度 / 颜色 / 类型]
		DrawBlood_Horizontal(Hp - 3, left + 5, top - 1, 7, height, D3DCOLOR_RGBA(1, 0, 238, 238), 1);
	}
	else if (Hp >= 65)
	{
		DrawBlood_Horizontal(Hp - 3, left + 5, top - 1, 7, height, D3DCOLOR_RGBA(1, 100, 195, 194), 1);
	}
	else if (Hp >= 45)
	{
		DrawBlood_Horizontal(Hp - 3, left + 5, top - 1, 7, height, D3DCOLOR_RGBA(1, 155, 185, 185), 1);
	}
	else
	{
		DrawBlood_Horizontal(Hp - 3, left + 5, top - 1, 7, height, D3DCOLOR_RGBA(255, 0, 0, 255), 1);
	}

	// 绘制血框 [坐标x / 坐标y / 高度 / 宽度 / 线条粒度 / 颜色]
	DrawBloodFrame(left, top, height, 10, 1, D3DCOLOR_RGBA(255, 0, 0, 255));

	// 透视方框 [坐标x / 坐标y / 宽度 / 高度 / 线条粒度 / 颜色]
	DrawBorderRectangle(left + 15, top, width, height, 1, D3DCOLOR_RGBA(254, 0, 0, 184));
}

// 绘制四角方框
void LyDrawLib::DrawFourCornersBox(float x, float y, float w, float h, float px, D3DCOLOR color)
{
	DrawLine(x, y, x + w*0.33, y, px, color);
	DrawLine(x, y, x, y + h*0.33, px, color);

	DrawLine(x + w * 0.66, y, x + w + 1, y, px, color);
	DrawLine(x + w, y, x + w, y + h * 0.33, px, color);

	DrawLine(x, y + h, x + w * 0.33, y + h, px, color);
	DrawLine(x, y + h * 0.66, x, y + h + 1, px, color);

	DrawLine(x + w * 0.66, y + h, x + w, y + h, px, color);
	DrawLine(x + w, y + h * 0.66, x + w, y + h + 1, px, color);
}

// 绘制角框
void LyDrawLib::DrawCornerFrame(float x, float y, float x2, float y2, float width, D3DCOLOR color)
{
	DrawLine(x, y, x + x2 * 0.33, y, width, color);
	DrawLine(x + x2 * 0.66, y, x + x2, y, width, color);

	DrawLine(x, y, x, y + y2 * 0.33, width, color);

	DrawLine(x, y + y2 * 0.66, x, y + y2, width, color);

	DrawLine(x + x2, y, x + x2, y + y2 * 0.33, width, color);

	DrawLine(x + x2, y + y2 * 0.66, x + x2, y + y2, width, color);
	DrawLine(x, y + y2, x + x2 * 0.33, y + y2, width, color);
	DrawLine(x + x2 * 0.66, y + y2, x + x2, y + y2, width, color);
}

// 中心血条
void LyDrawLib::DrawCenterBlood(float x, float y, float px, int distance, int hp, bool is_death)
{
	int color = 0;

	if (is_death == FALSE)
	{
		if (hp >= 75)
		{
			color = D3DCOLOR_RGBA(254, 0, 0, 184);
		}
		if (hp < 75 && hp >= 35)
		{
			color = D3DCOLOR_RGBA(211, 0, 0, 10);
		}
		if (hp < 35)
		{
			color = D3DCOLOR_RGBA(10, 0, 0, 54);
		}
	}

	color = D3DCOLOR_RGBA(254, 0, 0, 184);

	// 绘制填充方框
	DrawFilledRectangle(x - px / 2, y - 10, px - distance / 100, 5, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawFilledRectangle(x - px / 2 + 1, y - 9, hp*(px - distance / 100) / 100, 3, color);
}

// 横向血条
void LyDrawLib::DrawBlood_Transverse(float Hp, float x, float y, float width, float height, D3DCOLOR colorA, D3DCOLOR colorB)
{
	float temp_1 = 0, temp_2 = 0, temp_3 = 0, temp_4 = 0;

	temp_1 = x;
	temp_2 = y + height + 1;

	if (Hp >= 100)
	{
		Hp = 100;
	}

	temp_4 = width * Hp / 100;

	// 绘制矩形
	DrawBorderRectangle(temp_1, temp_2, width, 6, 1, colorA);

	// 填充方框
	DrawFilledRectangle(temp_1 + 1, temp_2 + 1, temp_4 - 2, 4, colorB);
}

// 画超级准星
void LyDrawLib::DrawSuperCollimation(float left, float top, D3DCOLOR color, int type)
{
	if (type == 1)
	{
		// 平面竖线
		PlaneVerticalLine(left, top, 1, 1, color);

		// 绘制圆形
		DrawCircle(left, top, 10, 1, color);

		// 平面横线
		PlaneHorizontalLine(left - 14, top, 8, 1, color);

		// 平面竖线
		PlaneVerticalLine(left, top - 12, 8, 1, color);

		// 平面横线
		PlaneHorizontalLine(left + 6, top, 8, 1, color);

		// 平面竖线
		PlaneVerticalLine(left, top + 6, 8, 1, color);
	}

	if (type == 2)
	{
		// 平面横线
		PlaneHorizontalLine(left - 12, top, 25, 1, color);

		// 平面竖线
		PlaneVerticalLine(left, top - 12, 25, 1, color);

		// 绘制线条
		DrawLine(left + 12, top + 12, left + 8, top + 8, 1, color);
		DrawLine(left - 12, top + 12, left - 8, top + 8, 1, color);
		DrawLine(left + 12, top - 12, left + 8, top - 8, 1, color);
		DrawLine(left - 12, top - 12, left - 8, top - 8, 1, color);

		// 平面竖线
		PlaneVerticalLine(left, top - 2, 5, 5, color);
	}

	if (type == 3)
	{
		// 平面横线
		PlaneHorizontalLine(left - 12, top, 25, 1, color);

		// 平面竖线
		PlaneVerticalLine(left, top - 12, 25, 1, color);
	}

	if (type == 4)
	{
		// 平面横线
		PlaneHorizontalLine(0, top, left * 2, 1, color);

		// 平面竖线
		PlaneVerticalLine(left, 0, top * 2, 1, color);
	}
}

// 绘制雷达
void LyDrawLib::DrawRadar(float left, float top)
{
	// 平面横线
	PlaneHorizontalLine(left - 80, top, 155, 2, D3DCOLOR_RGBA(254, 0, 0, 184));

	// 平面竖线
	PlaneVerticalLine(left, top - 80, 155, 2, D3DCOLOR_RGBA(254, 0, 0, 184));

	// 实心矩形
	DrawFilledRectangle(left - 3, top - 3, 6, 6, D3DCOLOR_RGBA(133, 0, 0, 147));

	// 空心矩形
	DrawBorderRectangle(left - 3, top - 3, 6, 6, 1, D3DCOLOR_RGBA(254, 0, 0, 184));
}

// 描边文本2
void LyDrawLib::DrawColourString2(const char* fmt, int fontsize, int left, int top, D3DCOLOR color)
{
	DrawString(fmt, fontsize, left - 1, top, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left + 1, top, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left, top - 1, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left, top + 1, D3DCOLOR_RGBA(254, 0, 0, 184));

	DrawString(fmt, fontsize, left, top, color);
}

// 描边文本3
void LyDrawLib::DrawColourString3(const char* fmt, int fontsize, int left, int top, D3DCOLOR color)
{
	DrawString(fmt, fontsize, left - 1, top, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left + 1, top, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left, top, D3DCOLOR_RGBA(254, 0, 0, 184));
}

// 初始化D3d绘图引擎
void LyDrawLib::D3dInit(HWND hWnd, int Width, int Height, const char* fontPath)
{
	m_strFontName = fontPath;
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &m_pObject)))
		exit(1);

	ZeroMemory(&m_pParams, sizeof(m_pParams));
	m_pParams.Windowed = TRUE;
	m_pParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_pParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_pParams.BackBufferWidth = Width;
	m_pParams.BackBufferHeight = Height;
	m_pParams.EnableAutoDepthStencil = TRUE;
	m_pParams.AutoDepthStencilFormat = D3DFMT_D16;
	m_pParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_pParams, 0, &m_pDevice)))
		exit(1);

	if (!m_pLine)
		D3DXCreateLine(m_pDevice, &m_pLine);
}

void LyDrawLib::D3DRender(HWND hGameHwnd, void(*pDrawThread)())
{
	if (m_pDevice == nullptr) return;

	if (hGameHwnd == GetForegroundWindow())
	{
		m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		m_pDevice->BeginScene();

		if (pDrawThread)
			pDrawThread();

		m_pDevice->EndScene();
		m_pDevice->PresentEx(0, 0, 0, 0, 0);
	}
}