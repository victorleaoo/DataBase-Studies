#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct - Carro
typedef struct{
    char placa[8]; // Tamanho 8 para ser possível armazenar no formato novo e antigo de placa.
    char renavam[11]; // Atualmente, 11, mas carros antigos tem 9
}Carro;

// Struct - Proprietário
typedef struct{
    char nome[30];
    char cpf[11]; // Apesar de ser um char de 11, haverá uma verificação durante o cadastro para que sejam aceitos somente números -> Chave primária/ordenação
    int qntd_carros;
    Carro carros[15]; // Número relativamente grande de carros, para que, assim, seja possivel o usuario cadastrar bastente carros para um usuario
}Proprietario;

int menu(){
    int op;
    printf("----------- Proprietario de Automoveis -----------\n\n");
    printf("1 - Cadastrar Proprietario e seus Automoveis\n");
    printf("2 - Listar Proprietarios e Automoveis cadastrados\n");
    printf("3 - Finalizar programa\n");
    printf("Escreva o numero relativo a sua opcao: \n");
    scanf("%d", &op);

    return op;
}

int verifica_CPF(char cpf[11]){
    int cpf_valido = 0, cpf_igual;
    Proprietario prop;
    for(int i = 0; i < 11; i++){
        if(cpf[i]<48 || cpf[i]>57){ //Inválido
            cpf_valido++;
        }
    }
    FILE *ponteiro = fopen("teste", "rb");
    if (!ponteiro){
        return cpf_valido;
    } else{
        while (fread(&prop, sizeof(Proprietario), 1, ponteiro)){
            cpf_igual = strcmp(prop.cpf, cpf);
            if(cpf_igual == 0){
                cpf_valido++;
            }
            fflush(stdin);
        }
        fclose(ponteiro);
    }

    return cpf_valido;
}

int compare(const void * a, const void * b){
    const Proprietario * cpf_1 = (Proprietario *) a;
    const Proprietario * cpf_2 = (Proprietario *) b;

    return strcmp(cpf_1 -> cpf, cpf_2 -> cpf);
}

int imprimir_dados(){
    Proprietario prop;
    Proprietario prop_arm[100];
    int cont = 0;
    FILE *ponteiro = fopen("teste", "rb");
    if (!ponteiro){
        printf("\nErro ao tentar ler o arquivo (ja cadastrou alguem uma vez?)\n\n");
        return 0;
    }
    printf("----------- Proprietarios e Automoveis cadastrados -----------\n");
    while (fread(&prop, sizeof(Proprietario), 1, ponteiro)){
        prop_arm[cont] = prop;
        fflush(stdin);
        cont++;
    }
    qsort(prop_arm, cont, sizeof(Proprietario), compare);
    for(int i = 0; i < cont; i++){
        printf("%s - %s\n", prop_arm[i].nome, prop_arm[i].cpf);
        printf("-> Automoveis desse Proprietario: \n");
        for(int j = 0; j < prop_arm[i].qntd_carros; j++){
            printf("%d) Placa: %s - RENAVAM: %s\n", j+1, prop_arm[i].carros[j].placa, prop_arm[i].carros[j].renavam);
        }
        printf("\n");
    }
    fclose(ponteiro);
    return 0;
}

Proprietario cadastrar_prop(int contador, Proprietario *prop){
    int n_auto, cpf_valido;
    char cpf[11];

    printf("----------- Cadastro de Proprietario -----------\n");
    printf("Informe o nome do proprietario: \n");
    scanf("%s%*c", &prop[contador].nome);
    do{
        printf("Informe o CPF do proprietario: \n");
        scanf("%s", &cpf);
        cpf_valido = verifica_CPF(cpf);
        if(cpf_valido == 0 && strlen(cpf) == 11){
            strcpy(prop[contador].cpf, cpf);
        } else{
            printf("CPF invalido. Essas sao as possiveis razoes: caracteres nao aceitaveis, tamanho invalido ou cpf ja cadastrado!\n");
        }
        
    } while(cpf_valido > 0);
    printf("----------- Cadastro de Automoveis -----------\n");
    printf("Quantos automoveis esse proprietario tem? \n");
    scanf("%d", &n_auto);
    prop[contador].qntd_carros = n_auto;
    for(int i = 0; i < n_auto; i++){
        printf("\nCadastro do automovel %d desse proprietario\n", (i+1));
        printf("Informe a placa do automovel: \n");
        scanf("%s%*c", &prop[contador].carros[i].placa);
        printf("Informe o RENAVAM do automovel: \n");
        scanf("%s%*c", &prop[contador].carros[i].renavam);
        printf("Veiculo cadastrado!\n\n");
    }
    
    // Criação/Abertura de um arquivo binário para a escrita
    FILE *ponteiro = fopen("teste", "ab");
    fwrite(&prop[contador], sizeof(Proprietario), 1, ponteiro);
    fclose(ponteiro);
    
    printf("Proprietario e seus automoveis cadastrados com sucesso! Caso queira cadastrar outro proprietario e seus carros, selecione a opcao 1 mais uma vez.\n\n");
}

int main(){
    int op, contador = 0;
    Proprietario prop[100]; // Número relativamente grande de proprietários, para que, assim, o usuario possa cadastrar quantas pessoas e carros quiser.

    do{
        op = menu();
        switch (op)
        {
        case 1:
            cadastrar_prop(contador, prop);
            contador ++;
            break;
        case 2:
            imprimir_dados();
            break;
        case 3:
            printf("Obrigado por usar!\n");
            return 0;
        default:
            printf("\nOpcao Invalida! Escolha entre 1 e 3.\n\n");
            break;
        }
    } while(op != 3);

    return 0;
}