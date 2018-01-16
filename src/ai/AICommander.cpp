#include "AICommander.h"

namespace RhobanSSL
{
    AICommander::AICommander(bool yellow)
    : yellow(yellow)
    {
    }

    void AICommander::set(uint8_t robot_id, bool enabled,
        double xSpeed, double ySpeed, double thetaSpeed)
    {
        Command command;
        command.enabled = enabled;
        command.robot_id = robot_id;
        command.xSpeed = xSpeed;
        command.ySpeed = ySpeed;
        command.thetaSpeed = thetaSpeed;

        commands.push_back(command);
    }

    void AICommander::stopAll()
    {
        for (int k=0; k<6; k++) {
            set(k, false, 0, 0, 0);
        }
    }
}