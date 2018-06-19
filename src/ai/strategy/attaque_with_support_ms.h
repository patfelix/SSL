/*
This file is part of SSL.

Copyright 2018 Bezamat Jérémy (jeremy.bezamat@gmail.com)

SSL is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SSL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with SSL.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __STRATEGY__ATTAQUEWITHSUPPORTMS__H__
#define __STRATEGY__ATTAQUEWITHSUPPORTMS__H__

#include "Strategy.h"
#include <robot_behavior/striker.h>
// #include <robot_behavior/robot_follower.h>
#include <robot_behavior/search_shoot_area.h>
#include <robot_behavior/pass.h>
#include <robot_behavior/wait_pass.h>
#include <core/machine_state.h>

namespace RhobanSSL {
  namespace Strategy {

    class AttaqueWithSupportMs : public Strategy {
      //Machine state
      struct state_name {
        // name : robot1_robot2
        static const constexpr char* strike_search = "strike_search" ;
        static const constexpr char* search_strike = "search_strike" ;

        static const constexpr char* pass_search = "pass_search" ;
        static const constexpr char* search_pass = "search_pass" ;

        static const constexpr char* search_waitpass = "search_waitpass" ;
        static const constexpr char* waitpass_search = "waitpass_search" ;
      };

      struct edge_name {
        // db1 = distance_ball_robot_1
        // db2 = distance_ball_robot_2
        static const constexpr char* db1_sup_db2 = "db1_sup_db2" ;
        static const constexpr char* db1_inf_db2 = "db1_inf_db2" ;

        // fgbm = find_goal_best_move
        static const constexpr char* fgbm_score_inf_seuil_1 = "fgbm_score_inf_seuil_1" ; //if striker is robot 1
        static const constexpr char* fgbm_score_inf_seuil_2 = "fgbm_score_inf_seuil_2" ; //if striker is robot 2

        static const constexpr char* infra_1 = "infra_1" ;
        static const constexpr char* infra_2 = "infra_2" ;

        // db1 = distance_ball_robot_1
        // db2 = distance_ball_robot_2
        static const constexpr char* db1_inf_seuil_or_time_inf_tempo = "db1_inf_seuil_or_time_inf_tempo" ;
        static const constexpr char* db2_inf_seuil_or_time_inf_tempo = "db2_inf_seuil_or_time_inf_tempo" ;
      };

      typedef construct_machine_state_infrastructure<
      std::string, Ai::AiData, Ai::AiData
      > machine_state_infrastructure;

    private:
      machine_state_infrastructure::MachineState machine;

      bool behaviors_are_assigned;
      std::shared_ptr<Robot_behavior::Striker> striker_behavior;
      std::shared_ptr<Robot_behavior::SearchShootArea> search_behavior;
      std::shared_ptr<Robot_behavior::Pass> pass_behavior;
      std::shared_ptr<Robot_behavior::WaitPass> wait_pass_behavior;

      double seuil_fgbm; // fgbm = find_goal_best_move
      double tempo;

      double fgbm_score;
      
      double begin_time;
      Ai::Robot robot_1;
      Ai::Robot robot_2;



    public:
      AttaqueWithSupportMs(Ai::AiData & ai_data);
      virtual ~AttaqueWithSupportMs();

      virtual int min_robots() const;
      virtual int max_robots() const;
      virtual Goalie_need needs_goalie() const;

      static const std::string name;

      virtual void start(double time);
      virtual void stop(double time);

      virtual void update(double time);

      virtual void assign_behavior_to_robots(
        std::function<
        void (int, std::shared_ptr<Robot_behavior::RobotBehavior>)
        > assign_behavior,
        double time, double dt
      );

      virtual std::list<
      std::pair<rhoban_geometry::Point,ContinuousAngle>
      > get_starting_positions( int number_of_avalaible_robots ) ;
      virtual bool get_starting_position_for_goalie(
        rhoban_geometry::Point & linear_position,
        ContinuousAngle & angular_position
      ) ;

      bool is_db1_sup_db2();


    };

  };
};
#endif
