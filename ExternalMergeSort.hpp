//
//  ExternalMegerSort.hpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama on 14/03/16.
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//


#ifndef ExternalMergeSort_hpp
#define ExternalMergeSort_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class ExternalMergeSort{

public:
    ExternalMergeSort(string path);

private:
    //referência para o arquivo original
    FILE *dataset;
    //fila dos arquivos iniciais geradores de acordo com o tamanho do buffer
    queue <string> filesQueue;
    
    const string CHUNK_PATH = "/Users/guilherme/Desktop/chunks/";
    struct chunk {
        FILE *file;
        int *buffer,MAX,pos;
    };
    string merge_sort(int size);
    void divideAndConquer(int size,string *files);
    void multiWayMerge(string file_name,string *files,int way,int buffer_size);
    int procuraMenor(chunk *chunks,int numArqs,int K,int *menor);
    void salvaArquivo(string fileName,int *B,unsigned long size,int breakLine);
    void preencheBuffer(chunk *chunk,int buffer_size);
    int generateInitialFiles();
    void binaryToText(string binary,string text,int size);
};

#endif /* ExternalMergeSort_hpp */
