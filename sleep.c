#include <stdio.h> // printf
#include <stdlib.h> // atoi
#include <unistd.h> // sleep

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("sleep.exe v0.0.1\n(C) 2024, krzysiu.net, MIT license\n\n- sleeps/stops executing synchronous script for given time.\nUsage:\n\tsleep <TIME>\nwhere <TIME> is delay in milliseconds (1/1000th of a second)\n\nExample:sleep 2000 - sleep for 2 seconds");
        return 1;
    }

    int sleep_duration = atoi(argv[1]) * 1000;

    printf("Before sleep...\n");
    usleep(sleep_duration);
    printf("After sleep...\n");

    return 0;
}
