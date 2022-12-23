#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 93794 Nome: Diogo Sousa 
## Nome do Módulo: faturacao.sh
## Descrição/Explicação do Módulo: 
##      cria o relatório de faturação com informação por condutor, caso já exista, substitui-o
##      o relatório contém o registo de cada passagem em portagem do condutor no relatório de utilização
##      e o total de créditos a que corresponde o gasto para as várias passagens
###############################################################################

# variaveis
PATH=$PATH:.
relatorio=relatorio_utilizacao.txt
faturas=faturas.txt
pessoas=pessoas.txt
portagens=portagens.txt

if [ ! -f $relatorio ]; then
    error 1 $relatorio
else
    # caso exista o ficheiro faturas apaga-o
    if [ -f $faturas ]; then
        rm $faturas
    fi

    if [ -s $relatorio ]; then
        # o $relatorio nao está vazio
        # vai a $pessoas buscar os campos nome e contribuinte e escreve no ficheiro faturas.txt o $relatorio por idcondutor
        # e verifica se recebeu algum dado no 1º campo em alternativa, para o caso do ficheiro nao terminar com nova linha
        while IFS=":" read carta nome contribuinte contacto || [ -n "$carta" ]; do
            echo "Cliente: $nome" >> $faturas
            grep "ID"$contribuinte":" $relatorio >> $faturas

            total=0
            # percorre o relatorio e se o campo <ID-condutor> for igual ao contribuinte do ficheiro pessoas ($idcondutor)
            # soma a taxa da portagem correspondente
            # o while retira os campos e também verifica se recebeu algum dado no 1º campo em alternativa
            # esta 2ª condição serve para o caso do ficheiro nao terminar com nova linha, se não esta seria ignorada erradamente
            while IFS=":" read idportagem lanco idcondutor matricula taxa data || [ -n "$idportagem" ]; do
                if [[ $idcondutor = "ID"$contribuinte ]]; then
                    total=$(( $total+$taxa ))
                fi
            done < $relatorio
            echo "Total: $total créditos" >> $faturas
        done < $pessoas
        success 5 $faturas
    fi
fi