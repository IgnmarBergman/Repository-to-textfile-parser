#include <iostream>
#include <string>
#include "RepoParser.h"

void RepoParser::addIgnore(std::vector<std::string> IgnoreList){
    Ignore = IgnoreList;
}


    // passer le path en reference "&" permet a la fonction d'acceder directement
    // au filepath au lieu de copier sa valeur dans le HEAP, ceci sauve de la memoire et 
    // accelere le processus.
    // const est la pour la securite et pour promettre de ne pas modifier la valeur original 
    // de ladresse memoire, elle n'est pas necessaire mais il est considere comme un bonne pratique
    // de la specifie
bool RepoParser::shouldIgnore(const fs::path& path){
    // fonctionne pour fichier et dir
    std::string relPath = fs::relative(path, RootDir).string();

    bool checkIgnore = false;
    for (auto current : Ignore){
        if (relPath.find(current) != std::string::npos){
            checkIgnore = true;
        }
    }
    std::string extension = path.extension().string();
    if (relPath.find("Build/") != std::string::npos || relPath.find("build/") != std::string::npos || (relPath.find(".git") != std::string::npos || extension==".md" || checkIgnore == true)){
        return true;
    }
    return false;
}

    // le constructor
    // on assigne les param du constructor au variable private existantes
RepoParser::RepoParser(std::string r, std::string o, std::string m)
        : RootDir(r), OutFileDir(o), Message(m) {}   



void RepoParser::generateOutputFile(){
    std::ofstream OutFile(OutFileDir);
    if (!OutFile.is_open()){
        std::cerr<<"Erreur de creation de fichier de sortie.\n";
        return;
    }


    // \t pour tab, \" pour un " sans sortir de la string
    OutFile<< "MESSAGE:\n\t\""<< Message << "\"\n\n";

    OutFile<< "ROOTFOLDER:\n\t\""<<RootDir.filename().string()<<"\" {\n";

    // auto est pour un datatype que le compiler devine par le type habituel des elements dans recursive_directory_iterator
    // donc pour chaque element dans la directory on ajoute son relative path au OutFile
    for (const auto& entry : fs::recursive_directory_iterator(RootDir)){
        if (shouldIgnore(entry.path())) continue;

        OutFile<<"\t"<<fs::relative(entry.path(), RootDir).string()<<"\n";
    }
    OutFile<<"}\n\n";

    OutFile<<"CONTENT:\n";
    for (const auto& entry : fs::recursive_directory_iterator(RootDir)) {
        // pour eliminer les directory ou fichiers speciaux
        if (shouldIgnore(entry.path()) || !entry.is_regular_file()) continue;

        // eliminer la lecture des fichiers dans include/
        std::string relPath = fs::relative(entry, RootDir).string();
        if (relPath.find("include/") != std::string::npos){
            continue;
        }
        // Checker si ce n'est pas un Linux executable
        fs::perms p = entry.status().permissions();
        // group et others exec au cas ou que c'est pas moi qui a creer la file
        bool isExecutable = ((p & fs::perms::owner_exec)  != fs::perms::none) ||
                            ((p & fs::perms::group_exec)  != fs::perms::none) ||
                            ((p & fs::perms::others_exec) != fs::perms::none);
        if (isExecutable) {
            continue;
        }

        std::string ext = entry.path().extension().string();
        // pour lire des fichiers lisible
        if (ext == ".o" || ext == ".bin" || ext == ".lib") continue;

        OutFile << "\"" << entry.path().filename().string() << "\" {\n";

        // assigner le contenu du fichier a une variable
        std::ifstream fileToRead(entry.path());
        std::string line;

        // copier les lignes du fichier et les formater au OutputFile
        while (std::getline(fileToRead, line)){
            OutFile<<"\t"<<line<<"\n";
        }
        OutFile<<"}\n\n";
    }
}
