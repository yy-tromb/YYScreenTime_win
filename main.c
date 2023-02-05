#include <minwindef.h>
#define UNICODE 1
#define _UNICODE 1
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <windows.h>
#include <Psapi.h>
#include <locale.h>

#include <winAPI_highDPI.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR lpCmdLine, int nCmdShow) {
   winAPI_highDPI();
   setlocale(LC_ALL, "");
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
   wchar_t *processNames[processCount];
   int processNamesCount = 0;

   for (int i = 0; i < processCount; i++) {
      HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                    FALSE, allProcess[i]);
      if (hProcess != NULL) {
         DWORD fileNameSize;
         for (fileNameSize = 256; fileNameSize <= 65536; fileNameSize <<= 1) {
            // ファイル名を格納
            wchar_t fileName[fileNameSize];
            if (QueryFullProcessImageNameW(hProcess, 0, fileName,
                                           &fileNameSize) == 0) {
               if (ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
                  continue;  // エラーなのでループを続けて文字数を増やす
               } else {
                  // QueryFullProcessImageNameWでエラーだからループから抜ける
                  break;
               }
            } else {
               // ループから抜けて次のPIDへ
               fileNameSize++;
               wchar_t *fileName_tmp =
                   (wchar_t *)malloc(sizeof(wchar_t) * fileNameSize);
               wcscpy_s(fileName_tmp, fileNameSize, fileName);
               processNames[processNamesCount++] = fileName_tmp;
               break;
            }
         }
         CloseHandle(hProcess);
         continue;  // 明示
      } else {
         // Couldn't Get Process Handle
         continue;  // 明示
      }
   }

   FILE *file = fopen("./o.txt", "w , ccs=UTF-8");
   for (int i = 0; i < processNamesCount; i++) {
      fwprintf(file,L"%ls\n",processNames[i]);
   }

   return 0;
}
