#include "./include/winutils.h"

#define HEAP_COUNT 1024

errno_t getProcesses(DWORD *all_processes, unsigned int *all_processes_count) {
   DWORD allProcesses_size;
   DWORD neededSize;
   DWORD *allProcesses_tmp=NULL;

      for (allProcesses_size = 2048; allProcesses_size <= 65536;
            allProcesses_size <<= 1) {
      allProcesses_tmp=(DWORD*)realloc(allProcesses_tmp,sizeof(DWORD)*allProcesses_size);
      neededSize = 0;
      if (EnumProcesses(allProcesses_tmp, allProcesses_size,
                        &neededSize) == 0) {
         if (ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
            char hoge[256] = "ee";
            MessageBoxA(NULL, hoge, "", MB_OK);
            continue;
         } else {
            char hoge[256]="e";
            MessageBoxA(NULL, hoge, "", MB_OK);
            all_processes = NULL;
            return (errno_t)GetLastError();
         }
      } else {
         all_processes = allProcesses_tmp;
         break;
      }
   }
   char hoge[256];
   sprintf(hoge,"%u",neededSize);
   MessageBoxA(NULL, hoge, "", MB_OK);
   *all_processes_count = neededSize / sizeof(DWORD);
   return EXIT_SUCCESS;
}

errno_t getProcessName(DWORD processID, wchar_t *exeName) {
   HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                 FALSE, processID);
   if (hProcess != NULL) {
      DWORD fileName_size;
      for (fileName_size = 256; fileName_size <= 65536; fileName_size <<= 1) {
         wchar_t fileName[fileName_size];
         DWORD result_name_size=fileName_size;
         if (QueryFullProcessImageNameW(hProcess, 0, fileName,
                                        &result_name_size) == 0) {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
               continue;  // 文字を増やす
            } else {
               CloseHandle(hProcess);
               return (errno_t)GetLastError();
            }
         } else {
            wchar_t *tmp=(wchar_t*)malloc(sizeof(wchar_t)*result_name_size);
            wcscpy_s(tmp,result_name_size,fileName);
            exeName = tmp;
            break;
         }
      }
      CloseHandle(hProcess);
      MessageBoxA(NULL, "gpn", "", MB_OK);
      return EXIT_SUCCESS;
   } else {
      //MessageBoxA(NULL, "err2", "", MB_OK);
      return (errno_t)GetLastError();
   }
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
   enumWindowProcData.hWindows =
       (HWND *)calloc(sizeof(HWND) * HEAP_COUNT, sizeof(HWND));
   if (enumWindowProcData.hWindows == NULL) {
      return ERROR_NOT_ENOUGH_MEMORY;
   } else {
      enumWindowProcData.hWindows_count = 0;
      enumWindowProcData.heap_count = HEAP_COUNT;
      enumWindowProcData.hWindows_count = 0;
      enumWindowProcData.error = 0;
      WINBOOL enunWindowError =
          EnumWindows(EnumWindowsProc, (LPARAM)&enumWindowProcData);
      hWindows = enumWindowProcData.hWindows;
      *hWindows_countP = enumWindowProcData.hWindows_count;
      if (enunWindowError == 0) {
         if (enumWindowProcData.error > 0) {
            return enumWindowProcData.error;
         } else {
            return (errno_t)GetLastError();
         }
      } else {
         return EXIT_SUCCESS;
      }
   }
}

BOOL CALLBACK EnumWindowsProc(HWND hWindow, LPARAM lParam) {
   static int count = 0;
   static HWND *hWindows;
   struct EnumWindowProcData *enumWindowProcDataP =
       (struct EnumWindowProcData *)lParam;
   int heap_count = enumWindowProcDataP->heap_count;
   hWindows = enumWindowProcDataP->hWindows;
   if (count > heap_count) {
      heap_count += 1024;
      enumWindowProcDataP->heap_count = heap_count;
      hWindows = realloc(hWindows, heap_count);
      if (hWindows == NULL) {
         enumWindowProcDataP->error = ERROR_NOT_ENOUGH_MEMORY;
         return FALSE;
      } else {
         enumWindowProcDataP->hWindows = hWindows;
      }
   }
   if (hWindow != NULL) {
      hWindows[count++] = hWindow;
      enumWindowProcDataP->hWindows_count = count;
      return TRUE;
   } else {
      return TRUE;  // continue
   }
};

errno_t getHWindowPID(HWND *hWindow, DWORD *processID){
   if ( GetWindowThreadProcessId(*hWindow,processID) == 0){
      return (errno_t)GetLastError();
   }else{
      return EXIT_SUCCESS;
   }
}
