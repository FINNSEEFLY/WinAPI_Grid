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
double const SCALE = 0.05;
char const *placeholder = "I'm a placeholder!";
static LOGFONT lf;

struct Cell {
    int left;
    int top;
    int right;
    int bottom;
    RECT text;
};

Cell Cells[NUM_OF_ROWS][NUM_OF_COLUMNS];


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


    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfPitchAndFamily = DEFAULT_PITCH;
    strcpy(lf.lfFaceName, "Open Sans");
    lf.lfHeight = 42;
    lf.lfWidth = 15;
    lf.lfWeight = 32;
    lf.lfEscapement = 0;

    RegisterClassEx(&wcex);

    hWnd = CreateWindow("SomeWindowClass", "Some Window",
                        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
                        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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

    // Фон
    LOGBRUSH br;
    br.lbStyle = BS_SOLID;
    br.lbColor = RGB(255, 255, 255);
    HBRUSH brush;
    brush = CreateBrushIndirect(&br);
    FillRect(memDC, &clientRect, brush);
    DeleteObject(brush);

    // Настройка шрифта
    HFONT hFont = CreateFontIndirect(&lf);
    SelectObject(memDC, hFont);
    SetTextColor(memDC, RGB(0, 0, 0));
    SetBkColor(memDC, RGB(255, 255, 255));

    // Таблица
    Rectangle(memDC, 0, 0, clientWidth, clientHeight);
    CellCalculation(clientWidth, clientHeight);
    PaintCells(memDC);
    SetStretchBltMode(winDC, COLORONCOLOR);

    BitBlt(winDC, 0, 0, clientWidth, clientHeight, memDC, 0, 0, SRCCOPY);


    DeleteObject(hFont);
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
            Cells[i][j].text.left = Cells[i][j].left + cellWidth * SCALE;
            Cells[i][j].text.top = Cells[i][j].top + cellHeight * SCALE;
            Cells[i][j].text.right = Cells[i][j].right - cellWidth * SCALE;
            Cells[i][j].text.bottom = Cells[i][j].bottom - cellHeight * SCALE;
        }
    }
}

void PaintCells(HDC &hDC) {
    for (int i = 0; i < NUM_OF_ROWS; i++) {
        for (int j = 0; j < NUM_OF_COLUMNS; j++) {
            Rectangle(hDC, Cells[i][j].left, Cells[i][j].top, Cells[i][j].right, Cells[i][j].bottom);
            DrawTextA(hDC, placeholder, 18, (LPRECT) &Cells[i][j].text, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
        }
    }
}





