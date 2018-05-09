#include "prepare_kickoff.h"
#include <robot_behavior/do_nothing.h>
#include <robot_behavior/position_follower.h>

namespace RhobanSSL {
namespace Strategy {

Prepare_kickoff::Prepare_kickoff(Ai::AiData & ai_data):
    Strategy(ai_data)
{
}

const std::string Prepare_kickoff::name="prepare_kickoff";

int Prepare_kickoff::min_robots() const {
    return 0;
}
int Prepare_kickoff::max_robots() const {
    return -1;
}
void Prepare_kickoff::start(double time){
    DEBUG("START PREPARE KICKOFF");
    behavior_has_been_assigned = false;
}

void Prepare_kickoff::stop(double time){
    DEBUG("STOP PREPARE KICKOFF");
}

Robot_behavior::RobotBehavior* Prepare_kickoff::create_follower(
    const Vector2d & follower_position,
    const ContinuousAngle& angle,
    double time, double dt
) const {
    Robot_behavior::PositionFollower* follower = new Robot_behavior::PositionFollower(ai_data, time, dt);
    follower->set_following_position(
        follower_position, angle
    );
    follower->set_translation_pid(
        ai_data.constants.p_translation,
        ai_data.constants.i_translation, 
        ai_data.constants.d_translation
    );
    follower->set_orientation_pid(
        ai_data.constants.p_orientation, ai_data.constants.i_orientation, 
        ai_data.constants.d_orientation
    );
    follower->set_limits(
        ai_data.constants.translation_velocity_limit,
        ai_data.constants.rotation_velocity_limit,
        ai_data.constants.translation_acceleration_limit,
        ai_data.constants.rotation_acceleration_limit
    );
    return follower;
}

void Prepare_kickoff::assign_behavior_to_robots(
    std::function<
        void (int, std::shared_ptr<Robot_behavior::RobotBehavior>)
    > assign_behavior,
    double time, double dt
){
    if( ! behavior_has_been_assigned ){
        int goalie_id = get_goalie();
        assign_behavior(
            goalie_id, std::shared_ptr<Robot_behavior::RobotBehavior>(
                 Prepare_kickoff::create_follower(
                     Vector2d(-2.0, 0.0), ContinuousAngle(M_PI/2.0),
                     time, dt
                 )
            )
        );

        int nb_robots = get_player_ids().size();
        //double fieldWidth = ai_data.field.fieldwidth;
        double robot_radius = ai_data.constants.robot_radius;
        for( int i=0; i<nb_robots; i++ ){
            int id = player_id(i);
            float y_pos = ( i - (nb_robots/2) )*4.0*robot_radius;
            Vector2d follower_linear_position(-1.0, y_pos);
            ContinuousAngle follower_angular_position(M_PI/2.0);
            assign_behavior(
                id, std::shared_ptr<Robot_behavior::RobotBehavior>(
                    Prepare_kickoff::create_follower(
                        follower_linear_position, follower_angular_position,
                        time, dt
                    )
                )
            );
        }
        behavior_has_been_assigned = true;
    }
}

Prepare_kickoff::~Prepare_kickoff(){
}

void Prepare_kickoff::set_kicking( bool value ){
    is_kicking = value;
}


}
}