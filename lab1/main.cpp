#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;
struct Monster {
    string name;
    int health;
    int attack;
    float specialAttackChance;
    string specialAttackType;
    time_t dateTime;
};

class MonsterDatabase {
private:
    vector<Monster> monsters;

public:
    void addMonster(const Monster& monster) {
        monsters.push_back(monster);
    }

    void saveToTextFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& monster : monsters) {
                file << monster.name << " " << monster.health << " " << monster.attack << " "
                     << monster.specialAttackChance << " " << monster.specialAttackType << " "
                     << monster.dateTime << endl;
            }
            file.close();
        } else {
            cout << "Не вдається відкрити файл" << endl;
        }
    }

    void restoreFromTextFile(const string& filename) {
        monsters.clear();
        ifstream file(filename);
        if (file.is_open()) {
            Monster monster;
            while (file >> monster.name >> monster.health >> monster.attack
                        >> monster.specialAttackChance >> monster.specialAttackType >> monster.dateTime) {
                monsters.push_back(monster);
            }
            file.close();
        } else {
            cout << "Не вдається відкрити файл" << endl;
        }
    }

    void printAllMonsters() {
        for (const auto& monster : monsters) {
            cout << "Ім'я: " << monster.name << ", Здоров'я: " << monster.health
                 << ", Атака: " << monster.attack << ", Шанс спеціальної атаки: "
                 << monster.specialAttackChance << ", Тип спеціальної атаки: "
                 << monster.specialAttackType << ", Дата і час: "
                 << ctime(&monster.dateTime);
        }
    }

    void searchMonsters(const string& fragment, const string& specialAttackType,
                        int minAttack, int maxAttack, time_t minDateTime) {
        bool found = false;

        if (!fragment.empty()) {
            cout << "Монстри, чиї імена починаються з '" << fragment << "':" << endl;
            for (const auto& monster : monsters) {
                if (monster.name.find(fragment) == 0) {
                    cout << "Ім'я: " << monster.name << ", Здоров'я: " << monster.health
                         << ", Атака: " << monster.attack << ", Шанс спеціальної атаки: "
                         << monster.specialAttackChance << ", Тип спеціальної атаки: "
                         << monster.specialAttackType << ", Дата і час: "
                         << ctime(&monster.dateTime);
                    found = true;
                }
            }
        }

        if (!specialAttackType.empty()) {
            cout << "Монстри з типом спеціальної атаки '" << specialAttackType << "' та атакою в діапазоні від "
                 << minAttack << " до " << maxAttack << ":" << endl;
            for (const auto& monster : monsters) {
                if (monster.specialAttackType == specialAttackType &&
                    monster.attack >= minAttack && monster.attack <= maxAttack) {
                    cout << "Ім'я: " << monster.name << ", Здоров'я: " << monster.health
                         << ", Атака: " << monster.attack << ", Шанс спеціальної атаки: "
                         << monster.specialAttackChance << ", Тип спеціальної атаки: "
                         << monster.specialAttackType << ", Дата і час: "
                         << ctime(&monster.dateTime);
                    found = true;
                }
            }
        }

        if (minDateTime != 0) {
            cout << "Монстри, що з'явилися після " << ctime(&minDateTime) << ":" << endl;
            for (const auto& monster : monsters) {
                if (monster.dateTime > minDateTime) {
                    cout << "Ім'я: " << monster.name << ", Здоров'я: " << monster.health
                         << ", Атака: " << monster.attack << ", Шанс спеціальної атаки: "
                         << monster.specialAttackChance << ", Тип спеціальної атаки: "
                         << monster.specialAttackType << ", Дата і час: "
                         << ctime(&monster.dateTime);
                    found = true;
                }
            }
        }

        if (!found) {
            cout << "Не знайдено монстрів" << endl;
        }
    }

    void deleteMonster(const string& name) {
        monsters.erase(remove_if(monsters.begin(), monsters.end(),
                                 [&](const Monster& monster) { return monster.name == name; }), monsters.end());
    }
};

void interactiveMode() {
    MonsterDatabase database;

    char choice;
    do {
        cout << "Оберіть операцію:" << endl;
        cout << "1. Додати монстра" << endl;
        cout << "2. Зберегти базу даних у текстовий файл" << endl;
        cout << "3. Відновити базу даних з текстового файлу" << endl;
        cout << "4. Вивести всіх монстрів" << endl;
        cout << "5. Шукати монстрів" << endl;
        cout << "6. Видалити монстра" << endl;
        cout << "7. Вийти" << endl;
        cout << "Введіть свій вибір: ";
        cin >> choice;

        switch (choice) {
            case '1': {
                Monster monster;
                cout << "Введіть ім'я: ";
                cin >> monster.name;
                cout << "Введіть здоров'я: ";
                cin >> monster.health;
                cout << "Введіть атаку: ";
                cin >> monster.attack;
                cout << "Введіть шанс спеціальної атаки: ";
                cin >> monster.specialAttackChance;
                cout << "Введіть тип спеціальної атаки: ";
                cin >> monster.specialAttackType;
                monster.dateTime = time(nullptr);
                database.addMonster(monster);
                break;
            }
            case '2': {
                string filename;
                cout << "Введіть назву файлу для збереження: ";
                cin >> filename;
                database.saveToTextFile(filename);
                break;
            }
            case '3': {
                string filename;
                cout << "Введіть назву файлу для відновлення: ";
                cin >> filename;
                database.restoreFromTextFile(filename);
                break;
            }
            case '4':
                cout << "Всі монстри:" << endl;
                database.printAllMonsters();
                break;
            case '5': {
                char searchChoice;
                cout << "Оберіть критерій пошуку:" << endl;
                cout << "1. Шукати за фрагментом імені" << endl;
                cout << "2. Шукати за типом спеціальної атаки та діапазоном атаки" << endl;
                cout << "3. Шукати за часом появи після певної дати" << endl;
                cout << "Введіть свій вибір: ";
                cin >> searchChoice;

                switch (searchChoice) {
                    case '1': {
                        string fragment;
                        cout << "Введіть фрагмент імені: ";
                        cin >> fragment;
                        database.searchMonsters(fragment, "", 0, 0, 0);
                        break;
                    }
                    case '2': {
                        string specialAttackType;
                        int minAttack, maxAttack;
                        cout << "Введіть тип спеціальної атаки: ";
                        cin >> specialAttackType;
                        cout << "Введіть мінімальну атаку: ";
                        cin >> minAttack;
                        cout << "Введіть максимальну атаку: ";
                        cin >> maxAttack;
                        database.searchMonsters("", specialAttackType, minAttack, maxAttack, 0);
                        break;
                    }
                    case '3': {
                        time_t minDateTime;
                        cout << "Введіть мінімальну дату і час: ";
                        cin >> minDateTime;
                        database.searchMonsters("", "", 0, 0, minDateTime);
                        break;
                    }
                    default:
                        cout << "Невірно, спробуйте ще раз" << endl;
                }
                break;
            }
            case '6': {
                string name;
                cout << "Введіть ім'я монстра для видалення: ";
                cin >> name;
                database.deleteMonster(name);
                break;
            }
            case '7':
                cout << "Вихід..." << endl;
                break;
            default:
                cout << "Невірно, спробуйте ще раз" << endl;
        }
    } while (choice != '7');
}

void demoMode() {
    MonsterDatabase database;

    database.addMonster({"Osama", 100, 20, 0.3, "Параліч", time(nullptr)});
    database.addMonster({"Pablo", 150, 30, 0.2, "Повторна атака", time(nullptr)});
    database.addMonster({"Gendalf", 500, 100, 0.1, "Збільшення шкоди", time(nullptr)});

    database.saveToTextFile("monsters_demo.txt");

    database.restoreFromTextFile("monsters_demo.txt");

    cout << "Всі монстри після відновлення з файлу:" << endl;
    database.printAllMonsters();

    cout << "Пошук монстрів за фрагментом імені 'O':" << endl;
    database.searchMonsters("O", "", 0, 0, 0);

    cout << "Пошук монстрів за типом спеціальної атаки 'Параліч' та діапазоном атаки від 10 до 30:" << endl;
    database.searchMonsters("", "Параліч", 10, 30, 0);

    cout << "Пошук монстрів за часом появи після певної дати (поточний час - 60 секунд):" << endl;
    time_t now = time(nullptr);
    database.searchMonsters("", "", 0, 0, now - 60);

    cout << "Видалення монстра з ім'ям 'Osama':" << endl;
    database.deleteMonster("Osama");

    cout << "Всі монстри після видалення 'Osama':" << endl;
    database.printAllMonsters();
}

void benchmarkMode() {
    const int NUM_MONSTERS = 1000000;
    const int MAX_HEALTH = 50000;
    const int MAX_ATTACK = 2000;
    const float MAX_SPECIAL_ATTACK_CHANCE = 1.0;

    MonsterDatabase database;

    srand(time(nullptr));
    for (int i = 0; i < NUM_MONSTERS; ++i) {
        Monster monster;
        monster.name = "Monster" + to_string(i);
        monster.health = rand() % MAX_HEALTH + 1;
        monster.attack = rand() % MAX_ATTACK + 1;
        monster.specialAttackChance = static_cast<float>(rand()) / RAND_MAX * MAX_SPECIAL_ATTACK_CHANCE;
        monster.specialAttackType = (rand() % 4 == 0) ? "Збільшення Удару" :
                                    (rand() % 4 == 1) ? "Повторна Атака" :
                                    (rand() % 4 == 2) ? "Лікування" : "Параліч";
        monster.dateTime = time(nullptr);
        database.addMonster(monster);
    }

    auto startTime = high_resolution_clock::now();

    for (int i = 0; i < NUM_MONSTERS; ++i) {
        Monster monster;
        monster.name = "Monster" + to_string(i);
        monster.health = rand() % MAX_HEALTH + 1;
        monster.attack = rand() % MAX_ATTACK + 1;
        monster.specialAttackChance = static_cast<float>(rand()) / RAND_MAX * MAX_SPECIAL_ATTACK_CHANCE;
        monster.specialAttackType = (rand() % 4 == 0) ? "Збільшення Удару" :
                                    (rand() % 4 == 1) ? "Повторна Атака" :
                                    (rand() % 4 == 2) ? "Лікування" : "Параліч";
        monster.dateTime = time(nullptr);
        database.addMonster(monster);
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(endTime - startTime);
    cout << "Час, необхідний для додавання " << NUM_MONSTERS << " монстрів: " << duration.count() << " мілісекунд" << endl;

    startTime = high_resolution_clock::now();

    database.searchMonsters("Monster", "Параліч", 0, 2000, 0);

    endTime = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(endTime - startTime);
    cout << "Час, необхідний для пошуку монстрів: " << duration.count() << " мілісекунд" << endl;

    startTime = high_resolution_clock::now();

    database.deleteMonster("Monster500000");

    endTime = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(endTime - startTime);
    cout << "Час, необхідний для видалення монстра: " << duration.count() << " мілісекунд" << endl;
}

int main() {


    char mode;
    cout << "Оберіть режим:" << endl;
    cout << "1. Інтерактивний режим" << endl;
    cout << "2. Демо режим" << endl;
    cout << "3. Бенчмарк" << endl;
    cout << "Введіть свій вибір: ";
    cin >> mode;

    switch (mode) {
        case '1':
            interactiveMode();
            break;
        case '2':
            demoMode();
            break;
        case '3':
            benchmarkMode();
            break;
        default:
            cout << "Вихід..." << endl;
            break;
    }

    return 0;
}
