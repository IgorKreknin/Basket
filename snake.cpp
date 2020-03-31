#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <thread>

using namespace std;

char direction = 'r';
bool samokusb = false;
int food[2] = {0, 2};
bool foodFinded = false;
int score = 0;
int sleepTime = 100200;

class segment{
    public:
    segment *next;
    int x;
    int y;
};

class snake{
    public:
        snake(int x, int y){
            segment *newHead = new segment();
            newHead->x = x;
            newHead->y = y;
            newHead->next = nullptr;
            this->head = newHead;
            this->tail = this->head;
        }

        void addSegment(){
            this->newSegment = true;
            segment *newSegment = new segment();
            newSegment->x = this->tail->x;
            newSegment->y = this->tail->y;
            newSegment->next = nullptr;
            this->tail->next = newSegment;
            this->tail = newSegment;
        }

        void passBack(){
            segment *currentNode = this->head;
            int storedX = this->head->x, storedY = this->head->y;
            while((currentNode != nullptr && !this->newSegment) || (this->newSegment && currentNode->next != nullptr)){
                cout << currentNode << endl;
                int localX = currentNode->x, localY = currentNode->y;
                currentNode->x = storedX;
                currentNode->y = storedY;
                storedX = localX;
                storedY = localY;
                currentNode = currentNode->next;
            }
            this->newSegment = false;
        }

        void moveHead(char direction){
            this->passBack();
            if (direction == 't'){
                this->head->y--;
            }else if (direction == 'b'){
                this->head->y++;
            }else if (direction == 'l'){
                this->head->x--;
            }else{
                this->head->x++;
            }
        }

        segment* getHead(){
            return this->head;
        }

    private:
        segment *head;
        segment *tail;
        bool newSegment = false;
};

class Field{
    public:
        Field(int sizeX, int sizeY){
            this->sizeX = sizeX;
            this->sizeY = sizeY;
            for (int i = 0; i < sizeY; i++){
                for (int j = 0; j < sizeX; j++){
                    this->field[i][j] = ' ';
                }
            }
        }

        void refreshField(segment *head){
            for (int i = 0; i < sizeY; i++){
                for (int j = 0; j < sizeX; j++){
                    this->field[i][j] = ' ';
                }
            }

            if (head->y == food[0] && head->x == food[1]){
                srand(time(0));
                food[0] = rand() % sizeY;
                food[1] = rand() % sizeX;
                foodFinded = true;
            }

            this->field[food[0]][food[1]] = '*';

            while (head != nullptr) {
                if (this->field[head->y][head->x] == 's' && !foodFinded) samokusb = true;
                this->field[head->y][head->x] = 's';
                head = head->next;
            }
        }

        void drowField(){
            system("clear");
            std::cout << "SCORE: " << score << "\n\n";
            for (int i = 0; i < this->sizeX + 2; i++) cout << "#";
            cout << endl;
            for (int i = 0; i < this->sizeY; i++){
                cout << "#";
                for (int j = 0; j  < this->sizeX; j++){
                    cout << this->field[i][j];
                }
                cout << "#" << endl;
            }
            for (int i = 0; i < this->sizeX + 2; i++) cout << "#";
            cout << endl;
        }

    private:
        char field[1000][1000];
        int sizeX, sizeY;
};

int getch()
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void keyReaderLoop(){
    while(true){
        int key1 = getch();
        if (key1 != 27) continue;
        int key2 = getch();
        if (key2 != 91) continue;

        int step = getch();
        if (step == 65){
            if (direction != 'b') direction = 't';
        }else if (step == 66){
            if (direction != 't') direction = 'b';
        }else if (step ==68){
            if (direction != 'r') direction = 'l';
        }else if (step == 67){
            if (direction != 'l') direction = 'r';
        }else{
            continue;
        }
    }
}



int main(){

    int y, x;

    std::cout << "Field parametrs:\nx=";
    std::cin >> x;
    std::cout << "y=";
    std::cin >> y;

    Field newField = *(new Field(x, y));
    snake *newSnake = new snake(0, 0);

    newField.refreshField(newSnake->getHead());

    newField.drowField();

    int ind = 0;

    auto krl = std::thread(keyReaderLoop);

    while(true){
        usleep(sleepTime);

        newSnake->moveHead(direction);
        if (samokusb){
            system("clear");
            std::cout << "GAME OVER\nSCORE: " << score;
            return 0; 
        }

        newField.refreshField(newSnake->getHead());

        if (foodFinded){
            sleepTime-=500;
            score++;
            newSnake->addSegment();
            foodFinded = false;
        }

        newField.drowField();
    }

    return 0;
}