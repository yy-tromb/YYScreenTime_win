#include "./include/procs.h"

LRESULT CALLBACK dialog_about_proc(HWND hDialog, UINT uMsg, WPARAM wParam,
                                   LPARAM lParam) {
   UNREFERENCED_PARAMETER(lParam);
   switch (uMsg) {
      case WM_INITDIALOG:
         return (INT_PTR)TRUE;

      case WM_COMMAND:
         if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDialog, LOWORD(wParam));
            return (INT_PTR)TRUE;
         }
   }
   return (INT_PTR)FALSE;
}

LRESULT CALLBACK page_top_proc(HWND hPage_top, UINT uMsg, WPARAM wParam,
                               LPARAM lParam) {
   return DefWindowProcW(hPage_top, uMsg, wParam, lParam);
}

LRESULT CALLBACK page_focusmode_proc(HWND hPage_focusmode, UINT uMsg,
                                     WPARAM wParam, LPARAM lParam) {
   return DefWindowProcW(hPage_focusmode, uMsg, wParam, lParam);
}

LRESULT CALLBACK page_apptimer_proc(HWND hPage_apptimer, UINT uMsg,
                                    WPARAM wParam, LPARAM lParam) {
   return DefWindowProcW(hPage_apptimer, uMsg, wParam, lParam);
}
