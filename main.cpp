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
void search_files(std::string searchString, std::string searchPath, bool ignorecase){
    std::ifstream input_file;
    std::string line;
    std::string search_line;

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
                            std::cout << line << std::endl;
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
                    std::cout << line << std::endl;
                }
            }
            input_file.close();
        }else{
            std::cerr << "Error opening file: " << searchPath << std::endl;
        }
    } else {
        std::cerr << "Error: " << searchPath << " is not a valid file or directory." << std::endl;
    }
}

// Searches for the searchString in standard input (for piped input)
void search_output(std::string searchString, bool ignorecase){
    std::string input_line;
    std::string search_line;

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
            std::cout << input_line << std::endl;
        }
    }
}

// Entry point of the program
int main(int argc, char** argv)
{
    bool ignorecase = false;
    std::string searchstring;
    std::string searchpath;
    int valid_arg_count = 2;

    // Check if -i flag is present for case-insensitive search
    if(std::string("-i").find(argv[1]) != std::string::npos){
        ignorecase = true;
        searchstring = argv[2];
        valid_arg_count++;
    }else{
        searchstring = argv[1];
    }

    // If input is piped, search in stdin
    if(!isatty(fileno(stdin))){
        search_output(searchstring, ignorecase);
    } else if(argc >= valid_arg_count){
        // Otherwise, search in files in the given directory
        if(ignorecase){
            searchpath = argv[3];
        } else {
            searchpath = argv[2];
        }
        search_files(searchstring, searchpath, ignorecase);
    } else {
        // Print usage if arguments are insufficient
        std::cout << "Usage: grep <search string> [search path]" << std::endl;
    }
    return 0;
}