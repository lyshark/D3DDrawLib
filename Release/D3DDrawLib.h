#ifndef __D3DDrawLib_H__
#define __D3DDrawLib_H__

#define BLOOD_UPTODOWN 1
#define BLOOD_DOWNTOUP 2

#include <string>
#include <windows.h>
#include <TlHelp32.h>
#include <dwmapi.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma warning( disable : 4244 )
#pragma warning( disable : 4838 )
#pragma warning( disable : 4996 )
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

typedef struct TVector3D
{
	float x;
	float y;
	float z;
}Vector3D;

typedef struct TVector2D
{
	float x;
	float y;
}Vector2D;

typedef struct TBox2D
{
	float x;
	float y;
	float w;
	float h;
}Box2D;

struct D3DTLVERTEX
{
	float x, y, z, rhw;
	DWORD color;
};

class D3DDrawLib
{
public:
	static D3DDrawLib* Start();
#pragma region AboutWindow

	// 获取窗口句柄
	DWORD FindWindowHwnd(const char* title, const char* className);

	// 得到窗体宽度
	DWORD GetWindowWidth(const char* title, const char* className);

	// 得到窗体高度
	DWORD GetWindowHeight(const char* title, const char* className);

	// 获取进程ID通过窗口句柄
	DWORD GetProcessIdByHwnd(DWORD hWnd);

	// 获取进程ID通过进程名
	DWORD GetProcessIdByName(LPCTSTR  name);

	// 取指定进程PID里模块句柄
	DWORD GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName);

	// 获取进程句柄
	HANDLE GetProcessHandle(DWORD pid);
#pragma endregion AboutWindow

#pragma region AboutD3DWindow

	// 创建窗口
	bool createWindow(std::string GameTitle, std::string GameClassName, std::string fontPath, void(*DrawThread)());

	// 刷新窗体
	void Render();

	MARGINS* getMARGINS() const { return m_Margin; }

	// 取高度
	int getHeight() const { return m_nHeight; }

	// 取宽度
	int getWidth() const { return m_nWidth; }

	// ---------------------------------------------------------------------------------
	// 线条绘制部分
	// ---------------------------------------------------------------------------------

	// 绘制线条
	void DrawLine(float x, float y, float x2, float y2, float width, D3DCOLOR color);

	// 平面横线
	void PlaneHorizontalLine(float left, float top, float size, float width, D3DCOLOR color);

	// 平面竖线
	void PlaneVerticalLine(float left, float top, float size, float height, D3DCOLOR color);

	// ---------------------------------------------------------------------------------
	// 进度条与方框部分
	// ---------------------------------------------------------------------------------

	// 绘制实心矩形
	void DrawFilledRectangle(float x, float y, float w, float h, D3DCOLOR color);

	// 绘制空心矩形
	void DrawBorderRectangle(float x, float y, float w, float h, float width, D3DCOLOR color);

	// 绘制四角方框
	void DrawFourCornersBox(float x, float y, float x2, float y2, float width, D3DCOLOR color);

	// 绘制角框
	void DrawCornerFrame(float x, float y, float x2, float y2, float width, D3DCOLOR color);

	// 绘制立体框
	void Box3D(int HeadX, int HeadY, int bottomX, int bottomY, D3DCOLOR color);

	// 绘制空线条 [坐标x / 坐标y / 高度 / 宽度 / 线条粒度 / 颜色]
	void DrawBloodFrame(float left, float top, float height, float width, int px, D3DCOLOR color);

	// 竖向进度条(基础)
	void DrawBlood_Vertical(float Hp, float x, float y, int width, int height, D3DCOLOR color);

	// 横向进度条(基础)
	void DrawBlood_Horizontal(float Hp, float x, float y, int width, int height, D3DCOLOR color);

	// 竖向进度条(封装)
	void DrawBlood_VerticalNew(float Hp, float x, float y, int width, int height, D3DCOLOR color, int style);

	// 横向进度条(封装)
	void DrawBlood_HorizontalNew(float Hp, float x, float y, int width, int height, D3DCOLOR color, int style);

	// 带有进度条的方框（左侧）[进度 / 坐标x / 坐标y / 宽度 / 高度]
	void DrawPerspectiveRectLeft(float Hp, float left, float top, int width, int height);

	// 带有进度条的方框（右侧）[进度 / 坐标x / 坐标y / 宽度 / 高度]
	void DrawPerspectiveRectRight(float Hp, float left, float top, int width, int height);

	// 带有进度条的方框（顶部）
	void DrawPerspectiveRectTop(float Hp, float left, float top, int width, int height);

	// 带有进度条的方框（底部）
	void DrawPerspectiveRectBottom(float Hp, float left, float top, int width, int height);

	// 横向进度条
	void DrawBlood_Transverse(float Hp, float x, float y, float width, float height, D3DCOLOR colorA, D3DCOLOR colorB);

	// 竖向进度条
	void DrawLeft_Vertical(float Hp, float x, float y, float width, float height, D3DCOLOR colorA, D3DCOLOR colorB);

	// 横向中心进度条
	void DrawHorizontalCenterBlood(float x, float y, float px, int distance, int hp, bool is_death);

	// 竖向中心进度条
	void DrawVerticalCenterBlood(float x, float y, float py, int distance, int hp, bool is_death);

	// ---------------------------------------------------------------------------------
	// 绘制其他图形
	// ---------------------------------------------------------------------------------

	// 画圆
	void DrawCircle(float x, float y, float r, float px, D3DCOLOR color);

	// 画点
	void DrawPoint(float x, float y, float px, D3DCOLOR color);

	// 画十字准星
	void DrawCollimation(float left, float top, D3DCOLOR color, int type);

	// 画超级准星 type = 1普通准星 2扩散准星 3十字准星 4全屏准星
	void DrawSuperCollimation(float left, float top, D3DCOLOR color, int type);

	// 绘制雷达
	void DrawRadar(float left, float top);

	// ---------------------------------------------------------------------------------
	// 绘制文本
	// ---------------------------------------------------------------------------------

	// 绘制文本
	void DrawString(const char* fmt, int fontsize, int x, int y, D3DCOLOR color);

	// 描边文本A
	void DrawColourStringA(const char* fmt, int fontsize, int x, int y, D3DCOLOR colour, D3DCOLOR color);

	// 描边文本B
	void DrawColourStringB(const char* fmt, int fontsize, int left, int top, D3DCOLOR color);

	// 描边文本C
	void DrawColourStringC(const char* fmt, int fontsize, int left, int top, D3DCOLOR color);

	// 绘制单一颜色文本
	void DrawColorfulStringA(const char* text, int x, int y, int fontsize, D3DCOLOR color);

	// 绘制双重文本
	void DrawStringAndString(const char* str, const char* fmt, int fontsize, int offset, int x, int y, D3DCOLOR colorA, D3DCOLOR colorB);

	// 绘制双重文字加强版
	void DrawStringAndStringEx(const char* str, const char* fmt, int fontsize, int offset, int x, int y, D3DCOLOR colorA, D3DCOLOR colorB);
	
#pragma endregion AboutD3DWindow
private:
	static D3DDrawLib* m_Instance;

#pragma region AboutD3DWindow
	int m_nWidth;
	int m_nHeight;
	HWND m_hWindow;
	HWND m_hGameHwnd;
	HWND m_nWindowHandle;
	std::string m_strGameTitle;
	std::string m_strGameClassName;
	std::string m_strFontName;
	RECT m_tGameSize;
	MSG m_Message;
	MARGINS* m_Margin;
	IDirect3D9Ex* m_pObject = 0;
	IDirect3DDevice9Ex* m_pDevice = 0;
	D3DPRESENT_PARAMETERS m_pParams;
	ID3DXFont* m_pFontSmall = 0;
	ID3DXLine* m_pLine = 0;
	void D3dInit(HWND hWnd, int Width, int Height, const char* fontPath);
	void D3DRender(HWND hGameHwnd, void(*pDrawThread)());
	void(*pDrawThread)();
#pragma endregion AboutD3DWindow
};
#endif