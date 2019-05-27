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

#include "goalie_strat.h"

namespace rhoban_ssl
{
namespace strategy
{
GoalieStrat::GoalieStrat(ai::AiData& ai_data)
  : Strategy(ai_data), degageur_(std::shared_ptr<robot_behavior::Degageur>(new robot_behavior::Degageur(ai_data)))
{
}

GoalieStrat::~GoalieStrat()
{
}

/*
 * We define the minimal number of robot in the field.
 * The goalkeeper is not counted.
 */
int GoalieStrat::minRobots() const
{
  return 0;
}

/*
 * We define the maximal number of robot in the field.
 * The goalkeeper is not counted.
 */
int GoalieStrat::maxRobots() const
{
  return 0;
}

GoalieNeed GoalieStrat::needsGoalie() const
{
  return GoalieNeed::YES;
}

const std::string GoalieStrat::name = "goalie_strat";

void GoalieStrat::start(double time)
{
  DEBUG("START PREPARE KICKOFF");
  goalie_ = std::shared_ptr<robot_behavior::Goalie>(new robot_behavior::Goalie(ai_data_));
  behaviors_are_assigned_ = false;
}
void GoalieStrat::stop(double time)
{
  DEBUG("STOP PREPARE KICKOFF");
}

void GoalieStrat::update(double time)
{
}

void GoalieStrat::assignBehaviorToRobots(
    std::function<void(int, std::shared_ptr<robot_behavior::RobotBehavior>)> assign_behavior, double time, double dt)
{
  // we assign now all the other behavior

  int goalieID = getGoalie();  // we get the first if in get_player_ids()

  if (allyPenaltyArea().is_inside(ballPosition()))
  {
    assign_behavior(goalieID, degageur_);
  }
  else
  {
    assign_behavior(goalieID, goalie_);
  }

  behaviors_are_assigned_ = true;
}

// We declare here the starting positions that are used to :
//   - place the robot during STOP referee state
//   - to compute the robot order of get_player_ids(),
//     we minimize the distance between
//     the startings points and all the robot position, just
//     before the start() or during the STOP referee state.
std::list<std::pair<rhoban_geometry::Point, ContinuousAngle> >
GoalieStrat::getStartingPositions(int number_of_avalaible_robots)
{
  assert(minRobots() <= number_of_avalaible_robots);
  assert(maxRobots() == -1 or number_of_avalaible_robots <= maxRobots());

  return { std::pair<rhoban_geometry::Point, ContinuousAngle>(allyGoalCenter(), 0.0) };
}

//
// This function return false if you don't want to
// give a staring position. So the manager will chose
// a default position for you.
//
bool GoalieStrat::getStartingPositionForGoalie(rhoban_geometry::Point& linear_position,
                                               ContinuousAngle& angular_position)
{
  linear_position = allyGoalCenter();
  angular_position = ContinuousAngle(0.0);
  return true;
}

rhoban_ssl::annotations::Annotations GoalieStrat::getAnnotations() const
{
  rhoban_ssl::annotations::Annotations annotations;

  for (auto it = this->getPlayerIds().begin(); it != this->getPlayerIds().end(); it++)
  {
    const rhoban_geometry::Point& robot_position = getRobot(*it).getMovement().linearPosition(time());
    // annotations.addText("Behaviour: " + this->name, robot_position.getX() + 0.15, robot_position.getY(), "white");
    annotations.addText("Strategy: " + this->name, robot_position.getX() + 0.15, robot_position.getY() + 0.30, "white");
  }
  return annotations;
}

}  // namespace strategy
}  // namespace rhoban_ssl