// pst.cpp
// A simple utility to bootstrap problem-solving folders and files

#include <windows.h>
#include <shlobj.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <objbase.h>

#ifndef FOLDERID_Desktop
const GUID FOLDERID_Desktop = 
{ 0xB4BFCC3A, 0xDB2C, 0x424C, { 0xB0, 0x29, 0x7F, 0xE9, 0x9A, 0x87, 0xC6, 0x41 } };
#endif

using namespace std;
namespace fs = std::filesystem;

constexpr int INVALID_COMMAND_INPUT = 2;

// --------------------------------------------------
// Utility helpers
// --------------------------------------------------

string read_file_to_string(const fs::path& path) {
    ifstream file(path);
    if (!file) return {};
    ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void replace_all(string& text, const string& from, const string& to) {
    size_t pos = 0;
    while ((pos = text.find(from, pos)) != string::npos) {
        text.replace(pos, from.length(), to);
        pos += to.length();
    }
}

// --------------------------------------------------
// Path helpers
// --------------------------------------------------

fs::path get_desktop_path() {
    PWSTR desktopPath = nullptr;
    if (FAILED(SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &desktopPath))) {
        return {};
    }

    fs::path result(desktopPath);
    CoTaskMemFree(desktopPath);
    return result;
}

fs::path get_executable_dir() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return fs::path(buffer).parent_path();
}

// --------------------------------------------------
// Base folder initialization
// --------------------------------------------------

fs::path base_folder_initializer() {
    fs::path desktop = get_desktop_path();
    if (desktop.empty()) {
        cerr << "Failed to locate Desktop\n";
        return {};
    }

    fs::path base = desktop / "Problem_Solving_Folder";

    error_code ec;
    fs::create_directories(base, ec);
    if (ec) {
        cerr << "Filesystem error: " << ec.message() << '\n';
        return {};
    }

    return base;
}

// --------------------------------------------------
// Core command logic
// --------------------------------------------------

bool command_validator(int argc, char** argv, const fs::path& base_folder) {
    if (argc < 2 || argc > 3) {
        cerr << "Usage:\n"
             << "  pst problem_name [file_name]\n";
        return false;
    }

    string problem_name = argv[1];
    string file_name = (argc == 3) ? argv[2] : "main";

    fs::path problem_folder = base_folder / problem_name;

    // ---- Create problem folder ----
    error_code ec;
    fs::create_directories(problem_folder, ec);
    if (ec) {
        cerr << "Failed to create folder: " << ec.message() << '\n';
        return false;
    }

    // ---- Resolve template ----
    fs::path template_path = get_executable_dir() / "templates" / "cpp_problem.cpp";

    if (!fs::exists(template_path)) {
        cerr << "Template not found: " << template_path << '\n';
        return false;
    }

    string content = read_file_to_string(template_path);
    if (content.empty()) {
        cerr << "Failed to read template\n";
        return false;
    }

    // ---- Replace placeholders ----
    unordered_map<string, string> vars{
        {"{{PROBLEM_NAME}}", problem_name},
        {"{{AUTHOR}}", "Your Name"},
        {"{{DATE}}", __DATE__}
    };

    for (auto& [key, value] : vars) {
        replace_all(content, key, value);
    }

    // ---- Create file ----
    fs::path output_file = problem_folder / (file_name + ".cpp");

    if (fs::exists(output_file)) {
        cout << "File already exists: " << output_file << '\n';
        return true;
    }

    ofstream out(output_file);
    if (!out) {
        cerr << "Failed to write file\n";
        return false;
    }

    out << content;
    out.close();

    cout << "Created: " << output_file << '\n';

    // ---- Optional: open folder in Explorer ----
    ShellExecuteA(
        NULL,
        "open",
        problem_folder.string().c_str(),
        NULL,
        NULL,
        SW_SHOW
    );

    return true;
}

// --------------------------------------------------
// Main
// --------------------------------------------------

int main(int argc, char** argv) {
    fs::path base_folder = base_folder_initializer();
    if (base_folder.empty()) {
        return 1;
    }

    if (!command_validator(argc, argv, base_folder)) {
        return INVALID_COMMAND_INPUT;
    }

    return 0;
}
