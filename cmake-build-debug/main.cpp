#include <iostream>
#include "Port.h"

using std::cin, std::cout, std::endl, std::cerr;

int main()
{
    const int portsCount = 25;
    for(int i = 0; i < portsCount; ++i)
    {
        std::string portName = "COM" + std::to_string(i);
        try
        {
            Port port(portName.data()); //const char* required in constructor
            Port::Position position;
            for(int j  = 0; j < 20000; j++)
            {
                position = port.getPosition();
                cout<<position.getX() << " " << position.getY() << " " << position.getPushed() << endl;
            }
        }
        catch(std::ios_base::failure& exc)
        {
            cerr << exc.what() << endl;
            continue;
        }
        break;
    }

    return 0;
}
