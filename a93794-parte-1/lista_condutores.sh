#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 93794 Nome: Diogo Sousa
## Nome do Módulo: lista_condutores.sh
## Descrição/Explicação do Módulo: 
##      Se o ficheiro dado não existir chama o erro 1. Ex output: ERRO: O ficheiro "ficheiro.txt" não existe
##      Caso contrário, escreve no ficheiro condutores.txt
##      cada linha do ficheiro dado na forma <ID>-<Nome>;<ID carta condução>;<Contacto>;<Nr Contribuinte>;<Saldo (em créditos)>
##      Chama o success 2 "condutores.txt", que permite ver o conteúdo final do ficheiro condutores.txt
###############################################################################

# variaveis
PATH=$PATH:.
ficheiro=pessoas.txt

if [ ! -f $ficheiro ]; then
    error 1 $ficheiro
else
    awk -F[:] '{print "ID" $3 "-" $2 ";" $1 ";" $4 ";" $3 ";" 150}' $ficheiro > condutores.txt
    success 2 "condutores.txt"
fi
