#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int frequency = 1000;
    int duration = 500;
    
    if (argc >= 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0) {
            printf("beep.exe v0.0.1\n");
            printf("(C) 2023, krzysiu.net, MIT license\n\n") ;
            
            printf("Usage: beep [frequency] [duration]\n\n");
            
            printf("Generate a system beep sound using current sound card.\n\n");
            
            printf("Parameters:\n");
            printf("\tfrequency\tfrequency of the beep sound in Hz (default: 1000 - like profanity censor bleep in TV)\n");
            printf("\tduration\tduration of the beep sound in milliseconds (default: 500)\n\n");
            
            printf("Tips:\n");
            printf("* For \"infinite\" (maximum possible - 1 193 hours) duration, set duration parameter to -1\n");
            printf("* beep.exe plays sound synchronously (it lasts as long as the sound) to give an easy means to interrupt sound.\n");
            printf("* To interrupt sound, kill process or press CTRL+C\n\n");
            printf("Examples:\n");
            printf("beep - plays 1000 Hz beep for 500 ms\n");
            printf("beep 70 - plays 70 Hz beep for 500 ms\n");
            printf("beep 300 20000 - plays 300 Hz beep for 20 000 ms (2 seconds)\n");
            printf("beep 1000 -1 - plays 1 kHz beep forever\n");

            return 0;
        }
        frequency = atoi(argv[1]);
    }
    if (argc >= 3) {
        
        if (strcmp(argv[2], "-1") == 0) duration = 4294967294; else duration = atoi(argv[2]);
    }
    Beep(frequency, duration);
    
    return 0;
}





























