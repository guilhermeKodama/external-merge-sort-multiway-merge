//
//  main.cpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama on 14/03/16.
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "ExternalMergeSort.hpp"

using namespace std;

const long FILE_SIZE = 4294967296/4;
const string DATASET_PATH = "/Users/guilherme/Desktop/dataset.txt";



void createDataSet(string path){
    
    FILE *file = fopen(DATASET_PATH.c_str(),"ab");
    
        
    int min = 10;
    int max = 1000000;
    
    for (int i = 0; i < FILE_SIZE; i++){
        
        int number = min + (rand() % (int)(max - min + 1));
        fwrite(&number, sizeof(int),1,file);
    
    }
    
    fflush(file);
    fclose(file);
}


int main(int argc, const char * argv[]) {
    
//    createDataSet(DATASET_PATH);
    
    ExternalMergeSort *mSort = new ExternalMergeSort(DATASET_PATH);
    
    return 0;
}

