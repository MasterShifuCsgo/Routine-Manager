#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

// editor
class editor {

public:
  std::vector<std::string> endresult{}; // holds data for each line seperately.
  editor(std::vector<std::string> dummydata) : endresult(dummydata) {}
  editor() {};

  void view() const {
    if (endresult.size() == 0) {
      std::cout << "nothing to print...\n";
    } else {
      for (int i = 0; i < endresult.size(); i++) {
        std::cout << i << ": " << endresult[i] << '\n';
      }
    }
  }

  void add(const std::string &item, int &line) {

    if (item.size() <= 1) {
      std::cout << "Could not add the item: \'" << item << "\'\n";
      return;
    }

    if (line > endresult.size()) {
      std::cout << "line size too large: " << line << '\n';
      line = endresult.size() - 1;
      std::cout << "edited line size: " << line << '\n';
    }

    endresult.insert(endresult.begin() + line, item);
    std::cout << "\nItem \'" << item << "\' added\n";
  }
  void remove(const int &line_num) {
    if (line_num <= 0 || line_num >= endresult.size()) {
      std::cout << "Error: Invalid line number.\n";
      return;
    }
    std::string temp = endresult[line_num];
    endresult.erase(endresult.begin() + line_num);
    std::cout << "\nRemoved: " << temp << '\n';
  }
  void replace(const int &line_num, const std::string &item) {
    endresult[line_num] = item;
    std::cout << "\nreplaced item: " << item << " at line " << line_num << '\n';
  }
  void switch_locations(const int &li1, const int &li2) {

    if (li1 >= 0 && li1 < endresult[li1].size() && li2 >= 0 &&
        li2 < endresult[li2].size()) {

      std::swap(endresult[li1], endresult[li2]);
      std::cout << "\nswtiched locations. \n";
    } else {
      std::cout << "switching failed. \n";
    }

    std::cout << "line 1: " << li1 << "\n line 2: " << li2 << '\n';
  }
};

// in calls editor.
void addItem(editor &ed);
void removeItem(editor &ed);
void replaceItem(editor &ed);
void switchLocations(editor &ed);
void saveToFile(editor &ed);
void loadFromFile(editor &ed);

std::unordered_map<int, std::function<void(editor &)>> commands = {
    {0, addItem},         {1, removeItem}, {2, replaceItem},
    {3, switchLocations}, {4, saveToFile}, {5, loadFromFile}};

constexpr std::array<const char *, 6> actions = {
    "ADD", "REMOVE", "REPLACE", "SWITCH LOCATIONS", "SAVE", "LOAD"};

// initalizing
// std::vector<std::string> dummydata{"data", "123", "here i am", "Johney"};
editor ed = editor(/* dummydata */); // enter dummy data here.

int main() {

  // user terminal
  while (true) {

    std::cout << "current working editor\n";
    std::cout << "--Start--\n";
    // print the editor
    ed.view();

    std::cout << "--End--\n";

    // print commands
    std::cout << "Terminal: \n";
    for (int i = 0; i < 6; i++) {
      std::cout << i << ". " << actions[i] << '\n';
    }

    // asks the user which command to use
    unsigned int chosen_command;
    while (true) {
      std::cout << "Choose command (number): ";

      // returns false if the input is invalid
      if (!(std::cin >> chosen_command)) {
        std::cin.clear(); // clear the flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                        '\n'); // reset the input buffer.
        std::cout << "Please enter a number\n";
        continue;
      }
      break; // input was valid
    }

    if (commands.find(chosen_command) != commands.end()) {
      commands[chosen_command](ed);
    } else {
      std::cout << "Invalid command.\n";
    }
  }

  return 0;
}

int getValidatedLineNumber() {
  int line_num;
  while (true) {
    std::cout << "Enter line number: ";
    if (!(std::cin >> line_num)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input. Please enter a valid line number.\n";
    } else {
      break;
    }
  }
  return line_num;
}
void addItem(editor &ed) {
  int line_num;
  std::cin.ignore();
  std::string item;
  std::cout << "What would you like to add: ";
  getline(std::cin, item);
  std::cin.ignore();
  line_num = getValidatedLineNumber();
  ed.add(item, line_num);
}
void removeItem(editor &ed) {
  int line_num;
  std::cout << "Enter the line you would like to remove: ";
  line_num = getValidatedLineNumber();
  ed.remove(line_num);
}
void replaceItem(editor &ed) {
  int line_num;
  std::cin.ignore();
  std::string item;
  std::cout << "Enter the line you would like to replace an item with: ";
  line_num = getValidatedLineNumber();
  std::cin.ignore();
  std::cout << "\nEnter the item you want to replace it with: ";
  getline(std::cin, item);
  ed.replace(line_num, item);
}
void switchLocations(editor &ed) {
  int line_num1, line_num2;
  std::cout << "Enter line 1 location: ";
  line_num1 = getValidatedLineNumber();
  std::cout << "\nEnter line 2 location: ";
  line_num2 = getValidatedLineNumber();
  ed.switch_locations(line_num1, line_num2);
}
void saveToFile(editor &ed) {
  std::string filename;

  while (true) {
    std::cin.ignore();
    std::cout << "Enter the file name you would like to save to: ";
    getline(std::cin, filename);

    if (filename.size() < 4 && filename.substr(filename.size() - 4) != ".txt") {
      std::cout << "Please enter a filename with \'.txt\' at the end.";
      continue;
    }
  }

  std::ofstream saveFile(filename, std::ios::out);
  if (!saveFile.is_open()) {
    std::cout << "Error: Unable to open file for saving.\n";
    return;
  }
  for (unsigned int i = 0; i < ed.endresult.size(); i++) {
    saveFile << i << ". " << ed.endresult[i] << '\n';
  }
  std::cout << "Save Completed\n";
}
void loadFromFile(editor &ed) {
  while (true) {
    std::cin.ignore();
    std::string filename;
    std::cout << "File format should be .txt\n";
    std::cout << "Enter the file name you would like to load from: ";
    getline(std::cin, filename);

    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
      std::cout << "Please enter a file with the suffix .txt\n";
      continue;
    }

    std::ifstream loadFile(filename);
    if (!loadFile.is_open()) {
      std::cout << "Error: Could not open file.\n";
      continue;
    }

    ed.endresult.clear(); // Clear existing data
    std::string line;
    while (getline(loadFile, line)) {
      ed.endresult.push_back(line);
    }
    std::cout << "Completed Loading.\n";
    break;
  }
}
