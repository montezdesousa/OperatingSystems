/******************************************************************************
 ** ISCTE-IUL: Trabalho prático 2 de Sistemas Operativos
 **
 ** Aluno: Nº: 93794 Nome: Diogo Sousa
 ** Nome do Módulo: cliente.c v1
 ** Descrição/Explicação do Módulo: Código comentado abaixo
 **
 **
 ******************************************************************************/
#include "common.h"
#include "utils.h"
#include <sys/stat.h>
// #define DEBUG_MODE FALSE             // To disable debug messages, uncomment this line

/* Variáveis globais */
Passagem pedido;                        // Variável que tem o pedido enviado do Cliente para o Servidor
int pidServidor;                        // Variável que tem o PID do processo Servidor
char passagemIniciada = FALSE;          // Variável que indica que o Servidor já enviou sinal de início da passagem

/* Protótipos de funções */
int getPidServidor();                   // C1: Função a ser implementada pelos alunos
int armaSinais();                       // C2: Função a ser implementada pelos alunos
Passagem getDadosPedidoUtilizador();    // C3: Função a ser implementada pelos alunos
int escrevePedido(Passagem);            // C4: Função a ser implementada pelos alunos
int configuraTemporizador();            // C5: Função a ser implementada pelos alunos
void trataSinalSIGUSR1(int);            // C6: Função a ser implementada pelos alunos
void trataSinalSIGTERM(int);            // C7: Função a ser implementada pelos alunos
void trataSinalSIGHUP(int);             // C8: Função a ser implementada pelos alunos
void trataSinalSIGINT(int);             // C9: Função a ser implementada pelos alunos
void trataSinalSIGALRM(int);            // C10: Função a ser implementada pelos alunos

/**
 * Main: Processamento do processo Cliente
 * 
 * @return int Exit value
 */
int main() {    // Os alunos em princípio não deverão alterar esta função
    // C1
    pidServidor = getPidServidor();
    exit_on_error(pidServidor, FILE_SERVIDOR);
    // C2
    pedido = getDadosPedidoUtilizador();
    exit_on_error(pedido.tipo_passagem, "getDadosPedidoUtilizador");
    // C3
    exit_on_error(armaSinais(), "armaSinais");
    // C4
    exit_on_error(escrevePedido(pedido), "escrevePedido");
    // C5
    exit_on_error(configuraTemporizador(), "configuraTemporizador");
    // Aguarda processamento por parte do Servidor
    while (TRUE) {
        debug("", "Aguarda processamento por parte do Servidor");
        pause();
    }
}

/**
 *  O módulo Cliente é responsável pelo pedido das passagens. Este módulo será utilizado para solicitar a passagem das viaturas pelas portagens.
 *  Após identificação da viatura, é realizado o pedido da respetiva passagem, ficando este módulo a aguardar até que o processamento esteja concluído.
 *  Assim, definem-se as seguintes tarefas a desenvolver:
 */

/**
 * C1   Lê a informação acerca do PID do Servidor de Passagens que deve estar registado no ficheiro FILE_SERVIDOR. 
 *      No caso de o ficheiro não existir ou de não ter um PID registado no ficheiro, dá error C1 "<Problema>" e termina o processo Cliente.
 *      Caso contrário, assume que a única informação no ficheiro (em formato de texto) é o PID do Servidor
 *      (pai, único PID do lado do Servidor que este Cliente conhece), e dá success C1 "<PID Servidor>";
 *
 * @return int Sucesso
 */
int getPidServidor() {
    debug("C1", "<");

    // Função que retorna o pid no caso deste estar escrito no ficheiro FILE_SERVIDOR

    int pidServidor = -1;   // Por omissão, retorna valor inválido

    FILE *f;
    f = fopen(FILE_SERVIDOR, "r");
    
    // Verifica se o ficheiro existe
    if (f == NULL) {
        error("C1", "O ficheiro %s não existe", FILE_SERVIDOR);
        return -1;
    }
    
    // No caso de existir lê o pid    
    fscanf (f, "%d", &pidServidor);
    fclose(f);

    // Verifica se o pid lido é <= 0 e dá erro nesse caso
    if (pidServidor <= 0){
        error("C1", "O conteúdo não é um pid");
        return -1;
    }

    success("C1","%d", pidServidor);

    debug("C1", ">");
    return pidServidor;
}

/**
 * C2   Pede ao Condutor (utilizador) que preencha os dados referentes à passagem da viatura (Matrícula e Lanço),
 *      criando um elemento do tipo Passagem com essas informações, e preenchendo o valor pid_cliente com o PID do seu próprio processo Cliente.
 *      Em caso de ocorrer qualquer erro, dá error C2 "<Problema>", e termina o processo Cliente;
 *      caso contrário dá success C2 "Passagem do tipo <Normal | Via Verde> solicitado pela viatura com matrícula <matricula> para o Lanço <lanco> e com PID <pid_cliente>";
 *
 * @return Passagem Elemento com os dados preenchidos. Se tipo_passagem = -1, significa que o elemento é imválido
 */
Passagem getDadosPedidoUtilizador() {
    debug("C2", "<");
    Passagem p;
    p.tipo_passagem = -1;   // Por omissão, retorna valor inválido

    // Pede ao condutor o tipo de passagem e valida
    printf ("Tipo de passagem [1-Normal, 2-Via Verde]: ");
    scanf("%d", &(p.tipo_passagem));
    
    if (p.tipo_passagem != 1 && p.tipo_passagem != 2) {
        error("C2", "Tipo de passagem deve ser 1 ou 2");
        p.tipo_passagem = -1;
        return p;
    }

    // Pede ao condutor a matrícula e valida
    printf ("Matrícula: ");
    my_gets(p.matricula, sizeof(p.matricula));
    if (strlen(p.matricula) == 0){
        error("C2", "Matrícula não pode estar vazia");
        p.tipo_passagem = -1;
        return p;
    }

    // Pede ao condutor o lanço e valida
    printf ("Lanço: ");
    my_gets(p.lanco, sizeof(p.lanco));
    if (strlen(p.lanco) == 0){
        error("C2", "Lanço não pode estar vazio");
        p.tipo_passagem = -1;
        return p;
    }

    // Preenche o valor pid_cliente com o próprio pid
    p.pid_cliente =  getpid();

    // Traduz o tipo de passagem para string
    char normal[7] = "Normal";
    char viaverde[10] = "Via Verde";
    char psgtype[10];
    if (p.tipo_passagem == 1)
        strcpy(psgtype, normal);
    else if (p.tipo_passagem == 2)
        strcpy(psgtype, viaverde);
    
    success("C2","Passagem do tipo %s solicitado pela viatura com matrícula %s para o Lanço %s e com PID %d", 
            psgtype, 
            p.matricula, 
            p.lanco, 
            p.pid_cliente);
    
    debug("C2", ">");
    return p;
}

/**
 * C3   Arma os sinais SIGUSR1 (ver C6), SIGTERM (ver C7), SIGHUP (ver C8), SIGINT (ver C9), e SIGALRM (ver C10),
 *      dando, no fim de os armar, a mensagem success C3 "Armei sinais";
 *
 * @return int Sucesso
 */
int armaSinais() {
    debug("C3", "<");
    
    // Arma os sinais com as respetivas funções
    signal(SIGUSR1, trataSinalSIGUSR1);
    signal(SIGTERM, trataSinalSIGTERM);
    signal(SIGHUP, trataSinalSIGHUP);
    signal(SIGINT, trataSinalSIGINT);
    signal(SIGALRM, trataSinalSIGALRM);    

    success("C3", "Armei sinais");

    debug("C3", ">");
    return 0;
}

/**
 * C4   Valida se o ficheiro com organização FIFO (named pipe) FILE_PEDIDOS existe.
 *      Se esse ficheiro FIFO não existir, dá error C4 e termina o processo Cliente.
 *      Caso contrário, escreve as informações do elemento Passagem (em formato binário) nesse FIFO FILE_PEDIDOS.
 *      Em caso de erro na escrita, dá error C4 e termina o processo Cliente, caso contrário, dá success C4 "Escrevi FIFO";
 *
 * @return int Sucesso
 */
int escrevePedido(Passagem dados) {
    debug("C4", "<");

    struct stat st; 

    // Valida se o ficheiro existe.
    int result = stat(FILE_PEDIDOS, &st);

    if (result == -1){
        error("C4", "O ficheiro %s não existe", FILE_PEDIDOS);
        return -1;
    }

    // Valida se o ficheiro é um FIFO.
    if (!S_ISFIFO(st.st_mode)) {
        error("C4", "O ficheiro existe mas não é um FIFO!");
        return -1;
    }

    // Abre o fifo para escrita validando se existe
    FILE *fifo = fopen(FILE_PEDIDOS, "w");
    exit_on_null(fifo, "O ficheiro não existe!");

    // Escreve no fifo validando se ocorreu algum erro na escrita
    if (fwrite(&dados, sizeof(dados), 1, fifo) < 1) {
        error("C4", "Erro na escrita do FIFO");
        return -1;
    }
    fclose(fifo);
    success("C4", "Escrevi FIFO");

    debug("C4", ">");
    return 0;
}

/**
 * C5   Configura um alarme com o valor de MAX_ESPERA segundos (ver C10),
 *      dá success C5 "Inicia Espera de <MAX_ESPERA> segundos",
 *      e fica a aguardar o resultado do processamento do pedido por parte do Servidor.
 *
 * @return int Sucesso
 */
int configuraTemporizador() {
    debug("C5", "<");

    // O prórprio processo irá receber um SIGALRM passados MAX_ESPERA segundos
    alarm(MAX_ESPERA);
    success("C5", "Inicia espera de %d segundos", MAX_ESPERA);

    debug("C5", ">");
    return 0;
}

/**
 * C6   O sinal armado SIGUSR1 serve para o Servidor Dedicado indicar que o processamento da passagem foi iniciado.
 *      Se o Cliente receber esse sinal, dá success C6 "Passagem Iniciada", assinala que o processamento iniciou,
 *      e retorna para continuar a aguardar a conclusão do processamento do lado do Servidor Dedicado;
 */
void trataSinalSIGUSR1(int sinalRecebido) {
    debug("C6", "<");

    // Assinala que a passagem foi iniciada pelo servidor dedicado
    success("C6", "Passagem Iniciada"); 
    passagemIniciada = TRUE; 
    return;

    debug("C6", ">");
}

/**
 * C7   O sinal armado SIGTERM serve para o Servidor Dedicado indicar que o processamento da passagem foi concluído.
 *      Se o Cliente receber esse sinal, dá success C7 "Passagem Concluída", e termina o processo Cliente.
 *      ATENÇÂO: Deverá previamente validar que anteriormente este Cliente já tinha recebido o sinal SIGUSR1 (ver C6),
 *               indicando que o processamento do lado do Servidor Dedicado teve início,
 *               caso contrário, em vez de sucesso, dá error C7 e termina o processo Cliente;
 */
void trataSinalSIGTERM(int sinalRecebido) {
    debug("C7", "<");

    // Verifica se já tinha iniciado a passagem, ou seja, já tinha recebido o SIGUSR1 e devolve a mensagem de acordo
    if (passagemIniciada == TRUE){
        success("C7", "Passagem Concluída"); 
        exit(0); 
    } else {
        error("C7", "Passagem não iniciada");
        exit(1);
    }   

    debug("C7", ">");
}

/**
 * C8   O sinal armado SIGHUP serve para o Servidor Dedicado indicar que o processamento a passagem não foi concluído.
 *      Se o Cliente receber esse sinal, dá success C8 "Processo Não Concluído e Incompleto", e termina o processo Cliente;
 */
void trataSinalSIGHUP(int sinalRecebido) {
    debug("C8", "<");

    // Se receber o SIGHUP deteta que o processo não foi concluído e termina o cliente
    success("C8", "Processo Não Concluído e Incompleto"); 
    exit(0);     

    debug("C8", ">");
}

/**
 * C9   O sinal armado SIGINT serve para que, no caso de o veículo ter uma avaria, ou por outro motivo qualquer,
 *      o condutor (utilizador) possa cancelar o pedido do lado do Cliente, usando o atalho <CTRL+C>.
 *      Se receber esse sinal (do utilizador via Shell), o Cliente envia o sinal SIGHUP ao Servidor,
 *      para que esta passagem seja sinalizada como anomalia, dá success C9 "Processo Cancelado pelo Cliente", 
 *      e retorna para aguardar que o Servidor Dedicado conclua o processo
 *      (o Servidor Dedicado deverá mais tarde enviar o sinal SIGHUP a este Cliente, ver C8);
 */
void trataSinalSIGINT(int sinalRecebido) {
    debug("C9", "<");

    // Envia SIGHUP ao servidor para cancelar o processo
    kill(pidServidor, SIGHUP); 
    success("C9", "Processo Cancelado pelo Cliente"); 
    return;

    debug("C9", ">");
}

/**
 * C10  O sinal armado SIGALRM serve para que, se o Cliente em C5 esperou mais do que MAX_ESPERA segundos sem resposta,
 *      o Cliente envia o sinal SIGHUP ao Servidor, para que esta passagem seja sinalizada como anomalia,
 *      dá success C10 "Timeout Cliente", e retorna para aguardar que o Servidor Dedicado conclua o processo
 *      (o Servidor Dedicado deverá mais tarde enviar o sinal SIGHUP a este Cliente, ver C8).
 */
void trataSinalSIGALRM(int sinalRecebido) {
    debug("C10", "<");

    // Se ainda não obteve resposta após MAX_ESPERA segundos envia SIGHUP ao servidor
    kill(pidServidor, SIGHUP); 
    success("C10", "Timeout Cliente");       
    return;

    debug("C10", ">");
}