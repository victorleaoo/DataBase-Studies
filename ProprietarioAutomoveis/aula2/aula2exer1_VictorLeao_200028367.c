#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char placa[8]; // Tamanho 8 para ser possível armazenar no formato novo (CCCDCDD) e antigo de placa(CCC-DDDD).
    char renavam[11]; // Atualmente, 11, mas carros antigos tem 9. RENAVAM escolhido pois com ele é possível encontrar o chassi e outras informações relevantes acerca do carro.
}Carro;

typedef struct{
    char nome[30];
    char cpf[11]; // Apesar de ser um char de 11, haverá uma verificação durante o cadastro para que sejam aceitos somente números -> Chave primária/ordenação
    int qntd_carros;
    Carro carros[15]; // Número relativamente grande de carros, para que, assim, seja possivel o usuario cadastrar uma boa quantidade de carros para um usuario
}Proprietario;

// Menu inicial do Programa
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

// Verifica se o CPF é válido
int verifica_CPF(char cpf[11]){
    int cpf_valido = 0, cpf_igual;
    Proprietario prop;

    if(strlen(cpf) != 11){
        cpf_valido++;
    }

    // Verificação dos caractéres cadastrados (somente números são aceitos)
    for(int i = 0; i < 11; i++){
        if(cpf[i]<48 || cpf[i]>57){
            cpf_valido++;
        }
    }

    // Verifica se não há outro CPF igual esse já cadastrado (deve ser único, já que é uma chave que não se pode repetir)
    FILE *ponteiro = fopen("arquivo", "rb");
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

    // Maior que 0, inválido. 
    return cpf_valido;
}

int verifica_renavam(char renavam[11]){
    int renavam_valido = 0;

    if(strlen(renavam) < 9){
        renavam_valido++;
    }

    // Verificação dos caractéres cadastrados (somente números são aceitos)
    for(int i = 0; i < strlen(renavam); i++){
        if(renavam[i]<48 || renavam[i]>57){
            renavam_valido++;
        }
    }

    return renavam_valido;
}

// Função usada para que os proprietários sejam ordenados de acordo com CPF
int compare(const void * a, const void * b){
    const Proprietario * prop_1 = (Proprietario *) a;
    const Proprietario * prop_2 = (Proprietario *) b;

    return strcmp(prop_1 -> cpf, prop_2 -> cpf);
}

// Imprimir os dados no arquivo
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
            printf("%d) Placa: %s - RENAVAM: %s\n", i+1, prop.carros[i].placa, prop.carros[i].renavam);
        }
        printf("\n");
        fflush(stdin);
    }
    fclose(ponteiro);

    return 0;
}

// Cadastro de proprietário e seus carros
Proprietario cadastrar_prop(int contador, Proprietario *prop){
    int n_auto, cpf_valido, renavam_valido;
    char cpf[11], renavam[11];

    // Cadastro do proprietário e seus carros
    printf("----------- Cadastro de Proprietario -----------\n");
    printf("Informe o nome do proprietario: \n");
    scanf("%s%*c", &prop[contador].nome);

    do{
        printf("Informe o CPF do proprietario: \n");
        scanf("%s", &cpf);
        cpf_valido = verifica_CPF(cpf);
        if(cpf_valido == 0){
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

        do{
            printf("Informe o RENAVAM do automovel: \n");
            scanf("%s", &renavam);
            renavam_valido = verifica_renavam(renavam);
            if(renavam_valido == 0){
                strcpy(prop[contador].carros[i].renavam, renavam);
            } else{
                printf("Renavam invalido. Essas sao as possiveis razoes: caracteres nao aceitaveis ou tamanho invalido!\n");
            }
        }while(renavam_valido > 0);

        printf("Veiculo cadastrado!\n\n");
    }

    // Array de armazenamento dos proprietários cadastrados para a ordenação
    Proprietario prop_sort[100], prop_recuperar;
    
    // Criação/Abertura de um arquivo binário para a escrita
    FILE *cadastro_inicial = fopen("arquivo", "ab");
    fwrite(&prop[contador], sizeof(Proprietario), 1, cadastro_inicial);
    fclose(cadastro_inicial);

    // Escrita, agora, do arquivo seguindo a chave (CPF)
    int cont = 0;
    FILE *recuperar = fopen("arquivo", "rb");
    while (fread(&prop_recuperar, sizeof(Proprietario), 1, recuperar)){
        prop_sort[cont] = prop_recuperar;
        fflush(stdin);
        cont++;
    }
    // Ordenação por CPF
    qsort(prop_sort, cont, sizeof(Proprietario), compare);
    fclose(recuperar);

    // "Sobrescreve" o arquivo agora com os proprietários na sequência correta (seguindo CPF).
    FILE *cadastro_chave = fopen("arquivo", "wb");
    for(int i = 0; i < cont; i++){
        fwrite(&prop_sort[i], sizeof(Proprietario), 1, cadastro_chave);
    }
    fclose(cadastro_chave);
    
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