#include <stdio.h>
#include <string.h>
#include <windows.h>

#define VERSION "1.0.0"

void waitForNoKeys(HANDLE hStdin) {
    FlushConsoleInputBuffer(hStdin);
    int keysDown;
    do {
        keysDown = 0;
        for (int vkey = 0x08; vkey <= 0xFF; vkey++) {
            if (GetAsyncKeyState(vkey) & 0x8000) {
                keysDown = 1;
                break;
            }
        }
        if (keysDown) Sleep(10);
    } while (keysDown);
    FlushConsoleInputBuffer(hStdin);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) {
            printf("pausex.exe v%s\n", VERSION);
            printf("(C) 2026, krzysiu.net, MIT license\n\n");
            printf("Usage: pausex\n\n");
            printf("Wait for any key press. Returns 130 on CTRL+C, 0 on any other key.\n");
            printf("Completely silent. No output unless --help is used.\n\n");
            printf("Tips:\n");
            printf("* Signals ^C to the caller (Exit Code 130).\n");
            printf("* Prevents accidental skips (Input Debouncing).\n");
            printf("* Ignores modifier keys (Shift, Ctrl, Win, etc.).\n\n");
            printf("Support:\n");
            printf("* If this tool saved your time, you can support me at: https://buymeacoffee.com/krzysiu\n\n");
            return 0;
        }
    }

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode, read;
    INPUT_RECORD ir;
    int returnCode = -1;

    if (hStdin == INVALID_HANDLE_VALUE) return 1;

    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_PROCESSED_INPUT);

    waitForNoKeys(hStdin);

    while (returnCode == -1) {
        if (!ReadConsoleInput(hStdin, &ir, 1, &read)) break;
        if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
            WORD vk = ir.Event.KeyEvent.wVirtualKeyCode;
            if (vk == VK_CONTROL || vk == VK_SHIFT || vk == VK_MENU || 
                vk == VK_CAPITAL || vk == VK_LWIN || vk == VK_RWIN) {
                continue;
            }
            if (ir.Event.KeyEvent.uChar.AsciiChar == 3) returnCode = 130;
            else returnCode = 0;
        }
    }

    SetConsoleMode(hStdin, mode);
    return returnCode;
}
