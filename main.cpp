#include <iostream>
#include <fstream>
#include <string>

// global variables
int screen_num = 0;
const int limit = 25;
std::string text[limit];
const int input_index = 2;
const int prompt_index = 25;

struct Node {
    int id;
    std::string name;
    double value;
    Node* next;

    Node(int i, std::string n, double v) {
        id = i;
        name = n;
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
    void addNode(int i, std::string n, double v) {
        // create a new book with arguments
        Node* node = new Node(i, n, v);
        Node* temp = head;
        // if nothing in the linked list yet, set head to it
        if (head == NULL) {
            head = node;
        }
        else {
            // if linked list not empty, new node is added to the end
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = node;
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
};

class State {
    int screen_num;

    public:
        State(std::istream &is) { is >> screen_num; }

        int get_screen_num() const { return screen_num; }

        void write_to(std::ostream &os) const { os << screen_num << std::endl; }

        void update(const std::string &input);

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

// used to be called update_screen_num
void State::update(const std::string &input) {
    if (0 == screen_num) { screen_num = 1; } 
    else if (screen_num == 1) { screen_num = 2; }
    else {
      int input_num = stoi(input);
      // screen 2
      if (input_num == 1) {
        screen_num = 3;
      } else if (input_num == 2) { 
        screen_num = 4; }
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