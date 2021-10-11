#include <bits/stdc++.h>
#define GRAUMINIMO 2
#define MAXNUMREGS 100
//t da arvore 
const int minNo = GRAUMINIMO-1;
const int maxNo = 2 * GRAUMINIMO-1;
using namespace std;



struct Registro{
    int chave;
    char nome[20];
    int idade;
};
struct Node{
  int n;
  int array[2*maxNo+1];
  bool leaf;
  Node(){
    n = 0;
    memset(array,-1,sizeof(array));
    leaf = false;
  }
};
struct ArvoreB{
  int raiz;
  int ultimoNo;
  FILE *arvore;
  ArvoreB(){
    raiz = 0;
    ultimoNo = 0;
    if(arvore = fopen("arvore","r+")){
      return;
    }
    arvore = fopen("arvore","w+");
    Node aux;
    for(int i = 0; i < MAXNUMREGS/maxNo; i++){
      fwrite(&aux, sizeof(Node), 1, arvore);
    }
    fclose(arvore);
    arvore = fopen("arvore","r+");
  }
  Node& operator[](int pos) {
    Node aux;
    fseek(arvore,pos * sizeof(Node),SEEK_SET);
    fread(&aux,sizeof(Node), 1, arvore);
    return aux;
  } 
  void close(){
    fclose(arvore);
  }
};
void insert(){

}

void makeFile(FILE *f) {
  f = fopen("dados", "w+");

  Registro r;
  for (int i = 0; i < MAXNUMREGS; i++) {
    fwrite(&r, sizeof(Registro), 1, f);
  }
  fclose(f);
}
int main(){
  FILE *f;
  if (!(f = fopen("dados", "r+"))) {
      makeFile(f);
      f = fopen("dados", "r+");
  }
  ArvoreB arvoreB;

  fclose(f);
  arvoreB.close();
}