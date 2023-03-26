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
#include <time.h>
#include <wchar.h>
#include <windows.h>
#include <Psapi.h>
#include <locale.h>
#include <commctrl.h>

#include "./winAPI_highDPI.h"
#include "./winutils.h"
#include "./strutil.h"
#include "./guiutils.h"
#include "./include/procs.h"
#include "./resource.h"

#define perY(percent) ((int)SIZE_y * (percent / 100))
#define perX(percent) ((int)SIZE_x * (percent / 100))

#define ShowDebugInfo(line) ({char hoge[64];sprintf_s(hoge,64,"%d",line);MessageBoxA(NULL,hoge,"",MB_OK);})
#define logError(line,error) ({fwprintf_s(logFile_g,L"error:%d line:%d\n",error,line);fflush(logFile_g);})

LRESULT CALLBACK WndProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);
