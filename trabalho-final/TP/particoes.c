
#include "funcionarios.h"
#include <stdlib.h>
#include <limits.h>

#include "particoes.h"

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc) {
    rewind(arq); //posiciona cursor no inicio do arquivo


    int reg = 0;

    while (reg != nFunc) {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq)) {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
       //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M) {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++) {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod)) {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL) {
            printf("Erro criar arquivo de saida\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[i], p);
                imprime_funcionario(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n)
{
    rewind(arq);

    TFunc *array[M], *menorRegistro, *proxRegistroEntrada;
    TFunc *reservatorio[M];

    int count = 0;
    int posArqEntrada = 0;
    int posMenorArray = 0;
    int posArqSaida = 0;
    int posReservatorio = 0;

    if(arq != NULL)
    {
        while(count < M)
        {
            fseek(arq, (posArqEntrada)*tamanho_registro(), SEEK_SET);
            array[count] = le_funcionario(arq);
            count++;
            posArqEntrada++;
        }

        printf("_____________________________________________________________________________\n");

        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;

        printf("======Abrindo novo arquivo de saida=====\n");
        FILE *arquivoSaida;
        arquivoSaida = fopen(nome_particao, "wb+");

        do
        {
            posMenorArray = menorChave(array, M);
            menorRegistro = array[posMenorArray];

            if (arquivoSaida != NULL)
            {
                printf("======Salva menor registro no Arq Saida: %d -- %s \n", menorRegistro->cod, menorRegistro->nome);
                salva_ArqSaida(arquivoSaida, posArqSaida, menorRegistro);
                posArqSaida++;
            }

            fseek(arq, (posArqEntrada)*tamanho_registro(), SEEK_SET);
            proxRegistroEntrada = le_funcionario(arq);
            array[posMenorArray] = proxRegistroEntrada;
            posArqEntrada ++;

            if(feof(arq))
            {
                posMenorArray = menorChave(array, M);
                menorRegistro = array[posMenorArray];
                salva_ArqSaida(arquivoSaida, posArqSaida, menorRegistro);
                break;
            }

            if(proxRegistroEntrada->cod < menorRegistro->cod)
            {
                if(posArqEntrada < nFunc)
                {

                    reservatorio[posReservatorio] = proxRegistroEntrada;
                    posReservatorio++;

                    posArqEntrada++;
                    fseek(arq, (posArqEntrada)*tamanho_registro(), SEEK_SET);
                    proxRegistroEntrada = le_funcionario(arq);
                    array[posMenorArray] = proxRegistroEntrada;

                    array[posMenorArray] = proxRegistroEntrada;
                }
                else
                {
                    reservatorio[posReservatorio] = proxRegistroEntrada;
                    array[posMenorArray] = NULL;

                    posMenorArray = menorChave(array, M);
                    menorRegistro = array[posMenorArray];
                    if (arquivoSaida != NULL)
                    {
                        salva_ArqSaida(arquivoSaida, posArqSaida, menorRegistro);
                        posArqSaida++;
                    }

                    posReservatorio++;
                }
            }

            if(posReservatorio >= M)
            {
                fclose(arquivoSaida);

                for(int i = 0; i < posReservatorio; i++)
                {
                    array[i] = reservatorio[i];
                }

                for(int r = 0; r < M; r++)
                {
                    reservatorio[r] = NULL;
                }
                posReservatorio = 0;

                char *nome_particao = nome_arquivos_saida->nome;
                nome_arquivos_saida = nome_arquivos_saida->prox;

                arquivoSaida = fopen(nome_particao, "wb+");
                posArqSaida = 0;
            }
        }
        while(posReservatorio < M);
    }
}

int menorChave(TFunc *array[], int size)
{
    int posMenor, i, posNotNULL;
    char posNull = 'N';
    TFunc *menor;

    for(i = 0; i < size; i++)
    {
        if(array[i] == NULL)
        {
            posNull = 'S';
        }
        else
        {
            posNotNULL = i;
        }
    }
    if(posNull == 'N')
    {
        for(i = 0; i < size; i++)
        {
            if(i == 0)
            {
                menor = array[0];
                posMenor = 0;
            }
            else if(menor->cod > array[i]->cod)
            {
                menor = array[i];
                posMenor = i;
            }
        }
    }
    else
    {
        return posNotNULL;
    }

    return posMenor;
}

void salva_ArqSaida(FILE *saida, int indice, TFunc *item)
{
    fseek(saida, (indice)*tamanho_registro(), SEEK_SET);
    salva_funcionario(item, saida);
}
