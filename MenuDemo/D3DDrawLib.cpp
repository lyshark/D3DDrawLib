#include "D3DDrawLib.h"

D3DDrawLib* D3DDrawLib::m_Instance = NULL;

// 回调函数
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		D3DDrawLib::Start()->Render();
		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, D3DDrawLib::Start()->getMARGINS());
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
D3DDrawLib* D3DDrawLib::Start()
{
	if (NULL == m_Instance)
	{
		m_Instance = new D3DDrawLib();
		m_Instance->m_nWidth = GetSystemMetrics(SM_CXSCREEN);
		m_Instance->m_nHeight = GetSystemMetrics(SM_CYSCREEN);
		m_Instance->m_Margin = new MARGINS{ 0, 0, m_Instance->m_nWidth, m_Instance->m_nHeight };
	}
	return m_Instance;
}

// 获取窗口句柄
DWORD D3DDrawLib::FindWindowHwnd(const char* title, const char* className)
{
	return (DWORD)FindWindowA(className, title);
}

// 得到窗体宽度
DWORD D3DDrawLib::GetWindowWidth(const char* title, const char* className)
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
DWORD D3DDrawLib::GetWindowHeight(const char* title, const char* className)
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
DWORD D3DDrawLib::GetProcessIdByHwnd(DWORD hWnd)
{
	DWORD pid;
	GetWindowThreadProcessId((HWND)hWnd, &pid);
	return pid;
}

// 获取进程ID通过进程名
DWORD D3DDrawLib::GetProcessIdByName(LPCTSTR  name)
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
DWORD D3DDrawLib::GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName)
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
HANDLE D3DDrawLib::GetProcessHandle(DWORD pid)
{
	return OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
}

// 创建透明窗体
bool D3DDrawLib::createWindow(std::string GameTitle, std::string GameClassName, std::string fontPath, void(*DrawThread)())
{
	// 保存输入参数到类成员变量
	m_strGameTitle = GameTitle;
	m_strGameClassName = GameClassName;
	m_strFontName = fontPath;
	pDrawThread = DrawThread;

	// 设置窗口属性
	WNDCLASSEX wClass;
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;

	// 背景色为黑色
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));

	// 使用默认箭头光标
	wClass.hCursor = LoadCursor(0, IDC_ARROW);

	// 使用默认应用程序图标
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);

	// 使用默认小图标
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = NULL;

	// 窗口过程函数
	wClass.lpfnWndProc = WinProc;

	// 窗口类名
	wClass.lpszClassName = L" ";

	// 窗口菜单名
	wClass.lpszMenuName = L" ";

	// 窗口样式：垂直和水平重绘
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	// 注册窗口类
	if (!RegisterClassEx(&wClass))
		exit(1);

	// 查找进程窗口句柄
	m_hGameHwnd = FindWindowA(m_strGameClassName.c_str(), m_strGameTitle.c_str());
	if (m_hGameHwnd)
	{
		// 获取窗口的尺寸
		GetWindowRect(m_hGameHwnd, &m_tGameSize);
		m_nWidth = m_tGameSize.right - m_tGameSize.left;
		m_nHeight = m_tGameSize.bottom - m_tGameSize.top;

		// 创建透明窗体
		m_hWindow = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW, L" ", L" ", WS_POPUP, 1, 1, m_nWidth, m_nHeight, 0, 0, wClass.hInstance, 0);

		// 设置透明度
		SetLayeredWindowAttributes(m_hWindow, 0, 1.0f, LWA_ALPHA);

		// 设置透明颜色键
		SetLayeredWindowAttributes(m_hWindow, 0, RGB(0, 0, 0), LWA_COLORKEY);

		// 显示窗体
		ShowWindow(m_hWindow, SW_SHOW);

		m_nWindowHandle = m_hWindow;
	}

	// 初始化D3D绘图
	D3dInit(m_hWindow, m_nWidth, m_nHeight, m_strFontName.c_str());

	// 进入消息循环
	while (true)
	{
		// 查找进程窗口句柄
		m_hGameHwnd = FindWindowA(m_strGameClassName.c_str(), m_strGameTitle.c_str());
		DWORD Pid;
		GetWindowThreadProcessId(m_hGameHwnd, &Pid);
		if (m_hGameHwnd)
		{
			// 获取窗口的尺寸
			GetWindowRect(m_hGameHwnd, &m_tGameSize);
			m_nWidth = m_tGameSize.right - m_tGameSize.left;
			m_nHeight = m_tGameSize.bottom - m_tGameSize.top;
			DWORD dwStyle = GetWindowLong(m_hGameHwnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				// 如果有边框，调整窗口尺寸
				m_tGameSize.top += 23;
				m_nHeight -= 23;
			}
			MoveWindow(m_hWindow, m_tGameSize.left, m_tGameSize.top, m_nWidth, m_nHeight, true);
		}
		else
		{
			// 如果找不到窗口，释放资源并退出
			m_pObject->Release();
			m_pDevice->Release();
			exit(1);
		}

		// 处理消息队列
		if (PeekMessage(&m_Message, m_hWindow, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&m_Message);
			TranslateMessage(&m_Message);
		}
		// 休眠1毫秒，降低CPU占用
		Sleep(1);
	}
	return true;
}

// 刷新窗体
void D3DDrawLib::Render()
{
	D3DRender(m_hGameHwnd, pDrawThread);
}

// ---------------------------------------------------------------------------------
// 线条绘制部分
// ---------------------------------------------------------------------------------

// 绘制线条
void D3DDrawLib::DrawLine(float x, float y, float x2, float y2, float width, D3DCOLOR color)
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
void D3DDrawLib::PlaneHorizontalLine(float left, float top, float size, float width, D3DCOLOR color)
{
	float v_left = left;
	float v_top = top;
	float v_left_size = left + size;
	float v_t = top;

	DrawLine(v_left, v_top, v_left_size, v_t, width, color);
}

// 平面竖线
void D3DDrawLib::PlaneVerticalLine(float left, float top, float size, float height, D3DCOLOR color)
{
	float v_left = left;
	float v_top = top;
	float v_t = top + size;

	DrawLine(v_left, v_top, v_left, v_t, height, color);
}

// ---------------------------------------------------------------------------------
// 进度条与方框部分
// ---------------------------------------------------------------------------------

// 绘制实心矩形
void D3DDrawLib::DrawFilledRectangle(float x, float y, float w, float h, D3DCOLOR color)
{
	D3DRECT rect = { x, y, x + w, y + h };
	m_pDevice->Clear(1, &rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

// 绘制空心矩形
void D3DDrawLib::DrawBorderRectangle(float x, float y, float w, float h, float width, D3DCOLOR color)
{
	DrawLine(x, y - 1, x + w, y - 1, width, color);
	DrawLine(x - 1, y, x - 1, y + h, width, color);
	DrawLine(x + w, y, x + w, y + h, width, color);
	DrawLine(x, y + h - 1, x + w, y + h - 1, width, color);
}

// 绘制四角方框
void D3DDrawLib::DrawFourCornersBox(float x, float y, float w, float h, float px, D3DCOLOR color)
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
void D3DDrawLib::DrawCornerFrame(float x, float y, float x2, float y2, float width, D3DCOLOR color)
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

// 绘制立体框
void D3DDrawLib::Box3D(int HeadX, int HeadY, int bottomX, int bottomY, D3DCOLOR Color)
{
	// 底部水平线
	DrawLine(bottomX - 5, bottomY, bottomX + 5, bottomY, 1.5, Color);
	// 顶部水平线
	DrawLine(HeadX - 5, HeadY, HeadX + 5, HeadY, 1.5, Color);
	// 左侧竖线
	DrawLine(HeadX - 5, HeadY, bottomX - 5, bottomY, 1.5, Color);
	// 右侧竖线
	DrawLine(HeadX + 5, HeadY, bottomX + 5, bottomY, 1.5, Color);
	// 顶部水平偏移线
	DrawLine(HeadX - 7, HeadY - 10, bottomX - 7, bottomY - 10, 1.5, Color);
	// 底部水平偏移线
	DrawLine(bottomX - 5, bottomY, bottomX - 7, bottomY - 10, 1.5, Color);
	// 左侧竖线偏移线
	DrawLine(HeadX - 5, HeadY, HeadX - 7, HeadY - 10, 1.5, Color);
	// 顶部右侧水平偏移线
	DrawLine((HeadX + 5) + 7, HeadY - 10, (bottomX + 5) + 7, bottomY - 10, 1.5, Color);
	// 底部右侧水平偏移线
	DrawLine(bottomX + 5, bottomY, (bottomX + 5) + 7, bottomY - 10, 1.5, Color);
	// 右侧竖线偏移线
	DrawLine(HeadX + 5, HeadY, (HeadX + 5) + 7, HeadY - 10, 1.5, Color);
	// 顶部水平偏移线
	DrawLine(HeadX - 7, HeadY - 10, (HeadX + 5) + 7, HeadY - 10, 1.5, Color);
	// 底部水平偏移线
	DrawLine(bottomX - 7, bottomY - 10, (bottomX + 5) + 7, bottomY - 10, 1.5, Color);
}

// 绘制进度条
void D3DDrawLib::DrawBloodFrame(float left, float top, float height, float width, int px, D3DCOLOR color)
{
	// 绘制竖向线条
	PlaneVerticalLine(left, top, height, px, color);
	PlaneVerticalLine(left + width, top, height, px, color);

	// 绘制横向线条
	PlaneHorizontalLine(left, top, width, px, color);
	PlaneHorizontalLine(left, top + height, width, px, color);
}

// 竖向进度条(基础)
void D3DDrawLib::DrawBlood_Vertical(float Hp, float x, float y, int width, int height, D3DCOLOR color)
{
	if (Hp < 0.0f) return;
	float temp1 = height / 100.0f * Hp;
	// 从下向上减
	// DrawLine(x, y + height, x, y + height - temp1, width, color);

	// 从上向下减
	DrawLine(x, y, x, y + temp1, width, color);
}

// 横向进度条(基础)
void D3DDrawLib::DrawBlood_Horizontal(float Hp, float x, float y, int width, int height, D3DCOLOR color)
{
	if (Hp < 0.0f) return;
	float temp1 = width / 100.0f * Hp;
	// 从左向右减
	DrawLine(x, y, x + temp1, y, height, color);
}

// 竖向进度条(封装)
void D3DDrawLib::DrawBlood_VerticalNew(float Hp, float x, float y, int width, int height, D3DCOLOR color, int style)
{
	// style = 0 从下往上减  style = 1 从上往下减
	if (Hp < 0.0f)
	{
		return;
	}
	float temp1 = height / 100 * Hp;
	float temp2 = (height / 100 * 100) - temp1;
	if (style == BLOOD_UPTODOWN)
	{
		// 从上往下减
		DrawLine(x, y + height, x, y + temp2, width, color);
	}
	else if (style == BLOOD_DOWNTOUP)
	{
		// 从下往上减
		DrawLine(x, y, x, y + temp1, width, color);
	}
}

// 横向进度条(封装)
void D3DDrawLib::DrawBlood_HorizontalNew(float Hp, float x, float y, int width, int height, D3DCOLOR color, int style)
{
	// style = 0 从左向右减  style = 1 从右向左减
	if (Hp < 0.0f)
	{
		return;
	}
	float temp1 = width / 100.0f * Hp;
	float temp2 = (width / 100.0f * 100) - temp1;
	if (style == BLOOD_UPTODOWN)
	{
		// 从左向右减
		DrawLine(x, y, x + temp1, y, height, color);
	}
	else if (style == BLOOD_DOWNTOUP)
	{
		// 从右向左减
		DrawLine(x + width, y, x + temp2, y, height, color);
	}
}

// 带有进度条的方框（左侧）
void D3DDrawLib::DrawPerspectiveRectLeft(float Hp, float left, float top, int width, int height)
{
	if (Hp >= 85)
	{
		// 竖向进度条 [进度 / 坐标x / 坐标y / 宽度 / 高度 / 颜色 / 类型]
		DrawBlood_VerticalNew(Hp - 3, left + 5, top - 1, 7, height, D3DCOLOR_RGBA(1, 0, 238, 238), 1);
	}
	else if (Hp >= 65)
	{
		DrawBlood_VerticalNew(Hp - 3, left + 5, top - 1, 7, height, D3DCOLOR_RGBA(1, 100, 195, 194), 1);
	}
	else if (Hp >= 45)
	{
		DrawBlood_VerticalNew(Hp - 3, left + 5, top - 1, 7, height, D3DCOLOR_RGBA(1, 155, 185, 185), 1);
	}
	else
	{
		DrawBlood_VerticalNew(Hp - 3, left + 5, top - 1, 7, height, D3DCOLOR_RGBA(255, 0, 0, 255), 1);
	}

	// 绘制进度条的方框 [坐标x / 坐标y / 高度 / 宽度 / 线条粒度 / 颜色]
	DrawBloodFrame(left, top, height, 10, 1, D3DCOLOR_RGBA(255, 0, 0, 255));

	// 绘制方框 [坐标x / 坐标y / 宽度 / 高度 / 线条粒度 / 颜色]
	DrawBorderRectangle(left + 15, top, width, height, 1, D3DCOLOR_RGBA(254, 0, 0, 184));
}

// 带有进度条的方框（右侧）
void D3DDrawLib::DrawPerspectiveRectRight(float Hp, float left, float top, int width, int height)
{
	// 计算进度条的位置和宽度
	float progressWidth = 10;
	float progressLeft = left + width + 5;  // 将进度条绘制在方框右侧

	if (Hp >= 85)
	{
		// 竖向进度条 [进度 / 坐标x / 坐标y / 宽度 / 高度 / 颜色 / 类型]
		DrawBlood_VerticalNew(Hp - 3, progressLeft + 5, top - 1, progressWidth - 3, height, D3DCOLOR_RGBA(1, 0, 238, 238), 1);
	}
	else if (Hp >= 65)
	{
		DrawBlood_VerticalNew(Hp - 3, progressLeft + 5, top - 1, progressWidth - 3, height, D3DCOLOR_RGBA(1, 100, 195, 194), 1);
	}
	else if (Hp >= 45)
	{
		DrawBlood_VerticalNew(Hp - 3, progressLeft + 5, top - 1, progressWidth - 3, height, D3DCOLOR_RGBA(1, 155, 185, 185), 1);
	}
	else
	{
		DrawBlood_VerticalNew(Hp - 3, progressLeft + 5, top - 1, progressWidth - 3, height, D3DCOLOR_RGBA(255, 0, 0, 255), 1);
	}

	// 绘制进度条的方框 [坐标x / 坐标y / 高度 / 宽度 / 线条粒度 / 颜色]
	DrawBloodFrame(progressLeft, top, height, progressWidth, 1, D3DCOLOR_RGBA(255, 0, 0, 255));

	// 方框 [坐标x / 坐标y / 宽度 / 高度 / 线条粒度 / 颜色]
	DrawBorderRectangle(left, top, width, height, 1, D3DCOLOR_RGBA(254, 0, 0, 184));
}

// 带有进度条的方框（顶部）
void D3DDrawLib::DrawPerspectiveRectTop(float Hp, float left, float top, int width, int height)
{
	// 计算进度条的位置和高度
	float progressHeight = 10;
	float progressTop = top - progressHeight - 5;  // 将进度条绘制在方框顶部

	if (Hp >= 85)
	{
		// 横向进度条 [进度 / 坐标x / 坐标y / 宽度 / 高度 / 颜色 / 类型]
		DrawBlood_HorizontalNew(Hp - 3, left, progressTop + 5, width, progressHeight, D3DCOLOR_RGBA(1, 0, 238, 238), BLOOD_UPTODOWN);
	}
	else if (Hp >= 65)
	{
		DrawBlood_HorizontalNew(Hp - 3, left, progressTop + 5, width, progressHeight, D3DCOLOR_RGBA(1, 100, 195, 194), BLOOD_UPTODOWN);
	}
	else if (Hp >= 45)
	{
		DrawBlood_HorizontalNew(Hp - 3, left, progressTop + 5, width, progressHeight, D3DCOLOR_RGBA(1, 155, 185, 185), BLOOD_UPTODOWN);
	}
	else
	{
		DrawBlood_HorizontalNew(Hp - 3, left, progressTop + 5, width, progressHeight, D3DCOLOR_RGBA(255, 0, 0, 255), BLOOD_UPTODOWN);
	}

	// 绘制进度条的方框 [坐标x / 坐标y / 宽度 / 高度 / 线条粒度 / 颜色]
	DrawBloodFrame(left, progressTop, progressHeight, width, 1, D3DCOLOR_RGBA(255, 0, 0, 255));

	// 方框 [坐标x / 坐标y / 宽度 / 高度 / 线条粒度 / 颜色]
	DrawBorderRectangle(left, top, width, height, 1, D3DCOLOR_RGBA(254, 0, 0, 184));
}

// 带有进度条的方框（底部）
void D3DDrawLib::DrawPerspectiveRectBottom(float Hp, float left, float top, int width, int height)
{
	// 计算进度条的位置和高度
	float progressHeight = 10;
	float progressBottom = top + height + 5;  // 将进度条绘制在方框底部

	if (Hp >= 85)
	{
		// 横向进度条 [进度 / 坐标x / 坐标y / 宽度 / 高度 / 颜色 / 类型]
		DrawBlood_HorizontalNew(Hp - 3, left, progressBottom + 5, width, progressHeight, D3DCOLOR_RGBA(1, 0, 238, 238), BLOOD_UPTODOWN);
	}
	else if (Hp >= 65)
	{
		DrawBlood_HorizontalNew(Hp - 3, left, progressBottom + 5, width, progressHeight, D3DCOLOR_RGBA(1, 100, 195, 194), BLOOD_UPTODOWN);
	}
	else if (Hp >= 45)
	{
		DrawBlood_HorizontalNew(Hp - 3, left, progressBottom + 5, width, progressHeight, D3DCOLOR_RGBA(1, 155, 185, 185), BLOOD_UPTODOWN);
	}
	else
	{
		DrawBlood_HorizontalNew(Hp - 3, left, progressBottom + 5, width, progressHeight, D3DCOLOR_RGBA(255, 0, 0, 255), BLOOD_UPTODOWN);
	}

	// 绘制进度条的方框 [坐标x / 坐标y / 宽度 / 高度 / 线条粒度 / 颜色]
	DrawBloodFrame(left, progressBottom, progressHeight, width, 1, D3DCOLOR_RGBA(255, 0, 0, 255));

	// 方框 [坐标x / 坐标y / 宽度 / 高度 / 线条粒度 / 颜色]
	DrawBorderRectangle(left, top, width, height, 1, D3DCOLOR_RGBA(254, 0, 0, 184));
}

// 横向进度条
void D3DDrawLib::DrawBlood_Transverse(float Hp, float x, float y, float width, float height, D3DCOLOR colorA, D3DCOLOR colorB)
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

// 竖向进度条
void D3DDrawLib::DrawLeft_Vertical(float Hp, float x, float y, float width, float height, D3DCOLOR colorA, D3DCOLOR colorB)
{
	float temp1 = x;
	float temp2 = y;

	if (Hp >= 100)
	{
		Hp = 100;
	}

	float temp4 = height * Hp / 100;
	float fillTop = y + height - temp4; // 计算填充矩形的顶部位置

	// 绘制矩形边框
	DrawBorderRectangle(temp1, temp2, 6, height, 1, colorA);

	// 填充矩形
	DrawFilledRectangle(temp1 + 1, fillTop + 1, 4, temp4 - 2, colorB);
}

// 横向中心进度条
void D3DDrawLib::DrawHorizontalCenterBlood(float x, float y, float px, int distance, int hp, bool is_death)
{
	D3DCOLOR color;

	if (!is_death)
	{
		if (hp >= 75)
		{
			color = D3DCOLOR_RGBA(0, 255, 0, 184); // 绿色表示高量
		}
		else if (hp >= 35)
		{
			color = D3DCOLOR_RGBA(255, 255, 0, 184); // 黄色表示中等量
		}
		else
		{
			color = D3DCOLOR_RGBA(255, 0, 0, 184); // 红色表示低量
		}
	}
	else
	{
		color = D3DCOLOR_RGBA(128, 128, 128, 184); // 灰色表示空
	}

	/*
	// 绘制填充方框背景
	DrawFilledRectangle(x - px / 2, y - 10, px, 5, D3DCOLOR_RGBA(50, 50, 50, 184));

	// 计算填充的宽度
	float filledWidth = (hp * (px - distance / 100.0f)) / 100.0f;

	// 绘制填充的血条
	DrawFilledRectangle(x - px / 2 + 1, y - 9, filledWidth, 3, color);
	*/

	// 绘制填充方框背景
	DrawFilledRectangle(x - px / 2, y - 15, px, 10, D3DCOLOR_RGBA(50, 50, 50, 184)); // 更宽的背景

	// 计算填充的宽度
	float filledWidth = (hp * (px - distance / 100.0f)) / 100.0f;

	// 绘制填充的血条
	DrawFilledRectangle(x - px / 2 + 1, y - 14, filledWidth, 8, color); // 更宽的填充
}

// 竖向中心进度条
void D3DDrawLib::DrawVerticalCenterBlood(float x, float y, float py, int distance, int hp, bool is_death)
{
	D3DCOLOR color;

	if (!is_death)
	{
		if (hp >= 75)
		{
			color = D3DCOLOR_RGBA(0, 255, 0, 184); // 绿色表示高量
		}
		else if (hp >= 35)
		{
			color = D3DCOLOR_RGBA(255, 255, 0, 184); // 黄色表示中等量
		}
		else
		{
			color = D3DCOLOR_RGBA(255, 0, 0, 184); // 红色表示低量
		}
	}
	else
	{
		color = D3DCOLOR_RGBA(128, 128, 128, 184); // 灰色表示无
	}

	// 绘制填充方框背景
	DrawFilledRectangle(x - 5, y - py / 2, 10, py, D3DCOLOR_RGBA(50, 50, 50, 184)); // 更宽的背景

	// 计算填充的高度
	float filledHeight = (hp * (py - distance / 100.0f)) / 100.0f;

	// 绘制填充的血条，从下往上
	DrawFilledRectangle(x - 4, y + py / 2 - filledHeight, 8, filledHeight, color); // 更宽的填充
}

// ---------------------------------------------------------------------------------
// 绘制其他图形
// ---------------------------------------------------------------------------------

// 画圆
void D3DDrawLib::DrawCircle(float x, float y, float r, float px, D3DCOLOR color)
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
void D3DDrawLib::DrawPoint(float x, float y, float px, D3DCOLOR color)
{
	DrawLine(x, y, x + 2, y + 2, px, color);
}

// 画十字准星
void D3DDrawLib::DrawCollimation(float left, float top, D3DCOLOR color, int type)
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

// 画超级准星
void D3DDrawLib::DrawSuperCollimation(float left, float top, D3DCOLOR color, int type)
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
void D3DDrawLib::DrawRadar(float left, float top)
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

// ---------------------------------------------------------------------------------
// 绘制文本
// ---------------------------------------------------------------------------------

// 绘制文本
void D3DDrawLib::DrawString(const char* fmt, int fontsize, int x, int y, D3DCOLOR color)
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


// 描边文本A
void D3DDrawLib::DrawColourStringA(const char* fmt, int fontsize, int x, int y, D3DCOLOR outlineColor, D3DCOLOR textColor)
{
	// 绘制描边
	DrawString(fmt, fontsize, x + 1, y, outlineColor); // 右
	DrawString(fmt, fontsize, x - 1, y, outlineColor); // 左
	DrawString(fmt, fontsize, x, y + 1, outlineColor); // 下
	DrawString(fmt, fontsize, x, y - 1, outlineColor); // 上

	// 绘制文本
	DrawString(fmt, fontsize, x, y, textColor);
}

// 描边文本B
void D3DDrawLib::DrawColourStringB(const char* fmt, int fontsize, int left, int top, D3DCOLOR color)
{
	DrawString(fmt, fontsize, left - 1, top, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left + 1, top, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left, top - 1, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left, top + 1, D3DCOLOR_RGBA(254, 0, 0, 184));

	DrawString(fmt, fontsize, left, top, color);
}

// 描边文本C
void D3DDrawLib::DrawColourStringC(const char* fmt, int fontsize, int left, int top, D3DCOLOR color)
{
	DrawString(fmt, fontsize, left - 1, top, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left + 1, top, D3DCOLOR_RGBA(254, 0, 0, 184));
	DrawString(fmt, fontsize, left, top, D3DCOLOR_RGBA(254, 0, 0, 184));
}

// 绘制单一颜色文本
void D3DDrawLib::DrawColorfulStringA(const char* text, int x, int y, int fontsize, D3DCOLOR color)
{
	RECT fontRect = { x, y, x + 1000, y + 100 }; // 设置足够大的矩形区域以避免文字截断

	// 创建字体对象
	ID3DXFont* pFont = NULL;
	D3DXCreateFont(m_pDevice, fontsize, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &pFont);

	// 绘制彩色文字
	pFont->DrawTextA(NULL, text, -1, &fontRect, DT_LEFT | DT_TOP | DT_NOCLIP, color);

	// 释放字体对象
	if (pFont)
	{
		pFont->Release();
		pFont = NULL;
	}
}

// 绘制双重文字
void D3DDrawLib::DrawStringAndString(const char* str, const char* fmt, int fontsize, int offset, int x, int y, D3DCOLOR colorA, D3DCOLOR colorB)
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

// 绘制双重文字加强版
void D3DDrawLib::DrawStringAndStringEx(const char* str, const char* fmt, int fontsize, int offset, int x, int y, D3DCOLOR colorA, D3DCOLOR colorB)
{
	RECT FontPos, fmtPos;
	FontPos.left = x;
	FontPos.top = y;
	FontPos.right = x + 1000; // 设置宽度以防止文字被截断
	FontPos.bottom = y + 100;

	fmtPos.left = x + offset;
	fmtPos.top = y;
	fmtPos.right = x + offset + 1000;
	fmtPos.bottom = y + 100;

	ID3DXFont* g_font = NULL;

	D3DXCreateFont(m_pDevice, fontsize, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_pFontSmall);

	// 绘制背景阴影
	RECT shadowPos = FontPos;
	shadowPos.left += 1;
	shadowPos.top += 1;

	RECT shadowFmtPos = fmtPos;
	shadowFmtPos.left += 1;
	shadowFmtPos.top += 1;

	CHAR format[128] = { '\0' };
	CHAR string[128] = { '\0' };

	// 文字提示
	va_list va_alist;
	va_start(va_alist, str);
	vsprintf_s(format, str, va_alist);
	va_end(va_alist);

	m_pFontSmall->DrawTextA(0, format, -1, &shadowPos, DT_NOCLIP, D3DCOLOR_RGBA(0, 0, 0, 128));
	m_pFontSmall->DrawTextA(0, format, -1, &FontPos, DT_NOCLIP, colorA);

	// 命令选择
	va_list vb_alist;
	va_start(vb_alist, fmt);
	vsprintf_s(string, fmt, vb_alist);
	va_end(vb_alist);

	m_pFontSmall->DrawTextA(0, string, -1, &shadowFmtPos, DT_NOCLIP, D3DCOLOR_RGBA(0, 0, 0, 128));
	m_pFontSmall->DrawTextA(0, string, -1, &fmtPos, DT_NOCLIP, colorB);

	// 释放字体资源
	if (m_pFontSmall)
	{
		m_pFontSmall->Release();
		m_pFontSmall = NULL;
	}
}

// 初始化D3d绘图引擎
void D3DDrawLib::D3dInit(HWND hWnd, int Width, int Height, const char* fontPath)
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

void D3DDrawLib::D3DRender(HWND hGameHwnd, void(*pDrawThread)())
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