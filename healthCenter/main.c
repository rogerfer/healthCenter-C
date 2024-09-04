#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define MAX_NAME_LENGTH 50



//Estrutura para utentes
typedef struct Utente
{
    int codigoUtente;
    char nome[MAX_NAME_LENGTH];
    int codigoFamilia;
    struct Utente* proximo;
} Utente;

//estrutura para medicos
typedef struct Medico
{
    int codigoDoutor;
    char nome[MAX_NAME_LENGTH];
    Utente* fila;
    struct Medico* proximo;
} Medico;

//funcao para adicionar UTENTES
Utente* adicionarUtente(int codigoUtente, const char* nome, int codigoFamilia)
{
    Utente* utente = (Utente*)malloc(sizeof(Utente));
    utente->codigoUtente = codigoUtente;
    strncpy(utente->nome, nome, MAX_NAME_LENGTH);
    utente->codigoFamilia = codigoFamilia;
    utente->proximo = NULL;
    return utente;
}
//funcao para adicionar MEDICOS
Medico* adicionarMedico(int codigoDoutor, const char* nome)
{
    Medico* medico = (Medico*)malloc(sizeof(Medico));
    medico->codigoDoutor = codigoDoutor;
    strncpy(medico->nome, nome, MAX_NAME_LENGTH);
    medico->fila = NULL;
    medico->proximo = NULL;
    return medico;
}

//funcao para remover utentes
void RemoverUtente(int codigoUtente, Medico* medico)
{
    Utente* utenteAnterior = NULL;
    Utente* utenteAtual = medico->fila;
    while (utenteAtual != NULL)
    {
        if (utenteAtual->codigoUtente == codigoUtente)
        {
            if (utenteAnterior == NULL)
            {
                medico->fila = utenteAtual->proximo;
            }
            else
            {
                utenteAnterior->proximo = utenteAtual->proximo;
            }
            free(utenteAtual);
            break;
        }
        utenteAnterior = utenteAtual;
        utenteAtual = utenteAtual->proximo;
    }
}

//funcao para colocar utentes em fila de espera
void utenteEmFila(Utente** frenteFila, Utente* utente)
{
    if (*frenteFila == NULL)
    {
        *frenteFila = utente;
    }
    else
    {
        Utente* corrente = *frenteFila;
        while (corrente->proximo != NULL)
        {
            corrente = corrente->proximo;
        }
        corrente->proximo = utente;
    }
}

//remove o proximo utente da fila do medico


//Imprime Utente
void ImprimeUtente(const Utente* utente)
{
    printf("Código de utente: %d\n", utente->codigoUtente);
    printf("Nome: %s\n", utente->nome);
    printf("Código de familía: %d\n", utente->codigoFamilia);
}

//Imprime Lista de espera



//funcao imprimir utentes por medico
void ImprimirUtentesPorMedico(const Medico* medicos)
{
    if (medicos == NULL)
    {
        printf("Médico não encontrado.\n");
        return;
    }
    int codigoDoutor;
    printf("Introduza código de médico: ");
    scanf("%d", &codigoDoutor);

    Medico* medicoAtual = medicos;
    while (medicoAtual != NULL)
    {
        if (medicoAtual->codigoDoutor == codigoDoutor)
        {
            if (medicoAtual->fila == NULL)
            {
                printf("Utentes não encontrados para médico %d.\n", codigoDoutor);
                return;
            }
            Utente* utenteAtual = medicoAtual->fila;
            while (utenteAtual != NULL)
            {
                ImprimeUtente(utenteAtual);
                printf("----------\n");
                utenteAtual = utenteAtual->proximo;
            }
            return;
        }
        medicoAtual = medicoAtual->proximo;
    }
    printf("Médico não encontrado.\n");
}

//funcao guarda utentes em ficheiro
void guardarUtentesFicheiro(const Utente* utentes)
{
    FILE* ficheiro = fopen("utentes.txt", "w");
    if (ficheiro == NULL)
    {
        printf("Erro ao abrir ficheiro.\n");
        return;
    }
    const Utente* utenteAtual = utentes;
    while (utenteAtual != NULL)
    {
        fprintf(ficheiro, "%d,%s,%d\n", utenteAtual->codigoUtente, utenteAtual->nome, utenteAtual->codigoFamilia);
        utenteAtual = utenteAtual->proximo;
    }
    fclose(ficheiro);
    printf("Dados de utente guardados em ficheiro.\n");
}

//funcao guarda medicos em ficheiro
void guardarMedicosFicheiro(const Medico* medicos)
{
    FILE* ficheiro = fopen("medicos.txt", "w");
    if (ficheiro == NULL)
    {
        printf("Erro ao abrir ficheiro.\n");
        return;
    }
    const Medico* medicoAtual = medicos;
    while (medicoAtual != NULL)
    {
        fprintf(ficheiro, "%d,%s\n", medicoAtual->codigoDoutor, medicoAtual->nome);
        medicoAtual = medicoAtual->proximo;
    }
    fclose(ficheiro);
    printf("Dados de médico guardados em ficheiro.\n");
}

//funcao carrega utentes em ficheiro
void carregarUtentesFicheiro(Utente** utentes)
{
    FILE* ficheiro = fopen("utentes.txt", "r");
    if (ficheiro == NULL)
    {
        printf("Erro ao abrir ficheiro.\n");
        return;
    }
    char linha[100];
    while (fgets(linha, sizeof(linha), ficheiro))
    {
        int codigoUtente, codigoFamilia;
        char nome[MAX_NAME_LENGTH];
        sscanf(linha, "%d,%[^,],%d", &codigoUtente, nome, &codigoFamilia);
        Utente* utente = adicionarUtente(codigoUtente, nome, codigoFamilia);
        utenteEmFila(utentes, utente);
    }
    fclose(ficheiro);
    /*printf("Daoos de utente carregado de ficheiro.\n");*/
}

//funcao carrega medicos de ficheiro
void carregarMedicosFicheiro(Medico** medicos)
{
    FILE* ficheiro = fopen("medicos.txt", "r");
    if (ficheiro == NULL)
    {
        printf("Erro ao abrir ficheiro.\n");
        return;
    }
    char linha[100];
    while (fgets(linha, sizeof(linha), ficheiro))
    {
        int codigoDoutor;
        char nome[MAX_NAME_LENGTH];
        sscanf(linha, "%d,%[^,]", &codigoDoutor, nome);
        Medico* medico = adicionarMedico(codigoDoutor, nome);
        if (*medicos == NULL)
        {
            *medicos = medico;
        }
        else
        {
            Medico* medicoAtual = *medicos;
            while (medicoAtual->proximo != NULL)
            {
                medicoAtual = medicoAtual->proximo;
            }
            medicoAtual->proximo = medico;
        }
    }
    fclose(ficheiro);
    /*printf("Dados doutor carregado de ficheiro.\n");*/
}


void imprimirMenu()
{
    printf("\n\n");
    printf("1 . Registar utente\n");
    printf("2 . Editar utente\n");
    printf("3 . Visualizar utente\n");
    printf("4 . Remover utente\n");
    printf("5 . Lista de utentes por médico\n");
    printf("6 . Adicionar médico\n");
    printf("7 . Editar médico\n");
    printf("8 . Visualizar lista de espera por médico **\n");
    printf("9 . Remover próximo utente da lista de espera por médico **\n");
    printf("10. Número de utentes em lista de espera por médico\n");
    printf("11. Médico com mais pacientes **\n");
    printf("12. Guardar dados para ficheiros\n");
    printf("0 . Sair\n\n");
    printf("Introduza opção: ");
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    Utente* utentes = NULL;
    Medico* medicos = NULL;

    carregarUtentesFicheiro(&utentes);
    carregarMedicosFicheiro(&medicos);

    int escolha;
    do
    {
        printf("\n\n\t\tCentro de Saude USF M.Cónegos \n");
        imprimirMenu();
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
        {
            int codigoUtente, codigoFamilia;
            char nome[MAX_NAME_LENGTH];
            printf("Introduza o código: ");
            scanf("%d", &codigoUtente);
            printf("Introduza o nome: ");
            scanf("%s", nome);
            printf("Introduza o código de familía: ");
            scanf("%d", &codigoFamilia);

            Utente* utente = adicionarUtente(codigoUtente, nome, codigoFamilia);
            int codigoDoutor;
            printf("Introduza o código de doutor para atribuir o utente: ");
            scanf("%d", &codigoDoutor);
            Medico* medicoAtual = medicos;
            while (medicoAtual != NULL)
            {
                if (medicoAtual->codigoDoutor == codigoDoutor)
                {
                    utenteEmFila(&(medicoAtual->fila), utente);
                    printf("Utente adicionado à lista de espera do Doutor %d.\n", codigoDoutor);
                    break;
                }
                medicoAtual = medicoAtual->proximo;
            }
            if (medicoAtual == NULL)
            {
                printf("Doutor não encontrado.\n");
            }
            break;
        }
        case 2:
        {
            int codigoUtente;
            printf("Introduza código de utente para editar: ");
            scanf("%d", &codigoUtente);
            Utente* utenteAtual = utentes;
            while (utenteAtual != NULL)
            {
                if (utenteAtual->codigoUtente == codigoUtente)
                {
                    printf("Introduza novo nome: ");
                    scanf("%s", utenteAtual->nome);
                    printf("Utente editado com sucesso.\n");
                    break;
                }
                utenteAtual = utenteAtual->proximo;
            }
            if (utenteAtual == NULL)
            {
                printf("Utente não encontrado.\n");
            }
            break;
        }
        case 3:
        {
            int codigoUtente;
            printf("Introduza código de utente para visualizar: ");
            scanf("%d", &codigoUtente);
            Utente* utenteAtual = utentes;
            while (utenteAtual != NULL)
            {
                if (utenteAtual->codigoUtente == codigoUtente)
                {
                    ImprimeUtente(utenteAtual);
                    break;
                }
                utenteAtual = utenteAtual->proximo;
            }
            if (utenteAtual == NULL)
            {
                printf("Utente não encontrado.\n");
            }
            break;
        }
        case 4:
        {
            int codigoUtente;
            printf("Introduza código utente para remover: ");
            scanf("%d", &codigoUtente);

            // Procurar que doutor esta relacionado com utente
            Medico* medicoAtual = medicos;
            while (medicoAtual != NULL)
            {
                Utente* utenteAtual = medicoAtual->fila;
                Utente* utenteAnterior = NULL;
                while (utenteAtual != NULL)
                {
                    if (utenteAtual->codigoUtente == codigoUtente)
                    {
                        if (utenteAnterior == NULL)
                        {
                            medicoAtual->fila = utenteAtual->proximo;
                        }
                        else
                        {
                            utenteAnterior->proximo = utenteAtual->proximo;
                        }
                        free(utenteAtual);
                        printf("Utente removido com sucesso.\n");
                        break;
                    }
                    utenteAnterior = utenteAtual;
                    utenteAtual = utenteAtual->proximo;
                }
                if (utenteAtual != NULL)
                {
                    // User found and removed, exit the loop
                    break;
                }
                medicoAtual = medicoAtual->proximo;
            }
            if (medicoAtual == NULL)
            {
                printf("Utente não encontrado.\n");
            }
            break;
        }

        case 5:
        {
            ImprimirUtentesPorMedico(medicos);
            break;
        }
        case 6:
        {
            int codigoDoutor;
            char nome[MAX_NAME_LENGTH];
            printf("Introduza o código de doutor: ");
            scanf("%d", &codigoDoutor);
            printf("Introduza o nome: ");
            scanf("%s", nome);
            Medico* medico = adicionarMedico(codigoDoutor, nome);
            if (medicos == NULL)
            {
                medicos = medico;
            }
            else
            {
                Medico* medicoAtual = medicos;
                while (medicoAtual->proximo != NULL)
                {
                    medicoAtual = medicoAtual->proximo;
                }
                medicoAtual->proximo = medico;
            }
            printf("Doutor adicionado com sucesso.\n");
            break;
        }
        case 7:
        {
            int codigoDoutor;
            printf("Introduza código de doutor para editar: ");
            scanf("%d", &codigoDoutor);
            Medico* medicoAtual = medicos;
            while (medicoAtual != NULL)
            {
                if (medicoAtual->codigoDoutor == codigoDoutor)
                {
                    printf("Introduza novo nome: ");
                    scanf("%s", medicoAtual->nome);
                    printf("Doutor editado com sucesso.\n");
                    break;
                }
                medicoAtual = medicoAtual->proximo;
            }
            if (medicoAtual == NULL)
            {
                printf("Doutor não encontrado.\n");
            }
            break;
        }
        case 8:
        {

        }
        case 9:
        {

            break;
        }
        case 10:
        {
            int codigoDoutor;
            printf("Introduza código de doutor para receber número de pacientes em lista de espera: ");
            scanf("%d", &codigoDoutor);
            Medico* medicoAtual = medicos;
            while (medicoAtual != NULL)
            {
                if (medicoAtual->codigoDoutor == codigoDoutor)
                {
                    int contador = 0;
                    Utente* corrente = medicoAtual->fila;
                    while (corrente != NULL)
                    {
                        contador++;
                        corrente = corrente->proximo;
                    }
                    printf("Número de pacientes aguardando por Doutor %d: %d\n", codigoDoutor, contador);
                    break;
                }
                medicoAtual = medicoAtual->proximo;
            }
            if (medicoAtual == NULL)
            {
                printf("Doutor não encontrado.\n");
            }
            break;
        }
        case 11:
        {

        }

        case 12:
        {
            guardarUtentesFicheiro(utentes);
            guardarMedicosFicheiro(medicos);
            break;
        }
        case 0:
            printf("Sair...\n");
            break;
        default:
            printf("Opção errada. Tente novamente.\n");
            break;
        }
    }
    while (escolha != 0);

    // Libertar memória
    Utente* utenteAtual = utentes;
    while (utenteAtual != NULL)
    {
        Utente* proximoUtente = utenteAtual->proximo;
        free(utenteAtual);
        utenteAtual = proximoUtente;
    }

    Medico* medicoAtual = medicos;
    while (medicoAtual != NULL)
    {
        Medico* proximoMedico = medicoAtual->proximo;
        free(medicoAtual);
        medicoAtual = proximoMedico;
    }
}
