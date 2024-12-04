/*
* TITLE: Sistemas Operativos
 * SUBTITLE: Práctica 1
 * AUTHOR 1: Pablo Herrero Diaz LOGIN 1: pablo.herrero.diaz
 * AUTHOR 2: Tiago Da Costa Teixeira Veloso E Volta LOGIN 2: tiago.velosoevolta
 * GROUP: 2.3
 * DATE: 25 / 10 / 24
 */

#include "Functions.h"

/*Función principal del programa donde se inicializan las listas de comandos, historial y archivos abiertos,
y se ejecuta el bucle principal para leer la entrada del usuario hasta que se indique que el programa ha terminado.*/
int main(void) {
    printf("Iniciando shell..\n");      //Indicativo de que se inicia otro proceso, otra ejecución de la shell
    bool finished = false;  //Variable que indica si el programa ha terminado.
    CommandListC cmdList;  //Lista de comandos.
    HistoryList historyList;  //Lista que almacena el historial de comandos ejecutados.
    OpenFileList openFileList;  //Lista que gestiona los archivos abiertos.
    MemoryBlockList memoryBlockList;
    ProcessList processList;
    DirectoryList directoryList;
    InitializateShellLists(&cmdList,&historyList,&openFileList,&memoryBlockList, &processList, &directoryList);
    while (!finished) {  //Bucle principal que se ejecuta mientras el programa no haya finalizado.
        printPrompt();  //Imprime el prompt para que el usuario ingrese un comando.
        readInput(&finished, &cmdList, &historyList, &openFileList, &memoryBlockList, &processList, &directoryList);  //Lee la entrada del usuario y procesa el comando.
    }

    return 0;  //Fin de la función.
}

