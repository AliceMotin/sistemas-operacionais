#ifndef fs_h
#define fs_h
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

typedef struct {
    int verificar_nodo = 0;
    char nome[20];
    int proximo = 0;
} Nodo;

/**
 * @param arquivoDaLista nome do arquivo em disco que contem a lista encadeada
 * @param novoNome nome a ser adicionado apos depoisDesteNome
 * @param depoisDesteNome um nome presente na lista
 */
void adiciona(std::string arquivoDaLista, std::string novoNome, std::string depoisDesteNome)
{
    fstream arquivo{arquivoDaLista, ios::in | ios::out | ios::binary};
    if(!arquivo.is_open())
    {
        cout << "Erro ao abrir arquivo" << endl;
        exit(EXIT_FAILURE);
    }

    Nodo nodo;

    arquivo.read((char*) &nodo.verificar_nodo, sizeof(int));
    int primeiro = nodo.verificar_nodo;
    cout << "Primeira posição: " << primeiro << endl;

    int inicio = 4;
    int tam_arquivo = 284;
    int bloco_livre = 0;

    for(int i = inicio; i < tam_arquivo; i+=sizeof(nodo)){
        arquivo.seekg(i);
        arquivo.read((char*) &nodo, sizeof(nodo));
        if(nodo.verificar_nodo == 0){
            bloco_livre = i;
            cout << "Endereço do bloco livre: " << bloco_livre << endl;
            break;
        }
    }

    int novo_endereco;
    int prev_primeiro;
    int z_posicao = -1;
    int z_proximo = -1;

    while(primeiro >= 0 && primeiro < tam_arquivo){
        arquivo.seekg(primeiro);
        arquivo.read(reinterpret_cast<char*>(&nodo), sizeof(Nodo));
        string nome_atual(nodo.nome, 20);

        if(nome_atual.substr(0, depoisDesteNome.length()) == depoisDesteNome){
            z_posicao = primeiro;
            z_proximo = nodo.proximo;
            break;
        }
        primeiro = nodo.proximo;
    }

    if(z_posicao != -1 && z_proximo != -1){
        
        Nodo nodo_Anderson;
        nodo_Anderson.verificar_nodo = 1;
        strncpy(nodo_Anderson.nome, novoNome.c_str(), 20);
        nodo_Anderson.proximo = z_proximo;
        arquivo.seekp(bloco_livre);
        arquivo.write((char*) &nodo_Anderson, sizeof(nodo_Anderson));

        arquivo.seekg(z_posicao);
        arquivo.read((char*) &nodo, sizeof(nodo));
        nodo.proximo = bloco_livre;
        arquivo.seekp(z_posicao);
        arquivo.write((char*) &nodo, sizeof(nodo));
    }

    arquivo.close();
}

#endif /* fs_h */