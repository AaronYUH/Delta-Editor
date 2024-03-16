#include <stdio.h>
#include <windows.h>

#define WINDOW_W 500
#define WINDOW_H 500
#define SAVE_FILE 0
#define PREF_PLACEHOLDER 1
#define CLEAR_FILE 2

// window procedure
LRESULT CALLBACK xdProcedure(HWND, UINT, WPARAM, LPARAM);

// menus
void AddMenus(HWND);

// controls
void AddControls(HWND);

HMENU hMenu;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR args, int ncmdshow) {
    
    // struct
    WNDCLASSW xd = {0};

    xd.hbrBackground = (HBRUSH)COLOR_WINDOW;
    xd.hCursor = LoadCursor(NULL, IDC_ARROW);
    xd.hInstance = hInst;
    xd.lpszClassName = L"deltaProject";
    xd.lpfnWndProc = xdProcedure;

    if (!(RegisterClassW(&xd)))
        return -1;

    CreateWindowW(L"deltaProject", L"Delta", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE , 0, 0, WINDOW_W, WINDOW_H, NULL, NULL, NULL, NULL);

    // struct
    MSG msg = {0};

    // window loop
    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK xdProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

    switch (msg) {
        case WM_COMMAND:

            switch (wp) {
                case PREF_PLACEHOLDER:
                    MessageBeep(MB_OK);
                    break;
                case SAVE_FILE:
                    MessageBox(NULL, "Not yet implemented", "Err", MB_OK);
                    MessageBeep(MB_YESNO);
                    break;
            }
         
            break;
        case WM_CREATE:
            AddMenus(hWnd);
            AddControls(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
            break;
    }

}

void AddMenus(HWND hWnd) {
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, "File");
    AppendMenu(hMenu, MF_STRING, 0, "Preferences");

    // preference menu
    {
        AppendMenu(hFileMenu, MF_STRING, SAVE_FILE, "Save");
        AppendMenu(hFileMenu, MF_STRING, CLEAR_FILE, "Clear");
    }

    SetMenu(hWnd, hMenu);

}

void AddControls(HWND hWnd) {
    CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_AUTOVSCROLL | ES_MULTILINE, 0, 0, WINDOW_W, WINDOW_H, hWnd, NULL, NULL, NULL);
}