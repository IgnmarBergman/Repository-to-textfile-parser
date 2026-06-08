#include <iostream>
#include <ostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "obj/RepoParser.h"

//ToText -c dir "message"
//ToText -t dir Outdir "message"
//ToText -cf dir "message" filename filename filename * // ou fc
//ToText -tf dir Outdir "message" filename filename filename * // ou ft

void PrintError(){
    std::cerr << "Erreur dans la structure de la commande : \nToText -flag *args\nDo : {ToText} to see options";
}

// argc est le nombre de param, argv est le contenu des param
int main(int argc, char* argv[]){

    // s'assurer que si le nombre de param depasse la limite un message d'erreur s'envoit
    if (argc == 2 || argc == 3) {
        PrintError();
        return 1;
    }else if (argc == 1){
        std::cout<<"\tGuide d'utilisation :\n\nStructure : ToText --flags *args\n\nflags:\n-c : copier le document de sortie au presse papier\n-t : enregistrer le document de sortie\n-cf : copier le documents et donner des fichiers ou dossiers a ne pas regarder\n-tf : enregistrer le documents et donner des fichiers ou dossiers a ne pas regarder\n";
        std::cout<<"\nExemples : \nToText -c dir \"message\"\nToText -t dir outfile.txt \"message\"\nToText -cf dir \"message\" \"filename.ext\" \"filename.ext\" [...]\nToText -tf dir outfile.txt \"message\" \"filename.ext\" \"filename.ext\" [...]\nNote! : -f flag marche pour les dossiers et fichiers\n";
    }else {
        std::string flag = argv[1];
        if (flag == "-c"){
            RepoParser brain(argv[2], "outfile.txt", argv[3]);
            brain.generateOutputFile();

            std::string Cflag = "cat outfile.txt | xclip -selection clipboard && rm -r outfile.txt";
            std::system(Cflag.c_str());
            std::cout<<"Copier au presse papier"<<std::endl;

        } else if (flag == "-t") {
            RepoParser brain(argv[2], argv[3], argv[4]);
            brain.generateOutputFile();
            std::cout<<"Fichier construit"<<std::endl;

        } else if (flag == "-cf" || flag == "fc"){
            std::vector<std::string> IgnoreList;
            if (argc<=4){
                PrintError();
            }
            for (int i=0; i<(argc- 4); i++){
                std::string current = argv[i+4];
                IgnoreList.push_back(current);
            }
            RepoParser brain(argv[2], "outfile.txt", argv[3]);
            brain.addIgnore(IgnoreList);
            brain.generateOutputFile();

            std::string Cflag = "cat outfile.txt | xclip -selection clipboard && rm -r outfile.txt";
            std::system(Cflag.c_str());
            std::cout<<"Copier au presse papier"<<std::endl;
        } else if(flag == "-tf" || flag == "-ft"){
            std::vector<std::string> IgnoreList;
            if (argc<=5){
                PrintError();
            }
            for (int i=0; i<(argc- 5); i++){
                std::string current = argv[i+5];
                IgnoreList.push_back(current);
            }

            RepoParser brain(argv[2], argv[3], argv[4]);
            brain.addIgnore(IgnoreList);
            brain.generateOutputFile();

            std::cout<<"Fichier construit"<<std::endl;
        } else {
            PrintError();
        }
    }

    return 0;
}