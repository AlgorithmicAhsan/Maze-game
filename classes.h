#include <iostream>
#include <ncurses.h>
#include <ctime>

class SNode
{
    public:
    int x, y;
    SNode* next;
    SNode(int x1, int y1) : x(x1), y(y1), next(nullptr){}
};
class Stack
{
    SNode* top;
    int size;
    
public:
    Stack() : top(nullptr), size(0){}
    
    ~Stack()
    {
        while (top != nullptr)
        {
            SNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
    void push(int x, int y)
    {
        SNode* newNode = new SNode(x, y);
        newNode->next = top;
        top = newNode;
        size++;
    }
    
    bool pop(int& x, int& y)
    {
        if (top == nullptr) return false;
        
        x = top->x;
        y = top->y;
        SNode* temp = top;
        top = top->next;
        delete temp;
        size--;
        return true;
    }
    bool isEmpty()
    {
        return top == nullptr;
    }
    int getSize(){ return size; }
};
// <----------------------------------------------------------------------------------------------------------->
class CNode
{
    public:
    int x, y;
    CNode* next;
    CNode(int x1, int y1) : x(x1), y(y1), next(nullptr){}
};

class List
{
    CNode* head;
    int count;
    
public:
    List() : head(nullptr), count(0){}
    
    ~List()
    {
        while (head != nullptr)
        {
            CNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void addCoin(int x, int y)
    {
        CNode* newNode = new CNode(x, y);
        newNode->next = head;
        head = newNode;
        count++;
    }
    void displayCollection()
    {
        int l = 31;
        CNode* current = head;
        while (current != nullptr)
        {
            mvprintw(l = l + 1 , 2, "Collected Coin at (%d,%d)", current->x, current->y);
            current = current->next;
        }
    }
    int getCount()
    { return count;
    }
};
//<----------------------------------------------------------------------------------------------------------->
struct Node
{
    Node *up, *down, *right, *left;
    char entity;
    bool visible;
    int x, y;
    Node(int x1 = 0, int y1 = 0) : up(nullptr), down(nullptr), right(nullptr), left(nullptr), entity('.'), visible(true), x(x1), y(y1) {}
};

class Grid
{
private:
    Node* first;
    int size;
    Node* playerNode;
    Node* keyNode;
    Node* doorNode;
    Stack moveHistory;
    List coins;
    int playerX, playerY;
    bool hasKey;
    int remMoves;
    int undoC;
    int maxUndo;
    int score;
    time_t lastCoinUpdate;
    int prevDist=0, currDist=0;
    
    void RandEmpPosition(int& x, int& y)
    {
        do
        {
         x = rand() % (size - 2) + 1;
         y = rand() % (size - 2) + 1;
        } while (getNodeAt(x, y)->entity != '.');
    }

    void updateCoins()
    {
        time_t currentT = time(nullptr);
        if (currentT - lastCoinUpdate >= 5)
        {
            for (int y = 0; y < size; y++)
            {
                for (int x = 0; x < size; x++)
                {
                    Node* node = getNodeAt(x, y);
                    if (node && node->entity == 'C')
                    {
                        node->entity = '.';
                    }
                }
            }
            CoinsAndBombs();
            lastCoinUpdate = currentT;
        }
    }   
public:
    Grid(int s = 10) : first(nullptr), size(s), playerNode(nullptr), keyNode(nullptr), doorNode(nullptr), hasKey(false), score(0), lastCoinUpdate(time(nullptr))
    {
        createGrid();
    }
    ~Grid()
    {
        Node* currentRow = first;
        while (currentRow != nullptr)
        {
            Node* current = currentRow;
            currentRow = currentRow->down;
            while (current != nullptr)
            {
                Node* temp = current;
                current = current->right;
                delete temp;
            }
        }
    }
    void createGrid()
    {
        first = new Node(0, 0);
        first->entity = '#';
        Node* curr = first;
        Node* rowStart = first;
        
        for (int i = 1; i < size; i++)
        {
            curr->right = new Node(i, 0);
            curr->right->left = curr;
            curr = curr->right;
            curr->entity = '#';
        }
        
        for (int i = 1; i < size; i++)
        {
            Node* newRowStart = new Node(0, i);
            newRowStart->up = rowStart;
            rowStart->down = newRowStart;
            newRowStart->entity = '#';
            curr = newRowStart;
            Node* prevRowCurr = rowStart;
            
            for (int j = 1; j < size; j++)
            {
                curr->right = new Node(j, i);
                curr->right->left = curr;
                curr->right->up = prevRowCurr->right;
                prevRowCurr->right->down = curr->right;
                
                curr = curr->right;
                prevRowCurr = prevRowCurr->right;
                if (j == size-1 || i == size-1)
                {
                    curr->entity = '#';
                }
            }
            rowStart = newRowStart;
        }
    }
    void StartGame(int d)
    {
        switch (d)
        {
            case 1:
                maxUndo = 6;
                break;
            case 2:
                maxUndo = 4;
                break;
            case 3:
                maxUndo = 1;
                break;
        }
        undoC = maxUndo;
        int px, py;
        RandEmpPosition(px, py);
        playerX = px;
        playerY = py;
        playerNode = getNodeAt(px, py);
        playerNode->entity = 'P';

        int kx, ky;
        RandEmpPosition(kx, ky);
        keyNode = getNodeAt(kx, ky);
        keyNode->entity = 'K';
        keyNode->visible = false;

        int dx, dy;
        RandEmpPosition(dx, dy);
        doorNode = getNodeAt(dx, dy);
        doorNode->entity = 'D';
        doorNode->visible = false;

        int keyD = Distance(px, py, kx, ky);
        int doorD = Distance(kx, ky, dx, dy);
        remMoves = keyD + doorD;

        if (d == 1)
            remMoves += 6;
        else if (d == 2)
        {
            remMoves += 2;
        }
        CoinsAndBombs();
    }
    
    Node* getNodeAt(int x, int y)
    {
        Node* curr = first;
        for (int i = 0; i < y; i++)
        {
            if (curr == nullptr)
                return nullptr;
            curr = curr->down;
        }
        for (int i = 0; i < x; i++)
        {
            if (curr == nullptr)
                return nullptr;
            curr = curr->right;
        }
        return curr;
    }
    int Distance(int x1, int y1, int x2, int y2)
    {
        return abs(x2 - x1) + abs(y2 - y1);
    }
    void CoinsAndBombs()
    {
        for (int i = 0; i < 3; i++)
        {
            int x, y;
            RandEmpPosition(x, y);
            getNodeAt(x, y)->entity = 'C';
        }
        for (int i = 0; i < 3; i++)
        {
            int x, y;
            RandEmpPosition(x, y);
            getNodeAt(x, y)->entity = 'B';
        }
    }
    int movePlayer(char direction)
    {
        updateCoins();
        if (remMoves <= 0) return false;
        int newX = playerX;
        int newY = playerY;
        switch (direction)
        {
            case 'w': newY--; break;
            case 's': newY++; break;
            case 'a': newX--; break;
            case 'd': newX++; break;
            default: return false;
        }
        Node* newNode = getNodeAt(newX, newY);
        if (!newNode || newNode->entity == '#')
        return false;

        if (!hasKey)
        {
            prevDist = Distance(newX, newY, keyNode->x, keyNode->y);
            currDist = Distance(playerX, playerY, keyNode->x, keyNode->y);
        }
        moveHistory.push(playerX, playerY);
        playerNode->entity = '.';
        playerX = newX;
        playerY = newY;
        if (newNode->entity == 'C')
        {
            coins.addCoin(newX, newY);
            score += 2;
            undoC++;
        }
        else if (newNode->entity == 'B')
        {
            return 2;
        }
        else if (newNode->entity == 'K')
        {
            hasKey = true;
            keyNode->visible = true;
        }
        else if (newNode->entity == 'D' && hasKey)
        {
            doorNode->visible = true;
            return true;
        }
        newNode->entity = 'P';
        playerNode = newNode;
        remMoves--;
        return true;
    }
    bool undoMove()
    {
        if (undoC <= 0 || moveHistory.isEmpty())
            return false;
        int prevX, prevY;
        if (!moveHistory.pop(prevX, prevY))
            return false;
        playerNode->entity = '.';
        playerX = prevX;
        playerY = prevY;
        playerNode = getNodeAt(prevX, prevY);
        playerNode->entity = 'P';
        undoC--;
        remMoves++;
        return true;
    }
    void display()
    {
        clear();
        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                Node* node = getNodeAt(x, y);
                if (!node)
                    continue;
                
                char displayChar = node->entity;
                if (!node->visible && (displayChar == 'K' || displayChar == 'D')) {
                    displayChar = '.';
                }
                mvaddch(y, x * 2, displayChar);
            }
        }
        mvprintw(size + 1, 0, "Moves left: %d", remMoves);
        mvprintw(size + 2, 0, "Undo moves: %d", undoC);
        mvprintw(size + 3, 0, "Score: %d", score);
        if (hasKey)
            mvprintw(size + 4, 0, "Key collected: Yes");
        else
            mvprintw(size + 4, 0, "Key collected: No");
        mvprintw(size+5, 0, "hint: %s", currDist<prevDist ? "Closer": "Farther");
        refresh();
    }
    
    void revealAll()
    {
        keyNode->visible = true;
        doorNode->visible = true;
        display();
        coins.displayCollection();
    }
    
    int calculateFinalScore()
    {
        return score + remMoves; 
    }
    bool hasRemainingMoves()
    {
        return remMoves > 0;
    }
    int getScore()
    {
        return score;
    }
    bool hasCollectedKey()
    {
        return hasKey;
    }
    bool hasReachedDoor()
    {
        return playerNode == doorNode;
    }
};

//<-------------------------------------------------------------------------------------------------->
class Game
{
private:
    Grid* grid;
    int diff;
    bool gameOver;
    int size;
    void displayWinMessage()
    {
        clear();
        mvprintw(size, size, "We got a winner");
        mvprintw(size+ 1, size, "Score: %d", grid->calculateFinalScore());
        refresh();
        napms(2000);
    }
    void displayLoseMessage()
    {
        clear();
        mvprintw(size, size, "Game Over");
        if (!grid->hasRemainingMoves())
        {
            mvprintw(size + 1, size, "Out of moves");
        }
        else
        {
            mvprintw(size + 1, size, "Hit a bomb");
        }
        mvprintw(size + 2, size, "Score: %d", grid->getScore());
        refresh();
        napms(2000);
    }
public:
    Game() : grid(nullptr), diff(1), gameOver(false), size(0)
    {
        srand(time(nullptr));
    }
    ~Game()
    {
        if (grid)
            delete grid;
        endwin();
    }
    int chooseDifficulty()
    {
        clear();
        mvprintw(0, 0, "Choose difficulty level:");
        mvprintw(1, 0, "1. Easy");
        mvprintw(2, 0, "2. Medium");
        mvprintw(3, 0, "3. Hard");
        refresh();
        
        int choice;
        while (true)
        {
            choice = getch() - '0';
            if (choice >= 1 && choice <= 3)
                break;
        }
        return choice;
    }
    void initialize()
    {
        initscr();
        noecho();
        curs_set(0);
        keypad(stdscr, TRUE);
        
        diff = chooseDifficulty();
        
        switch (diff)
        {
            case 1:
                size = 10;
                break;
            case 2:
                size = 15;
                break;
            case 3:
                size = 20;
                break;
            default:
                size = 10;
        }
        grid = new Grid(size);
        grid->StartGame(diff);
        gameOver = false;
    }
    void play()
    {
        while (!gameOver)
        {
            grid->display();
            int ch = getch();
            int vMove = false;
            if (ch == 'w' || ch =='s' || ch =='a' || ch == 'd')
            {
                vMove = grid->movePlayer(ch);
                if (vMove==1 && grid->hasCollectedKey() && grid->hasReachedDoor())
                {
                    gameOver = true;
                    displayWinMessage();
                }
                else if (vMove==2)
                {
                    gameOver= true;
                    displayLoseMessage();
                }
            }
            else if (ch=='u')
            {
                grid->undoMove();
            }
            else if(ch=='q')
            {
                gameOver = true;
            }
            if (!grid->hasRemainingMoves())
            {
                gameOver = true;
                displayLoseMessage();
            }
        }
        grid->revealAll();
        mvprintw(size + 6, 0, "Press any key to exit.");
        refresh();
        getch();
    }
};