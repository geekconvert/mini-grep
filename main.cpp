#include<iostream>
#include<fstream>
#include<filesystem>
#include<string>
#include<unistd.h>
#include<algorithm>

std::string string_to_lower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
    return str;
}

void search_files(std::string searchString, std::string searchPath, bool ignorecase){

    std::ifstream input_file;
    std::string line;
    std::string search_line;

    for(const auto &file : std::filesystem::directory_iterator(searchPath)){
        if(file.is_regular_file()){
            input_file.open(file.path());
            if(input_file){
                while(std::getline(input_file, line)){
                    if(ignorecase){
                        search_line = string_to_lower(line);
                    }else{
                        search_line = line;
                    }
                    
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
}

void search_output(std::string searchString, bool ignorecase){
    std::string input_line;
    std::string search_line;

    while(std::cin){
        getline(std::cin, input_line);
        
        if(ignorecase){
            search_line = string_to_lower(input_line);
        }else{
            search_line = input_line;
        }

        if(search_line.find(searchString) != std::string::npos){
            std::cout << input_line << std::endl;
        }
    }
}

int main(int argc, char** argv)
{
    bool ignorecase = false;
    std::string searchstring;
    std::string searchpath;

    int valid_arg_count = 2;

    if(std::string("-i").find(argv[1]) != std::string::npos){
        ignorecase = true;
        searchstring = argv[2];
        valid_arg_count++;
    }else{
        searchstring = argv[1];
    }

    if(!isatty(fileno(stdin))){
        search_output(searchstring, ignorecase);
    } else if(argc >= valid_arg_count){
       if(ignorecase){
           searchpath = argv[3];
       } else {
           searchpath = argv[2];
       }
       search_files(searchstring, searchpath, ignorecase);
    } else {
        std::cout << "Usage: grep <search string> [search path]" << std::endl;
    }

    return 0;
}