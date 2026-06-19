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

### Draw a box

There are various types of box drawing, such as calling DrawFilledRectangle to draw solid boxes, calling DrawBorderRectangle to draw hollow boxes, calling DrawFourkCornersBox to draw square hollow boxes, and the DrawPerspectiveRect series is used to draw complex box structures with progress bars, which can be displayed in four positions: top, bottom, left, and right.

**DrawFilledRectangle**

This function is used to draw a filled solid rectangle, where the first and second parameters specify the coordinates of the top left corner of the rectangle, the third and fourth parameters specify the width and height of the rectangle, and the fifth parameter specifies the fill color of the rectangle.
```c
int RectWidth = 90;     // rectangle width
int RectHeight = 150;   // Rectangular height
int X = 100;            // X
int Y = 200;            // Y
 
D3DDrawLib::Start()->DrawFilledRectangle(
    X,
    Y,
    RectWidth,
    RectHeight,
    D3DCOLOR_RGBA(254, 0, 0, 184)
);
```

**DrawBorderRectangle**

This function is used to draw a hollow rectangle with borders. The first and second parameters are used to specify the X and Y coordinates of the top left corner of the rectangle, the third and fourth parameters are used to specify the width and height of the rectangle, the fifth parameter is used to specify the line width of the rectangle, and the sixth parameter is used to specify the fill color of the rectangle.

```c
int RectWidth = 90;     // rectangle width
int RectHeight = 150;   // Rectangular height
int X = 100;            // X
int Y = 200;            // Y
 
D3DDrawLib::Start()->DrawBorderRectangle(
    X,
    Y,
    RectWidth,
    RectHeight,
    2,
    D3DCOLOR_RGBA(254, 0, 0, 184)
);
```

**DrawFourCornersBox**

This function is used to draw a hollow rectangle with four corner borders. In addition, the DrawCornerFrame function can also achieve the drawing effect. The first and second parameters are used to specify the X and Y coordinates of the upper left corner of the rectangle, the third and fourth parameters are used to specify the width and height of the rectangle, the fifth parameter is used to specify the line width of the rectangle, and the sixth parameter is used to specify the filling color of the rectangle.

```c
int RectWidth = 90;     // rectangle width
int RectHeight = 150;   // Rectangular height
int X = 100;            // X
int Y = 200;            // Y
 
D3DDrawLib::Start()->DrawFourCornersBox(
    X,
    Y,
    RectWidth,
    RectHeight,
    2,
    D3DCOLOR_RGBA(254, 0, 0, 184)
);
 
D3DDrawLib::Start()->DrawCornerFrame(
    X,
    Y,
    RectWidth,
    RectHeight,
    2,
    D3DCOLOR_RGBA(254, 0, 0, 184)
);
```

**DrawPerspectiveRect**

This function series is used to draw a composite hollow rectangle with a progress bar. Its main direction range includes left, right, top, bottom, and other directions. The first parameter is used to specify the progress percentage to be displayed, the second and third parameters are used to specify the X and Y coordinates of the upper left corner of the rectangle, and the fourth and fifth parameters are used to specify the width and height of the rectangle. The four different rectangles are called as follows.

```c
// Left progress bar rectangular
D3DDrawLib::Start()->DrawPerspectiveRectLeft(
    93,
    160,
    200,
    90,
    160
);
 
// Right progress bar rectangular
D3DDrawLib::Start()->DrawPerspectiveRectRight(
    65,
    300,
    200,
    60,
    120
);
 
// Top progress bar rectangular
D3DDrawLib::Start()->DrawPerspectiveRectTop(
    10,
    450,
    200,
    90,
    160
);
 
// Bottom progress bar rectangular
D3DDrawLib::Start()->DrawPerspectiveRectBottom(
    10,
    450,
    200,
    90,
    160
);
```

Draw three different box shapes using DrawFilledAngular, DrawBorderRectangle, and DrawFourCornersBox, and arrange them horizontally in sequence for display. The code is shown below;
```c
void Draw()
{
    int ScreenWidth = 1024; // screen width
    int RectWidth = 90;     // rectangle width
    int RectHeight = 150;   // Rectangular height
    int Padding = 20;       // The spacing between rectangles
 
    // Calculate the starting position of the first solid rectangle
    int x1 = (ScreenWidth - RectWidth * 3 - Padding * 2) / 2;
    int y1 = 20;
    D3DDrawLib::Start()->DrawFilledRectangle(
        x1,
        y1,
        RectWidth,
        RectHeight,
        D3DCOLOR_RGBA(254, 0, 0, 184)
    );
 
    // Calculate the starting position of the second hollow rectangle
    int x2 = x1 + RectWidth + Padding;
    int y2 = 20;
    D3DDrawLib::Start()->DrawBorderRectangle(
        x2,
        y2,
        RectWidth,
        RectHeight,
        2,
        D3DCOLOR_RGBA(15, 44, 0, 25)
    );
 
    // Calculate the starting position of the third corner box
    int x3 = x2 + RectWidth + Padding;
    int y3 = 20;
    D3DDrawLib::Start()->DrawFourCornersBox(
        x3,
        y3,
        RectWidth,
        RectHeight,
        2,
        D3DCOLOR_RGBA(254, 0, 0, 25)
    );
}
```
continue to use DrawPerspectiveRectLeft、DrawPerspectiveRectRight、DrawPerspectiveRectTop、DrawPerspectiveRectBottom To draw a composite box with a progress bar display, dynamic drawing can be achieved by calling these functions and passing in the corresponding position and size. The code is shown below;
```c
void Draw()
{
    // Draw the left progress bar
    D3DDrawLib::Start()->DrawPerspectiveRectLeft(
        93,
        160,
        200,
        90,
        160
    );
    
    // Draw the top progress bar
    D3DDrawLib::Start()->DrawPerspectiveRectTop(
        65,
        300,
        200,
        60,
        120
    );
 
    // Draw the progress bar on the right side
    D3DDrawLib::Start()->DrawPerspectiveRectRight(
        10,
        450,
        200,
        90,
        160
    );
 
    // Draw the bottom progress bar
    D3DDrawLib::Start()->DrawPerspectiveRectBottom(
        45,
        650,
        200,
        90,
        160
    );
}
```

### Draw progress bar

There are also multiple categories to choose from when drawing progress bars. To draw a hollow progress bar, you can call the DrawBloodFrame function. To draw a horizontal progress bar, you can call the DrawBlood-Horizontal function. To draw a vertical progress bar, you can call the DrawBlood_VerticalNew function. To draw a complete vertical progress bar, you can call the DrawBlood-HorizontalNew function. To draw a simple horizontal progress bar, you can call the DrawBlood_Transverse function. The corresponding vertical progress bar is implemented by calling the DrawLeft_Vertical function.

**DrawBloodFrame**

This function is used to implement the drawing of hollow progress boxes, which is actually a compressed version of the box. The first and second parameters are used to specify the X and Y coordinates of the upper left corner of the drawn rectangle, the third and fourth parameters are used to specify the width and height of the rectangle, the fifth parameter is used to specify the line width of the rectangle, and the sixth parameter is used to specify the filling color of the rectangle.

This function can not only be used to draw horizontal progress, but also to draw vertical progress bars by changing the height to width ratio of the progress bar. The vertical display effect can be achieved by swapping the frameHeight and frameWidth parameters as follows.

```c
// Screen width and height
int ScreenWidth = 1024;
int ScreenHeight = 768;
 
// parameter
float frameLeft = (ScreenWidth - 3 * 150) / 2;
float frameTop = 50;
float frameHeight = 20;
float frameWidth = 150;
int lineWidth = 2;
 
// Draw horizontal progress
D3DDrawLib::Start()->DrawBloodFrame(
    frameLeft,
    frameTop,
    frameHeight,
    frameWidth,
    lineWidth,
    D3DCOLOR_RGBA(255, 0, 0, 255)
);
 
// Draw vertical progress
D3DDrawLib::Start()->DrawBloodFrame(
    frameLeft,
    frameTop,
    frameWidth,
    frameHeight,
    lineWidth,
    D3DCOLOR_RGBA(255, 0, 0, 255)
);
```

**DrawBlood**

This series of functions is used to draw horizontal and vertical progress bars. The horizontal progress bar can be drawn using the DrawBlood-Horizontal function, while the vertical progress bar can be drawn using the DrawBlood_Vertical function. The parameter passing of both functions is consistent. The first parameter is used to specify the percentage of the progress bar, the second and third parameters are used to specify the X and Y coordinates of the upper left corner of the drawn rectangle, the fourth and fifth parameters are used to specify the width and height of the rectangle, and the sixth parameter is used to specify the color of the progress bar.

Firstly, taking the horizontal progress bar as an example, calling DrawBlood-Horizontal and passing in the corresponding parameters can achieve drawing. To change the progress bar to vertical, simply invert the width and height parameters and call the DrawBlood_Vertical function to achieve drawing;

```c
// Screen width and height
int ScreenWidth = 1024;
int ScreenHeight = 768;
 
// Parameters of horizontal progress bar
float x = (ScreenWidth - 3 * 150) / 2;
float y = 250;
 
float hp = 75.0f;
int width = 150;
int height = 20;
 
// Draw a horizontal progress bar
D3DDrawLib::Start()->DrawBlood_Horizontal(
    hp,
    x,
    y,
    width,
    height,
    D3DCOLOR_RGBA(255, 0, 0, 255)
);
 
// Draw a vertical progress bar
D3DDrawLib::Start()->DrawBlood_Vertical(
    hp,
    x,
    y,
    height,
    height,
    D3DCOLOR_RGBA(255, 0, 0, 255)
);
```

**DrawBloodNew**

This series of functions is also used to implement the drawing of horizontal and vertical progress bars. The difference is that this progress bar supports the increase or decrease of progress from top to bottom BLOOD-UPTODOWN or from bottom to top BLOOD_SOWNTOOP. To draw a vertical progress bar, the DrawBlood_VerticalNew function can be used, while the horizontal one can use the DrawBlood-HorizontalNew function, which has the same parameters as the DrawBlood function. The only difference is that a type parameter can be passed at the end.

```c
#define BLOOD_UPTODOWN 1
#define BLOOD_DOWNTOUP 2
 
// Screen width and height
int ScreenWidth = 1024;
int ScreenHeight = 768;
 
// Parameters of horizontal progress bar
float x = (ScreenWidth - 3 * 150) / 2;
float y = 250;
int width = 150;
int height = 20;
 
// Draw a horizontal progress bar
D3DDrawLib::Start()->DrawBlood_HorizontalNew(
    50.0f,
    x,
    y,
    width,
    height,
    D3DCOLOR_RGBA(255, 0, 0, 255),
    BLOOD_UPTODOWN
);
```

**DrawTraverse**

This series of functions is used to draw line based progress bars. To draw a horizontal progress bar, the DrawBlood_Transverse function can be called. To draw a vertical progress bar, the DrawLeft_Vertical function can be called. The two parameters are passed in the same way, with the first parameter specifying the percentage, the second and third parameters specifying the X and Y coordinate positions where the progress bar appears, the fourth and fifth parameters specifying the height and width of the drawing, the sixth parameter specifying the outer border color, and the seventh parameter specifying the internal progress bar color.

```c
// Progress bar parameters
float x = 100.0f;
float y = 100.0f;
float width = 200.0f;
float height = 20.0f;
 
D3DCOLOR colorA1 = D3DCOLOR_RGBA(255, 0, 0, 255);  // red
D3DCOLOR colorB1 = D3DCOLOR_RGBA(0, 255, 0, 255);  // green
 
// Start drawing horizontally
D3DDrawLib::Start()->DrawBlood_Transverse(
    75.0f,
    x,
    y,
    width,
    height,
    colorA1,
    colorB1
);
 
// Start drawing vertically
D3DDrawLib::Start()->DrawLeft_Vertical(
    50.0f,
    x,
    y,
    height,
    width,
    colorA1,
    colorB1
);
```

**DrawCenterBlood**

This series of functions is used to draw beautiful center progress bars. To draw a horizontal progress bar, the DrawHorizontalCenterBlood function can be called. To draw a vertical progress bar, the DrawVerticalCenterBlood function can be called. The parameter passing of the two is consistent. The first and second parameters are used to specify the X and Y coordinate positions where the progress bar appears, the third and fourth parameters are used to specify the height and width of the progress bar, the fifth parameter is used to specify the percentage of the progress bar, and the sixth parameter specifies whether it is empty. The default value is 0.

```c
// parameter
int x = 100;
int y = 90;
int height = 100;
int width = 10;
int hp = 100;
int is_bool = 0;
 
// Horizontal progress bar
D3DDrawLib::Start()->DrawHorizontalCenterBlood(
    x,
    y,
    height,
    width,
    hp,
    is_bool
);
 
// Vertical progress bar
D3DDrawLib::Start()->DrawVerticalCenterBlood(
    x,
    y,
    height,
    width,
    hp,
    is_bool
);
```

**DrawBloodFrame**

First, we use DrawBloodFrame to draw horizontal hollow frames. As shown in the code below, we first define the drawing position and set three different colors, implementing the drawing by calling DrawBloodFrame three times.
```c
void Draw()
{
    // Screen width and height
    int ScreenWidth = 1024;
    int ScreenHeight = 768;

    // Hollow frame parameters
    float frameLeft = (ScreenWidth - 3 * 150) / 2;
    float frameTop = 50;
    float frameHeight = 20;
    float frameWidth = 150;
    int lineWidth = 2;

    // Draw hollow frames, each with different colors
    D3DCOLOR color1 = D3DCOLOR_RGBA(255, 0, 0, 255);    // Red
    D3DCOLOR color2 = D3DCOLOR_RGBA(0, 255, 0, 255);    // Green
    D3DCOLOR color3 = D3DCOLOR_RGBA(0, 0, 255, 255);    // Blue

    // Start drawing
    D3DDrawLib::Start()->DrawBloodFrame(frameLeft, frameTop, frameHeight, frameWidth, lineWidth, color1);
    D3DDrawLib::Start()->DrawBloodFrame(frameLeft + 170, frameTop, frameHeight, frameWidth, lineWidth, color2);
    D3DDrawLib::Start()->DrawBloodFrame(frameLeft + 340, frameTop, frameHeight, frameWidth, lineWidth, color3);
}
```
To draw vertical progress frames, simply swap frameHeight and frameWidth, keeping the rest unchanged. The code is as follows;
```c
void Draw()
{
    // Screen width and height
    int ScreenWidth = 1024;
    int ScreenHeight = 768;

    // Hollow frame parameters
    float frameLeft = 50;
    float frameTop = (ScreenHeight - 3 * 150) / 2;
    float frameHeight = 150;
    float frameWidth = 20;
    int lineWidth = 2;

    // Draw hollow frames, each with different colors
    D3DCOLOR color1 = D3DCOLOR_RGBA(255, 0, 0, 255);    // Red
    D3DCOLOR color2 = D3DCOLOR_RGBA(0, 255, 0, 255);    // Green
    D3DCOLOR color3 = D3DCOLOR_RGBA(0, 0, 255, 255);    // Blue

    // Start drawing
    D3DDrawLib::Start()->DrawBloodFrame(frameLeft, frameTop, frameHeight, frameWidth, lineWidth, color1);
    D3DDrawLib::Start()->DrawBloodFrame(frameLeft + 100, frameTop, frameHeight, frameWidth, lineWidth, color2);
    D3DDrawLib::Start()->DrawBloodFrame(frameLeft + 200, frameTop, frameHeight, frameWidth, lineWidth, color3);
}
```

**DrawBlood**

To draw horizontal progress bars, call the DrawBlood_Horizontal function. The code below draws three horizontal progress bars.
```c
void Draw()
{
    // Screen width and height
    int ScreenWidth = 1024;
    int ScreenHeight = 768;

    // Horizontal progress bar parameters
    float x = (ScreenWidth - 3 * 150) / 2;
    float y = 250;
    int width = 150;
    int height = 20;

    // Draw horizontal progress bars, each with different colors
    D3DCOLOR color1 = D3DCOLOR_RGBA(255, 0, 0, 255);    // Red
    D3DCOLOR color2 = D3DCOLOR_RGBA(0, 255, 0, 255);    // Green
    D3DCOLOR color3 = D3DCOLOR_RGBA(0, 0, 255, 255);    // Blue

    D3DDrawLib::Start()->DrawBlood_Horizontal(50.0f, x, y, width, height, color1);
    D3DDrawLib::Start()->DrawBlood_Horizontal(75.0f, x + 170, y, width, height, color2);
    D3DDrawLib::Start()->DrawBlood_Horizontal(25.0f, x + 340, y, width, height, color3);
}
```
To draw vertical progress bars, call the DrawBlood_Vertical function. The code below draws three vertical progress bars.
```c
void Draw()
{
    float hp = 75.0f;
    float x = 100.0f;
    float y = 100.0f;
    int width = 20;
    int height = 100;
 
    D3DCOLOR fullHealthColor = D3DCOLOR_RGBA(0, 255, 0, 255);  // Green
    D3DCOLOR midHealthColor = D3DCOLOR_RGBA(255, 255, 0, 255); // Yellow
    D3DCOLOR lowHealthColor = D3DCOLOR_RGBA(255, 0, 0, 255);   // Red

    D3DCOLOR color;
    if (hp >= 75.0f)
    {
        color = fullHealthColor;
    }
    else if (hp >= 35.0f)
    {
        color = midHealthColor;
    }
    else
    {
        color = lowHealthColor;
    }

    // Start drawing
    D3DDrawLib::Start()->DrawBlood_Vertical(hp, x, y, width, height, color);
    D3DDrawLib::Start()->DrawBlood_Vertical(hp, x + 100, y, width, height, midHealthColor);
    D3DDrawLib::Start()->DrawBlood_Vertical(hp, x + 200, y, width, height, lowHealthColor);
}
```

**DrawBloodNew**

This function is an enhanced version of the DrawBlood series, allowing you to specify whether the progress bar fills from top to bottom or bottom to top. Use DrawBlood_VerticalNew for complete vertical progress bars and DrawBlood_HorizontalNew for complete horizontal progress bars.

The following demonstrates drawing three vertical progress bars with different colors using DrawBlood_VerticalNew;
```c
void Draw()
{
    // Screen width and height
    int ScreenWidth = 1024;
    int ScreenHeight = 768;

    // Vertical progress bar parameters
    float x = (ScreenWidth - 3 * 20) / 2;
    float y = 100;
    int width = 20;
    int height = 150;

    // Draw vertical progress bars, each with different colors and styles
    D3DCOLOR color1 = D3DCOLOR_RGBA(255, 0, 0, 255);    // Red
    D3DCOLOR color2 = D3DCOLOR_RGBA(0, 255, 0, 255);    // Green

#define BLOOD_UPTODOWN 1
#define BLOOD_DOWNTOUP 2

    D3DDrawLib::Start()->DrawBlood_VerticalNew(50.0f, x, y, width, height, color1, BLOOD_UPTODOWN);
    D3DDrawLib::Start()->DrawBlood_VerticalNew(75.0f, x + 30, y, width, height, color2, BLOOD_UPTODOWN);
    D3DDrawLib::Start()->DrawBlood_VerticalNew(90.0f, x + 60, y, width, height, color2, BLOOD_DOWNTOUP);
}
```

To draw horizontal progress bars, call DrawBlood_HorizontalNew. The code is as follows;
```c
void Draw()
{
    // Screen width and height
    int ScreenWidth = 1024;
    int ScreenHeight = 768;

    // Horizontal progress bar parameters
    float x = (ScreenWidth - 3 * 150) / 2;
    float y = 250;
    int width = 150;
    int height = 20;

    // Draw horizontal progress bars, each with different colors and styles
    D3DCOLOR color1 = D3DCOLOR_RGBA(255, 0, 0, 255);    // Red
    D3DCOLOR color2 = D3DCOLOR_RGBA(0, 255, 0, 255);    // Green

#define BLOOD_UPTODOWN 1
#define BLOOD_DOWNTOUP 2

    D3DDrawLib::Start()->DrawBlood_HorizontalNew(50.0f, x, y, width, height, color1, BLOOD_UPTODOWN);
    D3DDrawLib::Start()->DrawBlood_HorizontalNew(75.0f, x + 90, y, width, height, color2, BLOOD_UPTODOWN);
    D3DDrawLib::Start()->DrawBlood_HorizontalNew(25.0f, x + 180, y, width, height, color2, BLOOD_DOWNTOUP);
}
```

**DrawTransverse**

Used for drawing structured progress bars. DrawBlood_Transverse draws horizontal progress bars and DrawLeft_Vertical draws vertical progress bars. The following code draws three horizontal progress bars;
```c
void Draw()
{
    // Screen width and height
    int ScreenWidth = 1024;
    int ScreenHeight = 768;

    // Horizontal progress bar parameters
    float y = 100.0f;
    float height = 20.0f;
    float spacing = 30.0f;  // Spacing between each progress bar

    // First progress bar
    float x1 = 100.0f;
    float width1 = 200.0f;
    D3DCOLOR colorA1 = D3DCOLOR_RGBA(255, 0, 0, 255);  // Red
    D3DCOLOR colorB1 = D3DCOLOR_RGBA(0, 255, 0, 255);  // Green
    D3DDrawLib::Start()->DrawBlood_Transverse(75.0f, x1, y, width1, height, colorA1, colorB1);

    // Second progress bar
    float x2 = 100.0f + width1 + spacing;
    float width2 = 150.0f;
    D3DCOLOR colorA2 = D3DCOLOR_RGBA(0, 0, 255, 255);  // Blue
    D3DCOLOR colorB2 = D3DCOLOR_RGBA(255, 255, 0, 255);  // Yellow
    D3DDrawLib::Start()->DrawBlood_Transverse(50.0f, x2, y, width2, height, colorA2, colorB2);

    // Third progress bar
    float x3 = 100.0f + width1 + width2 + 2 * spacing;
    float width3 = 180.0f;
    D3DCOLOR colorA3 = D3DCOLOR_RGBA(255, 255, 0, 255);  // Yellow
    D3DCOLOR colorB3 = D3DCOLOR_RGBA(255, 0, 255, 255);  // Purple
    D3DDrawLib::Start()->DrawBlood_Transverse(85.0f, x3, y, width3, height, colorA3, colorB3);
}
```

For vertical drawing, simply change the drawing function to DrawLeft_Vertical, as shown in the code below;
```c
void Draw()
{
    // Screen width and height
    int ScreenWidth = 1024;
    int ScreenHeight = 768;

    // Vertical progress bar parameters
    float x = 300.0f;
    float width = 20.0f;
    float spacing = 30.0f;  // Spacing between each progress bar

    // First progress bar
    float y1 = 100.0f;
    float height1 = 200.0f;
    D3DCOLOR colorA1 = D3DCOLOR_RGBA(255, 0, 0, 255);  // Red
    D3DCOLOR colorB1 = D3DCOLOR_RGBA(0, 255, 0, 255);  // Green
    D3DDrawLib::Start()->DrawLeft_Vertical(50.0f, x, y1, width, height1, colorA1, colorB1);

    // Second progress bar
    float y2 = 100;
    float height2 = 150.0f;
    D3DCOLOR colorA2 = D3DCOLOR_RGBA(0, 0, 255, 255);  // Blue
    D3DCOLOR colorB2 = D3DCOLOR_RGBA(255, 255, 0, 255);  // Yellow
    D3DDrawLib::Start()->DrawLeft_Vertical(75.0f, x+100, y1, width, height2, colorA2, colorB2);

    // Third progress bar
    float y3 = 200;
    float height3 = 180.0f;
    D3DCOLOR colorA3 = D3DCOLOR_RGBA(255, 255, 0, 255);  // Yellow
    D3DCOLOR colorB3 = D3DCOLOR_RGBA(255, 0, 255, 255);  // Purple
    D3DDrawLib::Start()->DrawLeft_Vertical(85.0f, x+200, y1, width, height3, colorA3, colorB3);
}
```

**DrawCenterBlood**

Used for drawing centered progress bars, which are styled progress bars. DrawHorizontalCenterBlood draws horizontal progress and DrawVerticalCenterBlood draws vertical progress, as shown in the code below;
```c
void Draw()
{
    // Horizontal
    D3DDrawLib::Start()->DrawHorizontalCenterBlood(100, 200, 100, 10, 100, 0);  //  100%, center at (100, 200), width 100
    D3DDrawLib::Start()->DrawHorizontalCenterBlood(200, 200, 50, 10, 50, 0);   //  50%, center at (200, 200), width 50
    D3DDrawLib::Start()->DrawHorizontalCenterBlood(300, 200, 25, 10, 15, 0);   //  15%, center at (300, 200), width 25

    // Vertical
    D3DDrawLib::Start()->DrawVerticalCenterBlood(100, 90, 100, 10, 100, false);
    D3DDrawLib::Start()->DrawVerticalCenterBlood(200, 90, 100, 10, 50, false);
    D3DDrawLib::Start()->DrawVerticalCenterBlood(300, 90, 100, 10, 25, false);
}
```

**Bordered Text**

Four functions are provided for text rendering. DrawString renders normal text, DrawColourStringA renders outlined text, and DrawStringAndString or DrawStringAndStringEx render multiple text elements. Since text rendering is relatively simple, you can adjust the examples as needed.

```c
void Draw()
{
    char szBuf[256] = { 0 };
    strcpy(szBuf, "hello lyshark");

    // Normal text
    D3DDrawLib::Start()->DrawString(szBuf, 25, 10, 20, D3DCOLOR_RGBA(255, 0, 0, 255));

    // Outlined text
    D3DDrawLib::Start()->DrawColourStringA(szBuf, 30, 20, 40, D3DCOLOR_RGBA(255, 0, 0, 255), D3DCOLOR_RGBA(5, 0, 0, 2));

    // Multiple text
    D3DDrawLib::Start()->DrawStringAndString(szBuf, szBuf, 30, 25, 50, 100, D3DCOLOR_RGBA(255, 0, 0, 255), D3DCOLOR_RGBA(5, 0, 0, 2));

    D3DDrawLib::Start()->DrawStringAndStringEx(szBuf, szBuf, 30, 25, 70, 150, D3DCOLOR_RGBA(255, 0, 0, 255), D3DCOLOR_RGBA(5, 0, 0, 2));
}
```
