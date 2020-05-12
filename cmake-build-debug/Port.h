#ifndef SERIAL_READ_PORT_H
#define SERIAL_READ_PORT_H
#include <windows.h>
#include <string>
#include <ios> //ios::exception

class Port
{
    HANDLE hComm;
    DCB serialConfig = {0};
    COMMTIMEOUTS timeouts = {0};
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
    std::string readLine();
    Position getPosFromStr(std::string str);
public:
    explicit Port(const char* portNameParam = "COM3", DWORD BaudRate = CBR_9600);
    Position getPosition();
    ~Port();
};
#endif //SERIAL_READ_PORT_H
