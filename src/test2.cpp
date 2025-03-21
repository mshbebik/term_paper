#include <iostream>
#include <vector>
using namespace std;

class Poss {
public:
    int x;
    int y;
    Poss(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class Board {
public:
    int count = 0;
    int bd[10][10] = {0};
    vector<vector<Poss>> routes; // Для хранения всех маршрутов

    void print_board();
    void move(Poss start, Poss prev, int number, int nb_count, vector<Poss> currentRoute);
    void print_routes();
};

void Board::print_board() {
    cout << "   ";
    for (int i = 0; i < 10; i++) {
        cout << i + 1 << " ";
    }
    cout << endl << endl;
    for (int i = 0; i < 10; i++) {
        if (i + 1 == 10) cout << i + 1 << " ";
        else { cout << i + 1 << "  "; }

        for (int a = 0; a < 10; a++) {
            cout << bd[i][a] << " ";
        }
        cout << endl;
    }
}

void Board::move(Poss start, Poss prev, int number, int nb_count, vector<Poss> currentRoute) {
    // Проверка выхода за границы доски
    if (start.x < 0 || start.x >= 10 || start.y < 0 || start.y >= 10) {
        return;
    }

    // Проверка, чтобы не вернуться в предыдущую позицию
    if (start.x == prev.x && start.y == prev.y) {
        return;
    }

    // Если клетка уже заполнена, пропускаем
    if (this->bd[start.x][start.y] != 0) {
        return;
    }

    // Добавляем текущую позицию в маршрут
    currentRoute.push_back(start);

    // Заполняем клетку
    this->bd[start.x][start.y] = number;

    // Если достигли нужной длины пути, увеличиваем счетчик и сохраняем маршрут
    if (nb_count == number) {
        count++;
        routes.push_back(currentRoute); // Сохраняем маршрут
        this->bd[start.x][start.y] = 0; // Возвращаем клетку в исходное состояние
        return;
    }

    // Рекурсивно вызываем move для всех соседних клеток
    Poss ps[4] = { Poss(start.x - 1, start.y), Poss(start.x + 1, start.y), Poss(start.x, start.y - 1), Poss(start.x, start.y + 1) };
    for (int i = 0; i < 4; i++) {
        move(ps[i], start, number, nb_count + 1, currentRoute);
    }

    // Возвращаем клетку в исходное состояние после завершения рекурсии
    this->bd[start.x][start.y] = 0;
}

void Board::print_routes() {
    cout << "Total routes: " << routes.size() << endl;
    for (size_t i = 0; i < routes.size(); i++) {
        cout << "Route " << i + 1 << ": ";
        for (size_t j = 0; j < routes[i].size(); j++) {
            cout << "(" << routes[i][j].x + 1 << ", " << routes[i][j].y + 1 << ") ";
        }
        cout << endl;
    }
}

int main(void) {
    Board nw;
    nw.bd[5][5] = 4; // Начальная позиция (3, 3) и число 4
    nw.print_board();

    vector<Poss> initialRoute; // Начальный маршрут
    nw.move(Poss(5, 5), Poss(-1, -1), 4, 1, initialRoute);

    nw.print_board();
    cout << "Total variants: " << nw.count << endl;

    // Выводим все маршруты
    nw.print_routes();

    return 0;
}
