#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::ios_base;
using std::stringstream;

void printHelp () {
  cout << endl;
  cout << "Command    Parameters                 Description" << endl;
  cout << "-------------------------------------------------" << endl;
  cout << "list                                  List tasks"  << endl;
  cout << "add        <description>              Add task"    << endl;
  cout << "delete     <id>                       Delete task" << endl;
  cout << "help                                  Show help"   << endl;
}

void listTasks () {
  string line;
  int num = 1;
  ifstream tasksFile ("tasks.txt");

  if (tasksFile.is_open()) {
    while (getline(tasksFile, line)) {
      cout << num << ": " << line << endl;
      num++;
    }
  }
  tasksFile.close();
}

bool addTask (string description) {
  if (description == "") {
    return false;
  }

  ofstream tasksFile ("tasks.txt", ios_base::app);
  if (tasksFile.is_open()) {
    tasksFile << description << "\n";
    tasksFile.close();
  }
  return true;
}

bool deleteTask (string number) {
  int num;
  stringstream ss;
  ss << number;
  ss >> num;

  if (num <= 0) {
    return false;
  }

  vector<string> content;
  int counter = 1;
  string line;
  ifstream tasksFile ("tasks.txt");
  if (tasksFile.is_open()) {
    while (getline(tasksFile, line)) {
      if (counter != num) {
        content.push_back(line);
      }
      counter++;
    }
  }
  tasksFile.close();

  ofstream tempFile ("temp.txt", ios_base::app);
  if (tempFile.is_open()) {
    for (int i = 0; i < content.size(); i++) {
      tempFile << content[i] << "\n";
    }
  }
  tempFile.close();
  remove("tasks.txt");
  rename("temp.txt", "tasks.txt");

  return true;
}

int main (int argc, char *argv[]) {
  if (argc <= 1) {
    printHelp();
    return 0;
  }

  vector<string> arguments;
  arguments.assign(argv + 1, argv + argc);

  if (arguments[0] == "help") {
    printHelp();
  }

  if (arguments[0] == "list") {
    listTasks();
  }

  if (arguments[0] == "add") {
    string description = "";
    for (int i = 1; i < arguments.size(); i++) {
      description += arguments[i];
      if (i != arguments.size()) {
        description += " ";
      }
    }
    bool res = addTask(description);

    if (!res) {
      cout << "Error: task was not addded!" << endl;
      return -1;
    }
    cout << "Task was added!" << endl;
  }

  if (arguments[0] == "delete") {
    deleteTask(arguments[1]);
  }

  return 0;
}
