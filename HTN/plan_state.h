#pragma once
#ifndef PLAN_STATE_H
#define PLAN_STATE_H

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <boost\variant.hpp>

namespace htn
{
class StateObject
{
public:
	virtual ~StateObject() {}
	virtual int Distance(const std::shared_ptr<StateObject>&) const = 0;
};

typedef boost::variant<int, float, double, char, bool, std::string, std::shared_ptr<StateObject>> StateVariant;

class StateVisitor : public boost::static_visitor<int>
{
public:
	virtual inline int operator()(const std::shared_ptr<StateObject>&, const std::shared_ptr<StateObject>&) const = 0;
	template<class T> int operator()(const T& state_var, const T& compare_var) const
	{
		return 1 - (state_var == compare_var);
	}
	template<class T, typename U> int operator()(const T& state_var, const U& compare_var) const
	{
		return 1;
	}
};

class BaseVariable
{
public:
	BaseVariable(StateVariant state) : state_(state) {}
	virtual ~BaseVariable() {}

	virtual int Distance(const StateVariant&) const = 0;
	inline StateVariant state() const { return state_; }
	inline void set_state(const StateVariant& state_var) { state_ = state_var; }

private:
	StateVariant state_;
};

template<class T>
class StateVariable : public BaseVariable
{
public:
	StateVariable() {}
	StateVariable(const StateVariable& state_var) : BaseVariable(state_var.state()) {}
	StateVariable(const StateVariant& state_var) : BaseVariable(state_var) {}

	inline int Distance(const StateVariant& compare_var) const { return boost::apply_visitor(T(), state(), compare_var); }
};

class PlanState
{
public:
	PlanState() {}
	int DistanceFromGoal(const PlanState& goal_state) const
	{
		int total_distance = 0;
		auto goal_states = goal_state.plan_states();
		for (auto it = goal_states.begin(); it != goal_states.end(); ++it)
		{
			boost::apply_visitor(PrintVisitor(), it->second->state());
			auto plan_state = plan_states_.find(it->first);
			if (plan_state != plan_states_.end())
				total_distance += (it->second)->Distance(plan_state->second->state());
			else
				total_distance++;
		}
		return total_distance;
	}
	inline std::map<std::string, std::shared_ptr<BaseVariable>> plan_states() const { return plan_states_; }
	inline std::shared_ptr<BaseVariable> plan_state(std::string state_name) const { return plan_states_.at(state_name); }

	inline void insert(std::string name, std::shared_ptr<BaseVariable> variable) { plan_states_[name] = variable; }
	inline void update(std::string name, StateVariant state_var) { plan_states_[name]->set_state(state_var); }

private:
	std::map<std::string, std::shared_ptr<BaseVariable>> plan_states_;
};
}

#endif