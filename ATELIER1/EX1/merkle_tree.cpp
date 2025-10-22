#include <iostream>
#include <vector>
#include "sha256.h"

using namespace std;

string hash_string(const string &data) {
    return sha256(data);
}

struct Node {
    string hash;
    Node *left = nullptr;
    Node *right = nullptr;

    Node(const string &h) : hash(h) {}
};

class MerkleTree {
  public:
    MerkleTree(const vector<string> &leaves_data) {
        build_tree(leaves_data);
    }

    ~MerkleTree() { free_nodes(root); }

    string get_root_hash() const { return root ? root->hash : string(); }

    void print_tree_levels() const {
        vector<Node *> level;
        if (root) level.push_back(root);
        int levelnum = 0;
        while (!level.empty()) {
            cout << "Level " << levelnum << " : ";
            vector<Node *> next;
            for (Node *n : level) {
                cout << n->hash << " ";
                if (n->left) next.push_back(n->left);
                if (n->right) next.push_back(n->right);
            }
            cout << '\n';
            level.swap(next);
            ++levelnum;
        }
    }

  private:
    Node *root = nullptr;

    void free_nodes(Node *n) {
        if (!n) return;
        free_nodes(n->left);
        free_nodes(n->right);
        delete n;
    }

    void build_tree(const vector<string> &leaves_data) {
        if (leaves_data.empty()) {
            root = nullptr;
            return;
        }
        
        vector<Node *> leaves;
        leaves.reserve(leaves_data.size());
        for (const auto &d : leaves_data) {
            string h = hash_string(d);
            leaves.push_back(new Node(h));
        }


        auto make_power_of_two = [&](vector<Node *> &v) {
            if (v.empty()) return;
            size_t n = v.size();
            size_t p = 1;
            while (p < n) p <<= 1;
            while (v.size() < p) v.push_back(new Node(v.back()->hash));
        };

        make_power_of_two(leaves);

        vector<Node *> current = leaves;
        while (current.size() > 1) {
            vector<Node *> next;
            for (size_t i = 0; i < current.size(); i += 2) {
                Node *L = current[i];
                Node *R = current[i + 1];
                string concat = L->hash + R->hash;
                string parent_hash = hash_string(concat);
                Node *parent = new Node(parent_hash);
                parent->left = L;
                parent->right = R;
                next.push_back(parent);
            }
            current.swap(next);
        }

        root = current.front();
    }
};

int main() {
    cout << "=== Example 1 : 4 leafs simples ===\n";
    vector<string> data1 = {"Houssam", "zitan", "hous", "ssam"};
    MerkleTree tree1(data1);
    cout << "Root : " << tree1.get_root_hash() << "\n";
    tree1.print_tree_levels();

    cout << "\n";

    cout << "=== Example 2 : 3 leafs ===\n";
    vector<string> data2 = {"A", "B", "C"};
    MerkleTree tree2(data2);
    cout << "Root : " << tree2.get_root_hash() << "\n";
    tree2.print_tree_levels();

    cout << "\n";

    cout << "=== Example 3 : 1 leaf ===\n";
    vector<string> data3 = {"only-data"};
    MerkleTree tree3(data3);
    cout << "Root : " << tree3.get_root_hash() << "\n";
    tree3.print_tree_levels();

    return 0;
}

