#include "./include/winutils.h"

#define GET_WINDOWS_HEAP_COUNT 1024

errno_t getProcesses(DWORD **all_processes, unsigned int *all_processes_count) {
   DWORD allProcesses_size;
   DWORD neededSize;
   DWORD *allProcesses_tmp = NULL;

   for (allProcesses_size = 4096; allProcesses_size <= 65536;
        allProcesses_size <<= 1) {
      allProcesses_tmp = (DWORD *)realloc(allProcesses_tmp, allProcesses_size);
      neededSize = 0;
      if (EnumProcesses(allProcesses_tmp, allProcesses_size, &neededSize) ==
          0) {
         MessageBoxA(NULL, "any error", "", MB_OK);
         *all_processes = NULL;
         *all_processes_count = 0;
         free(allProcesses_tmp);
         return (errno_t)GetLastError();
      } else {
         if (neededSize >= allProcesses_size) {
            continue;  // メモリを増やす
         }
         break;
      }
   }
   *all_processes = allProcesses_tmp;
   *all_processes_count = neededSize / sizeof(DWORD);
   return EXIT_SUCCESS;
}

errno_t getProcessName(DWORD processID, wchar_t **processName) {
   HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                 FALSE, processID);
   if (hProcess != NULL) {
      DWORD fileName_size;
      wchar_t *fileName = NULL;
      for (fileName_size = 256; fileName_size <= 65536; fileName_size <<= 1) {
         fileName =
             (wchar_t *)realloc(fileName, sizeof(wchar_t) * fileName_size);
         DWORD result_name_size = fileName_size;
         if (QueryFullProcessImageNameW(hProcess, 0, fileName,
                                        &result_name_size) == 0) {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
               continue;  // 文字を増やす
            } else {
               CloseHandle(hProcess);
               return (errno_t)GetLastError();
            }
         } else {
            *processName = fileName;
            break;
         }
      }
      CloseHandle(hProcess);
      return EXIT_SUCCESS;
   } else {
      *processName = NULL;
      return (errno_t)GetLastError();
   }
}

// 共有はしない
BOOL CALLBACK EnumWindowsProc(HWND hWindow, LPARAM lParam);
bool isWindowAppearingInTaskbar(HWND hWindow, DWORD windowStyle,
                                DWORD windowExStyle);

struct EnumWindowProcData {
   HWND *hWindows;
   size_t hWindows_count;
   size_t heap_count;
   errno_t error;
   bool filter;
};

errno_t getWindowHandles(HWND **hWindows, size_t *hWindows_countP,
                         bool filter) {
   struct EnumWindowProcData enumWindowProcData;
   enumWindowProcData.hWindows =
       (HWND *)calloc(sizeof(HWND) * GET_WINDOWS_HEAP_COUNT, sizeof(HWND));
   if (enumWindowProcData.hWindows == NULL) {
      return (errno_t)ERROR_INSUFFICIENT_BUFFER;
   } else {
      enumWindowProcData.hWindows_count = 0;
      enumWindowProcData.heap_count = GET_WINDOWS_HEAP_COUNT;
      enumWindowProcData.error = 0;
      enumWindowProcData.filter=filter;
      WINBOOL enunWindowError =
          EnumWindows(EnumWindowsProc, (LPARAM)&enumWindowProcData);
      *hWindows = enumWindowProcData.hWindows;
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

   // memory check
   if (count > heap_count) {
      heap_count += 1024;
      enumWindowProcDataP->heap_count = heap_count;
      hWindows = realloc(hWindows, heap_count);
      if (hWindows == NULL) {
         enumWindowProcDataP->error = ERROR_INSUFFICIENT_BUFFER;
         return FALSE;
      } else {
         enumWindowProcDataP->hWindows = hWindows;
      }
   }
   // memory check end

   // window check
   if (hWindow != NULL) {
      if (enumWindowProcDataP->filter == true) {
         const DWORD windowStyle = (DWORD)GetWindowLongW(hWindow, GWL_STYLE);
         const DWORD windowExStyle =
             (DWORD)GetWindowLongW(hWindow, GWL_EXSTYLE);
         // condition
         if (isWindowAppearingInTaskbar(hWindow, windowStyle, windowExStyle) ==
             true) {
            hWindows[count++] = hWindow;
            enumWindowProcDataP->hWindows_count = count;
            return TRUE;
         } else {
            return TRUE;  // not in Taskbar. continue
         }
      }else{
         hWindows[count++] = hWindow;
         enumWindowProcDataP->hWindows_count = count;
         return TRUE;
      }
   } else {
      return TRUE;  // continue
   }
};

errno_t getHWindowPID(HWND hWindow, DWORD *processID) {
   if (GetWindowThreadProcessId(hWindow, processID) == 0) {
      return (errno_t)GetLastError();
   } else {
      return EXIT_SUCCESS;
   }
}

bool isWindowAppearingInTaskbar(HWND hWindow, DWORD windowStyle,
                                DWORD windowExStyle) {
   if ((windowStyle & WS_VISIBLE) == 0) return false;
   if ((windowExStyle & WS_EX_TOOLWINDOW) != 0) return false;
#if (WINVER >= 0x0602)
   if ((windowExStyle & WS_EX_NOREDIRECTIONBITMAP) != 0) return false;
#endif
   HWND hOwnerWindow = GetWindow(hWindow, GW_OWNER);
   if (hOwnerWindow != NULL && (((windowExStyle & WS_EX_APPWINDOW) == 0) ||
                                (IsIconic(hOwnerWindow) == TRUE))) {
      return false;
   } else {
      return true;
   }
}
