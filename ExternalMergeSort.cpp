//
//  ExternalMegerSort.cpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//

#include "ExternalMergeSort.hpp"

#define AVAILABLE_MEMORY 10
#define VIAS 4
#define FILE_SIZE 1000

using namespace std;

ExternalMergeSort::ExternalMergeSort(string path){
    
    dataset = fopen("/Users/guilherme/Desktop/dataset.txt", "r");
    
    generateInitialFiles();
    
    
//    string *files = new string[VIAS];
//    divideAndConquer(FILE_SIZE, files);
//    
//    /* realizar multiwaymerge com os arquivos criados nas intancias que retornaram */
//    
//    multiWayMerge("/Users/guilherme/Desktop/external_merge_sort/output.txt", files, VIAS, AVAILABLE_MEMORY/VIAS+1);
//    
//    delete[] files;
//    fclose(dataset);

}

void ExternalMergeSort::generateInitialFiles(){
    
    cout << "TO AQUI"<<endl;
    
    //alloca um buffer to tamanho da memoria disponível
    int *B = new int[AVAILABLE_MEMORY];
    
    int number;
    int pos = 0;
    int chunkFiles = 0;
    
    //lê o arquivo até o final
    while(!feof(dataset)){
        
        fscanf (dataset, "%d\n", &number);
        
        //preencho o buffer com os valores do arquivo muito grande
        B[pos] = number;
        pos++;
        
        //se eu preenchi todo o meu buffer
        if(pos == AVAILABLE_MEMORY){
            
            //eu ordeno o meu buffer
            sort(B,B+AVAILABLE_MEMORY);
            
            string fileName = CHUNK_PATH+"chunk_"+to_string(chunkFiles);
            filesQueue.push(fileName);
            
            //salvo tudo em um arquivo/chunk do arquivo original
            salvaArquivo(fileName,B,pos,0);
            
            pos = 0;
            chunkFiles++;
            
        }
        
    }
    
    //salva o resto que ficou no buffer (rebarba)
    if(pos != 0){
        sort(B,B+AVAILABLE_MEMORY);
        
        string fileName = CHUNK_PATH+"chunk_"+to_string(chunkFiles);
        filesQueue.push(fileName);
        
        salvaArquivo(fileName,B,pos,0);
        
        chunkFiles++;
    }
    
    fclose(dataset);
    
}

string ExternalMergeSort::merge_sort(int size){
    
    cout << "SIZE: " << size <<endl;
    
    if(size <= AVAILABLE_MEMORY){
        
        //criar chunk
        chunk *chunk = new struct chunk();
        //arquivo
        string file = CHUNK_PATH + to_string(random());
        chunk->file = dataset;
        //carrega esse segmento para a memória
        chunk->buffer = new int[size];
        preencheBuffer(chunk, size);
        //ordena
        sort(chunk->buffer,chunk->buffer+size);
        //salva em vários arquivos menores que cabem na memoria principal
        salvaArquivo(file,chunk->buffer,size,1);
        //free file
//        fclose(chunk->file);
        //free chunk and buffer
        delete[] chunk->buffer;
        delete chunk;
        return file;
        
    }
    
    string *files = new string[VIAS];
    divideAndConquer(size, files);
    
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
        
        chunks[i].file = fopen(files[i].c_str(), "r");
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

void ExternalMergeSort::divideAndConquer(int size,string *files){
    int newSize = size/VIAS;
    bool missing_value = (newSize * VIAS )!= size? true : false;
    for(int i = 0; i < VIAS; i++){
        if(i == 0){
            string file;
            if(missing_value){
                file = merge_sort(newSize+ (int)(size - (newSize * VIAS )));
            }else{
                file = merge_sort(newSize);
            }
            
            files[i] = (file);
        }else{
            string file = merge_sort(newSize);
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
         
        chunk->pos = 0;
        chunk->MAX = 0;
        
        int number = 0;
         
        
        for(int i=0; i < buffer_size; i++){
            if(!feof(chunk->file)){
                fscanf (chunk->file, "%d\n", &number);
                chunk->buffer[chunk->MAX] = number;
                chunk->MAX++;
            }else{
                fclose(chunk->file);
                chunk->file = NULL;
                return;
            }
         }
         
     } catch (exception& e) {
         std::cerr << e.what() << '\n';
     }
}


void ExternalMergeSort::salvaArquivo(string fileName,int *B,int size,int breakLine){
    
    try {
        FILE *f = fopen(fileName.c_str(),"a");
        
        for(int i =0; i<size-1;i++){
            fprintf(f,"%d\n",B[i]);
        }
        
        if (breakLine == 0) {
            fprintf(f,"%d",B[size-1]);
        }else{
            fprintf(f,"%d\n",B[size-1]);
        }
        
        fclose(f);
    } catch (std::ios_base::failure& e) {
        std::cerr << e.what() << '\n';
    }
    
    
}