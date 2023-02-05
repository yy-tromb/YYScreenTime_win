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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine,
                   int nCmdShow) {
   DWORD allProcess[1024];
   int processCount;
   DWORD neededSize;

   if(EnumProcesses(allProcess,sizeof(allProcess),&neededSize)==0){
      MessageBoxW(NULL,L"プロセスの取得に失敗しました",L"エラー",MB_OK);
      exit(EXIT_FAILURE);
      return EXIT_FAILURE;
   }
   
   //
}
