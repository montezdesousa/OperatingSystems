#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 93794 Nome: Diogo Sousa
## Nome do Módulo: altera_taxa_portagem.sh
## Descrição/Explicação do Módulo: 
##      permite alterar a taxa de utilização das portagens (registado no ficheiro portagens.txt)
##      o valor é alterado com base no lanço (independente da autoestrada)
##      caso ainda não exista o lanço dado adiciona-o ao ficheiro portagens.txt
###############################################################################

# variaveis
PATH=$PATH:.
lanco=$1
autoestrada=$2
taxa=$3
ficheiro=portagens.txt

# input com # de menor que 3?
if [ $# -lt 3 ]; then
    error 2
# input $lanco não é do formato "Partida-Destino"
elif [[ ! $lanco =~ ^[A-Za-z]+-{1,1}[A-Za-z]+$ ]]; then
    error 3 $lanco
# input $taxa diferente de inteiro positivo (primeiro algarismo entre 1 e 9 e depois números entre 0 e 9)
elif [[ ! $taxa =~ ^[1-9][0-9]*+$ ]]; then
    error 3 $taxa
else
    # se o ficheiro portagens.txt existir, vê se já existe o lanco, caso não exista adiciona-o
    if [ -f $ficheiro ]; then
        # procura no campo dos lancos o lanco dado e avalia a exit flag do grep para o lanco literal (não tolera substrings do tipo "Cartaxo-Santaré" e -q suprime o output)
        if awk -F[:] '{print $2}' $ficheiro | grep -q ^$lanco$ ; then
            # altera a taxa de utilização do lanco (direciona o output para um ficheiro tmp e depois altera 'mv' o seu nome para o $ficheiro destruindo o original)
            awk -F[:]  'BEGIN {} {if ($2 == "'"$lanco"'") print $1":"$2":"$3":""'"$taxa"'"; else print $0} END {}' $ficheiro > tmp
            mv tmp $ficheiro
        else
            # procura o maior <ID_portagem> e soma 1
            max=$(awk -F[:]  'BEGIN {max = -1} {if ($1 > max) max = $1;} END {print max+1}' $ficheiro)
            # regista um novo lanco com a sintax <ID_portagem>:<Lanço>:<Auto-estrada atribuída>:<Taxa de utilização (em créditos)>
            echo $max":"$lanco":"$autoestrada":"$taxa >> $ficheiro
        fi
    else
        # cria o ficheiro portagens.txt com o primeiro <ID_portagem> a 1
        echo "1:"$lanco":"$autoestrada":"$taxa > $ficheiro
    fi

    success 3 $lanco
    
    # o pipe sort altera o delimitador para ':' e ordena da coluna 3 até à coluna 3 (autoestrada) e de seguida da coluna 2 até à coluna 2 (lanco)
    success 4 $ficheiro | sort -t':' -k3,3 -k2,2
fi



