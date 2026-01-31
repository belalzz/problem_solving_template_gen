# pst CLI

A **command-line tool** to quickly create files from templates in any folder.  
Ideal for bootstrapping files, boilerplate code, or repetitive tasks.

---

## Features

- Create a folder (if needed) and file from a template.
- Automatically copies a template file into the new file.
- Matches the template file’s extension automatically.
- Saves root folder, template path, and author settings for future runs.
- Reset saved settings with `-r`.
- Works on any folder and file type, not tied to any specific use case.

---

## Installation

### Using CMake

1. Clone the repository:

```bash
git clone https://github.com/<belalzz>/pst-cli.git
cd pst-cli
```
2. Build
- mkdir build
- cd build
- cmake ..
- cmake --build .

3. .exe will be available in pst-cli\build\bin\Debug\pst.exe
