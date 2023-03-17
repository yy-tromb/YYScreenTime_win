#include "./include/guiutils.h"

errno_t settingTabControl(HWND *hWindowP, RECT *clientRectP, HWND *hTabP,
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
   *hTabP =
       CreateWindowExW(0, WC_TABCONTROLW, L"",
                       WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE |
                           TCS_RIGHTJUSTIFY |  TCS_HOTTRACK,
                       0, 0, clientRectP->right, clientRectP->bottom, *hWindowP,
                       (HMENU)ID_TAB, *hInstanceP, NULL);
   if (*hTabP == NULL) {
      return (errno_t)GetLastError();
   }
   PAINTSTRUCT paintStruct;
   HDC hDC = BeginPaint(*hWindowP, &paintStruct);
   HFONT hFont = CreateFontW(0, 0, 0, 0, FW_ULTRABOLD, FALSE, FALSE, FALSE,
                             SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             VARIABLE_PITCH | FF_DONTCARE, L"ＭＳ Ｐゴシック");
   HFONT hFont_prev = SelectObject(hDC,hFont);
   if (hFont_prev == NULL || hFont_prev == HGDI_ERROR) {
      return (hFont_prev == HGDI_ERROR) ? (errno_t)2 : (errno_t)1;
   }
   SendMessageW(*hTabP, WM_SETFONT, (WPARAM)hFont_prev, 0);
   tabItem.mask = TCIF_TEXT;
   tabItem.pszText = L"アプリタイマー";
   TabCtrl_InsertItem(*hTabP, 0, &tabItem);
   tabItem.pszText = L"フォーカスモード";
   TabCtrl_InsertItem(*hTabP, 0, &tabItem);
   tabItem.pszText = L"ホーム";
   TabCtrl_InsertItem(*hTabP, 0, &tabItem);
   SelectObject(hDC,hFont);
   //End Tab

   //child windows

   SendMessageW(*hWindowP,WM_SIZE,0,MAKELPARAM(clientRectP->right,clientRectP->bottom));
   return (errno_t)0;
}
