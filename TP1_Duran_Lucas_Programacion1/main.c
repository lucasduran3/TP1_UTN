#include <stdio.h>
#include <stdlib.h>

struct Song
{
    char* artist;
    char* title;
    char* duration;
    int year;
};
struct Song* enterSong();


int main()
{
    enterSong();
    return 0;
}

struct Song* enterSong()
{
    struct Song* newSong = malloc(sizeof(struct Song));
    printf("Artista: ");
    scanf("%s", newSong->artist);

    printf("Titulo: ");
    scanf("%s", newSong->title);

    printf("Duracion:(00:00) ");
    scanf("%s", newSong->duration);

    printf("Año de lanzamiento: ");
    scanf("%s", newSong->year);

    return newSong;
}
