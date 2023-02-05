#pragma once

#ifndef UNICODE
#define UNICODE 1
#endif

#ifndef _UNICODE
#define _UNICODE 1
#endif

#include <windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")
#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")

#ifndef _DPI_AWARENESS_CONTEXTS_
DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
#endif

#ifndef _DPI_AWARENESS_CONTEXTS_

typedef enum _DPI_AWARENESS {
   DPI_AWARENESS_INVALID = -1,
   DPI_AWARENESS_UNAWARE = 0,
   DPI_AWARENESS_SYSTEM_AWARE = 1,
   DPI_AWARENESS_PER_MONITOR_AWARE = 2,
} DPI_AWARENESS;

#define DPI_AWARENESS_CONTEXT_UNAWARE ((DPI_AWARENESS_CONTEXT)-1)
#define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE ((DPI_AWARENESS_CONTEXT)-2)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE ((DPI_AWARENESS_CONTEXT)-3)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#define DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED ((DPI_AWARENESS_CONTEXT)-5)

#endif

typedef HRESULT(_stdcall *GetDpiForMonitorFunc)(HMONITOR, MONITOR_DPI_TYPE, UINT *, UINT *);
typedef HRESULT(_stdcall* EnableNonClientDpiScalingFunc)(HWND);
typedef HRESULT(_stdcall* SetThreadDpiAwarenessContextFunc)(DPI_AWARENESS_CONTEXT);
typedef BOOL(_stdcall *SetProcessDPIAwareFunc)(VOID);

static HMODULE hModUser32;
static EnableNonClientDpiScalingFunc NCCfunc = 0;
static SetThreadDpiAwarenessContextFunc ThreadAwareFunc = 0;

typedef void(WINAPI *RtlGetVersion_FUNC)(OSVERSIONINFOEXW *);

int win10ver2(void);

int winAPI_highDPI(void);
