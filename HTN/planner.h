#pragma once
#ifndef PLANNER_H
#define PLANNER_H

#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "plan_functions.h"
#include "plan_state.h"

namespace htn
{
template<class T>
struct PlanWeightSorter
{
	bool PlanWeightSorter::operator()(const Plan<T>& a, const Plan<T>& b) const
	{
		return a.cost() > b.cost();
	}
};

template<class T>
struct PlanTaskWeightSorter
{
	bool PlanTaskWeightSorter::operator()(const PlanStepTask<T>& a, const PlanStepTask<T>& b) const
	{
		return a.cost() > b.cost();
	}
};

template<class T>
class Planner
{
	typedef PlanState(T::*EvaluatorFunction)(PlanState);
	typedef int(T::*HeuristicFunction)(PlanState);
	typedef PlanState(T::*ActionFunction)(PlanState);
	typedef PlanState(T::*StateCollector)();
	typedef PlanState(T::*GoalSetter)();
	typedef std::priority_queue<Plan<T>, std::vector<Plan<T>>, PlanWeightSorter<T>> PlanFrontier;
	typedef std::priority_queue<PlanStepTask<T>, std::vector<PlanStepTask<T>>, PlanTaskWeightSorter<T>> TaskFrontier;
	typedef std::pair<Plan<T>, std::set<PlanStepOperator<T>>> PlanStar;

public:
	Planner() {}
	Planner(const Planner<T>& planner)
	{
		function_index_ = planner.function_index_;
		operators_ = planner.operators_;
		methods_ = planner.methods_;
		plan_tasks_ = planner.plan_tasks_;
		stars_ = planner.stars_;
		statekey_operators_ = planner.statekey_operators_;
		statekey_methods_ = planner.statekey_methods_;
		statekey_tasks_ = planner.statekey_tasks_;
		operator_id_ = planner.operator_id_;
		method_id_ = planner.method_id_;
	}

	template<class T2>
	Planner(const Planner<T2>& planner)
	{
		function_index_ = planner.function_index();
		auto operators = planner.operators();
		for (auto oper_iter = operators.begin(); oper_iter != operators.end(); ++oper_iter)
		{
			PlanOperator<T> plan_oper_p = static_cast<PlanOperator<T>>(*oper_iter->second.get());
			std::shared_ptr<PlanOperator<T>> operator_ptr(&plan_oper_p);
			operators_[oper_iter->first] = operator_ptr;
		}
		methods_ = planner.methods();
		auto plan_tasks = planner.plan_tasks();
		for (auto task_iter = plan_tasks.begin(); task_iter != plan_tasks.end(); ++task_iter)
		{
			PlanTask<T> new_task = static_cast<PlanTask<T>>(task_iter->second);
			plan_tasks_[task_iter->first] = new_task;
		}
		statekey_operators_ = planner.statekey_operators();
		statekey_methods_ = planner.statekey_methods();
		statekey_tasks_ = planner.statekey_tasks();
		operator_id_ = planner.operator_id();
		method_id_ = planner.method_id();
	}
	std::map<int, std::shared_ptr<PlanOperator<T>>>& operator=(const std::map<int, std::shared_ptr<PlanOperator<T>>> p) { return *this; }
	template<class T2> std::map<int, std::shared_ptr<PlanOperator<T>>>& operator=(const std::map<int, std::shared_ptr<PlanOperator<T2>>> p2) { return *this; }

	std::map<std::string, int> function_index() const { return function_index_; }
	std::map<int, std::shared_ptr<PlanOperator<T>>> operators() const { return operators_; }
	std::map<int, PlanOperator<T>> regular_operators() const { return regular_operators_; }
	std::map<int, std::shared_ptr<PlanMethod>> methods() const { return methods_; }
	std::map<std::string, PlanTask<T>> plan_tasks() const { return plan_tasks_; }
	std::map<std::string, std::map<std::string, PlanStar>> stars() const { return stars_; }
	std::map<std::string, std::set<std::string>> statekey_operators() const { return statekey_operators_; }
	std::map<std::string, std::set<std::string>> statekey_methods() const { return statekey_methods_; }
	std::map<std::string, std::set<std::string>> statekey_tasks() const { return statekey_tasks_; }
	int operator_id() const { return operator_id_; }
	int method_id() const { return method_id_; }

	void DeclareOperator(std::string operator_name, EvaluatorFunction evaluator, HeuristicFunction heuristic);
	void DeclareOperator(std::string operator_name, EvaluatorFunction evaluator, HeuristicFunction heuristic, ActionFunction action);
	void DeclareOperator(std::string operator_name, EvaluatorFunction evaluator, HeuristicFunction heuristic, std::set<std::string> states);
	void DeclareOperator(std::string operator_name, EvaluatorFunction evaluator, HeuristicFunction heuristic, ActionFunction action, std::set<std::string> states);
	void DeclareOperator(std::string operator_name, PlanOperator<T> plan_operator);
	void DeclareMethod(std::string method_name, std::string method_type, std::vector<std::string> functions);
	void DeclareTask(std::string task_name, std::vector<std::string> task_steps, StateCollector state_collector, GoalSetter goal_setter, EvaluatorFunction evaluator_function, HeuristicFunction heuristic_function);

	void DeclareStars(std::string graph_name, std::vector<std::string> star_steps, bool recursive);
	void DeclareStar(std::string graph_name, std::string operator_name, std::vector<std::string> star_steps, bool recursive);
	void DeclareStarTree(std::string graph_name, std::map<std::string, std::vector<std::string>> star_tree, bool recursive);

	inline void DeclareStateCollector(std::string state_name, StateCollector state_collector) { state_collectors_[state_name] = state_collector; }
	void DeclareStateCollector(std::vector<std::string> state_names, StateCollector state_collector);

	PlanOperator<T> GetOperator(std::string operator_name);
	PlanMethod GetMethod(std::string method_name);

	std::string GetBestTask(T& t);

	void EvalTaskStep(T& t, Plan<T>& plan, const PlanState& goal_state);
	void EvalStarStep(T& t, Plan<T>& plan, const PlanState& goal_state);
	int GetBestTaskPlan(PlanFrontier& plans, const PlanState& goal_state);
	Plan<T> GetBestStarPlan(PlanFrontier& plans, const PlanState& goal_state);
	Plan<T> GetPlan(T& t, std::string plan_name, std::string plan_type, std::string greedy_type, PlanState current_state, const PlanState& goal_state);
	Plan<T> GetTaskPlan(T& t, std::string task_name, std::string greedy_type, PlanState current_state, const PlanState& goal_state);
	Plan<T> GetStarPlan(T& t, std::string graph_name, std::string star_name, std::string greedy_type, int max_steps, PlanState current_state, const PlanState& goal_state, bool repeat);

	Plan<T> MakePlan(int op_index, int meth_index, int parent_id, std::shared_ptr<PlanOperator<T>> plan_operator);
	Plan<T> MakePlan(int op_index, int meth_index, int parent_id, std::shared_ptr<PlanMethod> plan_method);

private:
	std::map<std::string, int> function_index_;
	std::map<int, std::shared_ptr<PlanOperator<T>>> operators_;
	std::map<int, std::shared_ptr<PlanMethod>> methods_;
	std::map<std::string, PlanTask<T>> plan_tasks_;
	std::map<std::string, std::map<std::string, PlanStar>> stars_;
	std::map<std::string, std::set<std::string>> statekey_operators_;
	std::map<std::string, std::set<std::string>> statekey_methods_;
	std::map<std::string, std::set<std::string>> statekey_tasks_;
	std::map<std::string, StateCollector> state_collectors_;

	int operator_id_ = 0;
	int method_id_ = 1;
}; //End of Planner class

//Begin Planner definitions
template<class T>
void Planner<T>::DeclareOperator(std::string operator_name, EvaluatorFunction evaluator, HeuristicFunction heuristic)
{
	function_index_.insert(std::pair<std::string, int>(operator_name, operator_id_));
	operators_.insert(std::pair<int, std::shared_ptr<PlanOperator<T>>>(operator_id_, std::make_shared<PlanOperator<T>>(PlanOperator<T>(operator_id_, operator_name, evaluator, heuristic))));
	operator_id_ += 2;
}

template<class T>
void Planner<T>::DeclareOperator(std::string operator_name, EvaluatorFunction evaluator, HeuristicFunction heuristic, ActionFunction action)
{
	function_index_.insert(std::pair<std::string, int>(operator_name, operator_id_));
	operators_.insert(std::pair<int, std::shared_ptr<PlanOperator<T>>>(operator_id_, std::make_shared<PlanOperator<T>>(PlanOperator<T>(operator_id_, operator_name, evaluator, heuristic, action))));
	operator_id_ += 2;
}

template<class T>
void Planner<T>::DeclareOperator(std::string operator_name, EvaluatorFunction evaluator, HeuristicFunction heuristic, std::set<std::string> states)
{
	function_index_.insert(std::pair<std::string, int>(operator_name, operator_id_));
	operators_.insert(std::pair<int, std::shared_ptr<PlanOperator<T>>>(operator_id_, std::make_shared<PlanOperator<T>>(PlanOperator<T>(operator_id_, operator_name, evaluator, heuristic, states))));
	operator_id_ += 2;
	for (auto it = states.begin(); it != states.end(); ++it)
		statekey_operators_[*it].insert(operator_name);
}

template<class T>
void Planner<T>::DeclareOperator(std::string operator_name, EvaluatorFunction evaluator, HeuristicFunction heuristic, ActionFunction action, std::set<std::string> states)
{
	function_index_.insert(std::pair<std::string, int>(operator_name, operator_id_));
	operators_.insert(std::pair<int, std::shared_ptr<PlanOperator<T>>>(operator_id_, std::make_shared<PlanOperator<T>>(PlanOperator<T>(operator_id_, operator_name, evaluator, heuristic, action, states))));
	operator_id_ += 2;
	for (auto it = states.begin(); it != states.end(); ++it)
		statekey_operators_[*it].insert(operator_name);
}

template<class T>
void Planner<T>::DeclareOperator(std::string operator_name, PlanOperator<T> plan_operator)
{
	function_index_.insert(std::pair<std::string, int>(operator_name, operator_id_));
	operators_.insert(std::pair<int, std::shared_ptr<PlanOperator<T>>>(operator_id_, std::make_shared<PlanOperator<T>>(plan_operator)));
	operator_id_ += 2;
	std::set<std::string> state_keys = plan_operator.statekeys();
	for (auto it = state_keys.begin(); it != state_keys.end(); ++it)
		statekey_operators_[*it].insert(operator_name);
}

template<class T>
void Planner<T>::DeclareMethod(std::string method_name, std::string method_type, std::vector<std::string> functions)
{
	PlanMethod new_method = PlanMethod(method_id_, method_name, method_type);
	for (int i = 0; i < functions.size(); i++)
	{
		auto function_iter = function_index_.find(functions[i]);
		if (function_iter != function_index_.end())
		{
			if (function_iter->second % 2 == 1)
			{
				auto method_iter = methods_.find(function_iter->second);
				if (method_iter != methods_.end())
				{
					if (method_iter->second->type() == method_type)
					{
						new_method.add_child_ids(method_iter->second->child_ids());
						new_method.add_children(method_iter->second->children());
					}
					else
					{
						new_method.add_child_id(function_iter->second);
						new_method.add_child(method_iter->second->name());
					}
					new_method.add_states(method_iter->second->statekeys());
				}
			}
			else
			{
				auto operator_iter = operators_.find(function_iter->second);
				if (operator_iter != operators_.end())
				{
					new_method.add_child_id(function_iter->second);
					new_method.add_child(operator_iter->second->name());
					new_method.add_states(operator_iter->second->statekeys());
				}
			}
		}
	}

	function_index_.insert(std::pair<std::string, int>(method_name, method_id_));
	methods_.insert(std::pair<int, std::shared_ptr<PlanMethod>>(method_id_, std::make_shared<PlanMethod>(new_method)));
	method_id_ += 2;
	std::set<std::string> states = new_method.statekeys();
	for (auto it = states.begin(); it != states.end(); ++it)
		statekey_methods_[*it].insert(method_name);
}

template<class T>
void Planner<T>::DeclareTask(std::string task_name, std::vector<std::string> task_steps, StateCollector state_collector, GoalSetter goal_setter, EvaluatorFunction evaluator_function, HeuristicFunction heuristic_function)
{
	std::vector<Plan<T>> task_plans;
	for (int i = 0; i < task_steps.size(); i++)
	{
		auto function_iter = function_index_.find(task_steps[i]);
		if (function_iter != function_index_.end())
		{
			if (function_iter->second % 2 == 0)
			{
				auto operator_iter = operators_.find(function_iter->second);
				if (operator_iter != operators_.end())
					task_plans.push_back(MakePlan(0, 1, -1, operator_iter->second));
				std::set<std::string> statekeys = operator_iter->second->statekeys();
				for (auto it = statekeys.begin(); it != statekeys.end(); ++it)
					statekey_tasks_[*it].insert(task_name);
			}
			else
			{
				auto method_iter = methods_.find(function_iter->second);
				if (method_iter != methods_.end()) 
					task_plans.push_back(MakePlan(0, 1, -1, method_iter->second));
				std::set<std::string> statekeys = method_iter->second->statekeys();
				for (auto it = statekeys.begin(); it != statekeys.end(); ++it)
					statekey_tasks_[*it].insert(task_name);
			}
		}
	}
	PlanTask<T> new_task(task_name, task_plans, state_collector, goal_setter, evaluator_function, heuristic_function);
	plan_tasks_[task_name] = new_task;
}

template<class T>
void Planner<T>::DeclareStars(std::string graph_name, std::vector<std::string> star_steps, bool recursive)
{
	for (int i = 0; i < star_steps.size(); i++)
	{
		std::string star_name = star_steps[i];
		auto function_iter = function_index_.find(star_name);
		if (function_iter != function_index_.end())
		{
			if (function_iter->second % 2 == 0)
			{
				auto operator_iter = operators_.find(function_iter->second);
				if (operator_iter != operators_.end())
				{
					std::set<PlanStepOperator<T>> plan_steps;
					for (int j = 0; j < star_steps.size(); j++)
					{
						if (recursive || i != j)
						{
							std::string next_name = star_steps[j];
							auto next_iter = function_index_.find(next_name);
							if (next_iter != function_index_.end())
							{
								auto next_op_iter = operators_.find(next_iter->second);
								if (next_op_iter != operators_.end())
								{
									PlanStepOperator<T> new_step = PlanStepOperator<T>(-1, -1, next_name, next_op_iter->second);
									plan_steps.insert(new_step);
								}
							}
						}
					}

					auto star_iter = stars_[graph_name].find(star_name);
					if (star_iter != stars_[graph_name].end())
						star_iter->second.second.insert(plan_steps.begin(), plan_steps.end());
					else
					{
						Plan<T> star_plan = MakePlan(0, 1, -1, operator_iter->second);
						stars_[graph_name].insert(std::pair<std::string, PlanStar>(star_name, PlanStar(star_plan, plan_steps)));
					}
				}
			}
		}
	}
}

template<class T>
void Planner<T>::DeclareStar(std::string graph_name, std::string operator_name, std::vector<std::string> star_steps, bool recursive)
{
	auto function_iter = function_index_.find(operator_name);
	if (function_iter != function_index_.end())
	{
		auto operator_iter = operators_.find(function_iter->second);
		if (operator_iter != operators_.end())
		{
			std::set<PlanStepOperator<T>> plan_steps;
			for (int i = 0; i < star_steps.size(); i++)
			{
				std::string star_name = star_steps[i];
				auto next_iter = function_index_.find(star_name);
				if (next_iter != function_index_.end())
				{
					auto next_op_iter = operators_.find(next_iter->second);
					if (next_op_iter != operators_.end())
					{
						PlanStepOperator<T> new_step_operator = PlanStepOperator<T>(-1, -1, star_name, next_op_iter->second);
						plan_steps.insert(new_step_operator);

						if (recursive)
						{
							auto star_iter = stars_[graph_name].find(star_name);
							PlanStepOperator<T> next_step_operator = PlanStepOperator<T>(-1, -1, operator_name, operator_iter->second);
							if (star_iter != stars_[graph_name].end())
								star_iter->second.second.insert(next_step_operator);
							else
							{
								Plan<T> star_plan = MakePlan(0, 1, -1, next_op_iter->second);
								stars_[graph_name].insert(std::pair<std::string, PlanStar>(star_name, PlanStar(star_plan, { next_step_operator })));
							}
						}
					}
				}
			}

			auto star_iter = stars_[graph_name].find(operator_name);
			if (star_iter != stars_[graph_name].end())
				star_iter->second.second.insert(plan_steps.begin(), plan_steps.end());
			else
			{
				Plan<T> star_plan = MakePlan(0, 1, -1, operator_iter->second);
				stars_[graph_name].insert(std::pair<std::string, PlanStar>(operator_name, PlanStar(star_plan, plan_steps)));
			}
		}
	}
}

template<class T>
void Planner<T>::DeclareStarTree(std::string graph_name, std::map<std::string, std::vector<std::string>> star_tree, bool recursive)
{
	for (auto star_iter = star_tree.begin(); star_iter != star_tree.end(); ++star_iter)
	{
		std::string star_name = star_iter->first;
		auto function_iter = function_index_.find(star_name);
		if (function_iter != function_index_.end())
		{
			auto operator_iter = operators_.find(function_iter->second);
			if (operator_iter != operators_.end())
			{
				std::set<PlanStepOperator<T>> plan_steps;
				for (int i = 0; i < star_iter->second.size(); i++)
				{
					std::string next_name = star_iter->second[i];
					auto next_iter = function_index_.find(next_name);
					if (next_iter != function_index_.end())
					{
						auto next_op_iter = operators_.find(next_iter->second);
						if (next_op_iter != operators_.end())
						{
							PlanStepOperator<T> new_step_operator = PlanStepOperator<T>(-1, -1, next_name, next_op_iter->second);
							plan_steps.insert(new_step_operator);
						}
					}
					if (recursive)
					{
						PlanStepOperator<T> new_step_operator = PlanStepOperator<T>(-1, -1, star_name, operator_iter->second);
						plan_steps.insert(new_step_operator);
					}
				}

				auto star_iter = stars_[graph_name].find(star_name);
				if (star_iter != stars_[graph_name].end())
				{
					std::unique_ptr<PlanStar> plan_star = std::make_unique<PlanStar>(star_iter->second);
					plan_star->second.insert(plan_steps.begin(), plan_steps.end());
				}
				else
				{
					Plan<T> star_plan = MakePlan(0, 1, -1, operator_iter->second);
					stars_[graph_name].insert(std::pair<std::string, PlanStar>(star_name, PlanStar(star_plan, plan_steps)));
				}
			}
		}
	}
}

template<class T>
void Planner<T>::DeclareStateCollector(std::vector<std::string> state_names, StateCollector state_collector)
{
	for (int i = 0; i < state_names.size(); i++)
	{
		state_collectors_[state_names[i]] = state_collector;
	}
}

template<class T>
PlanOperator<T> Planner<T>::GetOperator(std::string operator_name)
{
	auto function_iter = function_index_.find(operator_name);
	if (function_iter != function_index_.end())
	{
		auto operator_iter = operators_.find(function_iter->second);
		if (operator_iter != operators_.end())
			return *operator_iter->second;
	}
}

template<class T>
PlanMethod Planner<T>::GetMethod(std::string method_name)
{
	auto function_iter = function_index_.find(method_name);
	if (function_iter != function_index_.end())
	{
		auto method_iter = methods_.find(function_iter->second);
		if (method_iter != methods_.end())
			return *method_iter->second;
	}
}

template<class T>
std::string Planner<T>::GetBestTask(T& t)
{
	TaskFrontier frontier, completed, incomplete, failed;
	for (auto pit = plan_tasks_.begin(); pit != plan_tasks_.end(); ++pit)
	{
		frontier.push(PlanStepTask<T>(pit->second));
	}
	while (!frontier.empty())
	{
		PlanStepTask<T> current_task = frontier.top();
		frontier.pop();
		PlanTask<T> plan_task = current_task.plan_task();
		PlanState current_state = plan_task.CollectState(t);
		PlanState goal_state = plan_task.SetGoal(t);
		current_state = plan_task.RunEvaluator(t, current_state);
		int distance = current_state.DistanceFromGoal(goal_state);
		current_task.set_cost(plan_task.RunHeuristic(t, current_state));

		if (current_state.plan_states().empty())
			failed.push(current_task);
		else if (distance == 0)
			completed.push(current_task);
		else
			incomplete.push(current_task);
	}
	if (!completed.empty())
		return completed.top().name();
	else if (!incomplete.empty())
		return incomplete.top().name();
	else if (!failed.empty())
		return failed.top().name();
	
	return "";
}

template<class T>
void Planner<T>::EvalTaskStep(T& t, Plan<T>& plan, const PlanState& goal_state)
{
	std::unique_ptr<PlanStepOperator<T>> current_operator = std::make_unique<PlanStepOperator<T>>(plan.get_operator(plan.flat(plan.flat_index())));
	plan.set_state(current_operator->plan_operator()->RunEvaluator(t, plan.state()));
	int result_cost = current_operator->plan_operator()->RunHeuristic(t, plan.state());
	int distance = plan.state().DistanceFromGoal(goal_state);
	plan.inc_flat_index();

	if (plan.state().plan_states().empty())
		current_operator->set_status("failed");
	else if (distance == 0)
		current_operator->set_status("completed");
	else
		current_operator->set_status("incomplete");

	if (plan.type() == "operator")
	{
		plan.set_status(current_operator->status());
		plan.set_cost(result_cost);
		return;
	}

	if (current_operator->status() == "incomplete")
		current_operator->set_status("completed");

	int current_id = current_operator->step_id();
	int parent_id = current_operator->parent_id();
	int last_cost = result_cost;
	std::unique_ptr<PlanStepMethod> parent_method = plan.method(parent_id);
	while (parent_id != -1)
	{
		if (current_operator->status() == "failed")
			parent_method->set_status("failed");
		else if (parent_method->child_id(parent_method->child_ids_size() - 1) == current_id) //final op in method, so close
			parent_method->set_status("completed");
		if (parent_method->type() == "parallel")
		{
			if (parent_method->cost() < last_cost)
				parent_method->add_cost(last_cost);
		}
		else
			parent_method->add_cost(last_cost);

		last_cost = parent_method->cost();
		current_id = parent_method->step_id();
		parent_id = parent_method->parent_id();
		if (parent_id != -1)
			parent_method = plan.method(parent_id);
	}

	if (parent_id == -1)
	{
		if (plan.type() == "parallel")
		{
			if (plan.cost() < last_cost)
				plan.add_cost(last_cost);
		}
		else
			plan.add_cost(last_cost);

		plan.set_status(parent_method->status());
		if (plan.status() == "completed" && distance > 0)
			plan.status() = "incomplete";
	}
}

template<class T>
void Planner<T>::EvalStarStep(T& t, Plan<T>& plan, const PlanState& goal_state)
{
	std::unique_ptr<PlanStepOperator<T>> current_operator = std::make_unique<PlanStepOperator<T>>(plan.get_operator(plan.flat(plan.flat_index())));
	plan.set_state(current_operator->plan_operator()->RunEvaluator(t, plan.state()));
	int result_cost = current_operator->plan_operator()->RunHeuristic(t, plan.state());
	int distance = plan.state().DistanceFromGoal(goal_state);
	plan.add_cost(result_cost);
	plan.inc_flat_index();

	if (plan.state().plan_states().empty())
		plan.set_status("failed");
	else if (distance == 0)
		plan.set_status("completed");
}

template<class T>
int Planner<T>::GetBestTaskPlan(PlanFrontier& plans, const PlanState& goal_state)
{
	Plan<T> plan = plans.top();
	if (plans.size() == 1)
		return plan.plan_id();

	std::vector<int> best_plan_ids = { plan.plan_id() };
	int best_score = INT_MAX;
	int best_cost = plan.cost();
	while (plan.cost() == best_cost && !plans.empty())
	{
		plans.pop();
		int distance = plan.state().DistanceFromGoal(goal_state);
		int operators_used = plan.flat_size() - plan.flat_index();
		int score = distance * operators_used;
		if (score < best_score)
			best_plan_ids = { plan.plan_id() };
		else if (score == best_score)
			best_plan_ids.push_back(plan.plan_id());

		if (!plans.empty())
			plan = plans.top();
	}
	if (best_plan_ids.size() == 1)
		return best_plan_ids[0];
	else
		return best_plan_ids[rand() % (best_plan_ids.size() - 1)];
}

template<class T>
Plan<T> Planner<T>::GetBestStarPlan(PlanFrontier& plans, const PlanState& goal_state)
{
	Plan<T> plan = plans.top();
	if (plans.size() == 1)
	{
		plan.reset();
		return plan;
	}

	plans.pop();
	std::vector<Plan<T>> best_plan_ids = { plan };
	int best_score = INT_MAX;
	int best_cost = plan.cost();
	while (plan.cost() == best_cost && !plans.empty())
	{
		int distance = plan.state().DistanceFromGoal(goal_state);
		int operators_used = plan.flat_size() - plan.flat_index();
		int score = distance * operators_used;
		if (score < best_score)
			best_plan_ids = { plan };
		else if (score == best_score)
			best_plan_ids.push_back(plan);
		plans.pop();
	}
	if (best_plan_ids.size() == 1)
	{
		best_plan_ids[0].reset();
		return best_plan_ids[0];
	}
	else
	{
		int random_index = rand() % (best_plan_ids.size() - 1);
		best_plan_ids[random_index].reset();
		return best_plan_ids[random_index];
	}
}

template<class T>
Plan<T> Planner<T>::GetPlan(T& t, std::string plan_name, std::string plan_type, std::string greedy_type, PlanState current_state, const PlanState& goal_state)
{
	if (plan_type == "task")
		return GetTaskPlan(t, plan_name, greedy_type, current_state, goal_state);
	return Plan<T>();
}

template<class T>
Plan<T> Planner<T>::GetTaskPlan(T& t, std::string task_name, std::string greedy_type, PlanState current_state, const PlanState& goal_state)
{
	std::vector<Plan<T>> plans = plan_tasks_[task_name]->task_plans();
	int chosen_plan_id = -1;
	if (plans.size() == 1)
	{
		plans[0].reset();
		return plans[0];
	}

	if (greedy_type == "shortest")
	{
		int min_length = INT_MAX;
		Plan<T> min_plan = plans[0];
		for (int i = 1; i < plans.size(); i++)
		{
			if (plans[i].flat_size() < min_length)
			{
				min_length = plans[i].flat_size();
				min_plan = plans[i];
			}
		}
		return min_plan;
	}

	PlanFrontier frontier, completed, incomplete, failed;
	for (int i = 0; i < plans.size(); i++)
	{
		plans[i].set_state(current_state);
		plans[i].set_plan_id(i);
		frontier.push(plans[i]);
	}

	while (!frontier.empty())
	{
		Plan<T> current_plan = frontier.top();
		frontier.pop();
		EvalTaskStep(t, current_plan, goal_state);
		if (current_plan.status() == "open")
			frontier.push(current_plan);
		else if (current_plan.status() == "completed")
		{
			if (!frontier.empty())
			{
				Plan<T> top_plan = frontier.top();
				if (greedy_type == "goal" || current_plan.cost() < top_plan.cost())
					return plans[top_plan.plan_id()];
			}
			completed.push(current_plan);
		}
		else if (current_plan.status() == "incomplete")
			incomplete.push(current_plan);
		else if (current_plan.status() == "failed")
			failed.push(current_plan);
	}

	int plan_index = -1;
	if (!completed.empty())
		plan_index = GetBestTaskPlan(completed, goal_state);
	if (!incomplete.empty())
		plan_index = GetBestTaskPlan(incomplete, goal_state);
	if (!failed.empty())
		plan_index = GetBestTaskPlan(failed, goal_state);
	if (!frontier.empty())
		plan_index = GetBestTaskPlan(frontier, goal_state);

	if (plan_index != -1)
		return plans[plan_index];
	
	return Plan<T>();
}

template<class T>
Plan<T> Planner<T>::GetStarPlan(T& t, std::string graph_name, std::string star_name, std::string greedy_type, int max_steps, PlanState current_state, const PlanState& goal_state, bool repeat)
{
	PlanStar steps = stars_[graph_name][star_name];
	PlanFrontier frontier, completed, incomplete, failed;
	Plan<T> first_plan = steps.first;
	first_plan.set_state(current_state);

	frontier.push(first_plan);

	while (!frontier.empty())
	{
		Plan<T> current_plan = frontier.top();
		frontier.pop();

		EvalStarStep(t, current_plan, goal_state);
		if (current_plan.status() == "open")
		{
			if (max_steps != current_plan.flat_size())
			{
				int last_op_index = current_plan.flat(current_plan.flat_size() - 1);
				std::string last_op_name = current_plan.get_operator(last_op_index).name();
				PlanStar next_operators = stars_[graph_name][last_op_name];
				for (auto nit = next_operators.second.begin(); nit != next_operators.second.end(); ++nit)
				{
					std::string next_name = nit->name();
					auto current_operators = current_plan.operators();
					bool operator_found = false;
					for (auto it = current_operators.begin(); it != current_operators.end(); ++it)
					{
						if (it->second.name() == next_name)
						{
							operator_found = true;
							break;
						}
					}
					if ((operator_found && repeat) || !operator_found)
					{
						Plan<T> next_plan = current_plan;
						next_plan.add_operator(next_plan.flat_index(), *nit);
						next_plan.add_flat(next_plan.flat_index());
						frontier.push(next_plan);
					}
				}
			}
			current_plan.set_status("incomplete");
			incomplete.push(current_plan);
		}
		else if (current_plan.status() == "completed")
		{
			if (!frontier.empty())
			{
				Plan<T> top_plan = frontier.top();
				if (greedy_type == "goal" || current_plan.cost() < top_plan.cost())
				{
					current_plan.reset();
					return current_plan;
				}
			}
			completed.push(current_plan);
		}
		else if (current_plan.status() == "failed")
			failed.push(current_plan);
	}

	if (!completed.empty())
		return GetBestStarPlan(completed, goal_state);
	if (!incomplete.empty())
		return GetBestStarPlan(incomplete, goal_state);
	if (!failed.empty())
		return GetBestStarPlan(failed, goal_state);
	if (!frontier.empty())
		return GetBestStarPlan(frontier, goal_state);

	return Plan<T>();
}

template<class T>
Plan<T> Planner<T>::MakePlan(int operator_id, int method_id, int parent_id, std::shared_ptr<PlanOperator<T>> plan_operator)
{
	Plan<T> new_plan;
	new_plan.set_name(plan_operator->name());
	new_plan.add_operator(operator_id, PlanStepOperator<T>(operator_id, parent_id, plan_operator->name(), plan_operator));
	new_plan.add_flat(operator_id);
	new_plan.add_child_id(operator_id);
	new_plan.add_children(plan_operator->name());
	new_plan.set_type("operator");
	new_plan.set_operator_id(operator_id + 2);
	new_plan.set_method_id(method_id);
	new_plan.add_statekeys(plan_operator->statekeys());
	return new_plan;
}

template<class T>
Plan<T> Planner<T>::MakePlan(int operator_id, int method_id, int parent_id, std::shared_ptr<PlanMethod> plan_method)
{
	PlanStepMethod plan_step_method = PlanStepMethod(method_id, parent_id, plan_method->name(), plan_method->type());
	Plan<T> new_plan;
	new_plan.set_name(plan_method->name());
	new_plan.set_operator_id(operator_id);
	new_plan.set_type(plan_method->type());
	auto plan_method_child_ids = plan_method->child_ids();
	int next_id = method_id;
	std::string next_name;
	new_plan.set_method_id(method_id + 2);
	for (int i = 0; i < plan_method_child_ids.size(); i++)
	{
		Plan<T> child_plan;
		if (plan_method_child_ids[i] % 2 == 0)
		{
			next_id = new_plan.operator_id();
			auto operator_iter = operators_.find(plan_method_child_ids[i]);
			if (operator_iter != operators_.end())
				child_plan = MakePlan(new_plan.operator_id(), new_plan.method_id(), method_id, operator_iter->second);
			next_name = operator_iter->second->name();
		}
		else
		{
			next_id = new_plan.method_id();
			auto method_iter = methods_.find(plan_method_child_ids[i]);
			if (method_iter != methods_.end())
				child_plan = MakePlan(new_plan.operator_id(), new_plan.method_id(), method_id, method_iter->second);
			next_name = method_iter->second->name();
		}
		if (!child_plan.children().empty())
		{
			new_plan.add_operators(child_plan.operators());
			new_plan.add_methods(child_plan.methods());
			new_plan.add_flat(child_plan.all_flat());
			new_plan.add_child_id(next_id);
			new_plan.add_children(next_name);
			new_plan.add_statekeys(child_plan.statekeys());
			plan_step_method.add_child_id(next_id);
		}
		new_plan.set_operator_id(child_plan.operator_id());
		new_plan.set_method_id(child_plan.method_id());
	}
	new_plan.set_method(method_id, plan_step_method);
	return new_plan;
}
}

#endif