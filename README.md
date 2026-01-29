# PST — Problem Solving Template Tool

`pst` is a lightweight command-line utility for quickly setting up **problem-solving folders and C++ template files** on Windows. It automates folder creation on your Desktop, generates `.cpp` files from a template, and opens the folder immediately for coding.

---

##  Features

- Create a **dedicated folder** for each problem inside a main `Problem_Solving_Folder` on your Desktop.
- Automatically generate a **C++ file** from a reusable template.
- Supports **custom file names** (defaults to `main.cpp`).
- **Placeholder replacement** for:
  - `{{PROBLEM_NAME}}`
  - `{{AUTHOR}}`
  - `{{DATE}}`
- Opens the folder automatically in Windows Explorer.
- MinGW and MSVC compatible.

---

## 🗂 Directory Structure

pst/
├── pst.exe # Compiled executable
├── pst.cpp # Source code
└── templates/
└── template.cpp # Default C++ problem template

---

## Usage

Open a terminal in the folder containing `pst.exe` and run:

```bash
# Create a problem folder "folder_name" and file "file_name.cpp"
./pst.exe folder_name file_name

# Or specify a different file name
./pst.exe folder_name solution

Desktop/
└── Problem_Solving_Folder/
    └── folder_name/
        └── file_name.cpp
```
# Building from the Source using mingw: 
g++ -std=c++17 pst.cpp -o pst -lole32 -lshell32
