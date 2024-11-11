#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int fd[2]; // fd[0] to odczyt, fd[1] to wpisywanie
    pid_t pid;

    // Tworzenie pipe
    if (pipe(fd) == -1) {
        perror("UWAGA: Błąd przy tworzeniu pipe.\n");
        return 1;
    }

    // Tworzenie procesu
    pid = fork(); 
    if (pid < 0) { // Sprawdzenie, czy udało się stworzyć proces
        perror("UWAGA: Błąd przy tworzeniu procesu.\n");
        return 2;
    }

    if (pid > 0) {
        char buffer_written[BUFFER_SIZE];
        // Proces rodzica
        close(fd[0]); 
        printf("RODZIC: Wprowadź tekst do przesłania: ");
        fgets(buffer_written, BUFFER_SIZE, stdin); // Zapisanie tekstu w buferze
        if (write(fd[1], buffer_written, strlen(buffer_written) + 1) == -1) { // Sprawdzenie, czy udało się wpisać i wpisanie do pipe
            perror("UWAGA: Błąd przy wpisywaniu.\n");
            return 3;
        }
        close(fd[1]); 
        wait(NULL); 
    } else {
        char buffer_read[BUFFER_SIZE];
        // Proces potomny
        close(fd[1]); 
        if (read(fd[0], buffer_read, BUFFER_SIZE) == -1) { // Sprawdzenie, czy udało się odczytać i odczytywanie z pipe
            perror("UWAGA: Błąd przy wpisywaniu.\n");
            return 4;
        }
        printf("DZIECKO: Odebrany tekst: %s", buffer_read); // Odczytanie odebranego tekstu
        close(fd[0]); 
    }

    return 0;
}
