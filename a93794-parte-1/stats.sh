#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 93794 Nome: Diogo Sousa
## Nome do Módulo: stats.sh
## Descrição/Explicação do Módulo: permite obter informações sobre o sistema 
##      registos <min reg>: devolve os lanços com determinado número mínimo de registos (não permite 0)
##      listar: devolve o nome das autoestradas com lanços registados na plataforma
##      condutores: devolve os condutores registados na plataforma que tenham passado em alguma portagem
###############################################################################

#variáveis
PATH=$PATH:.
portagens=portagens.txt
relatorio=relatorio_utilizacao.txt
condutores=condutores.txt

if [ $# -lt 1 ] ; then
    error 2
elif [[ "$1" != "listar" && "$1" != "registos" && "$1" != "condutores" ]]; then
    error 3 $1
elif test $1 = "registos"; then   
    if [ -z $2 ]; then
        error 2
    elif [[ ! $2 =~ ^[1-9][0-9]*+$ ]]; then
        error 3 $2
    else
        if [[ -f $portagens && -f $relatorio ]]; then
            touch tmp
            # percorre cada lanco em $portagens e verifica se existe em nr maior ou igual a $2 vezes (não menor) no $relatorio
            while IFS=":" read idautoestrada lanco autoestrada taxa || [ -n "$idautoestrada" ]; do
                if [ ! $(grep ":"$lanco":" $relatorio | wc -l) -lt $2 ]; then
                    echo $lanco >> tmp
                fi
            done < $portagens

            success 6 tmp
            rm tmp
        fi
    fi
elif test $1 = "listar"; then
    if [ -f $portagens ]; then
        cat $portagens | awk -F[:] '{print $3}' | sort | uniq > tmp
        success 6 tmp
        rm tmp
    fi
elif test $1 = "condutores"; then
    if [ -f $condutores ]; then
        touch tmp
        # lista o nome de todos os condutores com veículos registados na plataforma que tenham passado portagens
        # percorre cada condutor em $condutores e verifica se existe mais do que 0 vezes no $relatorio
        while IFS=";" read idnome carta contacto contribuinte saldo || [ -n "$idnome" ]; do
            # separa o campo idnome em id e nome pelo caracter -
            IFS=- read id nome <<< $idnome
            if [ $(grep $id":" $relatorio | wc -l) -gt 0 ]; then
                echo $nome >> tmp
            fi
        done < $condutores

        success 6 tmp
        rm tmp
    fi
fi