#pragma once
#ifndef PLAN_FUNCTIONS_H
#define PLAN_FUNCTIONS_H

#include <set>
#include <vector>

#include <boost\variant.hpp>

#include "plan_state.h"

namespace htn
{
template<class T>
class PlanOperator
{
	typedef PlanState(T::*EvaluatorFunction)(PlanState);
	typedef int(T::*HeuristicFunction)(PlanState);
	typedef PlanState(T::*ActionFunction)(PlanState);

public:
	PlanOperator() {}
	PlanOperator(int id, std::string name, EvaluatorFunction evaluator, HeuristicFunction heuristic)
		: id_(id), name_(name), evaluator_(evaluator), heuristic_(heuristic) {}
	PlanOperator(int id, std::string name, EvaluatorFunction evaluator, HeuristicFunction heuristic, ActionFunction action)
		: id_(id), name_(name), evaluator_(evaluator), heuristic_(heuristic), action_(action) {}
	PlanOperator(int id, std::string name, EvaluatorFunction evaluator, HeuristicFunction heuristic, std::set<std::string> states)
		: id_(id), name_(name), evaluator_(evaluator), heuristic_(heuristic), statekeys_(states) {}
	PlanOperator(int id, std::string name, EvaluatorFunction evaluator, HeuristicFunction heuristic, ActionFunction action, std::set<std::string> states)
		: id_(id), name_(name), evaluator_(evaluator), heuristic_(heuristic), action_(action), statekeys_(states) {}
	
	template<class T2>
	PlanOperator(const PlanOperator<T2>& plan_operator)
	{
		id_ = plan_operator.id();
		name_ = plan_operator.name();
		statekeys_ = plan_operator.statekeys();
		evaluator_ = plan_operator.evaluator();
		heuristic_ = plan_operator.heuristic();
		action_ = plan_operator.action();
	}
	template<class T2>
	PlanOperator<T>& operator=(const PlanOperator<T2>& po)
	{ 
		id_ = plan_operator.id();
		name_ = plan_operator.name();
		statekeys_ = plan_operator.statekeys();
		evaluator_ = plan_operator.evaluator();
		heuristic_ = plan_operator.heuristic();
		action_ = plan_operator.action();
		return *this; 
	}

	inline PlanState RunEvaluator(T& t, PlanState current_state) const { return (t.*(evaluator_))(current_state); }
	inline int RunHeuristic(T& t, const PlanState& current_state) const { return (t.*(heuristic_))(current_state); }
	inline int id() const { return id_; }
	inline std::string name() const { return name_; }
	inline std::set<std::string> statekeys() const { return statekeys_; }
	inline EvaluatorFunction evaluator() const { return evaluator_; }
	inline HeuristicFunction heuristic() const { return heuristic_; }
	inline ActionFunction action() const { return action_; }

private:
	int id_;
	std::string name_;
	std::set<std::string> statekeys_;
	EvaluatorFunction evaluator_;
	HeuristicFunction heuristic_;
	ActionFunction action_;
};

class PlanMethod
{
public:
	PlanMethod() {}
	PlanMethod(int id, std::string name, std::string type)
		: id_(id), name_(name), type_(type) {}

	inline int id() const { return id_; }
	inline std::string name() const { return name_; }
	inline std::string type() const { return type_; }
	inline std::vector<int> child_ids() const { return child_ids_; }
	inline std::vector<std::string> children() const { return children_; }
	inline std::set<std::string> statekeys() const { return statekeys_; }

	inline void add_child_id(int child_id) { child_ids_.push_back(child_id); }
	inline void set_child_id(std::vector<int> child_ids) { child_ids_ = child_ids; }
	inline void add_child_ids(std::vector<int> child_ids)
	{
		child_ids_.insert(child_ids_.end(), child_ids.begin(), child_ids.end());
	}
	inline void add_child(std::string child) { children_.push_back(child); }
	inline void set_children(std::vector<std::string> children) { children_ = children; }
	inline void add_children(std::vector<std::string> children)
	{
		children_.insert(children_.end(), children.begin(), children.end());
	}

	inline void add_states(std::set<std::string> states)
	{
		statekeys_.insert(states.begin(), states.end());
	}

private:
	int id_;
	std::string type_;
	std::string name_;
	std::vector<int> child_ids_;
	std::vector<std::string> children_;
	std::set<std::string> statekeys_;
};

class PlanStep
{
public:
	PlanStep() {}
	PlanStep(int step_id, int parent_id, std::string name, std::string type) :
		step_id_(step_id), parent_id_(parent_id), name_(name), type_(type) {}

	inline int step_id() const { return step_id_; }
	inline int parent_id() const { return parent_id_; }
	inline int cost() const { return cost_; }
	inline std::string status() const { return status_; }
	inline std::string name() const { return name_; }
	inline std::string type() const { return type_; }

	inline void set_step_id(const int step_id) { step_id_ = step_id; }
	inline void set_parent_id(const int parent_id) { parent_id_ = parent_id; }
	inline void set_cost(const int cost) { cost_ = cost; }
	inline void add_cost(const int cost) { cost_ += cost; }
	inline void set_status(const std::string status) { status_ = status; }
	inline void set_name(const std::string name) { name_ = name; }
	inline void set_type(const std::string type) { type_ = type; }
	inline void reset() { cost_ = 0; status_ = "open"; }

protected:
	int step_id_ = -1, parent_id_ = -1;
	int cost_ = 0;
	std::string status_ = "open";
	std::string name_;
	std::string type_;
};

template<class T>
class PlanStepOperator : public PlanStep
{
public:
	PlanStepOperator() {}	
	PlanStepOperator(int step_id, int parent_id, std::string name, std::shared_ptr<PlanOperator<T>> plan_operator)
		: PlanStep(step_id, parent_id, name, "operator"), plan_operator_(plan_operator) {}

	PlanStepOperator(const PlanStepOperator& plan_step_operator) 
	{
		set_step_id(plan_step_operator.step_id());
		set_parent_id(plan_step_operator.parent_id());
		plan_operator_ = plan_step_operator.plan_operator_;
		set_name(plan_step_operator.name());
		set_type("operator");
	}
	template<class T2>
	PlanStepOperator(const PlanStepOperator<T2>& plan_step_operator)
	{
		set_step_id(plan_step_operator.step_id());
		set_parent_id(plan_step_operator.parent_id());
		PlanOperator<T> plan_op = static_cast<PlanOperator<T>>(*plan_step_operator.plan_operator());
		plan_operator_ = std::make_shared<PlanOperator<T>>(plan_op);
		set_name(plan_step_operator.name());
		set_type("operator");
	}

	inline std::shared_ptr<PlanOperator<T>> plan_operator() const { return plan_operator_; }
	bool operator<(const PlanStepOperator<T>& right) const { return name() < right.name(); }

private:
	std::shared_ptr<PlanOperator<T>> plan_operator_;
};

class PlanStepMethod : public PlanStep
{
public:
	PlanStepMethod() {}
	PlanStepMethod(int step_id, int parent_id, std::string name, std::string type)
		: PlanStep(step_id, parent_id, name, type) {}

	inline int child_id(int index) const { return child_ids_[index]; }
	inline int child_ids_size() const { return child_ids_.size(); }
	
	inline void add_child_id(int child) { child_ids_.push_back(child); }

private:
	std::vector<int> child_ids_;
};

template<class T>
class Plan
{
public:
	Plan() {}
	Plan(const Plan<T>& plan)
	{
		SetBasicVariables(plan);
		operators_ = plan.operators();
	}
	template<class T2>
	Plan(const Plan<T2>& plan) 
	{
		name_ = plan.name();
		plan_id_ = plan.plan_id();
		flat_index_ = plan.flat_index();
		cost_ = plan.cost();
		operator_id_ = plan.operator_id();
		method_id_ = plan.method_id();
		methods_ = plan.methods();
		children_ = plan.children();
		flat_ = plan.all_flat();
		status_ = plan.status();
		type_ = plan.type();
		state_ = plan.state();
		statekeys_ = plan.statekeys();
		auto operators = plan.operators();
		for (auto oit = operators.begin(); oit != operators.end(); ++oit)
			operators_[oit->first] = static_cast<PlanStepOperator<T>>(oit->second);
	}

	void SetBasicVariables(const Plan<T>& plan)
	{
		name_ = plan.name();
		plan_id_ = plan.plan_id();
		flat_index_ = plan.flat_index();
		cost_ = plan.cost();
		operator_id_ = plan.operator_id();
		method_id_ = plan.method_id();
		methods_ = plan.methods();
		children_ = plan.children();
		flat_ = plan.all_flat();
		status_ = plan.status();
		type_ = plan.type();
		state_ = plan.state();
		statekeys_ = plan.statekeys();
	}

	inline int plan_id() const { return plan_id_; }
	inline int operators_size() const { return operators_.size(); }
	inline int methods_size() const { return methods_.size(); }
	inline int flat(int index) const { return flat_[index]; }
	inline int flat_index() const { return flat_index_; }
	inline int flat_size() const { return flat_.size(); }
	inline int cost() const { return cost_; }
	inline int operator_id() const { return operator_id_; }
	inline int method_id() const { return method_id_; }
	inline std::map<int, PlanStepOperator<T>> operators() const { return operators_; }
	inline std::map<int, PlanStepMethod> methods() const { return methods_; }
	inline std::string name() const { return name_; }
	inline std::string type() const { return type_; }
	inline std::string status() const { return status_; }
	inline std::set<std::string> statekeys() const { return statekeys_; }
	inline std::vector<int> child_ids() const { return child_ids_; }
	inline std::vector<int> all_flat() const { return flat_; }
	inline std::vector<std::string> children() const { return children_; }
	inline PlanStepOperator<T> get_operator(int step_id) const { return operators_.at(step_id); }
	inline PlanStepMethod get_method(int step_id) const { return methods_.at(step_id); }
	inline std::unique_ptr<PlanStepMethod> method(int index) { return std::make_unique<PlanStepMethod>(methods_[index]); }
	inline PlanState state() const { return state_; }

	inline void set_name(std::string name) { name_ = name; }
	inline void set_plan_id(int plan_id) { plan_id_ = plan_id; }
	inline void set_cost(int cost) { cost_ = cost; }
	inline void set_operator_id(int operator_index) { operator_id_ = operator_index; }
	inline void set_method_id(int method_index) { method_id_ = method_index; }
	inline void add_operator(int step_id, PlanStepOperator<T> plan_step_operator) { operators_[step_id] = plan_step_operator; }
	inline void add_flat(int step_id) { flat_.push_back(step_id); }
	inline void add_flat(std::vector<int> flat) { flat_.insert(flat_.end(), flat.begin(), flat.end()); }
	inline void add_child_id(int step_id) { child_ids_.push_back(step_id); }
	inline void add_children(std::string step_name) { children_.push_back(step_name); }
	inline void add_operators(std::map<int, PlanStepOperator<T>> plan_step_operators)
	{
		operators_.insert(plan_step_operators.begin(), plan_step_operators.end());
	}
	inline void add_methods(std::map<int, PlanStepMethod> plan_step_methods)
	{
		methods_.insert(plan_step_methods.begin(), plan_step_methods.end());
	}
	inline void set_method(int step_id, PlanStepMethod plan_step_method) { methods_[step_id] = plan_step_method; }
	inline void set_type(std::string type) { type_ = type; }
	inline void set_state(PlanState state) { state_ = state; }
	inline void set_status(std::string status) { status_ = status; }
	inline void inc_flat_index() { flat_index_++; }
	inline void add_cost(int cost) { cost_ += cost; }
	inline void add_statekeys(std::set<std::string> statekeys) { statekeys_.insert(statekeys.begin(), statekeys.end()); }
	void reset()
	{
		flat_index_ = 0;
		cost_ = 0;
		state_ = PlanState();
		status_ = "open";

		for (int i = 0; i < methods_.size(); i++)
			methods_[i].reset();

		for (int i = 0; i < operators_.size(); i++)
			operators_[i].reset();
	}

private:
	int plan_id_ = -1;
	int flat_index_ = 0;
	int operator_id_ = 0;
	int method_id_ = 1;
	int cost_ = 0;
	std::map<int, PlanStepOperator<T>> operators_;
	std::map<int, PlanStepMethod> methods_;
	std::string name_;
	std::string status_ = "open";
	std::string type_;
	std::set<std::string> statekeys_;
	std::vector<int> flat_;
	std::vector<int> child_ids_;
	std::vector<std::string> children_;
	PlanState state_;
};

template<class T>
class PlanTask
{
	typedef PlanState(T::*StateCollector)();
	typedef PlanState(T::*GoalSetter)();
	typedef PlanState(T::*EvaluatorFunction)(PlanState);
	typedef int(T::*HeuristicFunction)(PlanState);

public:
	PlanTask() {}
	PlanTask(std::string name, std::vector<Plan<T>> task_plans, StateCollector state_collector, GoalSetter goal_setter, EvaluatorFunction evaluator, HeuristicFunction heuristic)
		: task_name_(name), task_plans_(task_plans), state_collector_(state_collector), goal_setter_(goal_setter), evaluator_(evaluator), heuristic_(heuristic)
	{}
	PlanTask(const PlanTask<T>& plan_task)
	{
		task_name_ = plan_task.task_name_;
		task_plans_ = plan_task.task_plans_;
		state_collector_ = plan_task.state_collector_;
		goal_setter_ = plan_task.goal_setter_;
		evaluator_ = plan_task.evaluator_;
		heuristic_ = plan_task.heuristic_;
	}
	template<class T2>
	PlanTask(const PlanTask<T2>& plan_task)
	{
		task_name_ = plan_task.task_name();
		auto task_plans = plan_task.task_plans();
		std::vector<Plan<T2>> p2_vec = task_plans;
		std::vector<Plan<T>> p_vec;
		for (int i = 0; i < p2_vec.size(); i++)
		{
			Plan<T> p = static_cast<Plan<T>>(p2_vec[i]);
			p_vec.push_back(p);
		}
		task_plans_ = p_vec;
		state_collector_ = plan_task.state_collector();
		goal_setter_ = plan_task.goal_setter();
		evaluator_ = plan_task.evaluator();
		heuristic_ = plan_task.heuristic();
	}

	inline std::string task_name() const { return task_name_; }
	inline std::vector<Plan<T>> task_plans() const { return task_plans_; }
	inline StateCollector state_collector() const { return state_collector_; }
	inline GoalSetter goal_setter() const { return goal_setter_; }
	inline EvaluatorFunction evaluator() const { return evaluator_; }
	inline HeuristicFunction heuristic() const { return heuristic_; }

	inline PlanState CollectState(T& t) { return (t.*(state_collector_))(); }
	inline PlanState SetGoal(T& t) { return (t.*(goal_setter_))(); }
	inline PlanState RunEvaluator(T& t, PlanState current_state) { return (t.*(evaluator_))(current_state); }
	inline int RunHeuristic(T& t, PlanState current_state) { return (t.*(heuristic_))(current_state); }

private:
	std::string task_name_;
	std::vector<Plan<T>> task_plans_;
	StateCollector state_collector_;
	GoalSetter goal_setter_;
	EvaluatorFunction evaluator_;
	HeuristicFunction heuristic_;
};

template<class T>
class PlanStepTask : public PlanStep
{
public:
	PlanStepTask(PlanTask<T> plan_task) : PlanStep(-1, -1, plan_task.task_name(), "task"), plan_task_(plan_task) {}

	inline PlanTask<T> plan_task() { return plan_task_; }

private:
	PlanTask<T> plan_task_;
};
}

#endif