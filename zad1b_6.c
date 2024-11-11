#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int main() {
    int fd[2]; // fd[0] to odczyt, fd[1] to wpisywanie

    if (pipe(fd) == -1) { // Tworzenie pipe
        perror("UWAGA: Nie udało się utworzyć strumienia.\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("UWAGA: Nie udało się utworzyć procesu.\n");
        return 2;

    } else if (pid == 0) { // Proces potomny
        close(fd[0]);
        int limit = 0;


        for (;;) {
            int x = write(fd[1], "Wiadomość! ", 13);
            if (x == -1) { // Sprawdzenie, czy jest błąd
                perror("UWAGA: Wystąpił błąd podczas zapisu.\n");
                break;
            }
            limit += x;
            printf("W bufforze jest: %d bajtow\n", limit); // Sprawdzenie limitu bufora w systemie
        }

        close(fd[1]);

    } else { // Proces rodzica
        close(fd[1]); 
        char buffer_read[BUFFER_SIZE];
        sleep(10); 
        if (read(fd[0], buffer_read, BUFFER_SIZE) == -1) { // Sprawdzenie, czy udało się odczytać i odczytywanie z pipe
            perror("UWAGA: Błąd przy wpisywaniu.\n");
            return 4;
        }
        printf("Odebrany tekst:\n %s", buffer_read); // Odczytanie odebranego tekstu
        close(fd[0]);
        printf("\n");
    }

    return 0;
}
