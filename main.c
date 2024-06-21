#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//Lucas Erazo, 21401559-5
//Pablo Silva, 20184240-9
//Jose Villamayor, 21439627-0

//Usar "adios" o "adiós" para cerrar el chatbot

#define MSG_SIZE 256

void procesoHijo(int readFd, int writeFd) {
    char buf[MSG_SIZE];
    while (1) {
        // Leer mensaje del padre
        read(readFd, buf, MSG_SIZE);
        printf("Pablou recibio: %s\n", buf);
        if (strcmp(buf, "adios") == 0 || strcmp(buf, "adiós") == 0) {
            break;
        }
        // Enviar respuesta al padre
        printf("Pablou: ");
        fgets(buf, MSG_SIZE, stdin);
        
        // Eliminar el salto de línea
        buf[strcspn(buf, "\n")] = '\0'; 
        
        write(writeFd, buf, strlen(buf) + 1);
        if (strcmp(buf, "adios") == 0 || strcmp(buf, "adiós") == 0) {
            exit(EXIT_SUCCESS);
        }
    }
    close(readFd);
    close(writeFd);
}

void procesoPadre(int read_fd, int write_fd) {
    char buf[MSG_SIZE];
    while (1) {
        // Enviar mensaje al hijo
        printf("luka: ");
        fgets(buf, MSG_SIZE, stdin);
        
        // Eliminar el salto de línea
        buf[strcspn(buf, "\n")] = '\0'; 
        
        write(write_fd, buf, strlen(buf) + 1);
        if (strcmp(buf, "adios") == 0 || strcmp(buf, "adiós") == 0) {
            exit(EXIT_SUCCESS);
        }
        // Leer respuesta del hijo
        read(read_fd, buf, MSG_SIZE);
        printf("Luka recibio: %s\n", buf);
        
        if (strcmp(buf, "adios") == 0 || strcmp(buf, "adiós") == 0) {
            exit(EXIT_SUCCESS);
        }
    }
    close(read_fd);
    close(write_fd);
    wait(NULL);  
}

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid;

    //Crear pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Crear un nuevo proceso
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {    
        // Proceso hijo
        close(pipe1[1]);  
        close(pipe2[0]);
        procesoHijo(pipe1[0], pipe2[1]);
        
    } else {            
        // Proceso padre
        close(pipe1[0]);  
        close(pipe2[1]);
        procesoPadre(pipe2[0], pipe1[1]);
    }

    return 0;
}
