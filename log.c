#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void appendToFile(const char *filename, const char *message) {
    FILE *file = fopen(filename, "a"); 
    
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    
    time_t current_time;
    struct tm *local_time;
    char timeString[20];
    
    current_time = time(NULL);
    local_time = localtime(&current_time);
    
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", local_time);
    
    
    fprintf(file, "[%s] %s\n", timeString, message);
    
    
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("File logging companion\n");
        printf("(C) 2023, krzysiu.net, MIT license\n\n");
        
        printf("Usage: %s filename message\n\n", argv[0]);
        printf("Example:\n");
        printf("log %TEMP%\\foo.txt bar\n");
        printf("Creates file foo.txt in temp directory or appends it if exists\n");
        printf("with contents timestamp will be current):\n");
        printf("[2024-06-10 15:48:01] bar");

        
        
        return 1;
    }
    
    char *filename = argv[1];
    char *message = argv[2];
    
    appendToFile(filename, message);
    printf("OK!\n");
    
    return 0;
}
