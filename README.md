# D3D Dynamic External Drawing Library

<img width="65%" height="114" alt="image" src="https://github.com/user-attachments/assets/81376fb7-46cc-4b0a-a590-69d702a2c054" />

This Windows platform dynamic drawing library, developed based on C/C++language, supports interaction with any third-party process to complete dynamic drawing. It has rich built-in drawing functions and can achieve precise dot and mark operations to optimize data visualization effects. It can adapt to various application scenarios such as security monitoring systems, industrial automation control, and educational research experiment data display, providing stable and reliable graphic solutions for various graphic drawing needs.

**Warning:** This tool is only used for technical demonstrations and academic research. Any illegal or irregular operations are strictly prohibited. If used in illegal situations without authorization, the user shall bear all legal responsibilities.

### LIB library configuration

First, install the Microsoft DirectX SDK (June 2010) and place the 'D3DDrawLib' header and library files into the 'Release' folder. Right click on the project to open its properties and switch to the 'Release+Win32' configuration. In the 'VC++' directory, add the 'Include' path and 'Release' folder path of the 'DXSDK' to the containing directory, and add the 'DXSDK Lib \ x86' path and 'Release' folder path to the library directory. Then, enter the linker input bar to add dependencies and fill in the 'D3DDrawLib. lib' field. Finally, write '# include<D3DDrawLib. h>' in the code header to use the drawing library.

<img width="944" height="376" alt="image" src="https://github.com/user-attachments/assets/fa231f36-7c15-4004-b076-3cdb36e4a778" />

The following example is developed based on the D3DDrawLib drawing library, with two major business interfaces reserved for the program body

- Function is used to carry core business logic, and the interaction and update logic for dynamic rendering of menus and graphics can be written here
- Draw specifically stores menu interface drawing related drawing code

The program running logic is to first retrieve the target main window through the built-in interface of the library, and then call ` D3DDrawLib:: Start() ->creatWindow() ` to mount a dedicated drawing sub window on the target window to complete the graphic rendering. The complete example code is as follows:

```c
#include <iostream>
#include <windows.h>
#include <D3DDrawLib.h>
 
#pragma comment(lib,"D3DDrawLib.lib")
 
int w = 0;
int h = 0;
 
void Function()
{
}

void Draw()
{
}
 
void Ptr()
{
    Function();
    Draw();
}
 
int main(int argc, char *argv[])
{
    // Specify the form class name
    const char* class_name = "DrawingBoard";
 
    // Obtain process PID through process name
    DWORD pid = D3DDrawLib::Start()->GetProcessIdByName(L"DrawingBoard.exe");
    printf("PID = %d \n", pid);
 
    // Get process handle
    HANDLE hwnd = D3DDrawLib::Start()->GetProcessHandle(pid);
    printf("process handle = %d \n", hwnd);
 
    w = D3DDrawLib::Start()->GetWindowWidth(class_name, class_name);
    h = D3DDrawLib::Start()->GetWindowHeight(class_name, class_name);
    D3DDrawLib::Start()->createWindow(class_name, class_name, "黑体", Ptr);
 
    return 0;
}
```







