//
//  generate_db.cpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama on 09/04/16.
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//
//  ====================================================================
//
//  COMPILAR : g++ -std=c++11 -o generate_db generate_db.cpp
//
//  USO :  generate_db <caminho_do_arquivo> <tamanho_do_arquivo_em_gigabytes>

#include <stdio.h>
#include <iostream>

#define GB_IN_BYTES (unsigned long long) 1073741824

using namespace std;

int main(int argc, const char * argv[]) {
    
    if(argc == 3){
        
        string database_path(argv[1]);
        unsigned long long file_size = 4;
        unsigned long long num_gb = 4;
        
        sscanf (argv[2],"%lld",&num_gb);
        
        //faz a conversão de quantos bytes tem na quantidade de gigas que o usuário digitou , dividido pelo tamanho de cada elemento
        file_size = ((num_gb * GB_IN_BYTES) / sizeof(int));
        
        cout << "======================" << endl;
        cout << "DATASET PATH : " << database_path << endl;
        cout << "DATASET SIZE (elementos) : "<< file_size << endl;
        cout << "DATASET SIZE (bytes) : "<< (num_gb * GB_IN_BYTES) << endl;
        cout << "DATASET SIZE (GB) : "<< num_gb << endl;
        cout << "======================" << endl;
        
        
        
        FILE *file = fopen(database_path.c_str(),"ab");
        
        
        int min = 10;
        int max = 1000000;
        
        for (unsigned long long i = 0; i < file_size; i++){
            
            int number = min + (rand() % (int)(max - min + 1));
            fwrite(&number, sizeof(int),1,file);
            
        }
        
        fflush(file);
        fclose(file);

    }else{
        cout << "USO : generate_db <caminho_do_arquivo> <tamanho_do_arquivo_em_gigabytes>" << endl;
    }
    
    
    return 0;
}