#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class GoalStackPlanner {
public:
    vector<string> initial_state;
    vector<string> goal_state;
    vector<string> current_state;
    vector<string> action_stack;
    vector<string> goal_stack;

    GoalStackPlanner(vector<string>& initial, vector<string>& goal) {
        initial_state = initial;
        goal_state = goal;
        current_state = initial;
        goal_stack = goal_state;
    }

    bool is_goal_achieved(const string& goal) {
        return find(current_state.begin(), current_state.end(), goal) != current_state.end();
    }

    void apply_action(const string& action) {
        if (action == "UNSTACK(A, B)") {
            // Remove "ON(A, B)" and add "CLEAR(B)", "HOLDING(A)"
            remove_from_state("ON(A, B)");
            current_state.push_back("CLEAR(B)");
            current_state.push_back("HOLDING(A)");
            action_stack.push_back(action);
        } else if (action == "STACK(B, A)") {
            // Remove "CLEAR(A)", "HOLDING(B)", and add "ON(B, A)"
            remove_from_state("CLEAR(A)");
            remove_from_state("HOLDING(B)");
            current_state.push_back("ON(B, A)");
            action_stack.push_back(action);
        } else if (action == "PICKUP(A)") {
            // Remove "ONTABLE(A)" and add "HOLDING(A)"
            remove_from_state("ONTABLE(A)");
            current_state.push_back("HOLDING(A)");
            action_stack.push_back(action);
        } else if (action == "STACK(A, B)") {
            // Remove "HOLDING(A)" and add "ON(A, B)", "CLEAR(B)"
            remove_from_state("HOLDING(A)");
            current_state.push_back("ON(A, B)");
            current_state.push_back("CLEAR(B)");
            action_stack.push_back(action);
        }
    }

    void remove_from_state(const string& action) {
        auto it = find(current_state.begin(), current_state.end(), action);
        if (it != current_state.end()) {
            current_state.erase(it);
        }
    }

    // Function to plan actions to achieve the goal state
    vector<string> plan() {
        while (!goal_stack.empty()) {
            string current_goal = goal_stack.back();

            if (is_goal_achieved(current_goal)) {
                goal_stack.pop_back(); // Goal is achieved, remove from the stack
                continue;
            }

            // Identify and apply actions needed to achieve the current goal
            if (current_goal == "ON(A, B)") {
                if (find(current_state.begin(), current_state.end(), "ON(A, B)") == current_state.end()) {
                    if (find(current_state.begin(), current_state.end(), "HOLDING(A)") != current_state.end()) {
                        apply_action("STACK(A, B)");
                    } else {
                        apply_action("PICKUP(A)");
                    }
                }
            } else if (current_goal == "HOLDING(A)") {
                if (find(current_state.begin(), current_state.end(), "HOLDING(A)") == current_state.end()) {
                    apply_action("PICKUP(A)");
                }
            } else if (current_goal == "CLEAR(A)") {
                if (find(current_state.begin(), current_state.end(), "CLEAR(A)") == current_state.end()) {
                    apply_action("UNSTACK(A, B)");
                }
            } else if (current_goal == "CLEAR(B)") {
                if (find(current_state.begin(), current_state.end(), "CLEAR(B)") == current_state.end()) {
                    apply_action("UNSTACK(B, C)");
                }
            }

            // Avoid infinite loops by checking if any goal was achieved after each action
            if (goal_stack.empty()) {  // If goal_stack is empty, all goals are achieved.
                break;
            }
        }

        return action_stack;
    }

    void print_state() {
        cout << "Current State: ";
        for (const auto& state : current_state) {
            cout << state << " ";
        }
        cout << endl;
    }
};

int main() {
    vector<string> initial_state;
    vector<string> goal_state;
    string state;

    // Ask user for the initial state
    cout << "Enter the initial state actions (e.g., ON(A, B), ON(B, C), etc.). Type 'done' when finished:\n";
    while (true) {
        cin >> state;
        if (state == "done") break;
        initial_state.push_back(state);
    }

    // Ask user for the goal state
    cout << "Enter the goal state actions (e.g., ON(A, B), ON(B, C), etc.). Type 'done' when finished:\n";
    while (true) {
        cin >> state;
        if (state == "done") break;
        goal_state.push_back(state);
    }

    GoalStackPlanner planner(initial_state, goal_state);
    vector<string> actions = planner.plan();

    // Output the actions
    cout << "Actions: ";
    for (const auto& action : actions) {
        cout << action << " ";
    }
    cout << endl;

    // Output the final state
    planner.print_state();

    return 0;
}
