//
//  ExternalMegerSort.cpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama on 14/03/16.
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//

#include "ExternalMergeSort.hpp"

#define AVAILABLE_MEMORY 100
#define VIAS 2
#define FILE_SIZE 1000

using namespace std;

ExternalMergeSort::ExternalMergeSort(string path){
    
//    int numArqs = createKSortedFiles(path);
//    
//    int K = AVAILABLE_MEMORY/ (numArqs+1);
//    
//    remove(path.c_str());
//    merge(path, numArqs, K);
//    
//    for(int i = 0; i < numArqs;i++){
//        string novo =  CHUNK_PATH + "chunk_"+to_string(i);
//        remove(novo.c_str());
//    }
//
    
    string *files = new string[VIAS];
    int size = FILE_SIZE/VIAS;
    for(int i = 0; i < VIAS; i++){
        if(i == 0){
            string file = merge_sort(size,0,size-1,i+1);
            files[i] = (file);
        }else{
            string file = merge_sort(size,(size*i),((size)*i+1)-1,i+1);
            files[i] = (file);
        }
    }
    
    /* realizar multiwaymerge com os arquivos criados nas intancias que retornaram */
    
    multiWayMerge("/Users/guilherme/Desktop/external_merge_sort/output.txt", files, VIAS, AVAILABLE_MEMORY/VIAS+1);

}

string ExternalMergeSort::merge_sort(int size,int ini,int end,int h){
    
    cout << "H: " << h << "SIZE: " << size <<endl;
    
    if(size <= AVAILABLE_MEMORY){
        string file = CHUNK_PATH + to_string(h);
        chunk *chunk = new struct chunk();
        fstream f("/Users/guilherme/Desktop/dataset.txt",ios::in);
        chunk->file = &f;
        chunk->file->seekg(ini);
        chunk->buffer = new int[size];
        preencheBuffer(chunk, size);
        sort(chunk->buffer,chunk->buffer+size);
        salvaArquivo(file,chunk->buffer,size,1);
        free(chunk->buffer);
        chunk->file->close();
        chunk->file = NULL;
        free(chunk);
        return file;
    }
    
    string *files = new string[VIAS];
    divideAndConquer(size, ini, h, files);
    
     /* realizar multiwaymerge com os arquivos criados nas intancias que retornaram */
    
    string file = CHUNK_PATH + to_string(h);
    multiWayMerge(file,files,VIAS, AVAILABLE_MEMORY/VIAS+1);
    
    
   return file;
}

void ExternalMergeSort::multiWayMerge(string file_name,string *files,int way,int buffer_size){
    
    cout << "VOU FAZER O MULTIWAY MERGE" << endl;
    
    int *buffer = new int[buffer_size];
    
    chunk *chunks = new chunk[VIAS];
    
    for(int i = 0; i < VIAS; i++){
        cout << "FILE: " << files[i] << endl;
        
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
        cout << "ENCONTREI O MENOR: "<< menor << endl;
        buffer[qtdBuffer] = menor;
        qtdBuffer++;
        if(qtdBuffer == buffer_size){
            cout << " VOU SALVAR NO ARQUIVO: "<<file_name<< endl;
            salvaArquivo(file_name,buffer,buffer_size,1);
            qtdBuffer = 0;
        }
    }
    
    //salva dados ainda no buffer
    if(qtdBuffer != 0){
        cout << " VOU SALVAR A REBARBA NO ARQUIVO: "<<file_name<< endl;
        salvaArquivo(file_name,buffer,qtdBuffer,1);
    }
    
    cout << "LIBERANDO GERAL "<<endl;
    for(int i =0; i < way;i++)
        free(chunks[i].buffer);
    
    free(chunks);
    free(buffer);
    
    /*remove os arquivo de input*/
    for(int i = 0; i < VIAS;i++){
        remove(files[i].c_str());
    }

}

void ExternalMergeSort::divideAndConquer(int size,int ini,int h,string *files){
    int newSize = size/VIAS;
    bool impar = size % 2 != 0? true : false;
    for(int i = 0; i < VIAS; i++){
        if(i == 0){
            string file;
            if(impar){
                file = merge_sort(newSize+1,ini,size-1,h*10+i);
            }else{
                file = merge_sort(newSize,ini,size-1,h*10+i);
            }
            
            files[i] = (file);
        }else{
            string file = merge_sort(newSize,(size*i),((size)*i+1)-1,h*10+i);
            files[i] = (file);
        }
        
    }

}

//void ExternalMergeSort::merge(string nome,int numArqs,int K){
//    
//    int *buffer = new int[K];
//    
//    chunk *arq = new chunk[numArqs];
//    
//    for(int i = 0; i < numArqs; i++){
//        string novo = CHUNK_PATH+"chunk_"+to_string(i);
//        fstream f(novo,ios::in);
//        
//        arq[i] = *new chunk();
//        
//        arq[i].file = &f;
//        arq[i].MAX = 0;
//        arq[i].pos = 0;
//        arq[i].buffer = new int[K];
//        
//        preencheBuffer(&arq[i],K);
//        
//    }
//    
//    //enquanto houver arquivos para processar
//    int menor,qtdBuffer = 0;
//    while(procuraMenor(arq,numArqs,K,&menor) == 1){
//        buffer[qtdBuffer] = menor;
//        qtdBuffer++;
//        if(qtdBuffer == K){
//            salvaArquivo(nome,buffer,K,1);
//            qtdBuffer = 0;
//        }
//    }
//    
//    //salva dados ainda no buffer
//    if(qtdBuffer != 0){
//        salvaArquivo(nome,buffer,qtdBuffer,1);
//    }
//    
//    
//    for(int i=0; i < numArqs; i++)
//        free(arq[i].buffer);
//    
//    free(arq);
//    free(buffer);
//}

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
        
         cout << "TELL: " << chunk->file->tellg() << endl;
         
         if(chunk->file->fail())
             cout << "ERROR AO PREENCHER O BUFFER" << endl;
         
        chunk->pos = 0;
        chunk->MAX = 0;
        
        int number = 0;
         
        
        for(int i=0; i < buffer_size; i++){
            if(*chunk->file >> number){
                cout << number <<endl;
                chunk->buffer[chunk->MAX] = number;
                chunk->MAX++;
            }else{
                chunk->file->close();
                chunk->file = NULL;
                return;
            }
         }
         
     } catch (exception& e) {
         std::cerr << e.what() << '\n';
     }
    
    cout << "TELL: " << chunk->file->tellg() << endl;
}

int ExternalMergeSort::createKSortedFiles(string path){
    
    fstream file (path,ios::in);
    
    if(file.fail())
        cerr << "Error: " << strerror(errno);
    
    //alloca um buffer to tamanho da memoria disponível
    int *B = new int[AVAILABLE_MEMORY];
    
    int number;
    int pos = 0;
    int chunkFiles = 0;
    
    //lê o arquivo até o final
    while(file >> number){
        
        //preencho o buffer com os valores do arquivo muito grande
        B[pos] = number;
        pos++;
        
        //se eu preenchi todo o meu buffer
        if(pos == AVAILABLE_MEMORY){
            
            //eu ordeno o meu buffer
            sort(B,B+AVAILABLE_MEMORY);
            
            string fileName = CHUNK_PATH+"chunk_"+to_string(chunkFiles);
            
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
        
        salvaArquivo(fileName,B,pos,0);
        
        chunkFiles++;
    }
    
    file.close();
    
    return chunkFiles;

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