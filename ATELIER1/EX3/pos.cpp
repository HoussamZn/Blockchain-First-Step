#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <random>
#include "sha256.h"


using namespace std;
using namespace std::chrono;

string hash_string(const string &data) {
    return sha256(data);
}

struct Block {
    int index;
    long long timestamp;
    string data;
    string prev_hash;
    int nonce = 0;
    string hash;

    Block(int idx, const string &d, const string &prev)
        : index(idx), data(d), prev_hash(prev) {
        timestamp = chrono::duration_cast<chrono::milliseconds>(
                        chrono::system_clock::now().time_since_epoch())
                        .count();
        hash = calculate_hash();
    }

    string calculate_hash() const {
        return hash_string(to_string(index) + to_string(timestamp) + data + prev_hash + to_string(nonce));
    }
};


void mine_block(Block &block, int difficulty) {
    string target(difficulty, '0');
    while (block.hash.substr(0, difficulty) != target) {
        block.nonce++;
        block.hash = block.calculate_hash();
    }
}

// pos
struct Validator {
    string name;
    double stake;
};

string select_validator(const vector<Validator> &validators) {
    double total_stake = 0.0;
    for (auto &v : validators) total_stake += v.stake;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, total_stake);

    double r = dis(gen);
    double cumulative = 0.0;

    for (auto &v : validators) {
        cumulative += v.stake;
        if (r <= cumulative)
            return v.name;
    }
    return validators.back().name;
}


class Blockchain {
public:
    vector<Block> chain;
    Blockchain() {
        chain.emplace_back(0, "Block", "0");
    }

    void add_block(Block new_block) {
        new_block.prev_hash = chain.back().hash;
        chain.push_back(new_block);
    }
};


int main() {
    Blockchain chain;
    vector<Validator> validators = {
        {"Houssam", 30.0},
        {"Zitan", 10.0},
        {"hhhahh", 60.0}
    };

    int difficulty = 5;

    cout << "\n========== Proof of Work ==========\n";
    Block powBlock(1, "Transaction Data (PoW)", chain.chain.back().hash);

    auto start_pow = high_resolution_clock::now();
    mine_block(powBlock, difficulty);
    auto end_pow = high_resolution_clock::now();

    chain.add_block(powBlock);
    auto duration_pow = duration_cast<milliseconds>(end_pow - start_pow).count();

    cout << "Block mined! Hash: " << powBlock.hash.substr(0, 10) << "...\n";
    cout << "Nonce: " << powBlock.nonce << "\n";
    cout << "Time (PoW): " << duration_pow << " ms\n";

    cout << "\n========== Proof of Stake ==========\n";
    Block posBlock(2, "Transaction Data (PoS)", chain.chain.back().hash);

    auto start_pos = high_resolution_clock::now();
    string chosen = select_validator(validators);
    posBlock.hash = posBlock.calculate_hash();
    chain.add_block(posBlock);
    auto end_pos = high_resolution_clock::now();
    auto duration_pos = duration_cast<milliseconds>(end_pos - start_pos).count();

    cout << "Validator chosen: " << chosen << "\n";
    cout << "Hash: " << posBlock.hash.substr(0, 10) << "...\n";
    cout << "Time (PoS): " << duration_pos << " ms\n";

    cout << "\n========== Comparison ==========\n";
    cout << "Proof of Work: " << duration_pow << " ms\n";
    cout << "Proof of Stake: " << duration_pos << " ms\n";

    return 0;
}
