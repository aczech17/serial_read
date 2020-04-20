#include <iostream>
#include <windows.h>
#include <string>

using std::cin, std::cout, std::endl, std::cerr;

class Port
{
    HANDLE hComm;
    DCB serialConfig = {0};
    const char* portName;
    DWORD BaudRate;
public:
    struct Position
    {
        int x;
        int y;
    };
private:
    std::string readLine()
    {
        char tempChar;
        std::string buffer;
        DWORD countBytesRead = 0;
        do
        {
            ReadFile(hComm, &tempChar, sizeof(tempChar), &countBytesRead, nullptr);
            buffer += tempChar;
        }while(tempChar != '\n');
        return buffer;
    }

    Position getPosFromStr(std::string str)
    {
        Position result = {0, 0};
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
public:

    explicit Port(const char* portNameParam = "COM3", DWORD BaudRateParam = CBR_9600) : portName(portNameParam), BaudRate(BaudRateParam)
    {
        hComm = CreateFile(portName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hComm == INVALID_HANDLE_VALUE)
        {
            std::string errMsg = "Cannot open port ";
            errMsg += portName;
            throw(std::ios_base::failure(errMsg));
        }

        serialConfig.DCBlength = sizeof(serialConfig); // ???
        serialConfig.BaudRate = CBR_9600;
        serialConfig.ByteSize = 8;
        serialConfig.StopBits = ONESTOPBIT;
        serialConfig.Parity = NOPARITY;
    }

    ~Port()
    {
        CloseHandle(hComm);
    }

    Position getPosition()
    {
        std::string str = readLine();
        return getPosFromStr(str);
    }
};


struct JoystickPosition
{
    int x, y;
    JoystickPosition& operator=(Port::Position pos)
    {
        x = pos.x;
        y = pos.y;
        return *this;
    }
};

int main(int argc, char **argv)
{
    Port p1;
    JoystickPosition pos = {0, 0};
    for(int i = 0; i < 20; ++i)
    {
        pos = p1.getPosition();
        cout<<"x: " << pos.x << " y: " << pos.y << endl;
    }

    return 0;
}
