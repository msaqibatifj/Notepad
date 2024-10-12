#pragma once
#include <iostream>
#include <cstring>
using namespace std;

class StackList {
private:
    string arr[5]; // Fixed-size array to hold up to 5 words
    int top;       // Tracks the index of the top element in the stack

public:
    StackList() : top(-1) {} // Initialize top to -1 (indicating an empty stack)

    // Check if the stack is empty
    bool isEmpty() { return top == -1; }

    // Check if the stack is full
    bool isFull() { return top == 4; }

    // Push a word onto the stack
    void push(const string& word) {
        if (isFull()) {
            // Shift elements left to discard the oldest one
            for (int i = 0; i < 4; i++) {
                arr[i] = arr[i + 1];
            }
            arr[4] = word; // Add the new word at the end
        }
        else {
            arr[++top] = word; // Increment top and add the word
        }
    }

    // Pop a word from the stack
    string pop() {
        if (isEmpty()) {
            cout << "No more actions to undo!" << endl;
            return "";
        }
        else {
            return arr[top--]; // Return the top element and decrement top
        }
    }

    void print() {
        for (int i = top; i >= 0; i--) {
            cout << arr[i] << endl;
        }
    }

    void clear() { top = -1; }

    void convertToWords(const string& input) {
        string newStr;
        for (char c : input) {
            if (c == ' ') {
                if (!newStr.empty()) {
                    push(newStr); // Push the completed word
                    newStr.clear(); // Clear for the next word
                }
            }
            else {
                newStr += c; // Build the word character by character
            }
        }
        if (!newStr.empty()) {
            push(newStr);
        }
    }
};