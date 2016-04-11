//
//  msort.cpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama on 14/03/16.
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//
//  ====================================================================
//
//  COMPILAR : g++ -std=c++11 -o msort msort.cpp ExternalMergeSort.cpp
//
//  USO : msort <caminho_do_dataset> <tamanho_da_memoria_em_MB> <numero_de_vias> <caminho arquivo final>

#include <stdio.h>
#include "ExternalMergeSort.hpp"

using namespace std;


int main(int argc, const char * argv[]) {
    
    
    if(argc == 5){
        
        string dataset_path(argv[1]);
        string output_path(argv[4]);
        int memory_in_mb = 1;
        int vias = 2;
        
        sscanf (argv[2],"%d",&memory_in_mb);
        sscanf (argv[3],"%d",&vias);
        
        ExternalMergeSort *mSort = new ExternalMergeSort(dataset_path,output_path,memory_in_mb,vias);
        mSort->sortFile();
        
    }else{
        
        cout << "USO : msort <caminho_do_dataset> <tamanho_da_memoria_em_MB> <numero_de_vias> <caminho arquivo final>" << endl;
        
    }
    
    
    
    return 0;
}

