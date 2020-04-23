#include <iostream>
#include <windows.h>
#include <string>

using std::cin, std::cout, std::endl, std::cerr;

class Port
{
public:
    HANDLE hComm;
    DCB serialConfig = {0};
    COMMTIMEOUTS timeouts = {0};
    const char* portName;
    DWORD BaudRate;
public:
    class Position
    {
        int x;
        int y;
        bool pushed;
        friend class Port;
    public:
        Position() : x(0), y(0), pushed(false) {}
        int getX() {return x;}
        int getY() {return y;}
        bool getPushed() {return pushed;}
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
        Position result;
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
        i = j;
        while(str[j] != ' ')
            j++;
        std::string str2 = str.substr(i, j - i);
        result.y = std::stoi(str2);

        //pushed
        j++;
        result.pushed = str[j] - '0';

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

        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;
        SetCommTimeouts(hComm, &timeouts);
    }

    Position getPosition()
    {
        std::string str = readLine();
        return getPosFromStr(str);
    }

    ~Port()
    {
        CloseHandle(hComm);
    }
};


struct JoystickPosition
{
    int x, y;
    bool pushed;
    JoystickPosition& operator=(Port::Position pos)
    {
        x = pos.getX();
        y = pos.getY();
        pushed = pos.getPushed();
        return *this;
    }
};




int main(int argc, char **argv)
{
        try
        {
            Port port("COM3");
            JoystickPosition position = {0, 0, false};
            size_t msgCount = 250;
            for(size_t i = 1; i <= msgCount; i++)
            {
                position = port.getPosition();
                cout << position.x << " " << position.y << " " << position.pushed << endl;
            }
        }
        catch(std::ios_base::failure& ex)
        {
            cerr << ex.what() << endl;
            return 1;
        }

    return 0;
}
