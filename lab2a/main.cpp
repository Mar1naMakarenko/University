#include <iostream>
#include <vector>
#include <chrono>
#include <stdexcept>

using namespace std;

struct Circle {
    double x, y, radius;
};

class FixedArrayList {
private:
    Circle* array;
    int capacity;
    int size;

public:
    FixedArrayList(int cap) : capacity(cap), size(0) {
        array = new Circle[capacity];
    }

    ~FixedArrayList() {
        delete[] array;
    }

    void create_empty() {
        size = 0;
    }

    void append(const Circle& circle) {
        if (size < capacity) {
            array[size++] = circle;
        } else {
            cout << "Помилка: Масив заповнений." << endl;
        }
    }

    void insert(int k, const Circle& circle) {
        if (k < 0 || k > size) {
            cout << "Помилка: Індекс поза межами." << endl;
            return;
        }
        if (size < capacity) {
            for (int i = size; i > k; --i) {
                array[i] = array[i - 1];
            }
            array[k] = circle;
            size++;
        } else {
            cout << "Помилка: Масив заповнений." << endl;
        }
    }

    void remove(int k) {
        if (k < 0 || k >= size) {
            cout << "Помилка: Індекс поза межами." << endl;
            return;
        }
        for (int i = k; i < size - 1; ++i) {
            array[i] = array[i + 1];
        }
        size--;
    }

    Circle get(int k) const {
        if (k < 0 || k >= size) {
            throw out_of_range("Індекс поза межами");
        }
        return array[k];
    }

    void set(int k, const Circle& circle) {
        if (k < 0 || k >= size) {
            throw out_of_range("Індекс поза межами");
        }
        array[k] = circle;
    }

    int length() const {
        return size;
    }
};

class DynamicArrayList {
private:
    vector<Circle> array;

public:
    void create_empty() {
        array.clear();
    }

    void append(const Circle& circle) {
        array.push_back(circle);
    }

    void insert(int k, const Circle& circle) {
        if (k < 0 || k > array.size()) {
            cout << "Помилка: Індекс поза межами." << endl;
            return;
        }
        array.insert(array.begin() + k, circle);
    }

    void remove(int k) {
        if (k < 0 || k >= array.size()) {
            cout << "Помилка: Індекс поза межами." << endl;
            return;
        }
        array.erase(array.begin() + k);
    }

    Circle get(int k) const {
        if (k < 0 || k >= array.size()) {
            throw out_of_range("Індекс поза межами");
        }
        return array[k];
    }

    void set(int k, const Circle& circle) {
        if (k < 0 || k >= array.size()) {
            throw out_of_range("Індекс поза межами");
        }
        array[k] = circle;
    }

    int length() const {
        return array.size();
    }
};

class LinkedList {
private:
    struct Node {
        Circle circle;
        Node* next;
        Node(const Circle& c) : circle(c), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void create_empty() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    void append(const Circle& circle) {
        Node* newNode = new Node(circle);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
    }

    void insert(int k, const Circle& circle) {
        if (k < 0 || k > size) {
            cout << "Помилка: Індекс поза межами." << endl;
            return;
        }
        Node* newNode = new Node(circle);
        if (k == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* temp = head;
            for (int i = 0; i < k - 1; ++i) {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    void remove(int k) {
        if (k < 0 || k >= size) {
            cout << "Помилка: Індекс поза межами." << endl;
            return;
        }
        Node* temp = head;
        if (k == 0) {
            head = head->next;
            delete temp;
        } else {
            for (int i = 0; i < k - 1; ++i) {
                temp = temp->next;
            }
            Node* toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
        }
        size--;
    }

    Circle get(int k) const {
        if (k < 0 || k >= size) {
            throw out_of_range("Індекс поза межами");
        }
        Node* temp = head;
        for (int i = 0; i < k; ++i) {
            temp = temp->next;
        }
        return temp->circle;
    }

    void set(int k, const Circle& circle) {
        if (k < 0 || k >= size) {
            throw out_of_range("Індекс поза межами");
        }
        Node* temp = head;
        for (int i = 0; i < k; ++i) {
            temp = temp->next;
        }
        temp->circle = circle;
    }

    int length() const {
        return size;
    }
};

void interactiveMode() {
    FixedArrayList fixedList(10);
    DynamicArrayList dynamicList;
    LinkedList linkedList;
    int choice, index;
    Circle circle;
    char listType;

    while (true) {
        cout << "\nОберіть тип списку (f: FixedArrayList, d: DynamicArrayList, l: LinkedList, 4: вийти): ";
        cin >> listType;
        if (listType == 'q') break;

        cout << "Оберіть операцію (1: create_empty, 2: append, 3: insert, 4: remove, 5: get, 6: set, 7: length, 0: вийти): ";
        cin >> choice;

        switch (choice) {
            case 0:
                return;
            case 1:
                if (listType == 'f') fixedList.create_empty();
                else if (listType == 'd') dynamicList.create_empty();
                else if (listType == 'l') linkedList.create_empty();
                break;
            case 2:
                cout << "Введіть x, y, радіус: ";
                cin >> circle.x >> circle.y >> circle.radius;
                if (listType == 'f') fixedList.append(circle);
                else if (listType == 'd') dynamicList.append(circle);
                else if (listType == 'l') linkedList.append(circle);
                break;
            case 3:
                cout << "Введіть індекс, x, y, радіус: ";
                cin >> index >> circle.x >> circle.y >> circle.radius;
                if (listType == 'f') fixedList.insert(index, circle);
                else if (listType == 'd') dynamicList.insert(index, circle);
                else if (listType == 'l') linkedList.insert(index, circle);
                break;
            case 4:
                cout << "Введіть індекс: ";
                cin >> index;
                if (listType == 'f') fixedList.remove(index);
                else if (listType == 'd') dynamicList.remove(index);
                else if (listType == 'l') linkedList.remove(index);
                break;
            case 5:
                cout << "Введіть індекс: ";
                cin >> index;
                try {
                    if (listType == 'f') circle = fixedList.get(index);
                    else if (listType == 'd') circle = dynamicList.get(index);
                    else if (listType == 'l') circle = linkedList.get(index);
                    cout << "Circle at index " << index << ": (" << circle.x << ", " << circle.y << ", " << circle.radius << ")\n";
                } catch (const out_of_range& e) {
                    cout << e.what() << endl;
                }
                break;
            case 6:
                cout << "Введіть індекс, x, y, радіус: ";
                cin >> index >> circle.x >> circle.y >> circle.radius;
                try {
                    if (listType == 'f') fixedList.set(index, circle);
                    else if (listType == 'd') dynamicList.set(index, circle);
                    else if (listType == 'l') linkedList.set(index, circle);
                } catch (const out_of_range& e) {
                    cout << e.what() << endl;
                }
                break;
            case 7:
                if (listType == 'f') cout << "Довжина: " << fixedList.length() << endl;
                else if (listType == 'd') cout << "Довжина: " << dynamicList.length() << endl;
                else if (listType == 'l') cout << "Довжина: " << linkedList.length() << endl;
                break;
            default:
                cout << "Некоректний вибір" << endl;
        }
    }
}

void demoMode() {
    FixedArrayList fixedList(10);
    DynamicArrayList dynamicList;
    LinkedList linkedList;

    cout << "\nFixedArrayList Демонстрація:\n";
    fixedList.append({1.0, 2.0, 3.0});
    fixedList.append({4.0, 5.0, 6.0});
    fixedList.insert(1, {7.0, 8.0, 9.0});
    cout << "Circle at index 1: (" << fixedList.get(1).x << ", " << fixedList.get(1).y << ", " << fixedList.get(1).radius << ")\n";
    fixedList.remove(1);
    cout << "Довжина після видалення: " << fixedList.length() << endl;

    cout << "\nDynamicArrayList Демонстрація:\n";
    dynamicList.append({1.0, 2.0, 3.0});
    dynamicList.append({4.0, 5.0, 6.0});
    dynamicList.insert(1, {7.0, 8.0, 9.0});
    cout << "Circle at index 1: (" << dynamicList.get(1).x << ", " << dynamicList.get(1).y << ", " << dynamicList.get(1).radius << ")\n";
    dynamicList.remove(1);
    cout << "Довжина після видалення: " << dynamicList.length() << endl;

    cout << "\nLinkedList Демонстрація:\n";
    linkedList.append({1.0, 2.0, 3.0});
    linkedList.append({4.0, 5.0, 6.0});
    linkedList.insert(1, {7.0, 8.0, 9.0});
    cout << "Circle at index 1: (" << linkedList.get(1).x << ", " << linkedList.get(1).y << ", " << linkedList.get(1).radius << ")\n";
    linkedList.remove(1);
    cout << "Довжина після видалення: " << linkedList.length() << endl;
}

void benchmarkMode() {
    const int N = 10000;
    FixedArrayList fixedList(N);
    DynamicArrayList dynamicList;
    LinkedList linkedList;

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        fixedList.append({double(i), double(i), double(i)});
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "FixedArrayList append: " << elapsed.count() << " секунд\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        fixedList.get(i);
    }
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "FixedArrayList get: " << elapsed.count() << " секунд\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        dynamicList.append({double(i), double(i), double(i)});
    }
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "DynamicArrayList append: " << elapsed.count() << " секунд\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        dynamicList.get(i);
    }
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "DynamicArrayList get: " << elapsed.count() << " секунд\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        linkedList.append({double(i), double(i), double(i)});
    }
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "LinkedList append: " << elapsed.count() << " секунд\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        linkedList.get(i);
    }
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "LinkedList get: " << elapsed.count() << " секунд\n";
}

int main() {
    int mode;
    cout << "Оберіть режим: 1) Інтерактивний, 2) Демо, 3) Бенчмарк: ";
    cin >> mode;

    switch (mode) {
        case 1:
            interactiveMode();
            break;
        case 2:
            demoMode();
            break;
        case 3:
            benchmarkMode();
            break;
        default:
            cout << "Некоректні дані" << endl;
    }

    return 0;
}

