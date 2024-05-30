#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <memory>
#include <set>
#include <map>

using namespace std;

string generate_random_string(size_t length) {
    const string CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> dist(0, CHARACTERS.size() - 1);
    string result;
    for (size_t i = 0; i < length; ++i) {
        result += CHARACTERS[dist(generator)];
    }
    return result;
}

class LinkedList {
    struct Node {
        string data;
        unique_ptr<Node> next;
        Node(const string& data) : data(data) {}
    };

    unique_ptr<Node> head;

public:
    void insert(const string& value) {
        auto new_node = make_unique<Node>(value);
        if (!head || head->data <= value) {
            new_node->next = move(head);
            head = move(new_node);
        } else {
            Node* current = head.get();
            while (current->next && current->next->data > value) {
                current = current->next.get();
            }
            new_node->next = move(current->next);
            current->next = move(new_node);
        }
    }

    bool remove(const string& value) {
        if (!head) return false;
        if (head->data == value) {
            head = move(head->next);
            return true;
        }
        Node* current = head.get();
        while (current->next && current->next->data != value) {
            current = current->next.get();
        }
        if (current->next && current->next->data == value) {
            current->next = move(current->next->next);
            return true;
        }
        return false;
    }

    void print() const {
        Node* current = head.get();
        while (current) {
            cout << current->data << " ";
            current = current->next.get();
        }
        cout << endl;
    }
};

class ArrayList {
    vector<string> data;

public:
    void insert(const string& value) {
        data.push_back(value);
        sort(data.begin(), data.end(), greater<>());
    }

    bool remove(const string& value) {
        auto it = find(data.begin(), data.end(), value);
        if (it != data.end()) {
            data.erase(it);
            return true;
        }
        return false;
    }

    void print() const {
        for (const auto& item : data) {
            cout << item << " ";
        }
        cout << endl;
    }
};

class BST {
    struct Node {
        string data;
        unique_ptr<Node> left;
        unique_ptr<Node> right;
        Node(const string& data) : data(data) {}
    };

    unique_ptr<Node> root;

    void insert(unique_ptr<Node>& node, const string& value) {
        if (!node) {
            node = make_unique<Node>(value);
        } else if (value > node->data) {
            insert(node->left, value);
        } else {
            insert(node->right, value);
        }
    }

    bool remove(unique_ptr<Node>& node, const string& value) {
        if (!node) return false;
        if (value > node->data) {
            return remove(node->left, value);
        } else if (value < node->data) {
            return remove(node->right, value);
        } else {
            if (!node->left) {
                node = move(node->right);
            } else if (!node->right) {
                node = move(node->left);
            } else {
                Node* minNode = node->right.get();
                while (minNode->left) {
                    minNode = minNode->left.get();
                }
                node->data = minNode->data;
                remove(node->right, minNode->data);
            }
            return true;
        }
    }

    void inorder(const unique_ptr<Node>& node) const {
        if (node) {
            inorder(node->left);
            cout << node->data << " ";
            inorder(node->right);
        }
    }

public:
    void insert(const string& value) {
        insert(root, value);
    }

    bool remove(const string& value) {
        return remove(root, value);
    }

    void print() const {
        inorder(root);
        cout << endl;
    }
};

class AVLTree {
    struct Node {
        string data;
        unique_ptr<Node> left;
        unique_ptr<Node> right;
        int height;
        Node(const string& data) : data(data), height(1) {}
    };

    unique_ptr<Node> root;

    int height(const unique_ptr<Node>& node) const {
        return node ? node->height : 0;
    }

    int balance_factor(const unique_ptr<Node>& node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void update_height(Node& node) {
        node.height = max(height(node.left), height(node.right)) + 1;
    }

    unique_ptr<Node> rotate_right(unique_ptr<Node> y) {
        auto x = move(y->left);
        y->left = move(x->right);
        x->right = move(y);
        update_height(*x->right);
        update_height(*x);
        return x;
    }

    unique_ptr<Node> rotate_left(unique_ptr<Node> x) {
        auto y = move(x->right);
        x->right = move(y->left);
        y->left = move(x);
        update_height(*y->left);
        update_height(*y);
        return y;
    }

    unique_ptr<Node> balance(unique_ptr<Node> node) {
        update_height(*node);
        int balance = balance_factor(node);
        if (balance > 1) {
            if (balance_factor(node->left) < 0) {
                node->left = rotate_left(move(node->left));
            }
            return rotate_right(move(node));
        }
        if (balance < -1) {
            if (balance_factor(node->right) > 0) {
                node->right = rotate_right(move(node->right));
            }
            return rotate_left(move(node));
        }
        return node;
    }

    unique_ptr<Node> insert(unique_ptr<Node> node, const string& value) {
        if (!node) return make_unique<Node>(value);
        if (value > node->data) {
            node->left = insert(move(node->left), value);
        } else {
            node->right = insert(move(node->right), value);
        }
        return balance(move(node));
    }

    unique_ptr<Node> remove(unique_ptr<Node> node, const string& value) {
        if (!node) return nullptr;
        if (value > node->data) {
            node->left = remove(move(node->left), value);
        } else if (value < node->data) {
            node->right = remove(move(node->right), value);
        } else {
            if (!node->left) {
                return move(node->right);
            } else if (!node->right) {
                return move(node->left);
            }
            Node* minNode = node->right.get();
            while (minNode->left) {
                minNode = minNode->left.get();
            }
            node->data = minNode->data;
            node->right = remove(move(node->right), minNode->data);
        }
        return balance(move(node));
    }

    void inorder(const unique_ptr<Node>& node) const {
        if (node) {
            inorder(node->left);
            cout << node->data << " ";
            inorder(node->right);
        }
    }

public:
    void insert(const string& value) {
        root = insert(move(root), value);
    }

    bool remove(const string& value) {
        root = remove(move(root), value);
        return root != nullptr;
    }

    void print() const {
        inorder(root);
        cout << endl;
    }
};

class TwoThreeTree {
    struct Node {
        vector<string> keys;
        vector<shared_ptr<Node>> children;

        Node(const string& key) {
            keys.push_back(key);
        }

        bool isLeaf() const {
            return children.empty();
        }

        bool isFull() const {
            return keys.size() == 3;
        }
    };

    shared_ptr<Node> root;

    void insert(shared_ptr<Node>& node, const string& key) {
        if (!node) {
            node = make_shared<Node>(key);
            return;
        }

        if (node->isLeaf()) {
            node->keys.push_back(key);
            sort(node->keys.begin(), node->keys.end());
            if (node->isFull()) {
                split(node);
            }
        } else {
            if (key < node->keys[0]) {
                insert(node->children[0], key);
            } else if (node->keys.size() == 1 || (node->keys.size() == 2 && key < node->keys[1])) {
                insert(node->children[1], key);
            } else {
                insert(node->children[2], key);
            }
        }
    }

    void split(shared_ptr<Node>& node) {
        if (node == root) {
            root = make_shared<Node>(node->keys[1]);
            root->children.push_back(make_shared<Node>(node->keys[0]));
            root->children.push_back(make_shared<Node>(node->keys[2]));
            if (!node->isLeaf()) {
                root->children[0]->children.push_back(node->children[0]);
                root->children[0]->children.push_back(node->children[1]);
                root->children[1]->children.push_back(node->children[2]);
                root->children[1]->children.push_back(node->children[3]);
            }
        } else {
        }
    }

    bool remove(shared_ptr<Node>& node, const string& key) {
        if (!node) return false;

        auto it = find(node->keys.begin(), node->keys.end(), key);
        if (it != node->keys.end()) {
            if (node->isLeaf()) {
                node->keys.erase(it);
                return true;
            } else {
                return false;
            }
        } else {
            for (size_t i = 0; i < node->keys.size(); ++i) {
                if (key < node->keys[i]) {
                    return remove(node->children[i], key);
                }
            }
            return remove(node->children.back(), key);
        }
    }

public:
    void insert(const string& key) {
        insert(root, key);
    }

    bool remove(const string& key) {
        return remove(root, key);
    }

    void inorder(const shared_ptr<Node>& node) const {
        if (!node) return;
        if (node->keys.size() > 0) {
            inorder(node->children[0]);
            cout << node->keys[0] << " ";
            inorder(node->children[1]);
            if (node->keys.size() > 1) {
                cout << node->keys[1] << " ";
                inorder(node->children[2]);
            }
        }
    }

    void print() const {
        inorder(root);
        cout << endl;
    }
};

void interactive_mode() {
    LinkedList linked_list;
    ArrayList array_list;
    BST bst;
    AVLTree avl_tree;
    TwoThreeTree two_three_tree;

    while (true) {
        cout << "Select data structure:\n";
        cout << "1. Linked List\n";
        cout << "2. Array List\n";
        cout << "3. Binary Search Tree\n";
        cout << "4. AVL Tree\n";
        cout << "5. 2-3 Tree\n";
        cout << "6. Exit\n";
        int choice;
        cin >> choice;

        if (choice == 6) break;

        cout << "Select operation:\n";
        cout << "1. Insert element\n";
        cout << "2. Remove element\n";
        cout << "3. Print elements\n";
        int operation;
        cin >> operation;

        string value;
        if (operation == 1 || operation == 2) {
            cout << "Enter value: ";
            cin >> value;
        }

        switch (choice) {
            case 1:
                if (operation == 1) linked_list.insert(value);
                else if (operation == 2) linked_list.remove(value);
                else linked_list.print();
                break;
            case 2:
                if (operation == 1) array_list.insert(value);
                else if (operation == 2) array_list.remove(value);
                else array_list.print();
                break;
            case 3:
                if (operation == 1) bst.insert(value);
                else if (operation == 2) bst.remove(value);
                else bst.print();
                break;
            case 4:
                if (operation == 1) avl_tree.insert(value);
                else if (operation == 2) avl_tree.remove(value);
                else avl_tree.print();
                break;
            case 5:
                if (operation == 1) two_three_tree.insert(value);
                else if (operation == 2) two_three_tree.remove(value);
                else two_three_tree.print();
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }
}

void demo_mode() {
    LinkedList linked_list;
    ArrayList array_list;
    BST bst;
    AVLTree avl_tree;
    TwoThreeTree two_three_tree;

    auto insert_elements = [](auto& container, const vector<string>& elements) {
        for (const auto& elem : elements) {
            container.insert(elem);
        }
    };

    auto remove_elements = [](auto& container, const vector<string>& elements) {
        for (const auto& elem : elements) {
            container.remove(elem);
        }
    };

    cout << "Demo Mode:" << endl;
    vector<string> demo_elements = {"banana", "apple", "orange", "kiwi", "grape"};
    insert_elements(linked_list, demo_elements);
    insert_elements(array_list, demo_elements);
    insert_elements(bst, demo_elements);
    insert_elements(avl_tree, demo_elements);
    insert_elements(two_three_tree, demo_elements);

    cout << "LinkedList: ";
    linked_list.print();
    cout << "ArrayList: ";
    array_list.print();
    cout << "BST: ";
    bst.print();
    cout << "AVL Tree: ";
    avl_tree.print();
    cout << "2-3 Tree: ";
    two_three_tree.print();

    vector<string> remove_demo_elements = {"banana", "kiwi"};
    remove_elements(linked_list, remove_demo_elements);
    remove_elements(array_list, remove_demo_elements);
    remove_elements(bst, remove_demo_elements);
    remove_elements(avl_tree, remove_demo_elements);
    remove_elements(two_three_tree, remove_demo_elements);

    cout << "\nAfter Removal:" << endl;
    cout << "LinkedList: ";
    linked_list.print();
    cout << "ArrayList: ";
    array_list.print();
    cout << "BST: ";
    bst.print();
    cout << "AVL Tree: ";
    avl_tree.print();
    cout << "2-3 Tree: ";
    two_three_tree.print();
}

void benchmark_mode() {
    LinkedList linked_list;
    ArrayList array_list;
    BST bst;
    AVLTree avl_tree;
    TwoThreeTree two_three_tree;

    auto insert_elements = [](auto& container, const vector<string>& elements) {
        for (const auto& elem : elements) {
            container.insert(elem);
        }
    };

    auto remove_elements = [](auto& container, const vector<string>& elements) {
        for (const auto& elem : elements) {
            container.remove(elem);
        }
    };

    cout << "\nBenchmark Mode:" << endl;
    const int num_elements = 10000;
    vector<string> random_elements;
    for (int i = 0; i < num_elements; ++i) {
        random_elements.push_back(generate_random_string(10));
    }

    auto benchmark = [&](auto& container, const string& name) {
        auto start = chrono::high_resolution_clock::now();
        insert_elements(container, random_elements);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        cout << name << " insertion time: " << duration.count() << " seconds" << endl;

        start = chrono::high_resolution_clock::now();
        remove_elements(container, random_elements);
        end = chrono::high_resolution_clock::now();
        duration = end - start;
        cout << name << " removal time: " << duration.count() << " seconds" << endl;
    };

    benchmark(linked_list, "LinkedList");
    benchmark(array_list, "ArrayList");
    benchmark(bst, "BST");
    benchmark(avl_tree, "AVL Tree");
    benchmark(two_three_tree, "2-3 Tree");
}

int main() {
    while (true) {
        cout << "Select mode:\n";
        cout << "1. Demo Mode\n";
        cout << "2. Benchmark Mode\n";
        cout << "3. Interactive Mode\n";
        cout << "4. Exit\n";
        int choice;
        cin >> choice;

        if (choice == 4) break;

        switch (choice) {
            case 1:
                demo_mode();
                break;
            case 2:
                benchmark_mode();
                break;
            case 3:
                interactive_mode();
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }
    return 0;
}
