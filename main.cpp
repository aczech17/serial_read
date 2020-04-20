#include <iostream>
#include <windows.h>
#include <string>

using std::cin, std::cout, std::endl, std::cerr;

HANDLE hComm;
DCB serialConfig = {0};
struct joystickPosition
{
    int x, y;
};

void openPort(const char* port)
{
    hComm = CreateFile(port, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hComm == INVALID_HANDLE_VALUE)
    {
        cerr << ("Perkele :DDD") << endl;
        exit(1);
    }
    else
        cout << ("Otwarto port ") << port << endl;

    serialConfig.DCBlength = sizeof(serialConfig); // ???
    serialConfig.BaudRate = CBR_9600;
    serialConfig.ByteSize = 8;
    serialConfig.StopBits = ONESTOPBIT;
    serialConfig.Parity = NOPARITY;
}

std::string readLine(const char* port)
{
    char tempChar;
    std::string buffer = "";
    DWORD countBytesRead = 0;
    do
    {
        ReadFile(hComm, &tempChar, sizeof(tempChar), &countBytesRead, nullptr);
        buffer+=tempChar;
    }while(tempChar != '\n');
    return buffer;
}

joystickPosition getPosFromStr(std::string str)
{
    joystickPosition result = {0, 0};
    int i = 0;
    //ignore artifacts
    while(str[i] < '0' || str[i] > '9')
        i++;
    //first number
    int j = i;
    while(str[j] != ' ')
        j++;
    std::string str1 = str.substr(i, j - i);
    result.x = std::stoi(str1);

    //second number
    j++;

    std::string str2 = str.substr(j, str.size() - j - 1);// no copy endl (no no no, why you copy my movies?)
    result.y = std::stoi(str2);

    return result;
}

joystickPosition getOneRecord(const char* port)
{
    std::string str = readLine(port);
    return getPosFromStr(str);
}

int main(int argc, char **argv)
{
    char *port;
    if(argc > 1)
        port = argv[1];
    else
        port = (char*)"COM3";

    openPort(port);

    for(int i = 0; i < 20; ++i)
    {
        joystickPosition position = getOneRecord(port);
        cout<<"x: " << position.x << " ";
        cout<<"y: " << position.y << endl;
    }

    CloseHandle(hComm);
    return 0;
}
