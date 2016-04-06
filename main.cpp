//
//  main.cpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama on 14/03/16.
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "ExternalMergeSort.hpp"

using namespace std;

const long FILE_SIZE = 100000; // 10 GB 100000000000
const string DATASET_PATH = "/Users/guilherme/Desktop/dataset.txt";



void createDataSet(string path){
    fstream file (path,ios::out | ios::in | ios::app);
    
    if(file.fail())
        cerr << "Error: " << strerror(errno);
    
    if(file.tellg() == 0){
        cout << "Arquivo está vazio" << endl;
        
        int min = 10;
        int max = 1000000;
        
        for (int i = 0; i < FILE_SIZE-1; i++){
            file << min + (rand() % (int)(max - min + 1)) << "\n";
        }
        
        file << min + (rand() % (int)(max - min + 1));
        
        file.flush();
        
    }
    
    file.close();
}


int main(int argc, const char * argv[]) {
    
//    createDataSet(DATASET_PATH);
    
    ExternalMergeSort *mSort = new ExternalMergeSort(DATASET_PATH);
    
    return 0;
}

