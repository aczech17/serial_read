#include "Port.h"
    std::string Port::readLine()
    {
        char tempChar;
        std::string buffer;
        DWORD countBytesRead = 0;
        do
        {
            ReadFile(hComm,            //file handle
                    &tempChar,         //char buffer
                    sizeof(tempChar),  //number of bytes to read
                    &countBytesRead,   //number of bytes currently read
                    nullptr            //lpOverlapped https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
                    );
            buffer += tempChar;
        }while(tempChar != '\n');
        return buffer;
    }

    Port::Position Port::getPosFromStr(std::string str)
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
        std::string tmpStr = str.substr(i, j - i);
        result.x = std::stoi(tmpStr);

        //second number
        j++;
        i = j;
        while(str[j] != ' ')
            j++;
        tmpStr = str.substr(i, j - i);
        result.y = std::stoi(tmpStr);

        //pushed
        j++;
        result.pushed = (bool)(str[j] - '0');

        return result;
    }

     Port::Port(const char* portName, DWORD BaudRate)
    {
        hComm = CreateFile(
                portName,      //port name
                GENERIC_READ,  //read only
                0,             //share mode (0 ==> cannot be shared)
                nullptr,       //security attributes (nullptr ==> handle cannot be inherited) http://winapi.freetechsecrets.com/win32/WIN32CreateFile.htm
                OPEN_EXISTING, //creation distribution (open only if existing)
                0,             //flags and attributes (link ^)
                nullptr        //handle to files to copy (none)
                );
        if (hComm == INVALID_HANDLE_VALUE)
        {
            std::string errMsg = "Cannot open port ";
            errMsg += portName;
            throw(std::ios_base::failure(errMsg));
        }

        serialConfig.DCBlength = sizeof(serialConfig); // ???
        serialConfig.BaudRate = BaudRate;
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

    Port::Position Port::getPosition()
    {
        std::string str = readLine();
        return getPosFromStr(str);
    }

    Port::~Port()
    {
        CloseHandle(hComm);
    }