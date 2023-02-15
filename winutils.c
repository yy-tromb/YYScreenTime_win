#include "./include/winutils.h"

int getProcesses(DWORD *allProcesses, TCHAR **allProcessesNames, int allProcesses_count) {
   DWORD allProcesses_size;
   DWORD neededSize;
   int count=0;

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
