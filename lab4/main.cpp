#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <queue>

using namespace std;
// 0:1, 1: 2,3, 2:7, 3:13, 4: 18, 5: 20, 6:22

struct TreeNode {
    int value;
    vector<TreeNode*> children;

    TreeNode(int val) : value(val) {}
};

TreeNode* create_tree_node(int value) {
    return new TreeNode(value);
}

TreeNode* get_node_by_path(TreeNode* root, const vector<int>& path) {
    TreeNode* current = root;
    for (int index : path) {
        if (index >= current->children.size()) return nullptr;
        current = current->children[index];
    }
    return current;
}

bool get_path_to_element(TreeNode* root, int value, vector<int>& path) {
    if (!root) return false;
    if (root->value == value) return true;

    for (int i = 0; i < root->children.size(); ++i) {
        path.push_back(i);
        if (get_path_to_element(root->children[i], value, path)) {
            return true;
        }
        path.pop_back();
    }
    return false;
}

void add_element(TreeNode* root, int parent_value, int value) {
    vector<int> path;
    if (get_path_to_element(root, parent_value, path)) {
        TreeNode* parent = get_node_by_path(root, path);
        parent->children.push_back(new TreeNode(value));
    }
}

void print_tree(TreeNode* root) {
    if (!root) return;
    cout << root->value;
    if (!root->children.empty()) {
        cout << " (";
        for (int i = 0; i < root->children.size(); ++i) {
            if (i > 0) cout << ", ";
            print_tree(root->children[i]);
        }
        cout << ")";
    }
}

void delete_elements(TreeNode*& root, int value) {
    if (!root) return;
    for (auto it = root->children.begin(); it != root->children.end();) {
        if ((*it)->value == value) {
            delete_elements(*it, value);
            delete *it;
            it = root->children.erase(it);
        } else {
            delete_elements(*it, value);
            ++it;
        }
    }
}


struct BinaryTreeNode {
    int value;
    BinaryTreeNode* left;
    BinaryTreeNode* right;

    BinaryTreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

BinaryTreeNode* add_to_binary_tree(BinaryTreeNode* root, int value) {
    if (!root) return new BinaryTreeNode(value);
    if (value < root->value) root->left = add_to_binary_tree(root->left, value);
    else root->right = add_to_binary_tree(root->right, value);
    return root;
}

void convert_to_threaded_tree(BinaryTreeNode* root, BinaryTreeNode*& prev) {
    if (!root) return;
    convert_to_threaded_tree(root->left, prev);
    if (prev) prev->right = root;
    prev = root;
    convert_to_threaded_tree(root->right, prev);
}

void print_binary_tree(BinaryTreeNode* root) {
    if (!root) return;
    cout << root->value << " ";
    print_binary_tree(root->left);
    print_binary_tree(root->right);
}


struct ExpressionNode {
    string value;
    ExpressionNode* left;
    ExpressionNode* right;

    ExpressionNode(const string& val) : value(val), left(nullptr), right(nullptr) {}
};

ExpressionNode* build_expression_tree(const string& expression) {
    return new ExpressionNode(expression);
}

double evaluate_expression(ExpressionNode* root, const unordered_map<string, double>& variables) {
    if (!root) return 0;
    if (isdigit(root->value[0]) || (root->value.size() > 1 && isdigit(root->value[1]))) {
        return stod(root->value);
    } else if (variables.find(root->value) != variables.end()) {
        return variables.at(root->value);
    } else {
        double left_val = evaluate_expression(root->left, variables);
        double right_val = evaluate_expression(root->right, variables);
        if (root->value == "+") return left_val + right_val;
        if (root->value == "-") return left_val - right_val;
        if (root->value == "*") return left_val * right_val;
        if (root->value == "/") {
            if (right_val == 0) throw runtime_error("Division by zero");
            return left_val / right_val;
        }
        if (root->value == "^") return pow(left_val, right_val);
    }
    return 0;
}

void print_expression(ExpressionNode* root) {
    if (!root) return;
    if (root->left) print_expression(root->left);
    cout << root->value << " ";
    if (root->right) print_expression(root->right);
}

// Інтерактивний режим для задачі 1-5
void interactive_mode_trees() {
    TreeNode* tree = nullptr;
    int choice, value, parent_value;
    vector<int> path;
    while (true) {
        cout << "\nМеню:\n1. Створити дерево\n2. Додати елемент\n3. Видалити елемент\n4. Вивести дерево\n5. Вийти\nВиберіть опцію: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Введіть значення кореня: ";
                cin >> value;
                tree = create_tree_node(value);
                break;
            case 2:
                cout << "Введіть значення батька: ";
                cin >> parent_value;
                cout << "Введіть значення нового елемента: ";
                cin >> value;
                add_element(tree, parent_value, value);
                break;
            case 3:
                cout << "Введіть значення для видалення: ";
                cin >> value;
                delete_elements(tree, value);
                break;
            case 4:
                cout << "Дерево: ";
                print_tree(tree);
                cout << endl;
                break;
            case 5:
                return;
            default:
                cout << "Невірна опція. Спробуйте ще раз.\n";
        }
    }
}

void demo_mode_trees() {
    TreeNode* tree = create_tree_node(1);
    add_element(tree, 1, 2);
    add_element(tree, 1, 3);
    add_element(tree, 2, 4);
    add_element(tree, 2, 5);
    cout << "Довільне дерево: ";
    print_tree(tree);
    cout << endl;

    delete_elements(tree, 2);
    cout << "Після видалення елементів зі значенням 2: ";
    print_tree(tree);
    cout << endl;
}

void interactive_mode_binary_tree() {
    BinaryTreeNode* binary_tree = nullptr;
    int choice, value;
    while (true) {
        cout << "\nМеню:\n1. Додати елемент\n2. Вивести дерево\n3. Вийти\nВиберіть опцію: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Введіть значення: ";
                cin >> value;
                binary_tree = add_to_binary_tree(binary_tree, value);
                break;
            case 2:
                print_binary_tree(binary_tree);
                cout << endl;
                break;
            case 3:
                return;
            default:
                cout << "Невірна опція. Спробуйте ще раз.\n";
        }
    }
}

void demo_mode_binary_tree() {
    BinaryTreeNode* binary_tree = nullptr;
    binary_tree = add_to_binary_tree(binary_tree, 5);
    add_to_binary_tree(binary_tree, 3);
    add_to_binary_tree(binary_tree, 7);
    add_to_binary_tree(binary_tree, 2);
    add_to_binary_tree(binary_tree, 4);
    add_to_binary_tree(binary_tree, 6);
    add_to_binary_tree(binary_tree, 8);

    cout << "Бінарне дерево: ";
    print_binary_tree(binary_tree);
    cout << endl;

    BinaryTreeNode* prev = nullptr;
    convert_to_threaded_tree(binary_tree, prev);

    cout << "Прошите дерево: ";
    print_binary_tree(binary_tree);
    cout << endl;
}

void interactive_mode_expression_tree() {
    string expression;
    cout << "Введіть вираз: ";
    cin >> expression;

    ExpressionNode* expr_tree = build_expression_tree(expression);

    unordered_map<string, double> variables;
    double result = evaluate_expression(expr_tree, variables);
    cout << "Результат виразу: " << result << endl;

    cout << "Арифметичний вираз: ";
    print_expression(expr_tree);
    cout << endl;
}

void demo_mode_expression_tree() {
    string expression = "3+5*2";
    ExpressionNode* expr_tree = build_expression_tree(expression);

    unordered_map<string, double> variables;
    double result = evaluate_expression(expr_tree, variables);
    cout << "Результат виразу: " << result << endl;

    cout << "Арифметичний вираз: ";
    print_expression(expr_tree);
    cout << endl;
}

int main() {
    int choice;
    while (true) {
        cout << "\nГоловне меню:\n1. Завдання 1-5 (довільне дерево)\n2. Завдання 6-7 (бінарне дерево)\n3. Завдання 8 (дерево виразів)\n4. Вийти\nВиберіть опцію: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Виберіть режим:\n1. Інтерактивний\n2. Демонстраційний\nВиберіть опцію: ";
                cin >> choice;
                if (choice == 1) interactive_mode_trees();
                else if (choice == 2) demo_mode_trees();
                else cout << "Невірно\n";
                break;
            case 2:
                cout << "Виберіть режим:\n1. Інтерактивний\n2. Демонстраційний\nВиберіть опцію: ";
                cin >> choice;
                if (choice == 1) interactive_mode_binary_tree();
                else if (choice == 2) demo_mode_binary_tree();
                else cout << "Невірно\n";
                break;
            case 3:
                cout << "Виберіть режим:\n1. Інтерактивний\n2. Демонстраційний\nВиберіть опцію: ";
                cin >> choice;
                if (choice == 1) interactive_mode_expression_tree();
                else if (choice == 2) demo_mode_expression_tree();
                else cout << "Невірно\n";
                break;
            case 4:
                return 0;
            default:
                cout << "Невірно \n";
        }
    }
    return 0;
}
