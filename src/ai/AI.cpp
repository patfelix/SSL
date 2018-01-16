#include "AI.h"
#include <timing/TimeStamp.hpp>
#include <cmath>
#include <unistd.h>

using namespace Utils::Timing;

namespace RhobanSSL
{


    AI::AI(AIVisionClient *vision, AICommander *commander)
    : vision(vision), commander(commander)
    {
        running = true;
    }


    void AI::tick()
    {
        auto gameState = vision->getGameState();

        // Moving the robot 0 to the center of the field
        auto robot = gameState.robots[GameState::Ally][0];
        if (robot.isOk()) {

            if( control.is_static() ){
                Eigen::Vector2d robot_position( 
                    robot.position.getX(), robot.position.getY()
                );
                control.set_translation_pid( 0.01, .00, .0);
                control.set_orientation_pid( 0.1, .00, .0);
                //control.set_translation_pid( 0.01, .00, .0);
                //control.set_orientation_pid( 0.001, .00, .0);
                robot_translation.position = robot_position;
                robot_rotation.orientation = robot.orientation.getSignedValue();
                double translation_velocity = 1.0;
                double translation_acceleration = 20.0;

                double angular_velocity = 2*3.15;  
                double angular_acceleration = 60.0;

                double calculus_step = 0.0001;
                double time = TimeStamp::now().getTimeMS()/1000.0;
                control.set_movment(
                    robot_translation,
                    translation_velocity,
                    translation_acceleration,
                    robot_rotation,
                    angular_velocity,
                    angular_acceleration,
                    calculus_step, time 
                );



//                DEBUG(
//                    "r con time : " << 
//                    control.curve.angular_consign.time_of_deplacement() 
//                );
//                DEBUG(
//                    "r mov time : " << 
//                    control.curve.rotation_movment.max_time() 
//                );
//                DEBUG(
//                    "r mov length : " << 
//                    control.curve.rotation_movment.size() 
//                );
//                DEBUG(
//                    "t con time : " << 
//                    control.curve.tranlsation_consign.time_of_deplacement() 
//                );
//                DEBUG(
//                    "t mov time : " << 
//                    control.curve.translation_movment.max_time() 
//                );
//                DEBUG(
//                    "t mov length : " << 
//                    control.curve.translation_movment.size() 
//                );



                //control.curve.print_translation_curve(.01);
                //control.curve.print_translation_movment(.001);
                //control.curve.print_rotation_curve(.1);
                //control.curve.print_rotation_movment(.1);
            }

            Eigen::Vector2d robot_position( 
                robot.position.getX(), robot.position.getY()
            );
            double robot_orientation = robot.orientation.getSignedValue();
            double time = TimeStamp::now().getTimeMS()/1000.0;
            

            control.update( time );

#if 0
            Control ctrl = control.absolute_control_in_robot_frame(
                robot_position, robot_orientation
            );
#else
            Control ctrl = control.relative_control_in_robot_frame(
                robot_position, robot_orientation
            );
#endif

//            DEBUG("vel trans : " << ctrl.velocity_translation);
//              std::cout << ctrl.velocity_translation.transpose() << std::endl;
//            DEBUG("vel rot : " << ctrl.velocity_rotation);
//            DEBUG(
//                "r : " << 
//                control.curve.rotation_movment( time - control.start_time ) 
//            );
//            DEBUG(
//                "t : " << 
//                control.curve.translation_movment( time - control.start_time ) 
//            );
             
            
            commander->set(
                0, true, ctrl.velocity_translation[0], ctrl.velocity_translation[1], ctrl.velocity_rotation
            );
            commander->flush();
        }
    }

    void AI::run()
    {
        double period = 1/100.0;    // 100 hz
        auto lastTick = TimeStamp::now();

        while (running) {
            auto now = TimeStamp::now();
            double elapsed = diffSec(lastTick, now);
            double toSleep = period - elapsed;
            if (toSleep > 0) {
                usleep(round(toSleep*1000000));
            }
            lastTick = TimeStamp::now();
            tick();
        }
    }

    void AI::stop()
    {
        running = false;
    }
}
