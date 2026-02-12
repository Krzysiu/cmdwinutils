#include <windows.h>
#include <stdio.h>
#include <time.h>

void log_to_file(const char* cmd, const char* output, DWORD exitCode) {
    char tempPath[MAX_PATH];
    char logFilePath[MAX_PATH];
    
    if (GetTempPathA(MAX_PATH, tempPath) == 0) return;
    snprintf(logFilePath, sizeof(logFilePath), "%squietps.log.txt", tempPath);

    FILE *f = fopen(logFilePath, "a"); 
    if (f) {
        time_t now = time(NULL);
        char *date = ctime(&now);
        if (date) date[strlen(date) - 1] = '\0';

        fprintf(f, "--- [%s] ---\n", date ? date : "unknown date");
        fprintf(f, "CMD: %s\n", cmd);
        fprintf(f, "OUT: %s\n", output[0] == '\0' ? "OK" : output);
        fprintf(f, "EXIT: %lu\n", exitCode);
        fprintf(f, "--------------------------------------------------\n\n");
        fclose(f);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        MessageBox(NULL, 
            "QUIETPS.EXE\n-=-=-=-=-=-\n(C) 2026, krzysiu.net, MIT license\n\n"
            "Runs PowerShell commands invisibly and logs output to %TEMP%\\quietps.log.txt\n\n"
            "Usage:\n\tquietps.exe \"<powershell_command>\"\n\n"
            "Example:\n\tquietps.exe \"Get-Process | Out-File C:\\procs.txt\"\n\n"
            "Quirks and things to remember:\n"
            "1) EXIT CODE of script WILL BE FORWARDED.\n"
            "2) ExecutionPolicy is automatically set to Bypass.\n"
            "3) Full output (stdout/stderr) is captured in the log file:\nquietps.log.txt\n"
            "4) USER PRIVILEGES ARE INHERITED BY SCRIPT.\n"
            "5) Use responsibly! Invisible processes can be hard to track.\n\n"
            "If you like this application please consider supporting my work:\n"
            "https://buymeacoffee.com/krzysiunet", 
            "quietps.exe - usage information", MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    // Zmienne przeniesione tutaj, zeby nie obciazac stosu przy wyswietlaniu helpa
    char fullCommand[32768];
    char psPath[MAX_PATH];
    char winDir[MAX_PATH];

    if (GetWindowsDirectoryA(winDir, MAX_PATH) == 0) return 1;
    
    snprintf(psPath, sizeof(psPath), "%s\\System32\\WindowsPowerShell\\v1.0\\powershell.exe", winDir);
    snprintf(fullCommand, sizeof(fullCommand), "%s -NoProfile -ExecutionPolicy Bypass -Command \"%s\"", psPath, argv[1]);

    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };

    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) return 1;
    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = hWrite; 
    si.hStdOutput = hWrite; 
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    ZeroMemory(&pi, sizeof(pi));

    char buffer[4096] = {0};
    DWORD exitCode = 0;

    if (CreateProcess(NULL, fullCommand, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hWrite); 

        DWORD bytesRead;
        ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
        
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exitCode);
        
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        
        log_to_file(fullCommand, buffer, exitCode);
    } else {
        CloseHandle(hWrite);
        log_to_file(fullCommand, "ERROR: CreateProcess failed", GetLastError());
    }

    CloseHandle(hRead);
    return (int)exitCode;
}