# Text Editor in C++ with OpenGL and Font Rendering

This is a lightweight text editor written in **C++** with an **OpenGL** backend. It features a robust font rendering system that leverages **fragment** and **vertex shaders**, allowing you to render any type of font with precision.  

## Features
- **File Handling**: 
  - Open an existing file using its file path.
  - Create a new file for editing if no file path is provided.
  - Automatically saves the file when closing the editor.
  
- **Font Rendering**:
  - Renders any font type using a shader-based rendering system.
  - Supports customizable font sizes and styles via shaders.
  
- **Editor Customization**:
  - Includes an `EditorSettings` struct for setting various editor properties such as:
    - Font size
    - Background color
    - Line spacing

## Getting Started

### Prerequisites (Already Provided for linux based systems, tested on Mac M1 chip)
- **C++17** or later
- **OpenGL** (version 3.3 or higher)
- **GLFW** (for window management)
- **FreeType** (for font rendering)

### To run this app
- ./app font_file_path_arg input_file_path_arg
