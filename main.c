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

#include "./include/winAPI_highDPI.h"
#include "./include/winutils.h"
#include "./include/strutil.h"

LRESULT CALLBACK WndProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR lpCmdLine, int nCmdShow) {
   winAPI_highDPI();
   const wchar_t appName[] = L"mytest";
   WNDCLASS wndClass;
   HWND hWindow;
   MSG message;
   FILE *file;
   errno_t file_error = fopen_s(&file, "./o.txt", "w,ccs=UTF-8");
if(file_error != 0){
   char hoge[256];
   sprintf("file error:%d", file_error);
   MessageBoxA(NULL, hoge, "", MB_OK);
}
   DWORD *all_processes;
   unsigned int all_processes_count=0;
   errno_t getProcesses_error=getProcesses(all_processes, &all_processes_count);
   if(getProcesses_error != 0){
      char hoge[256];
      sprintf("getProcesses error:%d", getProcesses_error);
      MessageBoxA(NULL, hoge, "", MB_OK);
   }
   int i;
   FILE *f=fopen("./f.txt", "w+");
   for (i = 0; i < all_processes_count; i++) {
      fprintf(f, "%ld", all_processes[i]);
      wchar_t *processName;
      errno_t getProcessName_error = getProcessName(all_processes[i], processName);
      if (getProcessName_error != 0) {
         if(getProcessName_error==5){
            continue;
         }
         char hoge[256];
         sprintf(hoge,"%d", getProcessName_error);
         MessageBoxA(NULL, hoge, "", MB_OK);
      }else{
      char hoge[256];
      sprintf(hoge,"%d\n", all_processes_count);
      MessageBoxA(NULL, hoge, "", MB_OK);
      fwprintf(file, L"%ls\n",processName);
      }


   }
   fclose(f);

   HWND *hWindows;
   size_t hWindows_count;
   errno_t getWindowHandles_error = getWindowHandles(hWindows, &hWindows_count);

   fclose(file);

   wndClass.style = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc = WndProc;
   wndClass.cbClsExtra = 0;
   wndClass.cbWndExtra = 0;
   wndClass.hInstance = hInstance;
   wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wndClass.lpszMenuName = NULL;
   wndClass.lpszClassName = appName;

   if (!RegisterClass(&wndClass)) {
      return 0;
   }

   hWindow = CreateWindowW(appName, L"yyhome-tromb test", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

   if (!hWindow) {
      return 0;
   }

   ShowWindow(hWindow, nCmdShow);
   UpdateWindow(hWindow);

   while (GetMessage(&message, NULL, 0, 0) > 0) {
      TranslateMessage(&message);
      DispatchMessage(&message);
   }

   return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT uMsg, WPARAM wParam,
                         LPARAM lParam) {
   static int MOVE_x, MOVE_y;
   static int SIZE_x, SIZE_y;
   switch (uMsg) {
      case WM_CREATE:
         LPCREATESTRUCT lpcs;
         wchar_t message_CREATE[512];
         lpcs = (LPCREATESTRUCT)lParam;
         swprintf_s(message_CREATE, sizeof(message_CREATE) / 2,
                    L"lpszClass: %ls\nlpszName: %ls\n"
                    L"x: %d\ny: %d\ncx: %d\ncy: %d\n",
                    lpcs->lpszClass, lpcs->lpszName, lpcs->x, lpcs->y, lpcs->cx,
                    lpcs->cy);
         MessageBox(hWindow, message_CREATE, L"WM_CREATE",
                    MB_OK | MB_ICONINFORMATION);
         return 0;

      case WM_MOVE:
         wchar_t message_MOVE[128];
         MOVE_y = HIWORD(lParam);  // y 座標を取り出す
         MOVE_x = LOWORD(lParam);  // x 座標を取り出す
         swprintf_s(message_MOVE, sizeof(message_MOVE) / 2, L"WM_MOVE (%d, %d)",
                    MOVE_x, MOVE_y);
         SetWindowText(hWindow, message_MOVE);
         return 0;

      case WM_SIZE:
         wchar_t message_SIZE[128];
         SIZE_y = HIWORD(lParam);  // y 座標を取り出す
         SIZE_x = LOWORD(lParam);  // x 座標を取り出す
         swprintf_s(message_SIZE, sizeof(message_SIZE) / 2, L"WM_SIZE (%d, %d)",
                    SIZE_x, SIZE_y);
         SetWindowText(hWindow, message_SIZE);
         return 0;

      case WM_CLOSE:
         int messageboxID =
             MessageBox(hWindow, L"本当に終了しますか？", L"終了の確認",
                        MB_YESNO | MB_ICONWARNING);
         if (messageboxID == IDYES) {
            DestroyWindow(hWindow);
         }
         return 0;

      case WM_DESTROY:
         PostQuitMessage(0);
         return 0;
   }

   return DefWindowProc(hWindow, uMsg, wParam, lParam);
}
