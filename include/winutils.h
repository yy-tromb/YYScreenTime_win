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

#define MAX_WINDOW_CLASS_NAME_LENGTH 256  // NULL 文字終端込み

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Psapi.h>

errno_t checkErrorExit(HWND *hWindow);

errno_t getProcesses(DWORD **all_processes, unsigned int *process_count);
errno_t getProcessName(DWORD processID, wchar_t **exeName);
errno_t getWindowHandles(HWND **hWindows, size_t *hWindows_countP,bool filter);
errno_t getHWindowPID(HWND hWindow,DWORD *processID);
errno_t searchWindow(wchar_t searchName, HWND *hWindowP);
