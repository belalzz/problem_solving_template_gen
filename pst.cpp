// pst.cpp
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <exception>
#include <fstream>

namespace fs = std::filesystem;

struct Data {
    fs::path root_folder;
    fs::path file;
    std::string author;
};

bool valid(const Data&);
bool valid_folder_path(const std::string&);
bool valid_file_path(const std::string&);
Data get_settings();
Data prompt_user_for_settings();
bool save_settings(const Data&);
bool init_problem(
    const Data& settings,
    const std::string& folder_name,
    const std::string& file_name
);
bool reset_settings();

int main(int argc, char** argv) {
    
    if (argc == 2 && std::string(argv[1]) == "-r") {
        reset_settings();
        return 0;
    }

    Data cur_data = get_settings();

    if (!valid(cur_data)) {
        std::cout << "Settings not found or invalid.\n";
        cur_data = prompt_user_for_settings();

        if (!save_settings(cur_data)) {
            std::cerr << "Failed to save settings.\n";
            return 1;
        }

        std::cout << "Settings saved successfully.\n";
    }

    // ---- argument parsing happens for every run ----
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: pst folder_name [file_name]\n";
        return 1;
    }

    std::string folder_name = argv[1];
    std::string file_name   = (argc == 3) ? argv[2] : "main";

    if (!init_problem(cur_data, folder_name, file_name)) {
    std::cerr << "Failed to initialize problem.\n";
    return 1;
}

std::cout << "Problem initialized successfully in "
          << (cur_data.root_folder / folder_name) << "\n";


    return 0;
}

bool reset_settings() {
    fs::path settings_file = "settings.txt";

    if (fs::exists(settings_file)) {
        std::error_code ec;
        fs::remove(settings_file, ec);
        if (ec) {
            std::cerr << "Failed to delete settings: " << ec.message() << '\n';
            return false;
        }
        std::cout << "Settings reset successfully.\n";
        return true;
    }

    std::cout << "No settings to reset.\n";
    return true;
}


bool init_problem(
    const Data& settings,
    const std::string& folder_name,
    const std::string& file_name
) {
    fs::path name = file_name;

    if (!name.has_extension())
        name += settings.file.extension();

    fs::path problem_dir = settings.root_folder / folder_name;
    fs::path output_file = problem_dir / (name);

    // 1. Create folder if needed
    if (!fs::exists(problem_dir)) {
        fs::create_directories(problem_dir);
        std::cout << "Created folder: " << problem_dir << '\n';
    }

    // 2. Do not overwrite existing file
    if (fs::exists(output_file)) {
        std::cerr << "File already exists: " << output_file << '\n';
        return false;
    }

    // 3. Open template
    std::ifstream template_file(settings.file);
    if (!template_file) {
        std::cerr << "Failed to open template file\n";
        return false;
    }

    // 4. Create output file
    std::ofstream out(output_file);
    if (!out) {
        std::cerr << "Failed to create output file\n";
        return false;
    }

    // 5. Copy template contents
    out << template_file.rdbuf();

    std::cout << "Created file: " << output_file << '\n';
    return true;
}


Data prompt_user_for_settings() {
    Data d;
    std::string input;

    // Root folder
    while (true) {
        std::cout << "Enter root folder path: ";
        std::getline(std::cin, input);

        if (valid_folder_path(input)) {
            d.root_folder = input;
            break;
        }
        std::cerr << "Invalid folder path. Try again.\n";
    }

    // Template file
    while (true) {
        std::cout << "Enter template file path: ";
        std::getline(std::cin, input);

        if (valid_file_path(input)) {
            d.file = input;
            break;
        }
        std::cerr << "Invalid file path. Try again.\n";
    }

    // Author
    while (true) {
        std::cout << "Enter author name: ";
        std::getline(std::cin, input);

        if (!input.empty()) {
            d.author = input;
            break;
        }
        std::cerr << "Author cannot be empty.\n";
    }

    return d;
}

bool save_settings(const Data& d) {
    if (!valid(d))
        return false;

    std::ofstream out("settings.txt");
    if (!out)
        return false;

    out << d.root_folder.string() << '\n';
    out << d.file.string() << '\n';
    out << d.author << '\n';
    return true;
}




bool valid_folder_path(const std::string& path_s) {
    fs::path p(path_s);
    return fs::exists(p) && fs::is_directory(p);
}

bool valid_file_path(const std::string& path_s) {
    fs::path p(path_s);
    return fs::exists(p) && fs::is_regular_file(p);
}


bool valid(const Data& d) {
    return !d.root_folder.empty()
        && !d.file.empty()
        && !d.author.empty();
}

Data get_settings() {
    fs::path settings_path =
    fs::path(std::filesystem::current_path()) / "settings.txt";

    if (!fs::exists(settings_path))
        return {};

    std::ifstream file(settings_path);
    if (!file)
        return {};

    std::string folder_path, file_path, author;

    if (!std::getline(file, folder_path)) return {};
    if (!std::getline(file, file_path)) return {};
    if (!std::getline(file, author)) return {};

    if (!valid_folder_path(folder_path)) return {};
    if (!valid_file_path(file_path)) return {};

    return { folder_path, file_path, author };
}

