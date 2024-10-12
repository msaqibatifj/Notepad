// M Saqib Atif
// 23I-0769
// CS-E


#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "Stack.h"

using namespace std;

StackList Undo;
StackList Redo;

struct LoadFiles {
    string file;
    LoadFiles* next = nullptr;
};

LoadFiles* firstFile = nullptr;

void drawBorders(int width, int height);
void gotoxy(int x, int y);

class Node {
public:
    char data;
    Node* up;
    Node* down;
    Node* left;
    Node* right;

    Node(char val) : data(val), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};

class TextEditor {
private:
    Node* head;
    Node* currentLine;
    Node* tracker;
    const int maxWidth;
    const int maxHeight;
    int widthCount;
    string word;
    int trackerX;
    int trackerY;

public:
    TextEditor() : head(new Node('\0')), currentLine(head), tracker(head), maxWidth(100), maxHeight(40), widthCount(0), word(""), trackerX(2), trackerY(2) {}

    ~TextEditor() {
        clearAllNodes();
    }

    int maxWidthGetter() {
        return this->maxWidth;
    }

    

    void DeleteFromLast(int size) {

        Node* curr = tracker;

        while (curr->right != nullptr) {
            curr = curr->right;
        }

        for (int i = 0; i < size; ++i) {
            if (curr->left != nullptr) {
                Node* check = curr;
                curr = curr->left;
                delete check;
                curr->right = nullptr;
            }
        }

        tracker = curr;
    }


    void UndoStack() {
        if (Undo.isEmpty()) {
            return;
        }

        string lastWord = Undo.pop();
        int size = lastWord.size();
        DeleteFromLast(size);
        int nonspace = size;
        Node* check = tracker;
        while (tracker->left != nullptr && tracker->data != ' ' && tracker->data != '\n') {
            tracker = tracker->left;
            size++;
        }
        DeleteFromLast(size - nonspace);
        Redo.push(lastWord);

        trackerX = 2;
        Node* temp = currentLine;
        while (temp != tracker) {
            temp = temp->right;
            trackerX++;
        }

        system("cls");
        drawBorders(maxWidth, 30);
        PrintLine();
    }

    void RedoStack() {
        if (Redo.isEmpty()) {
            return;
        }

        string wordToRedo = Redo.pop();
        InsertChars(' '); 

        for (size_t i = 0; i < wordToRedo.size(); i++) {
            InsertChars(wordToRedo[i]);  
        }

        Undo.push(wordToRedo); 

        system("cls");
        drawBorders(maxWidth, 30);
        PrintLine();
    }


   

    void InsertChars(char c) {
        // Check if we need to wrap the text or insert a newline
        Node* newNode = new Node(c);

        if (c == '\n' || widthCount == maxWidth - 3) {
            MoveWordsOnEnter(); // Adjust the rows and shift text if needed
            widthCount = 0;
            return;
        }

        // If width exceeds, adjust the tracker to wrap words properly
        else if (c != ' ' && widthCount >= maxWidth - 4) {
            // Move tracker back to find a space for wrapping
            while (tracker->left != nullptr && tracker->data != ' ') {
                tracker = tracker->left;
            }
            // Move words on enter after finding the space
            MoveWordsOnEnter();
            widthCount = 0; // Reset width count after moving words
        }

        // Insert the new character

        if (tracker->right != nullptr) {
            newNode->right = tracker->right;
            tracker->right->left = newNode;
        }
        tracker->right = newNode;
        newNode->left = tracker;

        tracker = newNode;
        widthCount++;
        trackerX++;

        // If space is inserted, store the word in the undo stack
        if (c == ' ' || c == '\n' || c == ('z' & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))) {
            if (!word.empty()) {
                Undo.push(word);
                word = "";
            }
        }
        else {
            word += c;
        }

        

        // Clear the screen and redraw
        system("cls");
        drawBorders(maxWidth, 30);
        PrintLine();
    }

    void MoveWordsOnEnter() {
        // Create a new row and set the appropriate pointers
        if (trackerY >= 27) {
            return;
        }
        Node* newLine = new Node('\0');
        newLine->up = currentLine;

        if (currentLine->down != nullptr) {
            newLine->down = currentLine->down;
            currentLine->down->up = newLine;
        }
        currentLine->down = newLine;

        Node* movetracker = tracker->right; // Start moving characters to the new row
        tracker->right = nullptr; // Disconnect the tracker from the current row

        Node* newLinetracker = newLine; // New row's tracker starts at the beginning

        // Move characters to the new row
        
        for (int i = 0; movetracker != nullptr; widthCount++) {
            Node* check = movetracker;
            movetracker = movetracker->right;

            // Insert character into the new row
            newLinetracker->right = check;
            check->left = newLinetracker;
            newLinetracker = check;
        }

        // Update pointers for currentLine and tracker
        currentLine = newLine;
        tracker = newLine;
        trackerX = 2;
        trackerY++;

        // If there's more content in the rows below, move them up recursively
        if (newLine->down != nullptr) {
            Node* checktracker = tracker;
            tracker = currentLine->down;
            for (int i = 0; tracker->down != nullptr; i++) {
                MoveWordsOnEnter();
                tracker = tracker->down;
            }
            tracker = checktracker;
        }

        // Clear the screen and redraw the updated content
        system("cls");
        drawBorders(maxWidth, 30);
        PrintLine();
    }


    void PrintLine() {
        Node* row = head;
        int rowCount = 2;  // Start from line 2 to leave space for the header

        while (row != nullptr) {
            Node* check = row->right;
            gotoxy(2, rowCount);  // Move to the correct row
            while (check != nullptr) {
                cout << check->data;
                check = check->right;
            }
            row = row->down;
            rowCount++;
        }
        gotoxy(trackerX, trackerY);
    }

    void DeleteChar() {
        if (tracker->left != nullptr) {
            Node* check = tracker;
            tracker = tracker->left;
            tracker->right = check->right;
            if (check->right != nullptr) {
                check->right->left = tracker;
            }
            trackerX--;
            delete check;
            widthCount--;

            if (!word.empty()) {
                word.pop_back();
            }
        }
        else if (tracker == currentLine && currentLine->up != nullptr) {
            currentLine = currentLine->up;
            tracker = currentLine;
            trackerX = 2;
            trackerY--;
            while (tracker->right != nullptr) {
                tracker = tracker->right;
                trackerX++;
            }
            widthCount = trackerX;
        }

        system("cls");
        drawBorders(maxWidth, 30);
        PrintLine();
        gotoxy(trackerX, trackerY);
    }



    void InsertExistingFileName(string fileName) {
        LoadFiles* newFile = new LoadFiles;
        newFile->file = fileName;
        newFile->next = nullptr;

        if (firstFile == nullptr) {
            firstFile = newFile;
        }
        else {
            LoadFiles* check = firstFile;
            while (check->next != nullptr) {
                check = check->next;
            }
            check->next = newFile;
        }
    }

    void createNewFile(string fileName) {
        fstream file(fileName + ".txt", ios::out);
        if (file.is_open()) {
            cout << "New file created: " << fileName << endl;
            file.close();
        }
    }

    void LoadExistingFiles(string fileName) {
        LoadFiles* check = firstFile;
        while (check != nullptr) {
            if (check->file == fileName) {

                ifstream file(check->file + ".txt");
                if (file.is_open()) {
                    char c;
                    while (file.get(c)) {
                        InsertChars(c);
                    }
                    file.close();
                }
                else {
                    cout << "Error opening file: " << check->file << endl;
                }
                return;
            }
            check = check->next;
        }
        cout << "File not found in the list." << endl;
    }

    int totalFiles() {
        LoadFiles* check = firstFile;
        int i = 1;
        while (check != nullptr) {
            check = check->next;
            i++;
        }

        return i;
    }

    void printExistingFiles() {
        LoadFiles* check = firstFile;
        int i = 1;
        while (check != nullptr) {
            cout << i << ". " << check->file << endl;
            check = check->next;
            i++;
        }
    }

    string getFileName(int val) {
        LoadFiles* check = firstFile;
        int i = 1;
        while (check != nullptr) {
            if (i == val) {
                return check->file;
            }
            check = check->next;
            i++;
        }
        return "";
    }

    void SaveFiles(string fileName) {
        InsertExistingFileName(fileName);

        fstream file(fileName + ".txt", ios::out);
        if (file.is_open()) {
            Node* row = head;
            while (row != nullptr) {
                Node* check = row->right;
                while (check != nullptr) {
                    file << check->data;
                    check = check->right;
                }
                file << endl;
                row = row->down;
            }
            file.close();
            cout << "File saved: " << fileName << endl;
        }
        else {
            cout << "Error saving file: " << fileName << endl;
        }
    }

    void MainMenu() {
        system("cls");

        cout << " /====================================================================\\" << endl;
        cout << "/                      WELCOME TO WORD EDITOR                          \\" << endl;
        cout << "\\                            MAIN MENU                                 /" << endl;
        cout << " \\====================================================================/" << endl;
        cout << " | 1. CREATE FILE                                                     | " << endl;
        cout << " | 2. LOAD FILES                                                      | " << endl;
        cout << " | 3. SAVE                                                            | " << endl;
        cout << " | 4. EXIT                                                            | " << endl;
        cout << " \\====================================================================/" << endl;

    }

    void trackerLeft() {
        if (tracker->left != nullptr) {  // If there is a character to the left
            tracker = tracker->left;      // Move left
            trackerX--;                  // Decrement X position
        }
        else if (currentLine->up != nullptr) {  // If at the start and there's a row above
            currentLine = currentLine->up;       // Move to the row above

            // Move to the farthest right (last character) in the previous row
            tracker = currentLine;
            int width = 0;
            while (tracker->right != nullptr) {
                tracker = tracker->right;
                width++;
            }

            trackerX = width + 2;  // Set the X position to the end of the previous row
            trackerY--;        // Decrement the Y position since we moved up
        }
        widthCount = trackerX;
        gotoxy(trackerX, trackerY);  // Update the tracker position
    }
    void trackerRight() {
        if (tracker->right != nullptr) {  // If there is a character to the right
            tracker = tracker->right;      // Move right
            trackerX++;                   // Increment X position
        }
        else if (currentLine->down != nullptr) {  // If at the end and there's a row below
            currentLine = currentLine->down;       // Move to the row below

            // Move to the farthest left (first character) in the next row
            tracker = currentLine;

            trackerX = 2;  // Set the X position to the start of the next row
            trackerY++;    // Increment the Y position since we moved down
        }

        widthCount = trackerX;
        gotoxy(trackerX, trackerY);  // Update the tracker position
    }


    void trackerDown() {
        if (currentLine->down != nullptr) {  // Check if there's a row below
            currentLine = currentLine->down;  // Move to the next row

            // Calculate the tracker's current position from the start of the row
            Node* check = tracker;
            int len = 0;
            while (check->left != nullptr) {  // Traverse left to count the position
                check = check->left;
                len++;
            }

            // Move tracker to the corresponding position in the new row (if possible)
            tracker = currentLine;
            int i = 0;
            while (tracker->right != nullptr && i < len) {  // Move right to match position
                tracker = tracker->right;
                i++;
            }

            // Update tracker X and Y positions
            trackerX = i + 2;  // New X position after moving down
            trackerY++;    // Increment Y since we moved down
        }


        gotoxy(trackerX, trackerY);  // Set the tracker position
    }

    void trackerUp() {
        if (currentLine->up != nullptr) {  // Check if there's a row above
            currentLine = currentLine->up;  // Move to the previous row

            // Calculate the tracker's current horizontal position from the start of the row
            Node* check = tracker;
            int len = 0;
            while (check->left != nullptr) {  // Traverse left to count the position
                check = check->left;
                len++;
            }

            // Move tracker to the corresponding position in the previous row (if possible)
            tracker = currentLine;
            int i = 0;
            while (tracker->right != nullptr && i < len) {  // Move right to match position
                tracker = tracker->right;
                i++;
            }

            // Update tracker X and Y positions
            trackerX = i + 2;  // New X position after moving up
            trackerY--;    // Decrement Y since we moved up
        }


        gotoxy(trackerX, trackerY);  // Set the tracker position
    }

private:
    void clearAllNodes() {
        Node* currentLine = head;

        while (currentLine) {
            Node* curr = currentLine;
            currentLine = currentLine->down;

            while (curr) {
                Node* check = curr;
                curr = curr->right;
                delete check;
            }
        }
    }
};

// Function to draw borders around the console window
void drawBorders(int width, int height) {
    cout << "{==============================================================================================}" << endl;
    cout << "{                                             NOTEPAD                                          }" << endl;
    cout << "{==============================================================================================}" << endl;
    for (int i = 0; i < width; i++) {
        cout << "_";
    }
    cout << endl;
    for (int i = 0; i < height; i++) {
        cout << "{";
        for (int j = 0; j < width - 2; j++) {
            cout << " ";
        }
        cout << "}" << endl;
    }
    cout << '{';
    for (int i = 0; i < width - 2; i++) {
        cout << "_";
    }
    cout << '}';
    cout << endl;
}

void gotoxy(int x, int y) {
    COORD c = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
int main(int argc, char* argv[]) {
    system("cls");
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD events = 0;
    DWORD eventsRead = 0;

    TextEditor editor;
    bool running = true;
    bool inMenu = true; // Track whether we are in the main menu
    string fileName;

    while (running) {
        if (inMenu) {
            editor.MainMenu();
            int choice;
            cout << "Select an option: ";
            cin >> choice;

            switch (choice) {
            case 1: // Create new file
                cout << "Enter new file name: ";
                cin >> fileName;
                editor.createNewFile(fileName);
                system("cls");
                drawBorders(editor.maxWidthGetter(), 30);
                editor.PrintLine();
                inMenu = false; // Switch to writing mode
                break;
            case 2: // Load existing file
                editor.printExistingFiles();
                cout << "Enter file name to load: ";
                cin >> fileName;
                editor.LoadExistingFiles(fileName);
                system("cls");
                drawBorders(editor.maxWidthGetter(), 30);
                editor.PrintLine();
                inMenu = false; // Switch to writing mode
                break;
            case 3: // Save the file
                cout << "Enter file name to save: ";
                cin >> fileName;
                editor.SaveFiles(fileName);
                break;
            case 4: // Exit
                running = false;
                break;
            default:
                cout << "Invalid choice. Please select a valid option." << endl;
                break;
            }
        }
        else {
            GetNumberOfConsoleInputEvents(inputHandle, &events);

            if (events != 0) {
                INPUT_RECORD eventBuffer[200];
                ReadConsoleInput(inputHandle, eventBuffer, events, &eventsRead);

                for (DWORD i = 0; i < eventsRead; ++i) {
                    if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {
                        switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
                        case VK_ESCAPE:
                            inMenu = true;
                            break;
                        case VK_UP:
                            editor.trackerUp();
                            break;
                        case VK_DOWN:
                            editor.trackerDown();
                            break;
                        case VK_LEFT:
                            editor.trackerLeft();
                            break;
                        case VK_RIGHT:
                            editor.trackerRight();
                            break;
                        case VK_BACK:
                            editor.DeleteChar();
                            break;
                        case VK_RETURN:
                            editor.InsertChars('\n');
                            break;
                        case 'Z': // Use 'Z' for Undo (Ctrl + Z) 
                            editor.InsertChars(eventBuffer[i].Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED));
                            if (eventBuffer[i].Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) {
                                editor.UndoStack();
                            }
                            break;
                        case VK_TAB: // Assuming you want to use Tab for Redo
                            editor.RedoStack();
                            break;

                        default:
                            char typedChar = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;
                            if (typedChar >= 32 && typedChar <= 126) { // Allow spaces
                                editor.InsertChars(typedChar);
                            }
                            break;
                        }
                    }

                }
            }
        }

    }

    return 0;
}