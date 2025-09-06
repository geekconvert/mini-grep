#include<iostream>
#include<fstream>
#include<filesystem>
#include<string>
#include<unistd.h>
#include<algorithm>

// Converts a string to lowercase for case-insensitive search
std::string string_to_lower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
    return str;
}

// Searches for the searchString in all regular files within the given directory or a single file
void search_files(std::string searchString, std::string searchPath, bool ignorecase, bool countOnly){
    std::ifstream input_file;
    std::string line;
    std::string search_line;
    int match_count = 0;

    std::filesystem::path path(searchPath);
    if (std::filesystem::is_directory(path)) {
        // Iterate over all files in the directory
        for(const auto &file : std::filesystem::directory_iterator(searchPath)){
            if(file.is_regular_file()){
                input_file.open(file.path());
                if(input_file){
                    // Read each line from the file
                    while(std::getline(input_file, line)){
                        if(ignorecase){
                            search_line = string_to_lower(line);
                        }else{
                            search_line = line;
                        }
                        // Print the line if searchString is found
                        if(search_line.find(searchString) != std::string::npos){
                            match_count++;
                            if(!countOnly){
                                std::cout << line << std::endl;
                            }
                        }
                    }
                    input_file.close();
                }else{
                    std::cerr << "Error opening file: " << file.path() << std::endl;
                }
                input_file.close();
            }
        }
    } else if (std::filesystem::is_regular_file(path)) {
        // Search in a single file
        input_file.open(searchPath);
        if(input_file){
            // Read each line from the file
            while(std::getline(input_file, line)){
                if(ignorecase){
                    search_line = string_to_lower(line);
                }else{
                    search_line = line;
                }
                // Print the line if searchString is found
                if(search_line.find(searchString) != std::string::npos){
                    match_count++;
                    if(!countOnly){
                        std::cout << line << std::endl;
                    }
                }
            }
            input_file.close();
        }else{
            std::cerr << "Error opening file: " << searchPath << std::endl;
        }
    } else {
        std::cerr << "Error: " << searchPath << " is not a valid file or directory." << std::endl;
    }
    if(countOnly){
        std::cout << match_count << std::endl;
    }
}

// Searches for the searchString in standard input (for piped input)
void search_output(std::string searchString, bool ignorecase, bool countOnly){
    std::string input_line;
    std::string search_line;
    int match_count = 0;
    // Read each line from stdin
    while(std::cin){
        getline(std::cin, input_line);
        if(ignorecase){
            search_line = string_to_lower(input_line);
        }else{
            search_line = input_line;
        }
        // Print the line if searchString is found
        if(search_line.find(searchString) != std::string::npos){
            match_count++;
            if(!countOnly){
                std::cout << input_line << std::endl;
            }
        }
    }
    if(countOnly){
        std::cout << match_count << std::endl;
    }
}

// Entry point of the program
int main(int argc, char** argv)
{
    bool ignorecase = false;
    bool countOnly = false;
    std::string searchstring;
    std::string searchpath;
    int valid_arg_count = 2;

    // Parse flags in any order
    int arg_index = 1;
    while (arg_index < argc && std::string(argv[arg_index]).rfind("-", 0) == 0) {
        if (std::string(argv[arg_index]) == "-i") {
            ignorecase = true;
            valid_arg_count++;
        } else if (std::string(argv[arg_index]) == "-c") {
            countOnly = true;
            valid_arg_count++;
        }
        arg_index++;
    }
    if(argc > arg_index){
        searchstring = argv[arg_index];
        arg_index++;
    }

    // If input is piped, search in stdin
    if(!isatty(fileno(stdin))){
        search_output(searchstring, ignorecase, countOnly);
    } else if(argc >= valid_arg_count){
        if(argc > arg_index){
            searchpath = argv[arg_index];
        }
        search_files(searchstring, searchpath, ignorecase, countOnly);
    } else {
        // Print usage if arguments are insufficient
        std::cout << "Usage: grep [-i] [-c] <search string> [search path]" << std::endl;
    }
    return 0;
}