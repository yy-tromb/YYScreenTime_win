#include "./include/winutils.h"

#define HEAP_COUNT 1024

errno_t getProcesses(DWORD *allProcesses, wchar_t **allProcessesNames,
                     int allProcesses_count) {
   DWORD allProcesses_size;
   DWORD neededSize;
   int count = 0;

   for (allProcesses_size = 2048; allProcesses_size <= 65536;
        allProcesses_size <<= 1) {
      DWORD allProcessesArray[allProcesses_size];
      neededSize = 0;
      if (EnumProcesses(allProcessesArray, sizeof(allProcessesArray),
                        &neededSize) == 0) {
         if (ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
            continue;
         } else {
            MessageBoxW(NULL, L"プロセスの取得に失敗しました", L"エラー",
                        MB_OK);
            return EXIT_FAILURE;
         }
      } else {
         allProcesses = allProcessesArray;
      }
   }

   count = neededSize / sizeof(DWORD);
   wchar_t *processNames[count];
   int processNames_count = 0;

   for (int i = 0; i < count; i++) {
      HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                    FALSE, allProcesses[i]);
      if (hProcess != NULL) {
         DWORD fileName_size;
         for (fileName_size = 256; fileName_size <= 65536;
              fileName_size <<= 1) {
            // ファイル名を格納
            wchar_t fileName[fileName_size];
            if (QueryFullProcessImageNameW(hProcess, 0, fileName,
                                           &fileName_size) == 0) {
               if (ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
                  continue;  // エラーなのでループを続けて文字数を増やす
               } else {
                  // QueryFullProcessImageNameWでエラーだからループから抜ける
                  break;
               }
            } else {
               // ループから抜けて次のPIDへ
               fileName_size++;
               wchar_t *fileName_tmp =
                   (wchar_t *)malloc(sizeof(wchar_t) * fileName_size);
               wcscpy_s(fileName_tmp, fileName_size, fileName);
               processNames[processNames_count++] = fileName_tmp;
               break;
            }
         }
         CloseHandle(hProcess);
         continue;  // 明示
      } else {
         // Couldn't OpenPeocess
         continue;  // 明示
      }
   }

   allProcessesNames = processNames;
   allProcesses_count = processNames_count;
   return EXIT_SUCCESS;
}

// 共有はしない
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

struct EnumWindowProcData {
   HWND *hWindows;
   size_t hWindows_count;
   size_t heap_count;
   errno_t error;
};

errno_t getWindowHandles(HWND *hWindows, size_t *hWindows_countP) {
   struct EnumWindowProcData enumWindowProcData;
   enumWindowProcData.hWindows=(HWND*)calloc(sizeof(HWND)*HEAP_COUNT,sizeof(HWND));
   if(enumWindowProcData.hWindows==NULL){
      return ERROR_NOT_ENOUGH_MEMORY;
   }else{
      enumWindowProcData.hWindows_count = 0;
      enumWindowProcData.heap_count=HEAP_COUNT;
      enumWindowProcData.hWindows_count=0;
      enumWindowProcData.error=0;
      WINBOOL enunWindowError = EnumWindows(EnumWindowsProc,(LPARAM)&enumWindowProcData);
      hWindows = enumWindowProcData.hWindows;
      *hWindows_countP = enumWindowProcData.hWindows_count;
      if(enunWindowError == 0){
         if(enumWindowProcData.error>0){
            return enumWindowProcData.error;
         }else{
         return (errno_t)GetLastError();
         }
      }else{
         //debug
         char hoge[256];
         sprintf(hoge,"%d",*hWindows_countP);
         MessageBoxA(NULL,hoge,"",NULL);
         //debug end
         return EXIT_SUCCESS;
      }
   }
}

BOOL CALLBACK EnumWindowsProc(HWND hWindow, LPARAM lParam) {
   static int count = 0;
   static HWND *hWindows;
   struct EnumWindowProcData *enumWindowProcDataP = (struct EnumWindowProcData*)lParam;
   int heap_count = enumWindowProcDataP->heap_count;
   hWindows=enumWindowProcDataP->hWindows;
   if(count > heap_count){
      heap_count+=1024;
      enumWindowProcDataP->heap_count=heap_count;
      hWindows=realloc(hWindows,heap_count);
      if(hWindows==NULL){
         enumWindowProcDataP->error=ERROR_NOT_ENOUGH_MEMORY;
         return FALSE;
      }else{
         enumWindowProcDataP->hWindows=hWindows;
      }
   }else{
      if(hWindow != NULL){
            hWindows[count++]=hWindow;
            enumWindowProcDataP->hWindows_count=count;
         }else{
            return TRUE; //continue
         }
   }
};
