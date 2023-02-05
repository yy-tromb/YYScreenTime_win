#include <minwindef.h>
#define UNICODE 1
#define _UNICODE 1
#include <Psapi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <winAPI_highDPI.h>
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR lpCmdLine, int nCmdShow) {
   winAPI_highDPI();
   DWORD init_allProcess[1024];
   DWORD *allProcess;
   DWORD neededSize;
   int processCount;

   if (EnumProcesses(init_allProcess, sizeof(init_allProcess), &neededSize) ==
       0) {
      MessageBoxW(NULL, L"プロセスの取得に失敗しました", L"エラー", MB_OK);
      exit(EXIT_FAILURE);
      return EXIT_FAILURE;
   }

   if (neededSize >= sizeof(init_allProcess)) {
      int instead_allprocess_Size = sizeof(init_allProcess);
      while (true) {
         if (neededSize >= instead_allprocess_Size) {
            DWORD instead_allProcess[(neededSize / sizeof(DWORD)) + 1024];
            neededSize = 0;
            if (EnumProcesses(instead_allProcess, sizeof(instead_allProcess),
                              &neededSize) == 0) {
               MessageBoxW(NULL, L"プロセスの取得に失敗しました", L"エラー",
                           MB_OK);
               exit(EXIT_FAILURE);
               return EXIT_FAILURE;
            } else {
               instead_allprocess_Size = sizeof(instead_allProcess);
               allProcess = instead_allProcess;
            }
         } else {
            break;
         }
      }
   } else {
      allProcess = init_allProcess;
   }

   processCount = neededSize / sizeof(DWORD);
   wchar_t processNames[processCount][MAX_PATH];
   int processNameCount = 0;

   for (int i = 0; i < processCount; i++) {
      HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                    FALSE, allProcess[i]);
      if (hProcess != NULL) {
         HMODULE hMod;
         DWORD neededSize;
      } else {
      }
   }
}
