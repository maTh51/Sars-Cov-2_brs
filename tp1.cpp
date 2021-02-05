#include <bits/stdc++.h>
//#include <sys/resource.h>
//#include <chrono>

using namespace std;
//using namespace std::chrono; 

struct no{
    int sub[2]; // Coordenadas da substring que ele representa no texto
    int tipo = 0; // 0 = interno, 1 = folha, 2 = folha-$
    vector<no *> filhos;   // vetor de filhos desse nó
};

vector<int> longest_r_sub(string T, no* atual, int num_c, int fim, vector<int> result){
    bool flag = false;
    for(int i = 0; i < atual->filhos.size(); i++){ 
        if(atual->filhos[i]->tipo == 0){    //checar se filho não é folha
            flag = true;
            fim = atual->filhos[i]->sub[1];
            int aux = num_c + atual->filhos[i]->sub[1] - atual->filhos[i]->sub[0] + 1;
            result = longest_r_sub(T, atual->filhos[i], aux, fim, result);
        }
    }

    if(!flag){
        if(num_c > result[0]){
            result.clear();    //trocar substring antiga pela nova, que é maior

            result.push_back(num_c);
            result.push_back(fim);
            for(int i = 0; i < atual->filhos.size(); i++){
                if(atual->filhos[i]->tipo == 1)
                    result.push_back(atual->filhos[i]->sub[0]);
                else{
                    result.push_back(atual->filhos[i]->sub[0]+(atual->sub[1]-atual->sub[0]+1));
                    //soma necessária por conta da notação da adaptação da Trie
                }
            }
        }
    }

    return(result);
    
}

void add_no(string T, no* atual, int ini){
    int char_num = 0;
    bool match_total = false;

    for(int i=atual->sub[0]; i<=atual->sub[1]; i++){  //Verificar quantos caracteres deram match
        if(T[i] == T[ini + char_num])
            char_num++;
        else
            break;
    }
    
    
    if(char_num == (atual->sub[1] - atual->sub[0] + 1)){ // Match total
        if(atual->tipo == 1){
            atual->tipo = 0;

            no* aux = new no;
            no* aux2 = new no;

            aux2->tipo = 2;
            aux2->sub[0] = atual->sub[0];
            aux2->sub[1] = atual->sub[1];
            atual->filhos.push_back(aux2); //equivalente ao nó com apenas $
            
            aux->tipo = 1;
            aux->sub[0] = ini + char_num;
            aux->sub[1] = T.length() - 1;
            atual->filhos.push_back(aux);

            atual->sub[0] = ini;
            atual->sub[1] = ini + char_num - 1;
        }
        else{
            int cont = -1;
            int i = 0;

            atual->sub[0] = ini;   //mantendo notação da Trie compacta
            atual->sub[1] = ini + char_num - 1;
            
            while(i < atual->filhos.size() && cont == -1){ //verificar matchs nos filhos
                if(atual->filhos[i]->tipo != 2 && T[atual->filhos[i]->sub[0]] == T[ini + char_num])
                    cont = i;
                
                i++;
            }
            if(cont != -1){  //match nos filhos -> chamar func recursivamente
                no* pas = new no;
                pas = atual->filhos[cont];
                add_no(T, pas, (ini+char_num));
            }
            else{   //sem match nos filhos, adicionar novo no como filho do atual
                no* aux = new no;
                aux->tipo = 1;
                aux->sub[0] = ini + char_num;
                aux->sub[1] = T.length() - 1;
                atual->filhos.push_back(aux);

            }
        }
    }
    else{   //Match incompleto
        //adicionando no intermediaro
        no* interm = new no;
        interm->sub[0] = atual->sub[0] + char_num;
        interm->sub[1] = atual->sub[1];
        for(int i=0; i<atual->filhos.size(); i++)
            interm->filhos.push_back(atual->filhos[i]);
        atual->filhos.clear();
        atual->filhos.push_back(interm);
        atual->sub[1] = atual->sub[0] + char_num -1;

        if(atual->tipo == 1){
            atual->tipo = 0;
            interm->tipo = 1;
        }
        //Adicionando novo no
        atual->sub[0] = ini;
        atual->sub[1] = ini + char_num - 1;
        
        no* aux = new no;
        aux->tipo=1;
        aux->sub[0] = ini + char_num;
        aux->sub[1] = T.length() - 1;
        atual->filhos.push_back(aux);
    }
}


int main(int argc, char *argv[]) {

    if( argc <= 1 ){  
        cerr << "Arquivo não informado" << endl;
        return -1;
    }

    ifstream input(argv[1]);
    if(!input.good()){
        cerr << "Erro ao abrir o arquivo: '"<< argv[1] << endl;
        return -1;
    }

    string line, cov;
    int tam=0;

    while( getline( input, line ) ){
        if( line.empty() || line[0] == '>' )
            continue;
        else 
            cov = cov + line; //tirando quebras de linha da string
    }

    no* Trie = new no;
    no* a = new no;

    Trie->sub[0] = -1;
    Trie->sub[1] = -1;

    a->sub[0] = cov.length() - 1;
    a->sub[1] = cov.length() - 1;
    a->tipo = 1;

    Trie->filhos.push_back(a);

    string T = cov;
    for(int i= T.length() - 2; i >= 0; i--){
        bool achou = false;
        for(int j = 0; j < Trie->filhos.size(); j++){
            if(T[Trie->filhos[j]->sub[0]] == T[i]){
                add_no(T, Trie->filhos[j], i);
                achou = true;
                break;
            }
        }
        if(!achou){ //adicionando manualmente primeira "leva" de filhos
            no* aux = new no;
            aux->tipo=true;
            aux->sub[0] = i;
            aux->sub[1] = T.length() - 1;
            Trie->filhos.push_back(aux);
        }
    }


    vector<int> v,u;
    v.push_back(0);
    v.push_back((cov.length()-1));
    u = longest_r_sub(T, Trie, 0, (cov.length()-1), v);


    cout << "A maior substring que se repete é a:" << endl;
    cout << T.substr((u[1] - u[0] + 1), u[0]) << endl << endl;
    cout << "As ocorrencias delas são nas seguintes posições:" << endl; 
    for(int i = 2; i<u.size(); i++)
        cout << (u[i]-u[0]) << ' ';
    cout << endl; 

    // struct rusage use;
    // if (0 == getrusage(RUSAGE_SELF, &use))
    // {
    //     printf("Max RSS: %ld\n", use.ru_maxrss);
    // }

    //auto start = high_resolution_clock::now();   
    //auto stop = high_resolution_clock::now(); 
    //auto duration = duration_cast<microseconds>(stop - start); 

    // cout << "Time taken by function: " << duration.count() << " microseconds" << endl; 

    return(0);
}