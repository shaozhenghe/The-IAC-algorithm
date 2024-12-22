#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <direct.h>


#define MAX_PATH 260


int main() {
    char Initial_path[MAX_PATH];
    // retrieve the current working directory
    getcwd(Initial_path, MAX_PATH);


    std::vector<std::string> origin_realsolution;
    std::vector<std::string> goal_realsolution;


//    std::string input_path = std::string(Initial_path) + "\\A.txt";
//    std::string output_path = std::string(Initial_path) + "\\B.txt";
    std::string input_path = std::string(Initial_path) + "\\Results\\Challenge\\16.1\\Final_solutions.txt"; //  input
    std::string output_path = std::string(Initial_path) + "\\Results\\Challenge\\16.1\\Final_solutions_clear.txt"; // output



    std::ifstream input_file(input_path.c_str());


    // Check if the input file has been successfully opened
    if (!input_file.is_open()) {
        std::cerr << "Fail to open input file" << std::endl;
        return 1;
    }


    std::string line;
    // Read the input file line by line and store it in origin_delalsolution
    while (std::getline(input_file, line)) {
        origin_realsolution.push_back(line);
    }
    input_file.close();


    // Perform deduplication operation on origin_delalsolution
    for (std::vector<std::string>::iterator it = origin_realsolution.begin(); it!= origin_realsolution.end(); ++it) {
        bool found = false;
        for (std::vector<std::string>::iterator inner_it = goal_realsolution.begin(); inner_it!= goal_realsolution.end(); ++inner_it) {
            if (*it == *inner_it) {
                found = true;
                break;
            }
        }
        if (!found) {
            goal_realsolution.push_back(*it);
        }
    }


    std::ofstream output_file(output_path.c_str());


    // Check if the output file has been successfully opened
    if (!output_file.is_open()) {
        std::cerr << "Fail to open output file" << std::endl;
        return 1;
    }


    // Write the deduplicated content to the output file
    for (std::vector<std::string>::iterator it = goal_realsolution.begin(); it!= goal_realsolution.end(); ++it) {
        output_file << *it << std::endl;
    }
    output_file.close();


    // Print the number of lines in the output file
    std::cout << "The number of real solutions is: " << goal_realsolution.size() << std::endl;


    std::cout << "Finish!" << std::endl;


    return 0;
}
