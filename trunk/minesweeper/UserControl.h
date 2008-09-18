#ifndef USERCONTROL_H_
#define USERCONTROL_H_

#include "Gameboard.h"

class UserControl {
    public:
        UserControl();
    private:
        char input [1024], * command;
        void sendCommand(char command [1024]);
};
#endif
