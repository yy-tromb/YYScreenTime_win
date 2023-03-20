#include "./include/guiutils.h"

errno_t setting_tabControl(HWND *hWindowP, RECT *clientRectP, HWND *hTabP,
                           HINSTANCE *hInstanceP) {
   INITCOMMONCONTROLSEX initCommonControlsExData;
   TCITEMW tabItem;
   initCommonControlsExData.dwSize = sizeof(INITCOMMONCONTROLSEX);
   initCommonControlsExData.dwICC = ICC_TAB_CLASSES;
   if (InitCommonControlsEx(&initCommonControlsExData) == FALSE) {
      return (errno_t)1;
   }
   errno_t GetClientRect_error = GetClientRect(*hWindowP, clientRectP);
   if (GetClientRect_error == 0) {
      return (errno_t)GetLastError();
   }
   *hTabP = CreateWindowExW(0, WC_TABCONTROLW, L"",
                            WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE |
                                TCS_RIGHTJUSTIFY | TCS_HOTTRACK,
                            0, 0, clientRectP->right, clientRectP->bottom,
                            *hWindowP, (HMENU)ID_TAB, *hInstanceP, NULL);
   if (*hTabP == NULL) {
      return (errno_t)GetLastError();
   }
   PAINTSTRUCT paintStruct;
   HDC hDC = BeginPaint(*hWindowP, &paintStruct);
   HFONT hFont = CreateFontW(0, 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE,
                             SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             VARIABLE_PITCH | FF_DONTCARE, L"ＭＳ Ｐゴシック");
   HFONT hFont_prev = SelectObject(hDC, hFont);
   if (hFont_prev == NULL || hFont_prev == HGDI_ERROR) {
      return (hFont_prev == HGDI_ERROR) ? (errno_t)2 : (errno_t)1;
   }
   SendMessageW(*hTabP, WM_SETFONT, (WPARAM)hFont_prev, 0);
   tabItem.mask = TCIF_TEXT;
   tabItem.pszText = L"ホーム";
   TabCtrl_InsertItem(*hTabP, 0, &tabItem);
   tabItem.pszText = L"フォーカスモード";
   TabCtrl_InsertItem(*hTabP, 1, &tabItem);
   tabItem.pszText = L"アプリタイマー";
   TabCtrl_InsertItem(*hTabP, 2, &tabItem);
   SelectObject(hDC, hFont);
   SendMessageW(*hWindowP, WM_SIZE, 0,
                MAKELPARAM(clientRectP->right, clientRectP->bottom));
   return (errno_t)0;
}

errno_t setting_pages(HWND *hWindowP, RECT *clientRectP, HWND *hTabP,
                      HINSTANCE *hInstanceP, HWND *hPage_topP,
                      HWND *hPage_focusmodeP, HWND *hPage_apptimerP,
                      WNDPROC page_top_procP, WNDPROC page_focusmode_procP,
                      WNDPROC page_apptimer_procP) {
   WNDCLASSEXW childWindowClass;
   childWindowClass.cbSize = sizeof(WNDCLASSEXW);
   childWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
   childWindowClass.cbClsExtra = 0;
   childWindowClass.cbWndExtra = 0;
   childWindowClass.hInstance = *hInstanceP;
   childWindowClass.hIcon = NULL;
   childWindowClass.hIconSm = NULL;
   childWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
   childWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   childWindowClass.lpszMenuName = NULL;

   childWindowClass.lpszClassName = L"YYScreenTime_win_page_top";
   childWindowClass.lpfnWndProc = *page_top_procP;
   if (RegisterClassExW(&childWindowClass) == 0) {
      return GetLastError();
   }
   *hPage_topP = CreateWindowExW(
       0, L"YYScreenTime_win_page_top", L"", WS_CHILD | WS_VISIBLE,
       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, *hWindowP,
       (HMENU)ID_PAGE_TOP, *hInstanceP, NULL);
   if (*hPage_topP == NULL) {
      return GetLastError();
   }

   //page:focusmode
   childWindowClass.lpszClassName = L"YYScreenTime_win_page_focusmode";
   childWindowClass.lpfnWndProc = *page_focusmode_procP;
   if (RegisterClassExW(&childWindowClass) == 0) {
      return GetLastError();
   }
   *hPage_focusmodeP = CreateWindowExW(
       0, L"YYScreenTime_win_page_focusmode", L"", WS_CHILD | WS_VISIBLE,
       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, *hWindowP,
       (HMENU)ID_PAGE_TOP, *hInstanceP, NULL);
   if (*hPage_focusmodeP == NULL) {
      return GetLastError();
   }

   //page:apptimer
   childWindowClass.lpszClassName = L"YYScreenTime_win_page_apptimer";
   childWindowClass.lpfnWndProc = *page_focusmode_procP;
   if (RegisterClassExW(&childWindowClass) == 0) {
      return GetLastError();
   }
   *hPage_apptimerP = CreateWindowExW(
       0, L"YYScreenTime_win_page_apptimer", L"", WS_CHILD | WS_VISIBLE,
       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, *hWindowP,
       (HMENU)ID_PAGE_TOP, *hInstanceP, NULL);
   if (*hPage_apptimerP == NULL) {
      return GetLastError();
   }
   return 0;
}
