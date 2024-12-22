#include <iostream>
#include <filesystem>
#include <string>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_PATH 1000
namespace fs = std::filesystem;

void make_dir(const char* folder_name, const char* parent_dir) {
    // Construct a complete directory path
    fs::path full_path(parent_dir);
    full_path /= folder_name;

    try {
        // Attempt to create a directory
        if (fs::create_directory(full_path)) {
            std::cout << "Directory created successfully: " << full_path << std::endl;
        } else {
            std::cout << "Directory already exists or failed to create: " << full_path << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        // Capture and print abnormal information
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    char Initial_path[MAX_PATH];
    char Problem_path[MAX_PATH];
    char Intermediate_parameters_path[MAX_PATH];
    char Results_path[MAX_PATH];

    getcwd(Initial_path, MAX_PATH);
    make_dir("Problem", Initial_path);
    make_dir("Intermediate_parameters", Initial_path);
    make_dir("Results", Initial_path);
    strcpy(Problem_path,Initial_path);
    strcpy(Intermediate_parameters_path,Initial_path);
    strcpy(Results_path,Initial_path);
    strcat(Problem_path,"/Problem");
    strcat(Intermediate_parameters_path,"/Intermediate_parameters");
    strcat(Results_path,"/Results");
    const char* problem_name="19.1";

    make_dir(problem_name,Intermediate_parameters_path);
    make_dir(problem_name,Results_path);

    

    return 0;
}