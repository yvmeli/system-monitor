#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <conio.h>  // For _kbhit()

void show_memory() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);

    if (GlobalMemoryStatusEx(&memInfo)) {
        DWORDLONG total = memInfo.ullTotalPhys;
        DWORDLONG freeMem = memInfo.ullAvailPhys;
        DWORDLONG used = total - freeMem;

        printf("== Memory ==\n");
        printf("Used: %.2f MB\n", used / (1024.0 * 1024));
        printf("Free: %.2f MB\n", freeMem / (1024.0 * 1024));
        printf("Total: %.2f MB\n\n", total / (1024.0 * 1024));
    } else {
        printf("Error retrieving memory information.\n\n");
    }
}

void show_processes() {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    printf("== Active processes ==\n");

    if (Process32First(hSnap, &pe)) {
        do {
            printf("PID: %5u | Name: %s\n", pe.th32ProcessID, pe.szExeFile);
        } while (Process32Next(hSnap, &pe));
    } else {
        printf("Failed to list processes.\n");
    }

    CloseHandle(hSnap);
}

int main() {
    printf("System monitor (press any key to exit)...\n\n");

    while (!_kbhit()) {  // Run until a key is pressed
        system("cls");   // Clear screen (Windows only)
        show_memory();
        show_processes();
        Sleep(1000);     // Wait 1 second
    }

    printf("\nExiting...\n");
    return 0;
}