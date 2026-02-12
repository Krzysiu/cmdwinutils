#include <windows.h>

#define VERSION "0.2.0"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        char msg[2048];
        wsprintfA(msg, 
            "QUIET.EXE v%s\n-=-=-=-=-=-=-=-\n(C) 2026, krzysiu.net, MIT license\n\n"
            "Run batch script or any executable without showing console window.\n\n"
            "Usage:\n\tquiet.exe <command> [parameters]\n\n"
            "Example:\n\tquiet.exe c:\\script.bat -force -verbose\n\n"
            "Quirks and things to remember:\n"
            "1) EXIT CODE of the process WILL BE FORWARDED.\n"
            "2) Parameters ARE forwarded to the target.\n"
            "3) USER PRIVILEGES ARE INHERITED.\n"
            "4) If the target opens its own window, it will be visible.\n"
            "5) Use responsibly! No console output is visible.\n\n"
            "If you like this application please consider supporting my work:\n"
            "https://buymeacoffee.com/krzysiunet", 
            VERSION);

        MessageBoxA(NULL, msg, "quiet.exe - usage information", MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    LPSTR cmd = GetCommandLineA();
    BOOL q = FALSE;

    while (*cmd) {
        if (*cmd == '\"') q = !q;
        else if (*cmd == ' ' && !q) {
            cmd++;
            while (*cmd == ' ') cmd++;
            break;
        }
        cmd++;
    }

    if (!*cmd) cmd = argv[1];

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD exitCode = 0;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcess(NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return (int)exitCode;
    }

    return (int)GetLastError();
}