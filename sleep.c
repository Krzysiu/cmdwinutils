#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define VERSION "0.2.1"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) {
            printf("sleep.exe v%s\n", VERSION);
            printf("(C) 2024, krzysiu.net, MIT license\n\n");
            
            printf("Usage: sleep [number][suffix]\n\n");
            
            printf("Suspend execution for an interval of time.\n\n");
            
            printf("Parameters:\n");
            printf("\tnumber\t\tthe amount of time to sleep\n");
            printf("\tsuffix\t\tunit of time: s (seconds, default), ms (milliseconds), m (minutes), h (hours)\n\n");
            
            printf("Tips:\n");
            printf("* If no suffix is provided, seconds are assumed (GNU compatibility).\n");
            printf("* Use 'ms' for high-precision loops (e.g., in batch monitoring scripts).\n");
            printf("* To interrupt sleep, kill process or press CTRL+C\n\n");
            
            printf("Examples:\n");
            printf("sleep 1\t\t- sleeps for 1 second\n");
            printf("sleep 50ms\t- sleeps for 50 milliseconds\n");
            printf("sleep 1.5s\t- sleeps for 1500 milliseconds\n\n");

            printf("Support:\n");
            printf("* If this tool saved your time, you can support me at: https://buymeacoffee.com/krzysiu\n\n");

            return 0;
        }
    }

    if (argc != 2) {
        fprintf(stderr, "sleep: missing operand\nTry 'sleep --help' for more information.\n");
        return 1;
    }

    char *arg = argv[1];
    char *endptr;
    double value = strtod(arg, &endptr);
    long long multiplier = 1000; 

    if (*endptr != '\0') {
        if (strcmp(endptr, "ms") == 0)      multiplier = 1;
        else if (strcmp(endptr, "s") == 0) multiplier = 1000;
        else if (strcmp(endptr, "m") == 0) multiplier = 60000;
        else if (strcmp(endptr, "h") == 0) multiplier = 3600000;
        else if (strcmp(endptr, "d") == 0) multiplier = 86400000;
        else {
            fprintf(stderr, "sleep: invalid time interval '%s'\n", endptr);
            return 1;
        }
    }

    if (value > 0) {
        Sleep((DWORD)(value * multiplier));
    }

    return 0;
}