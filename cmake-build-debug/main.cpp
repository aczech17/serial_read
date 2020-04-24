#include <iostream>
#include "Port.h"

using std::cin, std::cout, std::endl, std::cerr;

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

int main()
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
        catch(std::exception& ex)
        {
            cerr << ex.what() << endl;
            return 1;
        }

    return 0;
}
