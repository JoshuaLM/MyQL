#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;



// new data sets are represented by this
class ListBase {
    public: 
        vector<int> base;
        string name;
        vector<string> columnNames;
};

class GraphBase {
public:
    vector<string> rowNames;
    vector<string> columnNames;
    vector<vector<int>> base;
    string name;

};

//Pre-functions
void MyQL();
void help();
void addNewBase (vector<string> newName, vector<ListBase*>* g1);
void display    (vector<ListBase*>* g1);
void printBase  (ListBase d);
void printBase  (GraphBase d);
void load       (vector<string> fileName, vector<ListBase*>* g1);
void add        (vector<string> g2, vector<ListBase*>* g1);
void addToBase  (string thing, string dataBaseName, vector<ListBase*>* g1);
void addToBase  (vector<string> commands, vector<ListBase*>* g1);
void getInput(string line, vector<string>* commands);
void save       (vector<string> commands, vector<ListBase*>* g1);
void showBase   (vector<string> commands, vector<ListBase*>* g1);
void sortBase   (vector<string> commands, vector<ListBase*>* g1);
void clearBase  (vector<string> commands, vector<ListBase*>* g);
void deleteItem (vector<string> commands, vector<ListBase*>* g);
void removeBase (vector<string> commands, vector<ListBase*>* g);
void insertItem (vector<string> commands, vector<ListBase*>* g1);

int specialSTOI(string item);

// Current list of Commands
enum class Cmds { Invalid, AddNewBase, AddToBase, Display, Clear, Delete, Insert, Remove, Help, Load, Save, Show,  Sort };

// Current converted inputs
Cmds convertCommand(string input) {
    map<string, Cmds> u = {
        {"add",     Cmds::AddToBase},
        {"clear",   Cmds::Clear},
        {"delete",  Cmds::Delete},
        {"display", Cmds::Display},
        {"insert",  Cmds::Insert},
        {"help",    Cmds::Help},
        {"load",    Cmds::Load},
        {"new",     Cmds::AddNewBase},
        {"remove",  Cmds::Remove},
        {"save",    Cmds::Save},
        {"show",    Cmds::Show},
        {"sort",    Cmds::Sort}
        
        
    };
    return u[input];
}

// The purpose of this program is to provide a shell database/query function in the command prompt

int main()
{
    std::cout << "Type command to begin: (type help for list of commands)\n";
    MyQL();
    return 0;
}

void MyQL() {
    vector<ListBase*> g1;
    vector<string> commands;
    string line = "";
    
     
    while (line != "exit") {
        getline(cin, line);  // gets command line argument
        getInput(line, &commands); // converts input into commands
        
        if (!commands.empty()) { // Command directory

            switch (convertCommand(commands[0])) {
                case Cmds::AddNewBase:  addNewBase(commands, &g1);  break;
                case Cmds::AddToBase:   add(commands, &g1);         break;
                case Cmds::Clear:       clearBase(commands, &g1);   break;
                case Cmds::Delete:      deleteItem(commands, &g1);  break;
                case Cmds::Display:     display(&g1);               break;
                case Cmds::Insert:      insertItem(commands, &g1);  break;
                case Cmds::Load:        load(commands, &g1);        break;
                case Cmds::Help:        help();                     break;
                case Cmds::Remove:      removeBase(commands, &g1);  break;
                case Cmds::Save:        save(commands, &g1);        break;
                case Cmds::Show:        showBase(commands, &g1);    break;
                
                default: cout << "Unknown command: " << commands[0] << "\n";

            }

        }
        commands.clear();
    }
}


void getInput(string line, vector<string>* commands) {
    

    // Produces a char array from a string
    char* char_arr; char_arr = &line[0]; char* pch; char* s = NULL;

    pch = strtok_s(char_arr, " ,", &s);
    string command;
    while (pch != NULL)
    {
        // printf("%s\n", pch);

        // Get a word, convert to lowercase
        for (int i = 0; pch[i] != '\0'; i++) {
            command.push_back(tolower(pch[i]));
        }
        commands->push_back(command);
        command.clear();

        pch = strtok_s(NULL, " ,", &s);
    }

}
void insertItem(vector<string> commands, vector<ListBase*>* g1) {
    if (commands.size() < 4) {
        cout << "Not enough arguments for insert.\n";
        return;
    }
    string name = commands[1];
    int item = specialSTOI(commands[2]);
    int position = specialSTOI(commands[3]);
    for (ListBase* b : *g1) {
        if (b->name == name) {
            vector<int> copyBase = b->base;
            b->base.clear();
            for (int i = 0; i < copyBase.size(); i++) {
                if (i == position) {
                    b->base.push_back(item);
                }
                b->base.push_back(copyBase[i]);
            }
        }
        printBase(*b);
    }

}

void sortBase(vector<string> commands, vector<ListBase*>* g1) {
    if (commands.size() < 2) {
        cout << "Not enough arguments for sort.\n";
        return;
    }
    string name = commands[1];
    for (ListBase* d : *g1) {
        if (d->name == name) {

        }
    }
}

void addNewBase(vector<string> commands, vector<ListBase*> *g1) {
    /*Format Checker*/
    if (commands.size() < 2) {
        cout << "Lacking name, no new database made\n";
        return;
    }

    string newName = commands[1];
    /*Duplicate Checker*/
    for (ListBase* b : *g1) {
        if (b->name == newName) {
            cout << newName << " already exists\n";
            return;
        }
    }
    /*Action*/
    ListBase* newBase = new ListBase();
    newBase->name = newName;
    g1->push_back(newBase);
    cout << "Added new database called: " << newName << "\n";

    for (int i = 2; i < commands.size(); i++) {
        addToBase(commands[i], newName, g1);
    }
    
}

void load(vector<string> commands, vector<ListBase*>* g1) {
    /*Format Checker*/
    if (commands.size() < 2) {
        cout << "Lacking file name, no files loaded\n";
        return;
    }

    string line;
    ifstream myfile(commands[1]);
    if (myfile.is_open())
    {
        string r;
        vector<string> rr;
        while (getline(myfile, line))
        {
            char* char_arr; char_arr = &line[0]; char* pch; char* s = NULL; pch = strtok_s(char_arr, ",", &s);\
            
                while (pch != NULL)
            {
                // Get a word, convert to lowercase
                for (int i = 0; pch[i] != '\0'; i++) {
                    r.push_back(tolower(pch[i]));
                }
                rr.push_back(r);
                r.clear();

                pch = strtok_s(NULL, ",", &s);
            }
            
            bool named = false;
            for (string element : rr) {
                if (!named) {
                    vector<string> s;
                    s.push_back(element);
                    addNewBase(s, g1);
                    named = true;
                }
                else {
                    addToBase(element, rr[0], g1);
                }
            }
            named = false;
            rr.clear();
        }
        myfile.close();
    }

    else cout << "Unable to open file \n";
}

void add(vector<string> g2, vector<ListBase*>* g1) {
    bool exists = false;
    ListBase* g = new ListBase();
    for (ListBase* d : *g1) {
        if (d->name == g2[1]) {
            exists = true;
            g = d;
        }
    }

    for (int i = 2; i < g2.size(); i++) {
        addToBase(g2[i], g2[1], g1);
    }

    if (exists) {
        printBase(*g);
    }
    else {
        std::cout << "No database with name " << g2[1] << "\n";
    }
}

void addToBase(vector<string> commands, vector<ListBase*>* g1) {
    if (commands.size() < 3) {
        cout << "Not enough arguments for add.\n";
        return;
    }
    addToBase(commands[2], commands[1], g1);
}

void addToBase(string thing, string dataBaseName, vector<ListBase*>* g1) {
    bool baseFound = false;
    for (ListBase* d : *g1) {
        baseFound = true;
        if (d->name == dataBaseName) {
            (d->base).push_back(specialSTOI(thing));
        }
    }
    if (!baseFound) {
        std::cout << "No database with name \"" << dataBaseName << "\" found \n";
    }
}


void save(vector<string> commands, vector<ListBase*>* g1) {
    if (commands.size() < 2) {
        std::cout << "Lacking filename. No file loaded.\n";
        return;
    }
    string fileName = commands[1];
    ofstream myFile(fileName.append(".csv"));
    for (ListBase* d : *g1) {
        myFile << d->name;
        for (auto p : d->base) {
            myFile << "," << p;
        }
        myFile << "\n";
    }
    cout << "Sucessfully saved as " << fileName << "\n";
    myFile.close();
}


void showBase(vector<string> commands, vector<ListBase*>* g1) {
    if (commands.size() < 2) {
        cout << "No name provided, cannot show database\n";
        return;
    }
    string dataBaseName = commands[1];
    bool baseFound = false;
    for (ListBase* d : *g1) {
        baseFound = true;
        if (d->name == dataBaseName) {
            printBase(*d);
        }
    }
    if (!baseFound) {
        std::cout << "No database with name \"" << dataBaseName << "\" found \n";
    }
}

void showBase(vector<string> commands, vector<GraphBase*>* g1) {
    if (commands.size() < 2) {
        cout << "No name provided, cannot show database\n";
        return;
    }
    string dataBaseName = commands[1];
    bool baseFound = false;
    for (GraphBase* d : *g1) {
        baseFound = true;
        if (d->name == dataBaseName) {
            printBase(*d);
        }
    }
    if (!baseFound) {
        std::cout << "No database with name \"" << dataBaseName << "\" found \n";
    }
}

void help() {
    ifstream myFile("help.txt");
    string line;
    while (getline(myFile, line)) {
        cout << line << "\n";
    }
    myFile.close();
}

void display(vector<ListBase*>* g1) {
    for (ListBase* d : *g1) {
        printBase(*d);
    }
}


void printBaseWithHeader(ListBase d) {

}
void printBase(ListBase d) {
    std::cout << d.name << " : ";
    cout << "[";
    bool first = true;
    for (auto n : d.base) {
        if (!first) {
            cout << ",";
        }
        std::cout << n;
        first = false;
    }
    cout << "]\n";
}

void printBase(GraphBase d) {
    cout << "[" << d.name << "]\n";
    vector<int> lengthVector(d.base[0].size(),0);
    
    /*Generate row lengths for formatting*/
    for (auto row : d.base) {
        int i = 0;
        for (auto element : row) {
            int size = std::to_string(element).length();
            (lengthVector[i] < size) ? lengthVector[i] = size : NULL;
            i++;
            
        }
    }

    for (auto row : d.base) {
        int i = 0;
        for (auto element : row) {
            int size = std::to_string(element).length();
            cout << element;
            for (int j = 0; j < lengthVector[i] - size; j++) {
                cout << " ";
            }
            
            i++;
        }
        cout << "\n";
    }
}

void clearBase(vector<string> commands, vector<ListBase*>* g) {
    if (commands.size() < 2) {
        cout << "Not enough arguments to clear.\n";
        return;
    }
    string name = commands[1];
    for (ListBase* d : *g) {
        if (d->name == name) {
            d->base.clear();
            return;
        }
    }
}

void deleteItem(vector<string> commands, vector<ListBase*>* g) {
    if (commands.size() < 3) {
        cout << "Not enough arguments for delete.\n";
        return;
    }
    string item = commands[2];
    string baseName = commands[1];
    
    int m = specialSTOI(item);
    for (ListBase* d : *g) {
        if (d->name == baseName) {
            for (int i = 0; i < d->base.size(); i++) {
                if (d->base[i] == m) {
                    d->base.erase(d->base.begin() + i);
                    printBase(*d);
                    return;
                }
            }
        }
    }
}

void removeBase(vector<string> commands, vector<ListBase*>* g) {
    if (commands.size() < 2) {
        cout << "Not enough arguments for remove.\n";
        return;
    }
    string baseName = commands[1];
    int i = 0;
    for (ListBase* d : *g) {
        if (d->name == baseName) {
            g->erase(g->begin() + i);
        }
        i++;
    }
}

int specialSTOI(string item) {
    string newthing;
    return (stoi(newthing.append("0").append(item)));
}