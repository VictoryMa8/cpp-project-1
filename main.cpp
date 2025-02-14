#include <iostream>
#include <fstream>
#include <string>

// global variables
int screen_num = 0;
const int limit = 25;
std::string text[limit];
const int input_index = 2;
const int prompt_index = 25;

// node and linkedlist stuff
struct Node {
    int id;
    std::string name;
    double value;
    int quantity;
    Node* next;

    Node(int i, std::string n, double v) {
        id = i;
        name = n;
        quantity = 1;
        value = v;
        next = nullptr;
    }
};

class LinkedList {
    private:
        Node* head;

    public:
        LinkedList() {
            head = nullptr;
        }
    // adding a new node
    int addNode(int i, std::string n, int q, double v) {
        // create a new node with arguments
        Node* node = new Node(i, n, v);
        // set the quantity to q rather than 1
        node->quantity = q;
        Node* temp = head;
        // if nothing in the linked list yet, set head to it
        if (head == nullptr) {
            head = node;
            return 0;
        }
        else {
            // if linked list not empty, new node is added to the end
            while (temp->next != nullptr) {
                if (temp->name == n) {
                    temp->quantity += q;
                    return 0;
                }
                temp = temp->next;
            }
            temp->next = node;
            return 0;
        }
    }

    // removing a node by id
    void removeNode(int i) {
        if (head == nullptr) return;
        // if node to be removed is the head
        if (head->id == i) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node* temp = head;
        Node* prev = nullptr;
        while (temp != nullptr && temp->id != i) {
            prev = temp;
            temp = temp->next;
        }
        // if node not found
        if (temp == nullptr) return;
        prev->next = temp->next;
        delete temp;
    }

    // searching for a node by name
    bool searchNode(std::string n) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->name == n) {
                std::cout << "Node found with id of " << temp->id << std::endl;
                return true;
            }
            temp = temp->next;
        }
        std::cout << "Node not found" << std::endl;
        return false;
    }

    // display nodes
    void displayNodes() {
        Node* temp = head;
        while (temp != nullptr) {
            std::cout << temp->name << "[" << temp->quantity << "]" << std::endl;
            temp = temp->next;
        }
    }
};

class Character {
    public:
        std::string name;
        int health;
        int strength;
        int intelligence;
        int luck;

        Character(std::string n, int hp, int str, int itl, int lck) {
            name = n;
            health = hp;
            strength = str;
            intelligence = itl;
            luck = lck;
        }
};

class State {
    int screen_num;
    Character* protagonist;
    LinkedList linkedlist;

    public:
        State(std::istream &is) { is >> screen_num; }

        int get_screen_num() const { return screen_num; }

        void write_to(std::ostream &os) const { os << screen_num << std::endl; }

        void update(const std::string &input);

        void createProtagonist() {
            std::string name;
            int hp, str, itl, lck;
            std::cout << "Enter name: ";
            std::cin >> name;
            std::cout << "Enter health: ";
            std::cin >> hp;
            std::cout << "Enter strength: ";
            std::cin >> str;
            std::cout << "Enter intelligence: ";
            std::cin >> itl;
            std::cout << "Enter luck: ";
            std::cin >> lck;
            protagonist = new Character(name, hp, str, itl, lck);
        }

};

// read text data coming in from main.txt
void read_text_data() {
    std::ifstream fs("main.txt");
    std::string comment;
    for (int i = 0; i < limit; ++i) {
        getline(fs, text[i], '#');
        text[i].pop_back();
        getline(fs, comment);
    }
}

// display screen number and screen specific text
void display_text(const State &state, std::ostream &os) {
    int n = state.get_screen_num();
    os << "\033c               	"
 	        << "[screen " << n << "]" << std::endl
 	        << text[n];
    if (n >= limit) exit(0);
}

// update screen based on user input
void State::update(const std::string &input) {
    if (0 == screen_num) { screen_num = 1; } 
    // screen 1
    else if (screen_num == 1) { screen_num = 2; }
    else {
        try {
            int input_num = stoi(input);
            switch (screen_num) {
                // screen 2
                case 2:
                    // user inputs 1
                    if (input_num == 1) {
                        screen_num = 3;
                    }
                    // user inputs 2
                    else if (input_num == 2) { 
                        screen_num = 4; 
                    }
                    break;
                // screen 3
                case 3:
                    if (input_num == 1) {
                        screen_num = 5;
                    }
                    if (input_num == 2) {
                        screen_num = 6;
                    }
                // screen 5
                case 5:
                    if (input_num == 1) {
                        screen_num = 7;
                    }
                case 6:
                    createProtagonist();
                    screen_num = 7;
                case 7:
                    if (input_num == 1) {
                        screen_num = 8;
                    }
                    if (input_num == 2) {
                        screen_num = 9;
                    }
                default:
                    std::cout << "Invalid" << std::endl;
            }
        } catch (const std::invalid_argument& x) {
            std::cout << "Invalid" << std::endl;
        }
    }
}

int main() {
    read_text_data();
    bool just_starting = true;
    while (1) {
        std::ifstream fs1(just_starting ? "initial_state" : "curr_state");
        State state(fs1);
        fs1.close();

        std::string input;
        if (just_starting) just_starting = false;
        else getline(std::cin, input);
        state.update(input);
        display_text(state, std::cout);

        std::ofstream fs2("curr_state");
        state.write_to(fs2);
    }
}