#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2021/2022
##
## Aluno: Nº: 93794 Nome: Diogo Sousa
## Nome do Módulo: menu.sh
## Descrição/Explicação do Módulo: apresentar menu ao utilizador
##      Opção 0: terminar o menu.sh ou voltar para trás no caso de estar dentro de um sub-menu
##      Opção 1: listar os condutores registados e criar ficheiro condutores.txt com essa informação
##      Opção 2: alterar a taxa de utilização da portagem (no ficheiro portagens.txt) mediante os argumentos Lanço, Auto-estrada e Novo valor da taxa
##      Opção 3: obter estatísticas. Caso selecionada abre um submenu com 4 opções
##          Subopção 0: voltar ao menu principal
##          Subopção 1: listar o nome de todas as autoestadas com lanços registados
##          Subopção 2: obter registos de utilização de lanço para um determinado mínimo de registos
##          Subopção 3: obter listagem de condutores que tenham passado portagens
##      Opção 4: reporta a faturação por condutor e cria o ficheiro faturas.txt com essa informação
###############################################################################

#variáveis
PATH=$PATH:.
opcao=-1

while [ $opcao != 0 ]; do
    echo $'\n1. Listar condutores'
    echo '2. Altera taxa de portagem'
    echo '3. Stats'
    echo '4. Faturação'
    echo $'0. Sair\n'
    echo -n 'Opção: '; read opcao
    echo ""
    if [[ ! $opcao =~ ^[0-4]$ ]]; then
        error 3 $opcao
        # caso a opção seja vazia (sem opção <ENTER>) volta a colocar a opcao a -1 para não dar erro no while
        opcao=-1
    elif test $opcao = 1; then
        echo $'Listar condutores...\n'
        lista_condutores.sh
    elif test $opcao = 2; then
        echo $'Altera taxa de portagem...\n'
        echo -n 'Lanço             : '; read lanco
        echo -n 'Auto-estrada      : '; read autoestrada
        echo -n 'Novo valor taxa   : '; read taxa
        echo ""
        altera_taxa_portagem.sh $lanco $autoestrada $taxa
    elif test $opcao = 3; then
        echo $'Stats\n'
        subopcao=-1
        while [ $subopcao != 0 ]; do
            echo $'\n1. Nome de todas as Autoestradas'
            echo '2. Registos de utilização'
            echo '3. Listagem condutores'
            echo $'0. Voltar\n'
            echo -n 'Opção: '; read subopcao
            echo ""   
            if [[ ! $subopcao =~ ^[0-3]$ ]]; then
                error 3 $subopcao
                subopcao=-1
            elif test $subopcao = 1; then
                echo $'1. Nome de todas as Autoestradas\n'
                stats.sh listar
                subopcao=0
            elif test $subopcao = 2; then
                echo $'2. Registos de utilização\n'
                echo -n 'Mínimo de registos  : '; read minreg
                echo ""
                stats.sh registos $minreg
                subopcao=0
            elif test $subopcao = 3; then
                echo $'3. Listagem condutores\n'
                stats.sh condutores
                subopcao=0
            fi
        done
    elif test $opcao = 4; then
        echo $'Faturação...\n'
        faturacao.sh
    fi
done