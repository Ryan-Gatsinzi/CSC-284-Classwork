#include "LogAnalyzer.cpp"
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  try {
    string path = argv[1];

    cout << "Analyzing folder: " << path << endl;

    LogAnalyzer logAnalyzer = LogAnalyzer();
    vector<string> keywords = {"[WARN]", "[ERROR]", "[FATAL]",
                               "[INFO]", "[DEBUG]", "[TRACE]"};

    vector<thread> threads;

    int count = 0;
    for (const auto &file : std::filesystem::directory_iterator(path)) {
      if (file.path().extension() == ".log") {
        count++;
        threads.emplace_back([&logAnalyzer, &keywords, file]() {
          logAnalyzer.analyzeFile(file.path().string(), keywords);
        });
      }
    }

    for (auto &thrd : threads) {
      if (thrd.joinable())
        thrd.join();
    }

    logAnalyzer.printSummary();
    cout << "\nAnalysis complete. Processed "
         << logAnalyzer.getNumFilesSuccessfullyRead() << " files.";
  } catch (exception &e) {
    string path = (argc > 1 ? argv[1] : "");

    cout << "Failed to analyze logs in directory: " << path << endl
         << "Number of arguments: " << argc - 1;

    e.what();
  }
}