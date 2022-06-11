#include <stdio.h>
#include <stdlib.h>

// Struct - Carro
typedef struct{
    char cor[30];
    char placa[8]; // Tamanho 8 para ser possível armazenar no formato novo e antigo de placa.
    char marca[30];
    int ano;
}Carro;

// Struct - Proprietário
typedef struct{
    char nome[30];
    char cpf[14]; // Tamanho 14 para ser possível armazenar no seguinte formato: XXX.XXX.XXX-XX
    int qntd_carros;
    Carro carros[15]; // Número relativamente grande de carros, para que, assim, seja possivel o usuario cadastrar quantos carros quiser para um usuario
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

int imprimir_dados(){
    Proprietario prop;
    FILE *ponteiro = fopen("arquivo", "rb");
    if (!ponteiro){
        printf("\nErro ao tentar ler o arquivo (ja cadastrou alguem uma vez?)\n\n");
        return 0;
    }
    printf("----------- Proprietarios e Automoveis cadastrados -----------\n");
    while (fread(&prop, sizeof(Proprietario), 1, ponteiro)){
        printf("%s - %s\n", prop.nome, prop.cpf);
        printf("-> Automoveis desse Proprietario: \n");
        for(int i = 0; i < prop.qntd_carros; i++){
            printf("%s - %s - %s - %d\n", prop.carros[i].marca, prop.carros[i].cor, prop.carros[i].placa, prop.carros[i].ano);
        }
        printf("\n");
        fflush(stdin);
    }
    fclose(ponteiro);
    return 0;
}

Proprietario cadastrar_prop(int contador, Proprietario *prop){
    int n_auto;

    printf("----------- Cadastro de Proprietario -----------\n");
    printf("Informe o nome do proprietario: \n");
    scanf("%s%*c", &prop[contador].nome);
    printf("Informe o CPF do proprietario: \n");
    scanf("%s%*c", &prop[contador].cpf);
    printf("----------- Cadastro de Automoveis -----------\n");
    printf("Quantos automoveis esse proprietario tem? \n");
    scanf("%d", &n_auto);
    prop[contador].qntd_carros = n_auto;
    for(int i = 0; i < n_auto; i++){
        printf("\nCadastro do automovel %d desse proprietario\n", (i+1));
        printf("Informe a cor do automovel: \n");
        scanf("%s%*c", &prop[contador].carros[i].cor);
        printf("Informe a placa do automovel: \n");
        scanf("%s%*c", &prop[contador].carros[i].placa);
        printf("Informe a marca do automovel: \n");
        scanf("%s%*c", &prop[contador].carros[i].marca);
        printf("Informe o ano de adquirencia do automovel: \n");
        scanf("%d", &prop[contador].carros[i].ano);
        printf("Veiculo cadastrado!\n\n");
    }
    
    // Criação/Abertura de um arquivo binário para a escrita
    FILE *ponteiro = fopen("arquivo", "ab");
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