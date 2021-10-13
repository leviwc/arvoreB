#include <bits/stdc++.h>

#define GRAUMINIMO 2
const int minNo = GRAUMINIMO-1;
const int maxNo = 2 * GRAUMINIMO-1;

using namespace std;

struct Registro{
    int chave;
    char nome[20];
    int idade;
    Registro(){
      idade = chave = -1;
    }
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
  Node operator[](int pos) {
    Node aux;
    fseek(tree,pos * sizeof(Node),SEEK_SET);
    fread(&aux,sizeof(Node), 1, tree);
    return aux;
  }
  void write(int pos, Node aux){
    fseek(tree,pos * sizeof(Node),SEEK_SET);
    fwrite(&aux,sizeof(Node),1,tree);
  }
  int root(){
    return this->size() - 1;
  }
  void close(){
    fclose(tree);
  }
};
void swapNode(BTree &bTree, int pos1, int pos2){
  Node node1 = bTree[pos1], node2 = bTree[pos2];
  bTree.write(pos2, node1), bTree.write(pos1,node2);
}
void sendAnc(BTree &bTree, int anc, int r, Registro reg){
  // cout << "sendAnc" << reg.chave << " " << anc << " " << r <<endl;
  Node changesMade = bTree[anc]; 
  int &n = changesMade.n;
  for(int i = 0; i < n; i++){
    if(changesMade.registers[i].chave > reg.chave){
      swap(reg,changesMade.registers[i]);
      swap(r, changesMade.pointers[i+1]);
    }
  }
  changesMade.registers[n] = reg;
  changesMade.pointers[n+1] = r;
  n++;
  bTree.write(anc, changesMade);
}
int breakNode(BTree &bTree, int anc, int currentNode){
  // cout << "breakNode :" << currentNode << " " << anc << endl;
  Node changesMade = bTree[currentNode];
  int &n = changesMade.n;
  int mid = changesMade.registers[n/2].chave;
  Node newNode;
  newNode.leaf = changesMade.leaf;
  newNode.n = n/2;
  for(int i = n/2+1; i < n; i++){
    newNode.registers[i-(n/2+1)] = changesMade.registers[i];
  }
  bTree.createNode(newNode);
  n/=2;
  bTree.write(currentNode,changesMade);
  if(currentNode == bTree.size()-2){
    Node newRoot;
    newRoot.leaf = false;
    newRoot.registers[0] = changesMade.registers[n];
    newRoot.n = 1;
    newRoot.pointers[0] = currentNode;
    newRoot.pointers[1] = bTree.size()-1;
    bTree.createNode(newRoot);
  }else{
    swapNode(bTree,bTree.size()-1,bTree.size()-2);
    sendAnc(bTree,  bTree.size()-1,bTree.size()-2 , changesMade.registers[n]);
  }
  return mid;
}
bool insert(BTree &bTree, int anc, int currentNode, Registro reg){
  // cout << "insert: " << currentNode << " " << anc <<endl;
  // for(int i = 0; i < bTree[currentNode].n; i++){
  //   cout << bTree[currentNode].registers[i].chave << " ";
  // }
  // cout<<endl;
  if(bTree.size() == 0){
    Node newNode;
    newNode.n = 1;
    newNode.registers[0] = reg;
    bTree.createNode(newNode);
    return true;
  }
  if(bTree[currentNode].n == maxNo){
    int mid = breakNode(bTree, anc, currentNode);
    int newAnc = (anc == -1 ? 2 : anc);
    if(reg.chave< mid){
      return insert(bTree, newAnc, currentNode, reg);
    }else{
      return insert(bTree,newAnc,bTree.size()-2, reg);
    }
  }
  if(bTree[currentNode].leaf){
    Node changesMade = bTree[currentNode];
    for(int i = 0; i < bTree[currentNode].n; i++){
      if(changesMade.registers[i].chave == reg.chave){
        return false;
      }
      if(changesMade.registers[i].chave > reg.chave){
        swap(reg, changesMade.registers[i]);
      }
    }
    changesMade.registers[changesMade.n] = reg;
    changesMade.n++;
    bTree.write(currentNode, changesMade);
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
Registro consulta(BTree &bTree,int currentNode, int chave){
  if(currentNode == -1){
    return Registro();
  }
  int n = bTree[currentNode].n;
  for(int i = 0; i < n; i++){
    Registro aux = bTree[currentNode].registers[i];
    if(aux.chave == chave){
      return aux;
    }else if(chave < aux.chave){
      return consulta(bTree,bTree[currentNode].pointers[i],chave);
    }
  }
  return consulta(bTree,bTree[currentNode].pointers[n],chave);
}
void printO(BTree &bTree, int currentNode){
  if(currentNode == -1){
    return;
  }
  for(int i = 0; i < bTree[currentNode].n; i++){
    printO(bTree, bTree[currentNode].pointers[i]);
    cout << bTree[currentNode].registers[i].chave << endl;
  }
  printO(bTree,bTree[currentNode].pointers[bTree[currentNode].n]);
}
void printL(BTree &bTree){
  int sequenceNumber = 1;
  queue<pair<int,int> > nodes;
  nodes.push({bTree.size()-1, 1});
  while(!nodes.empty()){
    Node aux = bTree[nodes.front().first];
    int sequenceAux = nodes.front().second;
    nodes.pop();
    printf("No: %d:", sequenceAux);
    for(int i = 0; i < aux.n; i++){
      cout << " apontador: ";
      if(aux.pointers[i] == -1){
        cout << "null ";
      }else{
        cout << ++sequenceNumber << " ";
        nodes.push({aux.pointers[i],sequenceNumber});
      }
      cout << "chave: "<< aux.registers[i].chave;
    }
    cout << " apontador: ";
    if(aux.pointers[aux.n] == -1){
      cout << "null";
    }else{
      cout << ++sequenceNumber;
        nodes.push({aux.pointers[aux.n],sequenceNumber});
    }
    cout<<endl;
  }
}
int somN(int currentNode, BTree &bTree){
  if(currentNode == -1) return 0;
  int sum = bTree[currentNode].n;
  for(int i = 0; i < bTree[currentNode].n+1; i++){
    sum += somN(bTree[currentNode].pointers[i],bTree);
  }
  return sum;
}
int main(){
  BTree bTree;
  while(true){
    char operation;
    cin >> operation;
    if(operation == 'i'){
      Registro user;
      cin >> user.chave;
      cin.ignore();
      scanf("%[^\n]", user.nome);
      cin >> user.idade;
      if(insert(bTree,-1,bTree.root(),user)){
        cout <<  "insercao com sucesso: " << user.chave << endl;
      }else{
        cout << "chave ja existente: "<< user.chave << endl;
      }
    }else if(operation == 'c'){
      int chave;
      cin >> chave;
      Registro aux = consulta(bTree, bTree.root(), chave);
      if(aux.chave == -1){
        cout << "chave nao encontrada: "<<chave << endl;
      }else{
        cout << "chave: " << chave<<endl;
        cout << aux.nome << endl << aux.idade << endl;
      }
    }else if(operation == 'p'){
     if(bTree.size() == 0){
        cout << "árvore vazia"<<endl;
      }else{
        printL(bTree);
      }
    }else if(operation == 't'){
      cout <<fixed << setprecision(1) << "taxa de ocupacao: " << somN(bTree.root(),bTree) / (double)(maxNo * bTree.size()) << endl;
    
    }else if(operation == 'o'){
      if(bTree.size() == 0){
        cout << "árvore vazia"<<endl;
      }else{
        printO(bTree,bTree.root());
      }
    }else if(operation == 'e'){
      break;
    }
  }
  bTree.close();
}