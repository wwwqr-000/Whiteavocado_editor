#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>//Unique ptr

//Enumerators
enum FlexDirection { COLUMN, ROW };
//

#include "classes/dimensions.hpp"
#include "classes/frameObj.hpp"
#include "classes/frames/box.hpp"
#include "classes/frames/menuItem.hpp"
#include "classes/frames/menu.hpp"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("WhiteavocadoApp");

std::vector<std::unique_ptr<frameObj>> frameObjects;

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Whiteavocado Editor"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    //Menu test setup
    menuItem file(point2(10, 10), point2(110, 110), point3(255, 0, 0), 1, "File");

    std::vector<menuItem> menItems;

    menItems.emplace_back(file);

    menu mainMenu = menu(point2(5, 5), point2(115, 115), point3(0, 255, 0), menItems, "bar-top", ROW);

    frameObjects.emplace_back(std::make_unique<menu>(mainMenu));
    //

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void drawBox(HDC& hdc, point2 min_, point2 max_, point3 rgb, int width) {
    HPEN hPen;
    HPEN hOldPen;

    //Draw menuItem frame based on box
    hPen = CreatePen(PS_SOLID, width, RGB(rgb.x_i, rgb.y_i, rgb.z_i));
    hOldPen = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, min_.x_i, min_.y_i, NULL);//Top left -> Top right
    LineTo(hdc, max_.x_i, min_.y_i);

    MoveToEx(hdc, max_.x_i, min_.y_i, NULL);//Top right -> Bottom right
    LineTo(hdc, max_.x_i, max_.y_i);

    MoveToEx(hdc, max_.x_i, max_.y_i, NULL);//Bottom right -> Bottom left
    LineTo(hdc, min_.x_i, max_.y_i);

    MoveToEx(hdc, min_.x_i, max_.y_i, NULL);//Bottom left -> Top left
    LineTo(hdc, min_.x_i, min_.y_i);
    //
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HPEN hPen;
            HPEN hOldPen;

            for (const auto& obj : frameObjects) {
                const menu* menuObj = dynamic_cast<const menu*>(obj.get());

                if (menuObj) {
                    drawBox(hdc, menuObj->getMin(), menuObj->getMax(), menuObj->getColor(), menuObj->getLineSize());
                    for (const auto& menItem : menuObj->getItems()) {
                        drawBox(hdc, menItem.getMin(), menItem.getMax(), menItem.getColor(), menItem.getLineSize());
                    }
                }
            }

            //drawBox(hdc, point2(10, 10), point2(110, 110), point3(255, 0, 0), 1);

            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            EndPaint(hwnd, &ps);
        } break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
