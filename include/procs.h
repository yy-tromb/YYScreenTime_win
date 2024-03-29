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

LRESULT CALLBACK dialog_about_proc(HWND hDialog, UINT uMsg, WPARAM wParam,
                                   LPARAM lParam);
LRESULT CALLBACK page_top_proc(HWND hDialog, UINT uMsg, WPARAM wParam,
                               LPARAM lParam);
LRESULT CALLBACK page_focusmode_proc(HWND hDialog, UINT uMsg, WPARAM wParam,
                                     LPARAM lParam);
LRESULT CALLBACK page_apptimer_proc(HWND hDialog, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam);
