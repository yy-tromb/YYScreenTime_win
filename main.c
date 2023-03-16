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
   FILE *processes_out;
   FILE *windows_out;
   FILE *logFile;
   errno_t file_error =
       fopen_s(&processes_out, "./processes.txt", "w,ccs=UTF-8");
   if (file_error != 0) {
      char hoge[64];
      sprintf(hoge, "file error:%d", file_error);
      MessageBoxA(NULL, hoge, "", MB_OK);
      return 1;
   }
   file_error = fopen_s(&windows_out, "./windows.txt", "w,ccs=UTF-8");
   if (file_error != 0) {
      char hoge[64];
      sprintf(hoge, "file error:%d", file_error);
      MessageBoxA(NULL, hoge, "", MB_OK);
      return 1;
   }
   errno_t log_error = fopen_s(&logFile, "./log.txt", "w,ccs=UTF-8");
   if (log_error != 0) {
      return 1;
   } else {
      fputws(L"Start log...\n", logFile);
      fflush(logFile);
   }

   DWORD *all_processes;
   unsigned int all_processes_count = 0;
   errno_t getProcesses_error =
       getProcesses(&all_processes, &all_processes_count);
   if (getProcesses_error != 0) {
      fwprintf_s(logFile, L"error:%d @getProcesses line:%d\n",
                 getProcesses_error, __LINE__);
   } else {
      int i;
      for (i = 0; i < all_processes_count; i++) {
         wchar_t *processName;
         errno_t getProcessName_error =
             getProcessName(all_processes[i], &processName);
         if (getProcessName_error != 0) {
            fwprintf_s(logFile, L"error:%d @getProcessName line:%d\n",
                       getProcessName_error, __LINE__);
         } else {
            fwprintf_s(processes_out, L"%ls\n", processName);
         }
      }
   }
   fclose(processes_out);

   HWND *hWindows;
   size_t hWindows_count;
   errno_t getWindowHandles_error =
       getWindowHandles(&hWindows, &hWindows_count,false);
   if (getWindowHandles_error != 0) {
      fwprintf_s(logFile, L"error:%d @getWindowHandles line:%d\n",
                 getWindowHandles_error, __LINE__);
   } else {
      int i;
      for (i = 0; i < hWindows_count; i++) {
         DWORD processID;
         errno_t getHWindowPID_error = getHWindowPID(hWindows[i], &processID);
         if (getHWindowPID_error != 0) {
            fwprintf_s(logFile, L"error:%d @getHWindowPID line:%d\n",
                       getHWindowPID_error, __LINE__);
         } else {
            wchar_t *processName;
            errno_t getProcessName_error =
                getProcessName(processID, &processName);
            if (getProcessName_error != 0) {
               fwprintf_s(logFile, L"error:%d @getProcessName line:%d\n",
                          getProcessName_error, __LINE__);
            } else {
               fwprintf_s(windows_out, L"%ls\n", processName);
            }
         }
      }
   }
   fclose(windows_out);

   //wchar_t windowClassName[MAX_WINDOW_CLASS_NAME_LENGTH];
   //errno_t GetClassName_error = GetClassNameW(hWindow, windowClassName, MAX_WINDOW_CLASS_NAME_LENGTH);

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
         /*swprintf_s(message_CREATE, sizeof(message_CREATE) / 2,
                    L"lpszClass: %ls\nlpszName: %ls\n"
                    L"x: %d\ny: %d\ncx: %d\ncy: %d\n",
                    lpcs->lpszClass, lpcs->lpszName, lpcs->x, lpcs->y, lpcs->cx,
                    lpcs->cy);*/
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
