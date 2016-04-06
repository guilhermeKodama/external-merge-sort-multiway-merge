//
//  ExternalMegerSort.cpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama on 14/03/16.
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//

#include "ExternalMergeSort.hpp"

#define AVAILABLE_MEMORY 10
#define VIAS 3
#define FILE_SIZE 100000

using namespace std;

ExternalMergeSort::ExternalMergeSort(string path){
    
    string *files = new string[VIAS];
    divideAndConquer(FILE_SIZE, 0, files);
    
    /* realizar multiwaymerge com os arquivos criados nas intancias que retornaram */
    
    multiWayMerge("/Users/guilherme/Desktop/external_merge_sort/output.txt", files, VIAS, AVAILABLE_MEMORY/VIAS+1);
    
    delete[] files;

}

string ExternalMergeSort::merge_sort(int size,int ini){
    
    cout << "INI: " << ini << "SIZE: " << size <<endl;
    
    if(size <= AVAILABLE_MEMORY){
        string file = CHUNK_PATH + to_string(random());
        chunk *chunk = new struct chunk();
        fstream f("/Users/guilherme/Desktop/dataset.txt",ios::in);
        chunk->file = &f;
        chunk->file->seekg(ini);
        chunk->buffer = new int[size];
        preencheBuffer(chunk, size);
        sort(chunk->buffer,chunk->buffer+size);
        salvaArquivo(file,chunk->buffer,size,1);
        chunk->file->close();
        chunk->file = NULL;
        
       
        delete[] chunk->buffer;
        delete chunk;
        return file;
    }
    
    string *files = new string[VIAS];
    divideAndConquer(size, ini, files);
    
     /* realizar multiwaymerge com os arquivos criados nas intancias que retornaram */
    
    string file = CHUNK_PATH + to_string(random());
    multiWayMerge(file,files,VIAS, AVAILABLE_MEMORY/VIAS+1);
    
    delete[] files;
    
   return file;
}

void ExternalMergeSort::multiWayMerge(string file_name,string *files,int way,int buffer_size){
    
    int *buffer = new int[buffer_size];
    
    chunk *chunks = new chunk[VIAS];
    
    for(int i = 0; i < VIAS; i++){
        
        chunks[i] = *new chunk();
        
        chunks[i].file = new fstream(files[i],ios::in);
        chunks[i].MAX = 0;
        chunks[i].pos = 0;
        chunks[i].buffer = new int[AVAILABLE_MEMORY/VIAS+1];
        
        preencheBuffer(&chunks[i],AVAILABLE_MEMORY/VIAS+1);
        
        
    }
    
    
    //enquanto houver arquivos para processar
    int menor,qtdBuffer = 0;
    while(procuraMenor(chunks,way,buffer_size,&menor) == 1){
        buffer[qtdBuffer] = menor;
        qtdBuffer++;
        if(qtdBuffer == buffer_size){
            salvaArquivo(file_name,buffer,buffer_size,1);
            qtdBuffer = 0;
        }
    }
    
    //salva dados ainda no buffer
    if(qtdBuffer != 0){
        salvaArquivo(file_name,buffer,qtdBuffer,1);
    }
    
    for(int i =0; i < way;i++){
        delete chunks[i].file;
        delete[] chunks[i].buffer;
    }
    
    
    delete[] chunks;
    delete[] buffer;
    
    /*remove os arquivo de input*/
    for(int i = 0; i < VIAS;i++){
        remove(files[i].c_str());
    }


}

void ExternalMergeSort::divideAndConquer(int size,int ini,string *files){
    int newSize = size/VIAS;
    bool missing_value = (newSize * VIAS )!= size? true : false;
    for(int i = 0; i < VIAS; i++){
        if(i == 0){
            string file;
            if(missing_value){
                file = merge_sort(newSize+ (int)(size - (newSize * VIAS )),(size*i));
            }else{
                file = merge_sort(newSize,(size*i));
            }
            
            files[i] = (file);
        }else{
            string file = merge_sort(newSize,(size*i));
            files[i] = (file);
        }
        
    }

}


int ExternalMergeSort::procuraMenor(chunk *chunks, int numArqs, int K, int *menor){
    int idx = -1;
    
    for(int i=0; i < numArqs; i++){
        if(chunks[i].pos < chunks[i].MAX){
            if(idx == -1){
                idx = i;
            }else{
                if(chunks[i].buffer[chunks[i].pos] < chunks[idx].buffer[chunks[idx].pos])
                    idx = i;
            }
        }
    }
    
    if(idx != -1){
        *menor = chunks[idx].buffer[chunks[idx].pos];
        chunks[idx].pos++;
        if(chunks[idx].pos == chunks[idx].MAX)
            preencheBuffer(&chunks[idx], K);
        return 1;
    }else{
        return 0;
    }
}

void ExternalMergeSort::preencheBuffer(chunk *chunk,int buffer_size){
    
     try {
        if(chunk->file == NULL)
            return;
         
         if(chunk->file->fail())
             cout << "ERROR AO PREENCHER O BUFFER" << endl;
         
        chunk->pos = 0;
        chunk->MAX = 0;
        
        int number = 0;
         
        
        for(int i=0; i < buffer_size; i++){
            if(*chunk->file >> number){
                chunk->buffer[chunk->MAX] = number;
                chunk->MAX++;
            }else{
                chunk->file->close();
                delete chunk->file;
                chunk->file = NULL;
                return;
            }
         }
         
     } catch (exception& e) {
         std::cerr << e.what() << '\n';
     }
}


void ExternalMergeSort::salvaArquivo(string fileName,int *B,int size,int breakLine){
//    cout << fileName << endl;
    
    try {
        fstream file(fileName,ios::out | ios::app);
        
        if(file.fail())
            cerr << "Error ao salvar arquivo: " << strerror(errno) << endl;
        
        int pos = 0;
        while (pos < size-1) {
            file << B[pos] << "\n";
            pos++;
        }
        
        if (breakLine == 0) {
            file << B[size-1];
        }else{
            file << B[size-1] << "\n";
        }
        
        file.flush();
        file.close();
    } catch (std::ios_base::failure& e) {
        std::cerr << e.what() << '\n';
    }
    
    
}