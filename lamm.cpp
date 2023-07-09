#include <iostream>
#include <Windows.h>

#define MAX_VAL 2048
#define MAX_NAME 256

void envList()
{
    HKEY hKey;
    BYTE value[MAX_VAL];
    DWORD valueSize = MAX_VAL;
    TCHAR name[MAX_NAME];
    DWORD nameSize = MAX_NAME;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        for (DWORD i = 0; ; i++)
        {
            value[0] = '\0';
            name[0] = '\0';
            valueSize = MAX_VAL;
            nameSize = MAX_NAME;

            if (RegEnumValue(hKey, i, name, &nameSize, NULL, NULL, value, &valueSize) == ERROR_SUCCESS)
            {
                std::cout << (i < 9 ? "0" : "") << i + 1 << ". " << name << " = " << value << std::endl;
            }
            else
            {
                break;
            }
        }

        RegCloseKey(hKey);
    }
    else
    {
        std::cout << "Failed to open registry key." << std::endl;
    }
}

int main()
{
    std::cout << "Environment Variables List:" << std::endl;
    envList();
    return 0;
}
