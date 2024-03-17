#include <stdio.h>
#include <windows.h>

#define SAVE_FILE 0
#define PREF_PLACEHOLDER 1
#define CLEAR_FILE 2
#define OPEN_FILE 3
#define EXIT_APP 4

// window procedure
LRESULT CALLBACK xdProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ydProcedure(HWND, UINT, WPARAM, LPARAM);

// menus
void AddMenus(HWND);

// controls
void AddControls(HWND);
void AddControlsPref(HWND);

// file stuff (yuck)
void openF(HWND); // save for later very issue
void saveF(char[]);
void clearF(HWND);

// accesibility?
void adjustSize(HWND);

HMENU hMenu;
HWND hEdit;
HWND hParent;
HWND hPref;

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

    hParent = CreateWindowW(L"deltaProject", L"Delta", WS_OVERLAPPEDWINDOW | WS_VISIBLE , 0, 0, winWidth, winHeight, NULL, NULL, NULL, NULL);

    HINSTANCE relib = LoadLibrary("riched32.dll");

    if (relib == NULL)
        MessageBox(NULL, "couldn't load richedit32.dll", "", MB_ICONEXCLAMATION);

    // struct
    MSG msg = {0};

    // window loop
    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    /* 
        Creating Preference Window
    */

   // struct
    WNDCLASSW yd = {0};

    yd.hbrBackground = (HBRUSH)COLOR_WINDOW;
    yd.hCursor = LoadCursor(NULL, IDC_ARROW);
    yd.hInstance = hInst;
    yd.lpszClassName = L"prefWin";
    yd.lpfnWndProc = ydProcedure;

    if (!(RegisterClassW(&yd)))
        return -1;

    hPref = CreateWindowW(L"prefWin", L"Preferences", WS_OVERLAPPEDWINDOW | WS_VISIBLE , 0, 0, 400, 400, hParent, NULL, NULL, NULL);

    // struct
    MSG msgx = {0};

    // window loop
    while (GetMessage(&msgx, NULL, NULL, NULL)) {
        TranslateMessage(&msgx);
        DispatchMessage(&msgx);
    }

    return 0;
}

LRESULT CALLBACK xdProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    char text[1024];
    int closeMsg;

    switch (msg) {
        case WM_COMMAND:
            switch (wp) {
                case PREF_PLACEHOLDER:
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
                    break;
                case OPEN_FILE:
                    MessageBox(hWnd, "Not yet implemented.", "Alert", MB_OK);
                    break;
                case EXIT_APP:
                    closeMsg = MessageBox(hWnd, "Are you sure?", "Confirm", MB_OKCANCEL);
                    switch(closeMsg) {
                        case IDCANCEL:
                            return;
                            break;
                        case IDOK:
                            DestroyWindow(hWnd);
                            break;
                    }
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

LRESULT CALLBACK ydProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            AddControlsPref(hWnd);
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

    // file menu
    {
        AppendMenu(hFileMenu, MF_STRING, SAVE_FILE, "Save");
        AppendMenu(hFileMenu, MF_STRING, OPEN_FILE, "Open");
        AppendMenu(hFileMenu, MF_STRING, CLEAR_FILE, "Clear");
        AppendMenu(hFileMenu, MFT_SEPARATOR, NULL, NULL);
        AppendMenu(hFileMenu, MF_STRING, EXIT_APP, "Exit");
    }

    SetMenu(hWnd, hMenu);

}

void AddControls(HWND hWnd) {
    hEdit = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_AUTOVSCROLL | ES_MULTILINE, 0, 0, winWidth, winHeight, hWnd, NULL, NULL, NULL);
}

void AddControlsPref(HWND hWnd) {
    CreateWindowW(L"Button", L"Wow", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 100, 50, hWnd, NULL, NULL, NULL);
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

