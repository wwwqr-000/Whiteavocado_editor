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
#include <functional>//For function storage

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

//Global variables
HWND hwnd;
std::vector<std::unique_ptr<frameObj>> frameObjects;
int newTabCount = 1;//Home is also a tab
//

void test() {
    std::cout << "test\n";
}

void updateScreen() {
    InvalidateRect(hwnd, NULL, TRUE);
}

void addItemToMenu(std::string menuName) {
    for (auto& obj : frameObjects) {
        menu* menuObj = dynamic_cast<menu*>(obj.get());

        if (menuObj && menuObj->getName() == menuName) {
            menuItem newTab(point2(1, 21), point2(99, 39), point3(0, 0, 0), 1, ("newTab" + std::to_string(newTabCount)), test, true);
            ++newTabCount;
            menuObj->addItem(newTab);
            updateScreen();
            std::cout << "added new tab\n";
        }
    }
}

void toggleMenu(std::string menuName) {
    for (auto& obj : frameObjects) {
        menu* menuObj = dynamic_cast<menu*>(obj.get());

        if (menuObj && menuObj->getName() == menuName) {
            menuObj->setVisable(!menuObj->isVisable());
            updateScreen();
            std::cout << "changed visability\n";
        }
    }
}

//box onclick calls
void toggleFileBar() { toggleMenu("file-bar"); }
void createNewFile() { std::cout << "create new file\n"; toggleFileBar(); }
void openOtherFile() { std::cout << "Open file\n"; }
void saveCurrentFile() { std::cout << "Save file\n"; }
void showHomeTab() { std::cout << "show home tab\n"; }
//

void frameObjectSetup() {
    menuItem file(point2(2, 2), point2(40, 18), point3(0, 0, 0), 1, "File", toggleFileBar, false);
    std::vector<menuItem> menItems;
    menItems.emplace_back(file);
    menu mainMenu(point2(0, 0), point2((50 * 16), 20), point3(0, 0, 0), menItems, "nav-bar", ROW, true);

    //File bar
    std::vector<menuItem> fileBarItems;
    menuItem newFile(point2(2, 43), point2(118, 73), point3(0, 0, 0), 1, "New", createNewFile, true);
    menuItem openFile(point2(2, 78), point2(118, 108), point3(0, 0, 0), 1, "Open", openOtherFile, true);
    menuItem saveFile(point2(2, 113), point2(118, 143), point3(0, 0, 0), 1, "Save as", saveCurrentFile, true);
    fileBarItems.emplace_back(newFile);
    fileBarItems.emplace_back(openFile);
    fileBarItems.emplace_back(saveFile);

    menu fileMenu(point2(0, 41), point2(120, 144), point3(0, 255, 0), fileBarItems, "file-bar", COLUMN, false);
    //

    //Tabs bar
    std::vector<menuItem> tabs;
    menuItem homeTab(point2(1, 21), point2(99, 39), point3(0, 0, 0), 1, "homeTab", showHomeTab, true);
    tabs.emplace_back(homeTab);

    menu tabMenu(point2(0, 20), point2((50 * 16), 40), point3(0, 0, 0), tabs, "tabs", ROW, true);
    //

    frameObjects.emplace_back(std::make_unique<menu>(mainMenu));
    frameObjects.emplace_back(std::make_unique<menu>(fileMenu));
    frameObjects.emplace_back(std::make_unique<menu>(tabMenu));
}

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    //HWND hwnd;             /* This is the handle for our window */
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
           0,                               /* Extended possibilites for variation */
           szClassName,                     /* Classname */
           _T("Whiteavocado Editor"),       /* Title Text */
           WS_OVERLAPPEDWINDOW,             /* default window */
           CW_USEDEFAULT,                   /* Windows decides the position */
           CW_USEDEFAULT,                   /* where the window ends up on the screen */
           (50 * 16),                       /* The programs width */
           (50 * 9),                        /* and height in pixels */
           HWND_DESKTOP,                    /* The window is a child-window to desktop */
           NULL,                            /* No menu */
           hThisInstance,                   /* Program Instance handler */
           NULL                             /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    frameObjectSetup();

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
        case WM_CREATE: {
            //HWND editCtrl = CreateWindowEx(0, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 200, 20, hwnd, (HMENU)1, NULL, NULL);
        } break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HPEN hPen;
            HPEN hOldPen;

            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkMode(hdc, TRANSPARENT);

            for (const auto& obj : frameObjects) {
                const menu* menuObj = dynamic_cast<const menu*>(obj.get());

                if (menuObj && menuObj->isVisable()) {
                    drawBox(hdc, menuObj->getMin(), menuObj->getMax(), menuObj->getColor(), menuObj->getLineSize());
                    for (const auto& menItem : menuObj->getItems()) {
                        if (menItem.hasBorder()) {
                            drawBox(hdc, menItem.getMin(), menItem.getMax(), menItem.getColor(), menItem.getLineSize());
                        }
                        RECT textArea;
                        textArea.left = menItem.getMin().x_i;
                        textArea.top = menItem.getMin().y_i;
                        textArea.right = menItem.getMax().x_i;
                        textArea.bottom = menItem.getMax().y_i;
                        DrawText(hdc, menItem.getPlaceholder().c_str(), -1, &textArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    }
                }
            }

            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            EndPaint(hwnd, &ps);
        } break;
        case WM_LBUTTONDOWN: {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);

            for (const auto& obj : frameObjects) {
                const menu* menuObj = dynamic_cast<const menu*>(obj.get());

                if (menuObj && menuObj->isVisable()) {
                    for (const auto& menItem : menuObj->getItems()) {
                        if (menItem.isClickable() && xPos >= menItem.getMin().x_i && xPos <= menItem.getMax().x_i && yPos >= menItem.getMin().y_i && yPos <= menItem.getMax().y_i) {
                            menItem.click();
                        }
                    }
                }
            }
        } break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
