#include <iostream>
#include <windows.h>
#include <windowsx.h>


/* 2. Изучение вывода текста и шрифтов.

- Разработать программу, которая вписывает в окно текстовую таблицу (N строк на M столбцов) таким образом, что все столбцы таблицы равномерно
распределяются по ширине окна, а высота строк таблицы подбирается таким образом, чтобы вместить текст каждой ячейки.
- При изменении размеров окна таблица перерасчитывается и перерисовывается.

*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void CellCalculation(int width, int height);

void PaintGrid(HWND hWnd);

void PaintCells(HDC &hDC);

int const NUM_OF_COLUMNS = 5;
int const NUM_OF_ROWS = 5;

struct Cell {
    int left;
    int top;
    int right;
    int bottom;
};

static Cell Cells[NUM_OF_ROWS][NUM_OF_COLUMNS];

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine,
                     int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "SomeWindowClass";
    wcex.hIconSm = wcex.hIcon;

    RegisterClassEx(&wcex);


    hWnd = CreateWindow("SomeWindowClass", "Some Window",
                        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
                        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        UpdateWindow(hWnd);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_PAINT:
            PaintGrid(hWnd);
            break;
        case WM_SIZE:
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void PaintGrid(HWND hWnd) {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;

    HDC winDC = GetDC(hWnd);
    HDC memDC = CreateCompatibleDC(winDC);
    HBITMAP hBmp = CreateCompatibleBitmap(winDC, clientWidth, clientHeight);
    SelectBitmap(memDC, hBmp);

    HPEN hPen = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));
    HPEN oldPen = SelectPen(memDC, hPen);

    LOGBRUSH br;
    br.lbStyle = BS_SOLID;
    br.lbColor = RGB(255, 255, 255);
    HBRUSH brush;
    brush = CreateBrushIndirect(&br);
    FillRect(memDC, &clientRect, brush);
    DeleteObject(brush);

    Rectangle(memDC, 0, 0, clientWidth, clientHeight);
    CellCalculation(clientWidth,clientHeight);
    PaintCells(memDC);
    SetStretchBltMode(winDC, COLORONCOLOR);
    BitBlt(winDC, 0, 0, clientWidth, clientHeight, memDC, 0, 0, SRCCOPY);

    DeleteObject(hBmp);
    DeletePen(SelectPen(memDC, oldPen));
    DeleteDC(memDC);
    ReleaseDC(hWnd, winDC);
}

void CellCalculation(int width, int height) {
    int cellWidth = width / NUM_OF_COLUMNS;
    int cellHeight = height / NUM_OF_ROWS;
    for (int i = 0; i < NUM_OF_ROWS; i++) {
        for (int j = 0; j < NUM_OF_COLUMNS; j++) {
            Cells[i][j].left = j * cellWidth;
            Cells[i][j].right = (j + 1) * cellWidth;
            Cells[i][j].top = i * cellHeight;
            Cells[i][j].bottom = (i + 1) * cellHeight;
        }
    }
}

void PaintCells(HDC &hDC) {
    for (int i = 0; i < NUM_OF_ROWS; i++) {
        for (int j = 0; j < NUM_OF_COLUMNS; j++) {
            Rectangle(hDC,Cells[i][j].left,Cells[i][j].top,Cells[i][j].right,Cells[i][j].bottom);
        }
    }
}





