#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> init_state(const string& bits) {
    vector<int> state(bits.size());
    for (size_t i = 0; i < bits.size(); ++i)
        state[i] = (bits[i] == '1') ? 1 : 0;
    return state;
}

vector<int> evolve(const vector<int>& current, int rule_number) {
    int n = current.size();
    vector<int> next(n, 0);

    vector<int> rule(16);
    for (int i = 0; i < rule.size(); ++i)
        rule[i] = (rule_number >> i) & 1;

    for (int i = 0; i < n; ++i) {
        int left  = current[(i - 1 + n) % n];
        int self  = current[i];
        int right = current[(i + 1) % n];

        int pattern = (left << 2) | (self << 1) | right;
        // cout << "pattern :" << pattern << endl;

        next[i] = rule[pattern];
    }

    return next;
}

void print_state(const vector<int>& state) {
    for (int bit : state)
        cout << "|"<< (bit ? "#" : " ");
    cout << "|" << endl;
}

int main() {
    string initial_bits = "0000000001000000000";
    vector<int> state = init_state(initial_bits);

    int rule_number = 30;

    cout << "1D cellular automaton - Rule " << rule_number << endl;
    print_state(state);

    for (int t = 0; t < 100; ++t) {
        state = evolve(state, rule_number);
        print_state(state);
    }

    return 0;
}
