#define UNICODE 1
#define _UNICODE 1
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
#include <Psapi.h>
#include <time.h>

#include <winAPI_highDPI.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine,
                   int nCmdShow) {
   DWORD allProcess[1024];
   int processCount;
   DWORD neededSize;
}
