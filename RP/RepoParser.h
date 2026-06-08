#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class RepoParser {

private:
    std::vector<std::string> Ignore;
    fs::path RootDir;
    fs::path OutFileDir;
    std::string Message;
    bool shouldIgnore(const fs::path& path);
public:
    RepoParser(std::string r, std::string o, std::string m);
    void generateOutputFile();
    void addIgnore(std::vector<std::string> IgnoreList);

};