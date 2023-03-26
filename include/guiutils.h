#pragma once

#define UNICODE 1
#define _UNICODE 1
#ifndef __STDC_ISO_10646__
#define __STDC_ISO_10646__ 201706;
#endif
#ifndef __STDC_UTF_16__
#define __STDC_UTF_16__ 1;
#endif
#ifndef __STDC_UTF_32__
#define __STDC_UTF_32__ 1;
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>
#include "./resource.h"

#define TAB_NUM 3
#define TAB_TOP 0
#define TAB_FOCUSMODE 1
#define TAB_APPTIMER 2

errno_t setting_tabControl(HWND *hWindow,RECT *clientRect,HWND *hTab,HINSTANCE *hInstance);
errno_t setting_pages(HWND *hWindow, RECT *clientRect, HWND *hTab,
                      HINSTANCE *hInstance, HWND *hPage_top, HWND *hPage_focusmode,
                      HWND *hPage_apptimer, WNDPROC page_top_proc,
                      WNDPROC page_focusmode_proc, WNDPROC page_apptimer_proc);
errno_t changePage(int targetPageIndex,HWND tabs[]);
