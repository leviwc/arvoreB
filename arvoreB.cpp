#include <bits/stdc++.h>

#define GRAUMINIMO 2
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
  Registro registers[maxNo];
  int pointers[maxNo+1];
  bool leaf;
  Node(){
    n = 0;
    memset(pointers,-1,sizeof(pointers));
    leaf = true;
  }
};
struct BTree{
  FILE *tree;
  BTree(){
    if(tree = fopen("tree","r+")){
      return;
    }
    tree = fopen("tree","w+");
    fclose(tree);
    tree = fopen("tree","r+");
  }
  void createNode(){
    fseek(tree, 0, SEEK_END);
    Node aux;
    fwrite(&aux, sizeof(Node), 1, tree);
  }
  void createNode(Node aux){
    fseek(tree,0,SEEK_END);
    fwrite(&aux, sizeof(Node),1,tree);
  }
  int size(){
    fseek(tree,0,SEEK_END); 
    return ftell(tree) / sizeof(Node);
  }
  Node& operator[](int pos) {
    Node aux;
    fseek(tree,pos * sizeof(Node),SEEK_SET);
    fread(&aux,sizeof(Node), 1, tree);
    return aux;
  } 
  void close(){
    fclose(tree);
  }
};
void sendAnc(BTree &bTree, int anc, int r, Registro reg){
  int &n = bTree[anc].n;
  for(int i = 0; i < n; i++){
    if(bTree[anc].registers[i].chave > reg.chave){
      swap(reg,bTree[anc].registers[i]);
      swap(r, bTree[anc].pointers[i+1]);
    }
  }
  bTree[anc].registers[n] = reg;
  n++;
}
int breakNode(BTree &bTree, int anc, int currentNode){
  int n = bTree[currentNode].n;
  int mid = bTree[currentNode].registers[n/2].chave;
  Node newNode;
  newNode.leaf = bTree[currentNode].leaf;
  newNode.n = n/2;
  for(int i = n/2+1; i < n; i++){
    newNode.registers[i-(n/2+1)] = bTree[currentNode].registers[i];
  }
  bTree.createNode(newNode);
  n/=2;
  if(currentNode == bTree.size()-2){
    Node newRoot;
    newRoot.leaf = false;
    newRoot.registers[0] = bTree[currentNode].registers[n/2];
    newRoot.n = 1;
    newRoot.pointers[0] = currentNode;
    newRoot.pointers[1] = bTree.size()-1;
    bTree.createNode(newRoot);
  }else{
    swap(bTree[bTree.size()-1],bTree[bTree.size()-2]);
    sendAnc(bTree,  anc,bTree[currentNode].pointers[n/2+1] , bTree[currentNode].registers[n/2]);
  }
  return mid;
}
bool insert(BTree &bTree, int anc, int currentNode, Registro reg){
  if(bTree.size() == 0){
    Node newNode;
    newNode.n = 1;
    newNode.registers[0] = reg;
    bTree.createNode(newNode);
    return;
  }
  if(bTree[currentNode].n == maxNo){
    int mid = breakNode(bTree, anc, currentNode);
    if(reg.chave< mid){
      return insert(bTree, anc, currentNode, reg);
    }else{
      return insert(bTree,anc,bTree.size()-2, reg);
    }
    return;
  }
  if(bTree[currentNode].leaf){
    for(int i = 0; i < bTree[currentNode].n; i++){
      if(bTree[currentNode].registers[i].chave < reg.chave){
        swap(reg, bTree[currentNode].registers[i]);
      }
    }
    bTree[currentNode].registers[bTree[currentNode].n] = reg;
    bTree[currentNode].n++;
    return true;
  }
  for(int i = 0; i < bTree[currentNode].n; i++){
    if(reg.chave == bTree[currentNode].registers[i].chave){
      return false;
    }
    if(reg.chave <bTree[currentNode].registers[i].chave){
      return insert(bTree,currentNode,bTree[currentNode].pointers[i],reg);
      
    }
  }
  return insert(bTree,currentNode,bTree[currentNode].pointers[bTree[currentNode].n],reg);
}

int main(){
  BTree bTree;

  
  bTree.close();
}