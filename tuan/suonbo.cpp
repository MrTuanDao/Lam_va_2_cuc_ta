#include <Windows.h>
#include <iostream>

int main() {
    SYSTEMTIME systemTime;
    FILETIME fileTime;

    // Thiết lập giờ, phút và giây trong SYSTEMTIME
    systemTime.wYear = 2023;
    systemTime.wMonth = 7;
    systemTime.wDay = 7;
    systemTime.wHour = 8;
    systemTime.wMinute = 47;
    systemTime.wSecond = 53;
    systemTime.wMilliseconds = 0;

    // Chuyển đổi SYSTEMTIME sang FILETIME
    SystemTimeToFileTime(&systemTime, &fileTime);

    // Sử dụng fileTime để thực hiện các phép toán thời gian khác

    return 0;
}