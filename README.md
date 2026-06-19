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

In the following case demonstration, only the drawing code inside the Draw function needs to be changed, and other code remains unchanged. Next, we will demonstrate drawing various graphic elements such as lines, boxes, text, center of gravity, progress bars, circles, etc. one by one.

### Draw lines

The drawing of lines can be done using three encapsulated functions, among which DrawLine() can be used to draw ordinary lines, PlaneHrizontalLine() can be used to draw horizontal lines, and PlaneVerticalLine() can be used to draw vertical lines. As shown below, the author will introduce the calling cases of different functions and draw some specific graphics based on them.

**DrawLine**

This function is used to draw a regular flat line, which can freely specify its direction. The first and second parameters are used to specify the starting coordinates (x, y) of the line, the third and fourth parameters are used to specify the ending coordinates (x, y), the fifth parameter is used to specify the width of the line, and the sixth parameter is used to specify the color of the line, which is red (R=255, G=0, B=0, A=255).

```c
D3DDrawLib::Start()->DrawLine(
    50.0f, 
    50.0f, 
    200.0f, 
    200.0f, 
    2.0f, 
    D3DCOLOR_RGBA(255, 0, 0, 255)
);
```

**PlaneHrizontalLine**

This function is used to draw horizontal lines on a plane, which only support horizontal drawing. The first parameter is used to specify the x-coordinate of the starting point of the horizontal line, the second parameter is used to specify the y-coordinate of the starting point of the horizontal line, the third parameter is used to specify the length of the horizontal line, the fourth parameter is used to specify the width of the line, and the fifth parameter is used to specify the color of the line, which is green (R=0, G=255, B=0, A=255).

```c
D3DDrawLib::Start()->PlaneHorizontalLine(
    100.0f,
    100.0f,
    150.0f,
    2.0f,
    D3DCOLOR_RGBA(0, 255, 0, 255)
);
```

**PlaneVerticalLine**

This function is used to draw vertical lines on a plane, which only support drawing in the vertical direction. The first parameter is used to specify the x-coordinate of the starting point of the vertical line, the second parameter is used to specify the y-coordinate of the starting point of the vertical line, the third parameter is used to specify the length of the vertical line, the fourth parameter is used to specify the width of the line, and the fifth parameter is used to specify the color of the line, which is blue (R=0, G=0, B=255, A=255).

```c
D3DDrawLib::Start()->PlaneVerticalLine(
    300.0f,
    100.0f,
    150.0f,
    2.0f,
    D3DCOLOR_RGBA(0, 0, 255, 255)
);
```

To implement drawing using the above functions, readers only need to put the code into the Draw() drawing area within the framework. In the following code, we call the DrawLine, PlaneHrizontalLine, and PlaneVerticalLine functions in sequence;

```c
void Draw()
{
    // Draw a regular plane straight line
    D3DDrawLib::Start()->DrawLine(
        50.0f,
        50.0f,
        200.0f,
        200.0f,
        2.0f,
        D3DCOLOR_RGBA(255, 0, 0, 255)
    );
 
    // Draw a horizontal plane line
    D3DDrawLib::Start()->PlaneHorizontalLine(
        100.0f,
        100.0f,
        150.0f,
        2.0f,
        D3DCOLOR_RGBA(0, 255, 0, 255)
    );
 
    // Draw vertical lines on a plane
    D3DDrawLib::Start()->PlaneVerticalLine(
        300.0f,
        100.0f,
        150.0f,
        2.0f,
        D3DCOLOR_RGBA(0, 0, 255, 255)
    );
}
```

To draw a triangular structure, you only need to adjust the aspect ratio relationship. As shown in the code, the coordinates of the three vertices of the triangle are calculated and then drawn by calling the DrawLine function.
```c
void Draw()
{
    // Define the vertices of a triangle
    float x1 = 100.0f, y1 = 100.0f;
    float x2 = 200.0f, y2 = 100.0f;
    float x3 = 150.0f, y3 = 50.0f;
 
    // base
    D3DDrawLib::Start()->DrawLine(
        x1,
        y1,
        x2,
        y2,
        2.0f,
        D3DCOLOR_RGBA(255, 0, 0, 255)
    );
 
    // right side
    D3DDrawLib::Start()->DrawLine(
        x2,
        y2,
        x3,
        y3,
        2.0f,
        D3DCOLOR_RGBA(0, 255, 0, 255)
    );
 
    // left
    D3DDrawLib::Start()->DrawLine(
        x3,
        y3,
        x1,
        y1,
        2.0f,
        D3DCOLOR_RGBA(0, 0, 255, 255)
    );
}
```

To draw an equilateral hexagon, one only needs to calculate the proportional relationship of the six sides and call the drawing library function, as shown in the following code;
```c
void Draw()
{
    // Define the vertices of a hexagon
    float cx = 200.0f, cy = 200.0f;  // center point
    float radius = 50.0f;            // radius
    float angle = 3.14159f / 3;      // Radiance of each angle (60 degrees)
 
    // Calculate the six vertices of a hexagon
    float x1 = cx + radius * cos(0 * angle), y1 = cy + radius * sin(0 * angle);
    float x2 = cx + radius * cos(1 * angle), y2 = cy + radius * sin(1 * angle);
    float x3 = cx + radius * cos(2 * angle), y3 = cy + radius * sin(2 * angle);
    float x4 = cx + radius * cos(3 * angle), y4 = cy + radius * sin(3 * angle);
    float x5 = cx + radius * cos(4 * angle), y5 = cy + radius * sin(4 * angle);
    float x6 = cx + radius * cos(5 * angle), y6 = cy + radius * sin(5 * angle);
 
    // Draw the six sides of a hexagon
    D3DDrawLib::Start()->DrawLine(x1, y1, x2, y2, 2.0f, D3DCOLOR_RGBA(255, 0, 0, 255));
    D3DDrawLib::Start()->DrawLine(x2, y2, x3, y3, 2.0f, D3DCOLOR_RGBA(0, 255, 0, 255));
    D3DDrawLib::Start()->DrawLine(x3, y3, x4, y4, 2.0f, D3DCOLOR_RGBA(0, 0, 255, 255));
    D3DDrawLib::Start()->DrawLine(x4, y4, x5, y5, 2.0f, D3DCOLOR_RGBA(255, 255, 0, 255));
    D3DDrawLib::Start()->DrawLine(x5, y5, x6, y6, 2.0f, D3DCOLOR_RGBA(0, 255, 255, 255));
    D3DDrawLib::Start()->DrawLine(x6, y6, x1, y1, 2.0f, D3DCOLOR_RGBA(255, 0, 255, 255));
}
```
Continuing to extend the above code, we can embed a pentagon within a pentagon by defining a two-dimensional matrix, as shown in the following code;
```c
void Draw()
{
    // Define the vertices of a pentagram
    float cx = 200.0f, cy = 200.0f;  // center point
    float outerRadius = 50.0f;       // outer radius
    float innerRadius = 20.0f;       // inner radius
    float angle = 3.14159f / 5;      // Radiance of each angle (36 degrees)
 
    // Calculate the vertices of a pentagram
    float x[10], y[10];
    for (int i = 0; i < 10; ++i)
    {
        float r = (i % 2 == 0) ? outerRadius : innerRadius;
        x[i] = cx + r * cos(i * angle);
        y[i] = cy + r * sin(i * angle);
    }
 
    // Draw the five edges of a pentagram
    D3DDrawLib::Start()->DrawLine(x[0], y[0], x[2], y[2], 2.0f, D3DCOLOR_RGBA(255, 0, 0, 255));
    D3DDrawLib::Start()->DrawLine(x[2], y[2], x[4], y[4], 2.0f, D3DCOLOR_RGBA(0, 255, 0, 255));
    D3DDrawLib::Start()->DrawLine(x[4], y[4], x[6], y[6], 2.0f, D3DCOLOR_RGBA(0, 0, 255, 255));
    D3DDrawLib::Start()->DrawLine(x[6], y[6], x[8], y[8], 2.0f, D3DCOLOR_RGBA(255, 255, 0, 255));
    D3DDrawLib::Start()->DrawLine(x[8], y[8], x[0], y[0], 2.0f, D3DCOLOR_RGBA(255, 0, 255, 255));
 
    // Connect internal and external vertices
    D3DDrawLib::Start()->DrawLine(x[1], y[1], x[3], y[3], 2.0f, D3DCOLOR_RGBA(0, 255, 255, 255));
    D3DDrawLib::Start()->DrawLine(x[3], y[3], x[5], y[5], 2.0f, D3DCOLOR_RGBA(255, 128, 0, 255));
    D3DDrawLib::Start()->DrawLine(x[5], y[5], x[7], y[7], 2.0f, D3DCOLOR_RGBA(128, 0, 255, 255));
    D3DDrawLib::Start()->DrawLine(x[7], y[7], x[9], y[9], 2.0f, D3DCOLOR_RGBA(128, 128, 0, 255));
    D3DDrawLib::Start()->DrawLine(x[9], y[9], x[1], y[1], 2.0f, D3DCOLOR_RGBA(0, 128, 128, 255));
}
```













































































