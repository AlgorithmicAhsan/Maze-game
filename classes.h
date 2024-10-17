#include <iostream>
#include <ncurses.h>
using namespace std;

struct Node
{
    Node *up, *down, *right, *left;
    char entity;
    int x, y;
    Node(int x1 = 0, int y1 = 0) : up(nullptr), down(nullptr), right(nullptr), left(nullptr), entity('.'), x(x1), y(y1) {}
};

class Player {
public:
    int x, y;
    Player(int startX, int startY) : x(startX), y(startY) {}
};

class grid
{
    Node *first;
    int size;
    Player* player;
    
public:
    bool isPaused = false;
    grid(int s = 10) : first(nullptr), size(s), player(nullptr) {}

    void gridCreation()
    {
        first = new Node(0, 0);
        first->entity = '#';
        Node *curr = first;
        Node* rowStart = first;

        for (int i = 1; i < size; ++i) {
            curr->right = new Node(i, 0);
            curr->right->left = curr;
            curr = curr->right;
            curr->entity = '#';
        }

        for (int i = 1; i < size; ++i) {
            Node* newRowStart = new Node(0, i);
            newRowStart->up = rowStart;
            rowStart->down = newRowStart;

            curr = newRowStart;
            Node* prevRowCurr = rowStart;

            for (int j = 1; j < size; ++j) {
                curr->right = new Node(j, i);
                curr->right->left = curr;
                curr->right->up = prevRowCurr->right;
                prevRowCurr->right->down = curr->right;

                curr = curr->right;
                prevRowCurr = prevRowCurr->right;
                if (j==size-1 || i==size-1)
                curr->entity = '#';
            }
            rowStart = newRowStart;
        }

        // Set boundaries
        // Node* current = first;
        // for (int i = 0; i < size; i++)
        // {
        //     for (int j = 0; j < size; j++)
        //     {
        //         current->entity = '#';
        //         current->right->entity = '#';
        //         current->down->entity = '#';
        //         current->right->down->entity = '#';
        //         current = current->right;
        //     }
        // }
        
        // Initialize player
        player = new Player(1, 1);
    }

    void movePlayer(char direction) {
        int newX = player->x;
        int newY = player->y;

        switch(direction) {
            case 'w': newY--; break;
            case 's': newY++; break;
            case 'a': newX--; break;
            case 'd': newX++; break;
        }

        // Check if new position is valid
        if (newX > 0 && newX < size - 1 && newY > 0 && newY < size - 1) {
            player->x = newX;
            player->y = newY;
        }
    }

    Node* getNodeAt(int x, int y) {

    Node* current = first;
    for (int i = 0; i < y; i++) {
        current = current->down;
    }
    for (int i = 0; i < x; i++) {
        current = current->right;
    }
    return current;
}

    void display()
    {
        clear();
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (x == player->x && y == player->y) {
                mvaddch(y, x*2, 'P');
            } else {
                Node* node = getNodeAt(x, y);
                mvaddch(y, x*2, node->entity);
            }
        }
    }
    refresh();
    }
    int showMenu() {
    clear();
    mvprintw(5, 10, "MAZE RUNNER");
    mvprintw(7, 10, "1. Start Game");
    mvprintw(8, 10, "2. Exit");
    refresh();
    
    int choice;
    while(true) {
        choice = getch();
        if(choice == '1' || choice == '2') 
            return choice - '0';
    }
}

int chooseDifficulty() {
    clear();
    mvprintw(5, 10, "Choose Difficulty:");
    mvprintw(7, 10, "1. Easy");
    mvprintw(8, 10, "2. Medium");
    mvprintw(9, 10, "3. Hard");
    refresh();
    
    int choice;
    while(true) {
        choice = getch();
        if(choice >= '1' && choice <= '3') 
            return choice - '0';
    }
}
};

class StackNode
{
public:
    string url;
    StackNode *next;
    StackNode(string url) : next(nullptr)
    {
        this->url = url;
    }
};

class Stack
{
    StackNode *head;
public:
    Stack() : head(NULL) {}
    ~Stack()
    {
        removeAll();
    }
    void push(string link)
    {
        if (head)
        {
            StackNode* newNode = new StackNode(link);
            newNode->next = head;
            head = newNode;
            return;
        }
        head = new StackNode(link);
    }
    string pop()
    {
        if (head)
        {
            string newUrl = head->url;
            StackNode *temp = head;
            head = head->next;
            delete temp;
            return newUrl;
        }
        return "Non existent";
    }
    bool isEmpty()
    {
        return head == nullptr;
    }
    void removeAll()
    {
        StackNode* temp = head;
        while (head)
        {
            StackNode* next = temp->next;
            delete temp;
            temp = next;
        }
        temp = nullptr;
    }
};