#include <windows.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        MessageBox(NULL, "QUIET.EXE\n-=-=-=-=-\n© 2024, krzysiu.net, MIT license\n\nRun batch script without showing console.\n\nUsage:\n\tquiet.exe <path>\nwhere <path> is a path to batch file.\n\nExample:\tquiet.exe c:\\autoexec.bat && calc\nResult:\tExecute c:\\autoexec.bat and if no error code will be returned, run calculator.\n\nQuirks and things to remember:\n1) EXIT CODE of batch script WILL BE FORWARDED.\n2) This tool DOESN'T FORWARD PARAMETERS\n3) USER PRIVLEDGES ARE INHERITED BY SCRIPT. So, if you run quiet.exe as admin, script will be ran as admin as well.\n4) If batch script opens separate console window, it will show any ways, but you got tool to avoid that, don't you?\n5) Use responsibly! You won't be getting console output (duh) of what's happening and only way to interrupt script is to kill the task.\n\nIf you like this application please consider[[[ëeE?ß¦uoAáaö donating to Quark's Bar and Holding Company. COME TO QUARK'S, QUARK'S IS FUN, COME RIGHT NOW, DON'T WALK...?«a«ÍAç¦íúÝL?ä§ Excuse me, digital hiccup.", "quiet.exe - usage information", MB_OK | MB_ICONINFORMATION);
        return 1;
    }
    
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    if (!CreateProcess(NULL, argv[1], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) return GetLastError();
    
    
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return 0;
}
