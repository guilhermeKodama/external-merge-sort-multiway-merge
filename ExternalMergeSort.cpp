//
//  ExternalMegerSort.cpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//

#include "ExternalMergeSort.hpp"

#define MB_IN_BYTES (unsigned long long)1048576
#define GB_IN_BYTES (unsigned long long) 1073741824

using namespace std;

//incializando e declarando as variáveis estáticas
unsigned long long ExternalMergeSort::AVAILABLE_MEMORY = 1000;
int ExternalMergeSort::VIAS = 2;

ExternalMergeSort::ExternalMergeSort(string dataset_path,string output_path,int memory_in_mb,int vias){
    
    dataset = fopen(dataset_path.c_str(), "rb");
    AVAILABLE_MEMORY = (memory_in_mb * MB_IN_BYTES) / sizeof(int);
    VIAS = vias;
    this->output_path = output_path;
    
}

void ExternalMergeSort::sortFile(){
    //separa o arquivo muito grande em diversos arquivos menores ordenados que cabem na memória
    int numFiles = generateInitialFiles();
    
    //começa o processo de divisão em conquista onde esses arquivos iniciais serão intercalados em arquivos maiores ordenados de acordo com o
    // numero de vias especificado
    string *files = new string[ExternalMergeSort::VIAS];
    divideAndConquer(numFiles, files);
    
    /* realiza o multiwaymerge do ultimo grupo de arquivos para gerar o ultimo arquivo grande ordenado */
    multiWayMerge(output_path, files, ExternalMergeSort::VIAS, ExternalMergeSort::AVAILABLE_MEMORY/ExternalMergeSort::VIAS+1);
    
    delete[] files;
    
}

void ExternalMergeSort::binaryToText(string binary, string text ,int size){
    int *B = new int[size];
    FILE *fb = fopen(binary.c_str(),"rb");
    FILE *ft = fopen(text.c_str(), "w");
    
    fread(B, sizeof(int), size, fb);
    
    for(int i =0; i < size; i++){
        fprintf(ft, "%d\n",B[i]);
    }
    
    fclose(fb);
    fclose(ft);
}

int ExternalMergeSort::generateInitialFiles(){
    
    //alloca um buffer to tamanho da memoria disponível
    int *B = new int[ExternalMergeSort::AVAILABLE_MEMORY];
    
    int chunkFiles = 0;
    
    //lê o arquivo até o final
    while(!feof(dataset)){
        
        unsigned long read = fread(B,sizeof(int),ExternalMergeSort::AVAILABLE_MEMORY,dataset);
    
        //eu ordeno o meu buffer
        sort(B,B+read);
        
        string fileName = CHUNK_PATH+"chunk_"+to_string(chunkFiles);
        filesQueue.push(fileName);
        
        //salvo tudo em um arquivo/chunk do arquivo original
        salvaArquivo(fileName,B,read,0);
        
        chunkFiles++;

    }
    
    fclose(dataset);
    delete[] B;
    
    return chunkFiles;
    
}

string ExternalMergeSort::merge_sort(int size){
    
    if(size == 1){
        string file = filesQueue.front();
        filesQueue.pop();
        return file;
    }
    
    string *files = new string[VIAS];
    divideAndConquer(size, files);
    
     /* realizar multiwaymerge com os arquivos criados nas intancias que retornaram */
    
    string file = CHUNK_PATH + to_string(random());
    multiWayMerge(file,files,ExternalMergeSort::VIAS, ExternalMergeSort::AVAILABLE_MEMORY/ExternalMergeSort::VIAS+1);
    
    delete[] files;
    
   return file;
}

void ExternalMergeSort::multiWayMerge(string file_name,string *files,int way,int buffer_size){
    
    int *buffer = new int[buffer_size];
    
    chunk *chunks = new chunk[VIAS];
    
    for(int i = 0; i < VIAS; i++){
        
        chunks[i] = *new chunk();
        
        chunks[i].file = fopen(files[i].c_str(), "rb");
        chunks[i].MAX = 0;
        chunks[i].pos = 0;
        chunks[i].buffer = new int[ExternalMergeSort::AVAILABLE_MEMORY/ExternalMergeSort::VIAS+1];
        
        preencheBuffer(&chunks[i],ExternalMergeSort::AVAILABLE_MEMORY/ExternalMergeSort::VIAS+1);
        
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
    for(int i = 0; i < ExternalMergeSort::VIAS;i++){
        remove(files[i].c_str());
    }


}

void ExternalMergeSort::divideAndConquer(int size,string *files){
    int newSize = size/ExternalMergeSort::VIAS;
    int numFiles = 0;
    
    //checa se o numero de intancias é menor que o numero de vias
    if(newSize == 0){
        numFiles = size;
        newSize = 1;
    }else{
        numFiles = ExternalMergeSort::VIAS;
    }
    
    bool missing_value = (newSize * numFiles )!= size? true : false;
    for(int i = 0; i < numFiles; i++){
        if(i == 0){
            string file;
            if(missing_value){
                file = merge_sort(newSize+ (int)(size - (newSize * numFiles )));
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
         
        unsigned long read = fread(chunk->buffer, sizeof(int), buffer_size, chunk->file);
        chunk->MAX = read;
         
         if(feof(chunk->file)){
             fclose(chunk->file);
             chunk->file = NULL;
         }
        
         
     } catch (exception& e) {
         std::cerr << e.what() << '\n';
     }
}


void ExternalMergeSort::salvaArquivo(string fileName,int *B,unsigned long size,int breakLine){
    
    try {
        FILE *f = fopen(fileName.c_str(),"ab");
        
        fwrite(B,sizeof(int), size, f);
        
        fclose(f);
    } catch (std::ios_base::failure& e) {
        std::cerr << e.what() << '\n';
    }
    
    
}