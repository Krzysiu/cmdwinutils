#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define VERSION "0.1.0"

// Flag needed to enable ANSI colors if missing in older headers
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void print_help() {
    printf("phppause.exe v%s\n", VERSION);
    printf("(C) 2026, krzysiu.net, MIT license\n\n");
    
    printf("Usage: phppause [script_name.php] [arguments]\n\n");
    
    printf("Transparent wrapper for PHP interpreter with ANSI color support and window title auto-naming.\n\n");
    
    printf("Parameters:\n");
    printf("\tscript_name.php\tThe path to the PHP script you want to execute\n");
    printf("\targuments\tAdditional arguments passed directly to PHP\n\n");
    
    printf("Tips:\n");
    printf("* The window title will automatically change to the name of the script being run.\n");
    printf("* ANSI escape codes are enabled by default for beautiful terminal output.\n");
    printf("* Ensure 'php.exe' is in your system PATH or same directory.\n\n");
    
    printf("Support:\n");
    printf("* If this tool saved your time, you can support me at: https://buymeacoffee.com/krzysiu\n\n");
}

int main() {
    // --- ACTIVATE ANSI COLORS ---
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
    // ----------------------------

    int argc;
    LPWSTR *argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
    
    // Display help if no arguments provided or help flag detected
    if (argc <= 1 || (argc > 1 && (wcscmp(argvW[1], L"-h") == 0 || wcscmp(argvW[1], L"--help") == 0 || wcscmp(argvW[1], L"/?") == 0))) {
        print_help();
        if (argvW) LocalFree(argvW);
        return 0;
    }

    char *fullCmdLine = GetCommandLineA();
    char *phpPath = "php.exe";
    char finalCmd[32768];
    char windowTitle[512] = "PHP Wrapper";
    DWORD exitCode = 1;

    // Window title logic: extract script name from arguments
    if (argvW != NULL) {
        for (int i = 1; i < argc; i++) {
            if (argvW[i][0] != L'-') {
                char scriptName[256];
                wcstombs(scriptName, argvW[i], 256);
                char *lastSlash = strrchr(scriptName, '\\');
                if (!lastSlash) lastSlash = strrchr(scriptName, '/');
                sprintf(windowTitle, "%s - PHP", lastSlash ? lastSlash + 1 : scriptName);
                break;
            }
        }
        LocalFree(argvW);
    }
    SetConsoleTitleA(windowTitle);

    // Prepare command line for CreateProcess
    char *args = strchr(fullCmdLine, ' ');
    if (args == NULL) args = "";
    sprintf(finalCmd, "%s %s", phpPath, args);

    // Print the command line in red (using ANSI)
    printf("\033[31mCommand line: %s%s\033[0m\n\n", phpPath, args);

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Execute PHP
    if (CreateProcessA(NULL, finalCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exitCode);
        printf("\n-------------------------------\nPHP exited with code: %lu\n", exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        exitCode = GetLastError();
        printf("\033[31mCreateProcess Error: %lu\033[0m\n", exitCode);
        printf("Make sure 'php.exe' is accessible.\n");
    }

    // Update title and wait for user before closing
    strcat(windowTitle, " (Finished)");
    SetConsoleTitleA(windowTitle);
    printf("\nPress any key to exit...");
    _getch();

    return (int)exitCode;
}