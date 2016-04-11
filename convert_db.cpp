//
//  convert_db.cpp
//  paa trabalho1
//
//  Created by Guilherme Kenji Kodama on 10/04/16.
//  Copyright © 2016 Guilherme Kenji Kodama. All rights reserved.
//
//  ====================================================================
//
//  COMPILAR : g++ -std=c++11 -o convert_db convert_db.cpp
//
//  USO :  convert_db <caminho_do_arquivo_de_input> <tipo {TEXT,BINARY}> <caminho_do_arquivo_de_output> <tipo {TEXT,BINARY}>
//
//  OBSERVAÇÃO :  se o arquivo de output existir ele será sobre-escrito

#include <stdio.h>
#include <iostream>


using namespace std;

void writeBinaryToText(string input,string output){
    
    FILE *f_input = fopen(input.c_str(),"rb");
    FILE * f_output = fopen(output.c_str(),"w");
    
    while(!feof(f_input)){
        int number = 0;
        
        fread(&number, sizeof(int), 1, f_input);
        
        fprintf(f_output, "%d ",number);
        
    }
    
    fflush(f_input);
    fclose(f_input);
    
    fflush(f_output);
    fclose(f_output);
    
}

void writeTextToBinary(string input,string output){
    FILE *f_input = fopen(input.c_str(),"r");
    FILE * f_output = fopen(output.c_str(),"wb");
    
    while(!feof(f_input)){
        int number = 0;
        
        fscanf(f_input,"%d ",&number);
        
        fwrite(&number, sizeof(int), 1, f_output);
        
    }
    
    fflush(f_input);
    fclose(f_input);
    
    fflush(f_output);
    fclose(f_output);
}

int main(int argc, const char * argv[]) {
    
    if (argc == 5) {
        
        string input(argv[1]);
        string type_input(argv[2]);
        
        string output(argv[3]);
        string type_output(argv[4]);
        
        cout << "INPUT_FILE : " << input << endl;
        cout << "TYPE_INPUT : " << type_input << endl;
        cout << "OUTPUT_FILE : " << output << endl;
        cout << "TYPE_OUTPUT : " << type_output << endl;
        
        if(toupper(type_input[0]) == 'T' && toupper(type_output[0]) == 'B'){
            
            writeTextToBinary(input, output);
            
        }else if(toupper(type_input[0]) == 'B' && toupper(type_output[0]) == 'T'){
            
            writeBinaryToText(input, output);
            
        }else{
            cout << "conversão de tipos incorreta, as únicas combinações possíveis são : TEXT -> BINARY ou BINARY -> TEXT" << endl;
        }
        
        
        
    }else{
        cout << "USO : convert_db <caminho_do_arquivo_de_input> <tipo {TEXT,BINARY}> <caminho_do_arquivo_de_output> <tipo {TEXT,BINARY}> " << endl;
    }

}



