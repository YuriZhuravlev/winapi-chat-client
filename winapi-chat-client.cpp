// winapi-chat-client.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "winapi-chat-client.h"
#include "ChatClient.h"
#include <vector>

#pragma warning(disable : 4996)

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
HWND hWnd;                                      // MainWindow
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Login(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPICHATCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPICHATCLIENT));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPICHATCLIENT));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPICHATCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 700, 410, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


// Мои константы
const int ID_BUTTON_SEND = 51000;
const int ID_LIST_BOX_MESSAGES = 51001;
const int ID_EDIT_TEXT_MESSAGES = 51002;
const int ID_CHECKBOX = 51003;

// Логические элементы
bool checked = false;
ChatClient* ptClient;
char* lpUtf = new char[100];
LPWSTR username = new WCHAR[30];
LPWSTR lpString = new WCHAR[100];
std::vector<LPWSTR> itemList = std::vector<LPWSTR>();

// UI-элементы
HANDLE hImg;
HCURSOR hCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1));
HWND hList, hEditText;

// Мои функции
void onMessage(char* buffer, int length) {
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, buffer, -1, lpString, length);
    for (int i = length; i < 100; ++i) {
        lpString[i] = '\0';
    }
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)lpString);
}

void onCreate(HWND hWnd) {
    hImg = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
    DialogBox(hInst, MAKEINTRESOURCE(IDD_LOGIN), hWnd, Login);
    WCHAR send[5];
    HWND tmp;
    LoadString(hInst, ID_BUTTON_SEND, send, 5);
    tmp = CreateWindow(TEXT("STATIC"), username, WS_VISIBLE | WS_CHILD, 520, 10, 150, 26, hWnd, NULL, hInst, NULL);
    SetClassLongPtr(tmp, GCL_HCURSOR, (LONG)hCursor);
    
    tmp = CreateWindow(TEXT("BUTTON"), TEXT("SEND"), WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
        455, 310, 50, 26, hWnd, (HMENU)ID_BUTTON_SEND, hInst, NULL);
    SetClassLongPtr(tmp, GCL_HCURSOR, (LONG)hCursor);

    hEditText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""), WS_VISIBLE | WS_CHILD,
        10, 310, 440, 26, hWnd, (HMENU)ID_EDIT_TEXT_MESSAGES, hInst, NULL);

    hList = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), TEXT("listbox"), LBS_NOSEL |
        WS_VISIBLE | WS_CHILD | WS_VSCROLL,
        10, 10, 500, 300, hWnd, (HMENU)ID_LIST_BOX_MESSAGES, hInst, NULL);
    SetClassLongPtr(hList, GCL_HCURSOR, (LONG)hCursor);

    HWND hComboBox = CreateWindow(TEXT("COMBOBOX"), TEXT("COMBOBOX"),
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        520, 310, 150, 100, hWnd, NULL, hInst, NULL);
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Messages"));
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Images"));
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Canals"));
    SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Groups"));
    SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
    SetClassLongPtr(hComboBox, GCL_HCURSOR, (LONG)hCursor);

    tmp = CreateWindow(TEXT("BUTTON"), TEXT("CHECKBOX"), BS_CHECKBOX | WS_CHILD | WS_VISIBLE, 
        520, 270, 150, 26, hWnd, (HMENU)ID_CHECKBOX, hInst, NULL);
    SetClassLongPtr(tmp, GCL_HCURSOR, (LONG)hCursor);
    
    tmp = CreateWindow(TEXT("BUTTON"), TEXT("RADIO_BUTTON1"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        520, 210, 150, 26, hWnd, NULL, hInst, NULL);
    SetClassLongPtr(tmp, GCL_HCURSOR, (LONG)hCursor);

    tmp = CreateWindow(TEXT("BUTTON"), TEXT("RADIO_BUTTON2"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        520, 240, 150, 26, hWnd, NULL, hInst, NULL);
    SetClassLongPtr(tmp, GCL_HCURSOR, (LONG)hCursor);
    return;
}

void onFailed() {
    int wParam = MAKEWORD(0, IDM_LOGOUT);
    PostMessage(hWnd, WM_COMMAND, wParam, 0);
}

void onResume() {
    char* utfUsername = new char[30];
    WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, username, -1, utfUsername, 30, NULL, NULL);
    WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, lpString, -1, lpUtf, 100, NULL, NULL);
    ptClient = new ChatClient(lpUtf, utfUsername, onMessage, onFailed);
    CreateThread(0, 1024, ptClient->staticThreadStart, (void*)ptClient, 0, NULL);
    return;
}

BOOL CALLBACK DestoryChildCallback(HWND hwnd, LPARAM lParam)
{
    if (hwnd != NULL) {
        DestroyWindow(hwnd);
    }
    return TRUE;
}


//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            onCreate(hWnd);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_LOGOUT:
                EnumChildWindows(hWnd, DestoryChildCallback, NULL);
                ptClient->~ChatClient();
                onCreate(hWnd);
                break;
            case IDM_ABOUT:
                CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_BUTTON_SEND:
            {
                int n = GetWindowText(hEditText, lpString, 100);
                if (n > 0) {
                    SetWindowText(hEditText, TEXT(""));
                    WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, lpString, -1, lpUtf, 100, NULL, NULL);
                    ptClient->sendMessage(lpUtf);
                }
                break;
            }
            case ID_CHECKBOX:
                checked = !checked;
                if (checked) {
                    CheckDlgButton(hWnd, ID_CHECKBOX, BST_CHECKED);
                }
                else {
                    CheckDlgButton(hWnd, ID_CHECKBOX, BST_UNCHECKED);
                }
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_RBUTTONDOWN:
    {
        HMENU hMenu = CreatePopupMenu();
        AppendMenu(hMenu, MF_STRING, IDM_ABOUT, TEXT("About"));
        AppendMenu(hMenu, MF_STRING, ID_BUTTON_SEND, TEXT("Send message"));
        POINT p;
        p.x = LOWORD(lParam);
        p.y = HIWORD(lParam);
        ClientToScreen(hWnd, &p);
        TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN,
            p.x, p.y, 0, hWnd, NULL);
        break;
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HDC hCompatibleDC;
            PAINTSTRUCT PaintStruct;
            HANDLE hBitmap, hOldBitmap;
            BITMAP Bitmap;

            GetObject(hImg, sizeof(BITMAP), &Bitmap);
            hCompatibleDC = CreateCompatibleDC(hdc);
            hOldBitmap = SelectObject(hCompatibleDC, hImg);
            StretchBlt(hdc, 520, 100, 100, 100, hCompatibleDC, 0, 0, 100,
                100, SRCCOPY);
            SelectObject(hCompatibleDC, hOldBitmap);
            DeleteDC(hCompatibleDC);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        delete ptClient;
        delete lpUtf;
        delete username;
        delete lpString;
        DeleteObject(hImg);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна "Авторизация".
INT_PTR CALLBACK Login(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_IPADDRESS_DIALOG, TEXT("192.168.0.103"));
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDOK:
            {
                if (GetDlgItemText(hDlg, IDC_EDIT1, username, 30) > 0) {
                    GetDlgItemText(hDlg, IDC_IPADDRESS_DIALOG, lpString, 100);
                    onResume();
                    EndDialog(hDlg, LOWORD(wParam));
                }
                else {
                    SetDlgItemText(hDlg, IDC_EDIT1, TEXT("EMPTY NAME!!"));
                }
                return (INT_PTR)TRUE;
            }
            case IDCANCEL:
            {
                exit(1);
                return (INT_PTR)TRUE;
                break;
            }
            default:
                break;
            }
        }
    }
    return (INT_PTR)FALSE;
}
