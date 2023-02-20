#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <ctime>
#include <random>

using namespace std;

//sx, sy - snake x, y
//ex, ey - eat x, y
//dir - direction
//lr - liczba ruchow funkcji setDirection i makeStep

int ex = 0, ey = 0, sx = 0, sy = 0;
int dir = 0;
int lrd = 0, lrs = 0;
bool gameOver = false;

class Snake;

struct position {
    int x;
    int y;
};


// w wektorze snake zapisane sa pola ktore zawiera waz
vector<position> snake;

class Game {
    friend Snake;
    int width, length;
    string** fields;
    int points = 0;
public:

    //wywolujac metode Game wprowadzane sa wymiary planszy
    //KONSTRUKTOR
    Game(int width, int length) {

        this->width = width;
        this->length = length;

        //tworzenie planszy, domyslnie pola sa puste
        fields = new string * [length];
        for (int iter = 0; iter < length; iter++) {
            fields[iter] = new string[width];
            for (int jiter = 0; jiter < width; jiter++) {
                fields[iter][jiter] = " ";
            }
        }

        // domyslnie waz generuje sie na srodku
        // sx,sy to obecne wspolrzedne glowy weza
        sx = width / 2;
        sy = length / 2;

        //tworze obiekt z wymiarami x y, w ktorym bede zapisywal wspoldrzedne
        position tmp;

        //dodanie do wektora pierwszego elementu weza
        tmp.x = sx;
        tmp.y = sy;
        snake.push_back(tmp);
    }

    void moveUp() {
        sy = sy - 1;
    }
    void moveDown() {
        sy = sy + 1;
    }
    void moveLeft() {
        sx = sx - 1;
    }
    void moveRight() {
        sx = sx + 1;
    }
    void step(Snake& s);
    void endCheck(Snake& s);
    void show();
    void addPoints() {
        points += 1;
    }
    int getPoints() {
        return points;
    }


    //pobieranie ruchu z klawiatury - _kbhit
    void keyboardInput() {
        if (_kbhit())
        {
            switch (_getch())
            {
            case 'w':
                dir = 0;
                break;
            case 's':
                dir = 1;
                break;
            case 'a':
                dir = 2;
                break;
            case 'd':
                dir = 3;
                break;
            }

            //ustawiony kierunek przekazywany jest do funkcji setDirection
            setDirection(dir);
        }
    }

    void setDirection(int direction) {

        //w zaleznosci od kierunku wykonywany jest ruch w dana strone
        switch (direction) {
        case (0):
            moveUp();
            break;
        case (1):
            moveDown();
            break;
        case (2):
            moveLeft();
            break;
        case (3):
            moveRight();
            break;

        }
        //zapamietywany jest kierunek i zwiekszany licznik zmian
        dir = direction;
        lrd += 1;
    }
};

class Snake {
    friend Game;
public:

    void makeStep(Game& g) {

        //jezeli kierunek nie byl zmieniony to ruch wykona sie w tym kierunku co poprzedni
        if (lrs > lrd) {
            g.setDirection(dir);
            lrs = 1;
            lrd = 1;
        }
        //pierwszy ruch wykona sie zgodnie z ustawionym stanem poczatkowym dir
        else if (lrs == 0 && lrd == 0) {
            g.setDirection(dir);
            lrs = 1;
            lrd = 1;
        }

        position tmp;
        tmp.x = sx;
        tmp.y = sy;

        //sprawdzenie czy waz dotarl do jedzenia
        //jezeli tak - generuje sie nowe - funkcja eatCheck - i waz wydluza sie o to pole
        //jezeli nie, idzie dalej - usuwa sie koncowa czesc weza, dodaje nowa z przodu
        
        if ((g.fields[tmp.y][tmp.x]) == ((g.fields[ey][ex]))) {
            eatCheck(g);
            snake.insert(snake.begin(), tmp);
        }

        else {
            snake.pop_back();
            snake.insert(snake.begin(), tmp);
        }
        lrs += 1;
    }

    void endGame(Game& g) {

        if (sy == 0 && dir == 0) {
            gameOver = true;
        }

        if (sy == g.length-1 && dir == 1) {
            gameOver = true;
        }


        if (sx == 0 && dir == 2) {
            gameOver = true;
        }
        
        if (sx == g.width-1 && dir == 3) {
            gameOver = true;
        }
        
    }

    void eatCheck(Game& g) {

        //sprawdzenie czy glowa weza jest na tym samym polu, gdzie bylo jedzenie
        //jezeli tak, to dodaje sie punkt i tworzy nowe pole z jedzeniem

        if (ey == sy && ex == sx) {
            g.addPoints();
        }

        position temp;

        temp.x = rand() % (g.width);
        temp.y = rand() % (g.length);

        ex = temp.x;
        ey = temp.y;
    }
};


void Game::step(Snake& s) {
    s.makeStep(*this);
}

void Game::endCheck(Snake& s) {
    s.endGame(*this);
}

void Game::show() {

    //rysowanie graficzne planszy

    for (int p = 0; p < snake.size(); p++)
        fields[(snake[p]).y][(snake[p]).x] = "X";

    fields[ey][ex] = "*";

    for (int iter = 0; iter < length; iter++) {
        for (int jiter = 0; jiter < width; jiter++) {
            cout << fields[iter][jiter] << "|";
        }
        cout << "\n";
    }

    for (int p = 0; p < snake.size(); p++)
        fields[(snake[p]).y][(snake[p]).x] = " ";
}


void bartek(Snake& s) {
    Game* g = new Game(10, 10);
    s.eatCheck(*g);
    g->show();
    while (1) {
        Sleep(200);
        //czysci konsole
        system("CLS");
        g->keyboardInput();
        g->endCheck(s);
        if (gameOver) {
            g->show();
            cout << endl <<" ******* KONIEC GRY! *******" << endl;
            cout <<" **** ZDOBYTE PUNKTY: " << g->getPoints()<< " ****" << endl;
            break;
        }
        g->step(s);
        g->show();
        cout << "Punkty:" << g->getPoints() << endl;
    }
}

int main() {

    Snake* s = new Snake;
    bartek(*s);

    return 0;
}
