#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <cstring>
#include <tchar.h>

using namespace std;

#define BUF_SIZE 256
TCHAR* szName = new TCHAR[256];
TCHAR* Msg = new TCHAR[256];

int _tmain()
{
    using namespace std;

    string szNameDir;

    std::cout << "Enter shared memory name:   ";
    std::cin >> szNameDir;

    strcpy(szName, ("Global\\" + szNameDir).c_str());

    HANDLE hFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szName);
    LPCTSTR pBuf = (LPTSTR)MapViewOfFile(hFile,FILE_MAP_ALL_ACCESS,0,0,BUF_SIZE);


    if ((hFile == NULL) && (GetLastError() == 2))
    {
        hFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,szName);
    }
    if (hFile == NULL)
    {
        _tprintf(TEXT("Could not create file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }

    

    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(hFile);

        return 1;
    }

    string status;

    while (1)
    {
        std::cout << "\nPlease choose an action to do:\n\n"
            << "R>ead message from shared memory;\n"
            << "W>rite message to shared memory;\n"
            << "Q>uit.\n\n";
        std::cin >> status;

        if (status == "R")
        {
            std::cout << "The message says:  " << pBuf << std::endl;
        }
        else {
            if (status == "W")
            {
                memset((void*)pBuf, 0, BUF_SIZE);
                std::cout << "Enter message text:  ";
                std::cin >> Msg;
                CopyMemory((PVOID)pBuf, Msg, (_tcslen(Msg) * sizeof(TCHAR)));
            }
            else {
                if (status == "Q") {
                    exit(1);
                }
                else if ((status != "R") && (status != "W") && (status != "Q"))
                {
                    std::cout << "Please enter R, W or Q <in any case>\n\n";
                }
            }
        }
    }

    UnmapViewOfFile(pBuf);
    CloseHandle(hFile);

    return 0;
}