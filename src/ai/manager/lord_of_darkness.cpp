/*
    This file is part of SSL.

    Copyright 2019 SCHMITZ Etienne (hello@etienne-schmitz.com)

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

// Strategies
#include "lord_of_darkness.h"
#include <strategy/halt.h>
#include <strategy/keeper/keeper_strat.h>
#include <strategy/wall_2.h>

namespace rhoban_ssl
{
namespace manager
{
LordOfDarkness::LordOfDarkness(std::string name)
  : ManagerWithGameState(name)
  , dumb_strats_(1 + ai::Config::NB_OF_ROBOTS_BY_TEAM)
  , halt_strats_(1 + ai::Config::NB_OF_ROBOTS_BY_TEAM)
{
  // strategies arrays begin at 1(case 0 unused) to directly acces the good strategy by giving number of disponible
  // dumb_strats
  dumb_strats_[8] = { strategy::KeeperStrat::name, strategy::Wall_2::name };
  dumb_strats_[7] = { strategy::KeeperStrat::name, strategy::Wall_2::name };
  dumb_strats_[6] = { strategy::KeeperStrat::name, strategy::Wall_2::name };
  dumb_strats_[5] = { strategy::KeeperStrat::name, strategy::Wall_2::name };
  dumb_strats_[4] = { strategy::KeeperStrat::name, strategy::Wall_2::name };
  dumb_strats_[3] = { strategy::KeeperStrat::name, strategy::Wall_2::name };
  dumb_strats_[2] = { strategy::KeeperStrat::name, strategy::Wall_2::name };
  dumb_strats_[1] = { strategy::KeeperStrat::name };

  // halt_strats
  halt_strats_[8] = { strategy::Halt::name };
  halt_strats_[7] = { strategy::Halt::name };
  halt_strats_[6] = { strategy::Halt::name };
  halt_strats_[5] = { strategy::Halt::name };
  halt_strats_[4] = { strategy::Halt::name };
  halt_strats_[3] = { strategy::Halt::name };
  halt_strats_[2] = { strategy::Halt::name };
  halt_strats_[1] = { strategy::Halt::name };

  // Register strategy.
  registerStrategy(strategy::Halt::name, std::shared_ptr<strategy::Strategy>(new strategy::Halt()));
  registerStrategy(strategy::KeeperStrat::name, std::shared_ptr<strategy::Strategy>(new strategy::KeeperStrat()));
  registerStrategy(strategy::Wall_2::name, std::shared_ptr<strategy::Strategy>(new strategy::Wall_2()));
}

void LordOfDarkness::startStop()
{
  DEBUG("STARTSTOP");
  // setBallAvoidanceForAllRobots(true);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}

void LordOfDarkness::startRunning()
{
  DEBUG("START RUNNING");
  // setBallAvoidanceForAllRobots(false);
  declareAndAssignNextStrategies(future_strats_);
}
void LordOfDarkness::startHalt()
{
  DEBUG("START halt");
  // setBallAvoidanceForAllRobots(true);
  future_strats_ = halt_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}

void LordOfDarkness::startDirectKickAlly()
{
  // setBallAvoidanceForAllRobots(false);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}
void LordOfDarkness::startDirectKickOpponent()
{
  // setBallAvoidanceForAllRobots(true);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}

void LordOfDarkness::startIndirectKickAlly()
{
  // setBallAvoidanceForAllRobots(false);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}
void LordOfDarkness::startIndirectKickOpponent()
{
  // setBallAvoidanceForAllRobots(true);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}

void LordOfDarkness::startPrepareKickoffAlly()
{
  // setBallAvoidanceForAllRobots(true);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}
void LordOfDarkness::startPrepareKickoffOpponent()
{
  // setBallAvoidanceForAllRobots(true);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}

void LordOfDarkness::startKickoffAlly()
{
  // setBallAvoidanceForAllRobots(false);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}
void LordOfDarkness::startKickoffOpponent()
{
  // setBallAvoidanceForAllRobots(true);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}

void LordOfDarkness::startPenaltyAlly()
{
  // setBallAvoidanceForAllRobots(false);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}
void LordOfDarkness::startPenaltyOpponent()
{
  // setBallAvoidanceForAllRobots(true);
  future_strats_ = dumb_strats_[Manager::getValidPlayerIds().size() + 1];
  declareAndAssignNextStrategies(future_strats_);
}

// Continue

void LordOfDarkness::continueStop()
{
}

void LordOfDarkness::continueRunning()
{
  //  if (ballPosition().getX() <= 0 and not(ball_was_in_ally_part_))
  //  {
  //    clearStrategyAssignement();
  //    future_strats_ = defensive_strats_[Manager::getValidPlayerIds().size() + 1];
  //    ball_was_in_ally_part_ = true;
  //   declareAndAssignNextStrategies(future_strats_);
  //  }
  //  else if (ballPosition().getX() > 0 and ball_was_in_ally_part_)
  //  {
  //   clearStrategyAssignement();
  //    future_strats_ = offensive_strats_[Manager::getValidPlayerIds().size() + 1];
  //    ball_was_in_ally_part_ = false;
  //    declareAndAssignNextStrategies(future_strats_);
  //  }
}
void LordOfDarkness::continueHalt()
{
}

void LordOfDarkness::continueDirectKickAlly()
{
}
void LordOfDarkness::continueDirectKickOpponent()
{
}

void LordOfDarkness::continueIndirectKickAlly()
{
}
void LordOfDarkness::continueIndirectKickOpponent()
{
}

void LordOfDarkness::continuePrepareKickoffAlly()
{
}
void LordOfDarkness::continuePrepareKickoffOpponent()
{
}

void LordOfDarkness::continueKickoffAlly()
{
}
void LordOfDarkness::continueKickoffOpponent()
{
}

void LordOfDarkness::continuePenaltyAlly()
{
}
void LordOfDarkness::continuePenaltyOpponent()
{
}

LordOfDarkness::~LordOfDarkness()
{
}

};  // namespace manager
};  // namespace rhoban_ssl
