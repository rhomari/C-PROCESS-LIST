#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tlhelp32.h>
BOOL ListProcesses();
int main(){
    ListProcesses();
    printf("Enter Process ID to kill: ");
    long pid;
    scanf("%ld",&pid);
    if (!TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid),0)){
        printf("Error: %d\n",GetLastError());
        return 1;
    }
    printf("Process killed\n");
    return 0;
}   
BOOL ListProcesses(){
     HANDLE processhandle;
     FILETIME creationtime,exittime,kerneltime,usertime;
     SYSTEMTIME st ={0};
     PROCESSENTRY32 pe;
     HANDLE hprocess=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
     if (hprocess==INVALID_HANDLE_VALUE){
          printf("Error: %d\n",GetLastError());
          return 1;
     }
     pe.dwSize = sizeof(PROCESSENTRY32);
     if (!Process32First(hprocess,&pe)){
          printf("Error: %d\n",GetLastError());
          return FALSE;
     }
    
    printf("%-40s%-15s%-10s\n","Process name","Process ID","Creation time");
     do{
        printf("%-40s%-15d",pe.szExeFile,pe.th32ProcessID);
        processhandle= OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe.th32ProcessID);
        if (processhandle!=INVALID_HANDLE_VALUE){
          if (GetProcessTimes(processhandle,&creationtime,&exittime,&kerneltime,&usertime)){
            FileTimeToSystemTime(&creationtime,&st);
            printf("%d-%d-%d at %d:%d:%d",st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
          
          }
        }     
        printf("%d\n"); 
        CloseHandle(processhandle);
    }while(Process32Next(hprocess,&pe));
   
    CloseHandle(hprocess);
    return TRUE;
}
