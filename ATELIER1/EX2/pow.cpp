#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using steady_clock = chrono::steady_clock;

string to_hex_size_t(size_t x) {
    stringstream ss;
    ss << hex << setw(sizeof(size_t) * 2) << setfill('0') << x;
    return ss.str();
}

string hash_string(const string &data) {
    size_t h = std::hash<string>{}(data);
    return to_hex_size_t(h);
}

struct Block {
    int index;
    long long timestamp;
    string data;
    string prev_hash;
    uint64_t nonce;
    string hash;

    Block(int idx, const string &d, const string &prev)
        : index(idx), data(d), prev_hash(prev), nonce(0) {
        auto now = chrono::system_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
        timestamp = ms;
    }

    string compute_hash() const {
        stringstream ss;
        ss << index << "|" << timestamp << "|" << data << "|" << prev_hash << "|" << nonce;
        return hash_string(ss.str());
    }
};

class SimpleChain {
  public:
    SimpleChain() { 
        Block genesis(0, "Block", "0");
        genesis.hash = genesis.compute_hash();
        chain.push_back(genesis);
    }

    const Block &latest() const { return chain.back(); }

    pair<Block, long long> mine_block(const string &data, int difficulty) {
        Block b(chain.size(), data, latest().hash);
        string target_prefix = string(difficulty, '0');

        auto t0 = steady_clock::now();
        while (true) {
            b.hash = b.compute_hash();
            if (has_leading_zeros(b.hash, difficulty)) break;
            ++b.nonce;
            // Optional: avoid busy tight spin on some environments
            if ((b.nonce & 0xFFFF) == 0) this_thread::yield();
        }
        auto t1 = steady_clock::now();
        long long ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
        chain.push_back(b);
        return {b, ms};
    }

    bool validate_chain() const {
        for (size_t i = 1; i < chain.size(); ++i) {
            const Block &cur = chain[i];
            const Block &prev = chain[i - 1];
            if (cur.prev_hash != prev.hash) return false;
            if (cur.compute_hash() != cur.hash) return false;
        }
        return true;
    }

    void print_chain() const {
        for (const auto &b : chain) {
            cout << "----------------------------\n";
            cout << "Index: " << b.index << "\n";
            cout << "Timestamp(ms): " << b.timestamp << "\n";
            cout << "Data: " << b.data << "\n";
            cout << "PrevHash: " << b.prev_hash << "\n";
            cout << "Nonce: " << b.nonce << "\n";
            cout << "Hash: " << b.hash << "\n";
        }
        cout << "----------------------------\n";
    }

  private:
    vector<Block> chain;

    static bool has_leading_zeros(const string &hexhash, int difficulty) {
        if (difficulty <= 0) return true;
        if ((int)hexhash.size() < difficulty) return false;
        for (int i = 0; i < difficulty; ++i) if (hexhash[i] != '0') return false;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SimpleChain chain;

    vector<int> difficulties = {0, 1, 2, 3, 4};

    for (int d : difficulties) {
        cout << "Mining with difficulty = " << d << " (leading hex zeros)\n";
        auto [blk, ms] = chain.mine_block("Block at difficulty " + to_string(d), d);
        cout << " Mined block index=" << blk.index << " nonce=" << blk.nonce << " time=" << ms << " ms" << " hash=" << blk.hash << "\n\n";
    }

    cout << "Validating chain: " << (chain.validate_chain() ? "OK" : "FAIL") << "\n\n";

    cout << "Full chain:\n";
    chain.print_chain();

    return 0;
}
