# Text Editor

## Overview

This project is a simple console-based text editor implemented in C++. It features a custom linked list to manage text and a self-implemented stack for undo and redo functionality. The editor allows users to create, load, and save text files, providing basic text editing capabilities such as character insertion, deletion, and navigation.

## Features

- **Linked List for Text Management**: 
  - Each line of text is represented as a linked list of nodes. Each node holds a character and pointers to the next and previous nodes, enabling efficient insertion and deletion operations.
  - The editor supports multiple lines, with each line also linked to the next line through a vertical linked list structure.

- **Undo and Redo Functionality**: 
  - A self-implemented stack is used to store changes for undo and redo operations. The undo stack tracks the last modifications, allowing users to revert changes easily, while the redo stack allows reapplying undone changes.

- **Basic Text Editing Operations**: 
  - Users can insert characters, delete characters, move the cursor left, right, up, and down, and handle line breaks correctly.
  - Supports text wrapping within a defined width.

- **File Operations**: 
  - Users can create new files, load existing text files, and save their edits to files.
  - A simple file management system maintains a list of loaded files.

## Implementation Details

### Data Structures

- **Node Class**: Represents a character in the text and contains pointers to adjacent characters both horizontally (left and right) and vertically (up and down).
  
- **Linked List**: 
  - Each line of text is managed as a linked list of `Node` objects. Lines themselves are also linked, allowing for easy traversal and manipulation of text across multiple lines.
  
- **Stack Class**: 
  - A self-implemented stack manages the undo and redo operations. Each change (word insertion or deletion) is pushed onto the undo stack, and actions can be popped from this stack to revert changes. The redo stack functions similarly, allowing for reapplication of undone changes.

### Navigation and Input Handling

- The editor processes keyboard input for navigation and editing, utilizing arrow keys, backspace, and the Enter key to manipulate the text.
- Special key combinations, like Ctrl + Z for undo, are supported through the handling of keyboard events.

### Usage

1. **Compile and Run**: Ensure you have a C++ compiler installed. Compile the source code and run the executable.
2. **Menu Options**: Follow the on-screen prompts to create or load files, edit text, and save your changes.
3. **Navigation**: Use arrow keys for navigation, backspace to delete characters, and Enter to create new lines.

## Requirements

- C++11 or later
- Windows (due to the use of Windows-specific headers for console operations)

## Future Enhancements

- Find and replace will be added soon.
- Word Suggestions will be provided as you write.
- Optimize memory management for linked list operations.

---

Thank you for checking out this text editor project! Happy coding!
