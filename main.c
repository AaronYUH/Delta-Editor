#include <stdio.h>
#include <windows.h>
#include <commdlg.h>


#define SAVE_FILE 0
#define PREF_PLACEHOLDER 1
#define CLEAR_FILE 2
#define OPEN_FILE 3

// window procedure
LRESULT CALLBACK xdProcedure(HWND, UINT, WPARAM, LPARAM);

// menus
void AddMenus(HWND);

// controls
void AddControls(HWND);

// file stuff (yuck)
void openF(HWND); // save for later very issue
void saveF(char[]);
void clearF(HWND);

// accesibility?
void adjustSize(HWND);

HMENU hMenu;
HWND hEdit;

int winWidth = 500;
int winHeight = 500;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR args, int ncmdshow) {
    ncmdshow = 0;
    // struct
    WNDCLASSW xd = {0};

    xd.hbrBackground = (HBRUSH)COLOR_WINDOW;
    xd.hCursor = LoadCursor(NULL, IDC_ARROW);
    xd.hInstance = hInst;
    xd.lpszClassName = L"deltaProject";
    xd.lpfnWndProc = xdProcedure;

    if (!(RegisterClassW(&xd)))
        return -1;

    CreateWindowW(L"deltaProject", L"Delta", WS_OVERLAPPEDWINDOW | WS_VISIBLE , 0, 0, winWidth, winHeight, NULL, NULL, NULL, NULL);

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
    char text[1024];

    switch (msg) {
        case WM_COMMAND:
            switch (wp) {
                case PREF_PLACEHOLDER:
                    MessageBeep(MB_OK);
                    break;
                case SAVE_FILE:
                    GetWindowText(hEdit, text, sizeof(text));
                    printf(text);
                    saveF(text);
                    MessageBox(hWnd, "File Saved", "Notification", MB_OK);
                    break;
                case CLEAR_FILE:
                    clearF(hEdit);
                    MessageBox(hWnd, "File Cleared", "Notification", MB_OK);
                case OPEN_FILE:
                    MessageBeep(MB_OK);
                    break;
            }   
            break;
        case WM_CREATE:
            AddMenus(hWnd);
            AddControls(hWnd);
            break;
        case WM_SIZE:
            adjustSize(hWnd);
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
    AppendMenu(hMenu, MF_STRING, PREF_PLACEHOLDER, "Preferences");

    // preference menu
    {
        AppendMenu(hFileMenu, MF_STRING, SAVE_FILE, "Save");
        AppendMenu(hFileMenu, MF_STRING, OPEN_FILE, "Open");
        AppendMenu(hFileMenu, MF_STRING, CLEAR_FILE, "Clear");
    }

    SetMenu(hWnd, hMenu);

}

void AddControls(HWND hWnd) {
    hEdit = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_AUTOVSCROLL | ES_MULTILINE, 0, 0, winWidth, winHeight, hWnd, NULL, NULL, NULL);
}

void openF(HWND hWnd) {
    return;
}

void saveF(char text[]) {
    FILE *fptr;

    fptr = fopen("main.txt", "w");

    fprintf(fptr, text);
    fclose(fptr);
}

void clearF(HWND hWnd) {
    SetWindowText(hWnd, "");
}

void adjustSize(HWND hWnd) {
    RECT r;

    if (GetWindowRect(hWnd, &r)) {
        int w = r.right - r.left;
        int h = r.bottom - r.top;
        
        SetWindowPos(hEdit, NULL, 0, 0, w, h, SWP_NOMOVE);
    }
}