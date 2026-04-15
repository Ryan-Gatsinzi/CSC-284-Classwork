#include "LogAnalyzer.h"
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <algorithm>

// custom function to find substring (just for fun :) ) way slower than find method
bool hasSubstring(std::string original_string, std::string target){
    std::string current = "";
    int start_index = 0, end_index = 1;

    while(start_index < original_string.length()){
        if(current == target)
            return true;

        current = original_string.substr(start_index, end_index-start_index);

        if(end_index == original_string.length()){
            start_index++;
            end_index = start_index+1;
        }else{
            end_index++;
        }
    }

    return false;
}


void LogAnalyzer::analyzeFile(const std::string& filename, const std::vector<std::string>& keywords) {
    std::lock_guard<std::mutex> lock(countMutex);

    std::ifstream file(filename);
    if (!file) {
        std::cout << "Error: Could not open file " << filename << "\n";
        return;
    }
    
    std::cout << "Processing: " << filename << std::endl;
    
    std::string log;
    
    while(getline(file, log)){
        
        for(std::string keyword : keywords){
            if(keywordCounts.find(keyword) == keywordCounts.end()){
                keywordCounts[keyword] = 0;
            }
    
            if(log.find(keyword) != std::string::npos)
                keywordCounts[keyword]++;

            // if(hasSubstring(log, keyword))
            //     keywordCounts[keyword]++;
        }   
    }

    numFilesSuccesfullyRead++;
}

int LogAnalyzer::getNumFilesSuccessfullyRead(){ return numFilesSuccesfullyRead;}

void LogAnalyzer::printSummary() const {
    std::cout << "\n--- Keyword Summary ---\n";

    std::for_each(keywordCounts.begin(), keywordCounts.end(), [](auto &key_value_pair){ // pair<string, int>
        std::cout << key_value_pair.first << ": " << key_value_pair.second << std::endl;
    });

    std::cout << "-----------------------\n";
}


/*
if i lock the entire analyzeFile method and each process is using the same object i.e same method then this is basically serial and not multi processed ?
I feel like it would make more sense to make a new object for each process and call a static method updateKeywordCount or something
*/