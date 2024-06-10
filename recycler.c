#include <windows.h>
#include <shellapi.h>  
#include <stdio.h>

int moveToRecycleBin(const wchar_t* filePath) {
    SHFILEOPSTRUCTW fileOp;
    wchar_t pathBuffer[MAX_PATH];

    wcsncpy(pathBuffer, filePath, MAX_PATH - 1);
    pathBuffer[MAX_PATH - 1] = L'\0';
    pathBuffer[wcslen(pathBuffer) + 1] = L'\0'; // Double null termination

    ZeroMemory(&fileOp, sizeof(fileOp));
    fileOp.wFunc = FO_DELETE;
    fileOp.pFrom = pathBuffer;
    fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_ALLOWUNDO;

    int result = SHFileOperationW(&fileOp);
    if (result != 0) {
        printf("Failed to delete file. Error code: %d\n", result);
        return 1;
    } else {
    // optional message on success
    }

    return 0;
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc != 2) {
        printf("recycler.exe v0.0.1\n(C) 2024, krzysiu.net, MIT license\nMoves file/directory to recycle bin in Windows\nUsage: recycler <file_path>\n", argv[0]);
        return 1;
    }

    return moveToRecycleBin(argv[1]);
}
