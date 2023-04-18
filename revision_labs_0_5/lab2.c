#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main() {
    // otwieranie pliku w trybie binarnym za pomocą fopen()
    FILE *file_ptr = fopen("test.bin", "wb");
    if (file_ptr == NULL) {
        perror("Nie udało się otworzyć pliku.");
        exit(EXIT_FAILURE);
    }

    // przygotowanie danych do zapisu
    char data[] = "To jest testowa zawartość pliku.";
    size_t data_size = sizeof(data);

    // zapisanie danych za pomocą fwrite()
    fwrite(data, 1, data_size, file_ptr);

    // zamknięcie pliku za pomocą fclose()
    fclose(file_ptr);

    // otwieranie pliku w trybie binarnym za pomocą open()
    int file_descriptor = open("test.bin", O_RDONLY);
    if (file_descriptor == -1) {
        perror("Nie udało się otworzyć pliku.");
        exit(EXIT_FAILURE);
    }

    // ustawienie wskaźnika pliku za pomocą lseek()
    off_t offset = 5;
    lseek(file_descriptor, offset, SEEK_SET);

    // odczytanie danych za pomocą read()
    char buffer[10];
    ssize_t bytes_read = read(file_descriptor, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Nie udało się odczytać danych.");
        exit(EXIT_FAILURE);
    }
    printf("Odczytano %zd bajtów: %.*s\n", bytes_read, (int)bytes_read, buffer);

    // zamknięcie pliku za pomocą close()
    close(file_descriptor);

    // mierzenie czasu wykonania
    clock_t start = clock();
    for (int i = 0; i < 1000000000; i++) {
        // wykonanie pustej operacji, aby zmierzyć czas
        asm("");
    }
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Czas wykonania: %f sekund\n", elapsed_time);

    return 0;
}
