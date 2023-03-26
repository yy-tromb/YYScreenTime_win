#include "./include/main.h"

HINSTANCE hInstance_g;
HWND hWindow_g;
HWND hPage_top_g;
HWND hPage_focusmode_g;
HWND hPage_apptimer_g;
FILE *logFile_g;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR lpCmdLine, int nCmdShow) {
   winAPI_highDPI();
   hInstance_g = hInstance;
   const wchar_t appName[] = L"YYScreenTime_win";
   WNDCLASS wndClass;
   HWND hWindow;
   MSG message;
   FILE *processes_out;
   FILE *windows_out;
   FILE *logFile;
   errno_t file_error =
       fopen_s(&processes_out, "./processes.txt", "w,ccs=UTF-8");
   if (file_error != 0) {
      char hoge[64];
      sprintf(hoge, "file error:%d", file_error);
      MessageBoxA(NULL, hoge, "", MB_OK);
      return 1;
   }
   file_error = fopen_s(&windows_out, "./windows.txt", "w,ccs=UTF-8");
   if (file_error != 0) {
      char hoge[64];
      sprintf(hoge, "file error:%d", file_error);
      MessageBoxA(NULL, hoge, "", MB_OK);
      return 1;
   }
   logFile = fopen("./log.txt", "w,ccs=UTF-8");
   if (logFile == NULL) {
      return 1;
   } else {
      fputws(L"Start log...\n", logFile);
      fflush(logFile);
      logFile_g = logFile;
   }

   DWORD *all_processes;
   unsigned int all_processes_count = 0;
   errno_t getProcesses_error =
       getProcesses(&all_processes, &all_processes_count);
   if (getProcesses_error != 0) {
      fwprintf_s(logFile, L"error:%d @getProcesses line:%d\n",
                 getProcesses_error, __LINE__);
   } else {
      int i;
      for (i = 0; i < all_processes_count; i++) {
         wchar_t *processName;
         errno_t getProcessName_error =
             getProcessName(all_processes[i], &processName);
         if (getProcessName_error != 0) {
            fwprintf_s(logFile, L"error:%d @getProcessName line:%d\n",
                       getProcessName_error, __LINE__);
         } else {
            fwprintf_s(processes_out, L"%ls\n", processName);
         }
      }
   }
   fclose(processes_out);

   HWND *hWindows;
   size_t hWindows_count;
   errno_t getWindowHandles_error =
       getWindowHandles(&hWindows, &hWindows_count, false);
   if (getWindowHandles_error != 0) {
      fwprintf_s(logFile, L"error:%d @getWindowHandles line:%d\n",
                 getWindowHandles_error, __LINE__);
   } else {
      int i;
      for (i = 0; i < hWindows_count; i++) {
         DWORD processID;
         errno_t getHWindowPID_error = getHWindowPID(hWindows[i], &processID);
         if (getHWindowPID_error != 0) {
            fwprintf_s(logFile, L"error:%d @getHWindowPID line:%d\n",
                       getHWindowPID_error, __LINE__);
         } else {
            wchar_t *processName;
            errno_t getProcessName_error =
                getProcessName(processID, &processName);
            if (getProcessName_error != 0) {
               fwprintf_s(logFile, L"error:%d @getProcessName line:%d\n",
                          getProcessName_error, __LINE__);
            } else {
               fwprintf_s(windows_out, L"%ls\n", processName);
            }
         }
      }
   }
   fclose(windows_out);

   // wchar_t windowClassName[MAX_WINDOW_CLASS_NAME_LENGTH];
   // errno_t GetClassName_error = GetClassNameW(hWindow, windowClassName,
   // MAX_WINDOW_CLASS_NAME_LENGTH);

   wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
   wndClass.lpfnWndProc = WndProc;
   wndClass.cbClsExtra = 0;
   wndClass.cbWndExtra = 0;
   wndClass.hInstance = hInstance;
   wndClass.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(APPICON));
   wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wndClass.lpszMenuName = MAKEINTRESOURCEW(ID_MENU);
   wndClass.lpszClassName = appName;

   if (RegisterClassW(&wndClass) == 0) {
      errno_t error = GetLastError();
      fwprintf_s(logFile, L"error:%d @RegisterClassW", error);
      return error;
   }

   hWindow = CreateWindowW(appName, L"YYScreenTime_win", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

   if (hWindow == NULL) {
      errno_t error = GetLastError();
      fwprintf_s(logFile, L"error:%d @CreateWindowExW", error);
      return error;
   }
   hWindow_g = hWindow;

   ShowWindow(hWindow, nCmdShow);
   UpdateWindow(hWindow);

   HICON hAppIcon = LoadImageW(hInstance, MAKEINTRESOURCEW(APPICON), IMAGE_ICON,
                               0, 0, LR_DEFAULTCOLOR);
   SendMessageW(hWindow, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hAppIcon);

   while (GetMessage(&message, NULL, 0, 0) > 0) {
      TranslateMessage(&message);
      DispatchMessage(&message);
   }

   fclose(logFile_g);
   return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT uMsg, WPARAM wParam,
                         LPARAM lParam) {
   static int MOVE_x, MOVE_y;
   static int SIZE_x, SIZE_y;
   static int wm_id, wm_event;
   static HMENU hMenu;
   static unsigned int resident_flag, startup_flag;
   static HWND tabs[TAB_NUM];
   static int selectedTab=0,previousTab=0;
   static RECT clientRect, tabRect;
   static dx, dy;
   static HWND hTab;
   static POINT window_point, tab_point, page_top_point, page_focus_point,
       page_timer_point;

   HDC hDC;
   static PAINTSTRUCT paintStruct;
   static HPEN hPen, hPen_prev;
   static HBRUSH hBrush, hBrush_prev;
   static HFONT hFont, hFont_prev;

   hMenu = GetMenu(hWindow);

   switch (uMsg) {
      case WM_CREATE:
         LPCREATESTRUCTW lpcs;
         wchar_t message_CREATE[512];
         lpcs = (LPCREATESTRUCTW)lParam;
         errno_t setting_tabControl_error =
             setting_tabControl(&hWindow, &clientRect, &hTab, &hInstance_g);
         errno_t setting_pages_error = setting_pages(
             &hWindow, &clientRect, &hTab, &hInstance_g, &hPage_top_g,
             &hPage_focusmode_g, &hPage_apptimer_g, page_top_proc,
             page_focusmode_proc, page_apptimer_proc);
         if (setting_tabControl_error != 0 || setting_pages_error != 0) {
            checkErrorExit(&hWindow);
            return 0;
         }
         tabs[TAB_TOP]=hPage_top_g;
         tabs[TAB_FOCUSMODE]=hPage_focusmode_g;
         tabs[TAB_APPTIMER]=hPage_apptimer_g;
         SendMessageW(hWindow, WM_SIZE, 0,
                      MAKELPARAM(clientRect.right, clientRect.bottom));
         /*swprintf_s(message_CREATE, sizeof(message_CREATE) / 2,
                    L"lpszClass: %ls\nlpszName: %ls\n"
                    L"x: %d\ny: %d\ncx: %d\ncy: %d\n",
                    lpcs->lpszClass, lpcs->lpszName, lpcs->x, lpcs->y, lpcs->cx,
                    lpcs->cy);*/
         return 0;
         break;

      case WM_COMMAND:
         wm_id = LOWORD(wParam);
         wm_event = HIWORD(wParam);
         switch (wm_id) {
            case IDM_FOCUSMODE:
               previousTab=selectedTab;
               selectedTab=TAB_FOCUSMODE;
               TabCtrl_SetCurSel(hTab,TAB_FOCUSMODE);
               changePage(selectedTab,tabs);
               break;

            case IDM_APPTIMER:
               previousTab = selectedTab;
               selectedTab = TAB_FOCUSMODE;
               TabCtrl_SetCurSel(hTab, TAB_FOCUSMODE);
               changePage(selectedTab, tabs);
               break;

            case IDM_RESIDENT:
            case IDM_STARTUP:
               MENUITEMINFOW menuItemInfo = {sizeof(MENUITEMINFOW), MIIM_STATE};
               GetMenuItemInfoW(hMenu, wm_id, FALSE, &menuItemInfo);
               menuItemInfo.fState = (menuItemInfo.fState == MFS_CHECKED)
                                         ? MFS_UNCHECKED
                                         : MFS_CHECKED;
               SetMenuItemInfoW(hMenu, wm_id, FALSE, &menuItemInfo);
               InvalidateRect(hWindow, NULL, FALSE);
               break;

            case IDM_ABOUT:
               DialogBoxW(hInstance_g, MAKEINTRESOURCEW(IDD_ABOUT), hWindow,
                          dialog_about_proc);
               break;

            case IDM_EXIT:
               if (MessageBoxW(hWindow, L"本当に終了しますか？", L"終了の確認",
                               MB_YESNO | MB_ICONWARNING) == IDYES) {
                  DestroyWindow(hWindow);
               } else {
                  return 0;
               }
               break;

            default:
               return DefWindowProcW(hWindow, uMsg, wParam, lParam);
         }
         break;

      case WM_GETMINMAXINFO:
         MINMAXINFO *minMaxInfo;
         minMaxInfo = (MINMAXINFO *)lParam;
         minMaxInfo->ptMinTrackSize.x = 960;  // 最小幅
         minMaxInfo->ptMinTrackSize.y = 640;  // 最小高
         return 0;
         break;

      case WM_MOVE:
         wchar_t message_MOVE[128];
         MOVE_y = HIWORD(lParam);  // y 座標を取り出す
         MOVE_x = LOWORD(lParam);  // x 座標を取り出す
         swprintf_s(message_MOVE, sizeof(message_MOVE) / 2, L"WM_MOVE (%d, %d)",
                    MOVE_x, MOVE_y);
         SetWindowText(hWindow, message_MOVE);
         return 0;
         break;

      case WM_SIZE:
         wchar_t message_SIZE[128];
         SIZE_y = HIWORD(lParam);  // y 座標を取り出す
         SIZE_x = LOWORD(lParam);  // x 座標を取り出す
         WINBOOL GetClientRect_error = GetClientRect(hWindow, &clientRect);
         if (GetClientRect_error == 0) {
            logError(__LINE__,GetLastError());
            checkErrorExit(&hWindow);
            return 0;
         }
         MoveWindow(hTab, 0, 0, SIZE_x, SIZE_y, TRUE);
         GetClientRect_error = GetClientRect(hTab, &tabRect);
         if (GetClientRect_error == 0) {
            logError(__LINE__,GetLastError());
            checkErrorExit(&hWindow);
            return 0;
         }
         tabRect = clientRect;
         TabCtrl_AdjustRect(hTab, FALSE, &tabRect);

         window_point.x = window_point.y = 0;
         WINBOOL ClientToScreen_error = ClientToScreen(hWindow, &window_point);
         if (ClientToScreen_error == 0) {
            logError(__LINE__,GetLastError());
            checkErrorExit(&hWindow);
            return 0;
         }
         tab_point.x = tab_point.y = 0;
         ClientToScreen_error= ClientToScreen(hTab, &tab_point);
         if (ClientToScreen_error == 0) {
            logError(__LINE__,GetLastError());
            checkErrorExit(&hWindow);
            return 0;
         }

         dx = tab_point.x - window_point.x;
         dy = tab_point.y - window_point.y;
         WINBOOL MoveWindow_error = MoveWindow(hPage_top_g, tabRect.left + dx, tabRect.top + dy,
                    tabRect.right - tabRect.left, tabRect.bottom - tabRect.top,
                    TRUE);
         if (MoveWindow_error == 0) {
            logError(__LINE__,GetLastError());
            checkErrorExit(&hWindow);
            return 0;
         }
         MoveWindow_error = MoveWindow(hPage_focusmode_g, tabRect.left + dx, tabRect.top + dy,
                    tabRect.right - tabRect.left, tabRect.bottom - tabRect.top,
                    TRUE);
         if (MoveWindow_error == 0) {
            logError(__LINE__,GetLastError());
            checkErrorExit(&hWindow);
            return 0;
         }
         MoveWindow_error = MoveWindow(hPage_apptimer_g, tabRect.left + dx, tabRect.top + dy,
                    tabRect.right - tabRect.left, tabRect.bottom - tabRect.top,
                    TRUE);
         if (MoveWindow_error == 0) {
            logError(__LINE__,GetLastError());
            checkErrorExit(&hWindow);
            return 0;
         }
         swprintf_s(message_SIZE, sizeof(message_SIZE) / 2, L"WM_SIZE (%d, %d)",
                    SIZE_x, SIZE_y);
         SetWindowTextW(hWindow, message_SIZE);
         return 0;
         break;

      case WM_NOTIFY:
         switch (((NMHDR *)lParam)->code) {
            case TCN_SELCHANGE:
               InvalidateRect(hTab, NULL, TRUE);
               selectedTab = TabCtrl_GetCurSel(hTab);
               changePage(selectedTab,tabs);
               return 0;
               break;

            case TCN_SELCHANGING:
               previousTab = TabCtrl_GetCurSel(hTab);
               return 0;
               break;
         }
         break;

      case WM_CLOSE:
         if (MessageBoxW(hWindow, L"本当に終了しますか？", L"終了の確認",
                         MB_YESNO | MB_ICONWARNING) == IDYES) {
            DestroyWindow(hWindow);
         } else {
            return 0;
         }
         break;

      case WM_DESTROY:
         PostQuitMessage(0);
         return 0;
         break;
   }

   return DefWindowProcW(hWindow, uMsg, wParam, lParam);
}

