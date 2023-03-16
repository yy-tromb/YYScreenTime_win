#include "./include/guiutils.h"

errno_t settingTabControl(HWND *hWindow, RECT *clientRect, HWND *hTab,
                          HINSTANCE *hInstance) {
   INITCOMMONCONTROLSEX initCommonControlsExData;
   TCITEMW tabItem;
   initCommonControlsExData.dwSize = sizeof(INITCOMMONCONTROLSEX);
   initCommonControlsExData.dwICC = ICC_TAB_CLASSES;
   if (InitCommonControlsEx(&initCommonControlsExData) == FALSE) {
      return (errno_t)1;
   }
   errno_t GetClientRect_error = GetClientRect(*hWindow, clientRect);
   if (GetClientRect_error == 0) {
      return (errno_t)GetLastError();
   }
   *hTab =
       CreateWindowExW(0, WC_TABCONTROLW, L"",
                       WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE |
                           TCS_RIGHTJUSTIFY |  TCS_HOTTRACK,
                       0, 0, clientRect->right, clientRect->bottom, *hWindow,
                       (HMENU)ID_TAB, *hInstance, NULL);
   if (*hTab == NULL) {
      return (errno_t)GetLastError();
   }
   PAINTSTRUCT paintStruct;
   HDC hDC = BeginPaint(*hWindow, &paintStruct);
   HFONT hFont =
       CreateFontW(0, 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                   VARIABLE_PITCH | FF_DONTCARE, L"Noto Sans");
   HFONT hFont_prev = SelectObject(hDC,hFont);
   if (hFont_prev == NULL || hFont_prev == HGDI_ERROR) {
      return (hFont_prev == HGDI_ERROR) ? (errno_t)2 : (errno_t)1;
   }
   SendMessageW(*hTab, WM_SETFONT, (WPARAM)hFont_prev, 0);
   tabItem.mask = TCIF_TEXT;
   tabItem.pszText = L"アプリタイマー";
   TabCtrl_InsertItem(*hTab, 0, &tabItem);
   tabItem.pszText = L"フォーカスモード";
   TabCtrl_InsertItem(*hTab, 0, &tabItem);
   tabItem.pszText = L"ホーム";
   TabCtrl_InsertItem(*hTab, 0, &tabItem);
   SelectObject(hDC,hFont);
   return (errno_t)0;
}
