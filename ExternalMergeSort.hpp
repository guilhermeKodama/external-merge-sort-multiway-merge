//
//  ExternalMegerSort.hpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama on 14/03/16.
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//

/*
 
 N = numero total de elementos que posso carregar na RAM
 K = numero máximo de elementos em cada arquivo
 F = numero total de arquivos gerados
 
 */

#ifndef ExternalMergeSort_hpp
#define ExternalMergeSort_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>

using namespace std;

class ExternalMergeSort{

public:
    ExternalMergeSort(string path);

private:
    const string CHUNK_PATH = "/Users/guilherme/Desktop/external_merge_sort/";
    struct chunk {
        fstream *file;
        int *buffer,MAX,pos;
    };
    string merge_sort(int size,int ini);
    void divideAndConquer(int size,int ini,string *files);
    void multiWayMerge(string file_name,string *files,int way,int buffer_size);
    int procuraMenor(chunk *chunks,int numArqs,int K,int *menor);
    void salvaArquivo(string fileName,int *B,int size,int breakLine);
    void preencheBuffer(chunk *chunk,int buffer_size);
};

#endif /* ExternalMergeSort_hpp */
