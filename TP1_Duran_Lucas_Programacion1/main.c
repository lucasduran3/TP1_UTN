#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/*
********TRABAJO PRACTINO N°1 PROGRAMACION UTN********
*/

struct Song
{
    char artist[100];
    char title[100];
    char duration[5];
    int year;
};

int checkDurationFormat(char* s);
int getLinesOfFile(FILE* f);
int partition(struct Song songs[], int low, int high);
FILE* loadFile(char* dir, char* mode);
void saveNewSong(struct Song* newSong);
void enterSong();
void listSongs();
void listenToSong();
void showHistory();
void mainOptionSelector();
void showMenu();
void swap(struct Song* a, struct Song* b);
void quickSort(struct Song songs[], int low, int high);


int main()
{
    showMenu();
    return 0;
}

/*
----Funciones Principales----
*/
//1- Ingresar nueva cancion
void enterSong()
{
    int shouldContinue = 1;

    struct Song* newSong = malloc(sizeof(struct Song));
    if(!newSong){
        fprintf(stderr, "Error: no se pudo reservar memoria para una nueva cancion.\n");
        return 1;
    }

    do {
        printf("\n######INGRESE UNA NUEVA CANCION#####\n");
        printf("Artista: ");
        scanf("%s", newSong->artist);
        fflush(stdin);

        printf("Titulo: ");
        scanf("%s", newSong->title);
        int result;
        fflush(stdin);

        do {
            printf("Duracion (00:00): ");
            scanf("%s", newSong->duration);
            result = checkDurationFormat(newSong->duration);
            if (!result) {
                printf("Formato invalido. Usa 00:00 (por ejemplo, 03:45).\n");
                fflush(stdin);
            }
            fflush(stdin);

        } while (!result);

        do {
            printf("Ano de lanzamiento: ");
            result = scanf("%d", &newSong->year);
            if (result != 1) {
                printf("Ano no valido. Por favor ingresa un numero.\n");
                fflush(stdin);
            }
            fflush(stdin);

        } while (result != 1);

        saveNewSong(newSong);
        printf("####################################\n");
        printf("\nIngresa una opcion:\n0. Volver a Menu Principal\n1. Ingresar otra cancion\n");
        scanf("%d", &shouldContinue);
        fflush(stdin);
    } while (shouldContinue == 1);

    free(newSong);
    showMenu();
}

//2- Listas todas las canciones
void listSongs() {
    FILE* library = loadFile("songsLibrary.txt", "r");
    rewind(library);

    int lines = getLinesOfFile(library);
    struct Song songs[lines];

    for (int i = 0; i < lines; i++) {
        if (fscanf(library, "%s %s %s %d",
                   songs[i].artist,
                   songs[i].title,
                   &songs[i].duration,
                   &songs[i].year) != 4) {
            fprintf(stderr, "Formato incorrecto en línea %d\n", i + 1);
            break;
        }
    };
    fclose(library);

    quickSort(songs, 0, lines - 1);

    printf("\n######LISTA DE CANCIONES######\n");
    printf("#   TITULO - ARTISTA\n");

    for (int i = 0; i < lines; i++) {
        printf("# %d.   %s - %s\n", i+1, songs[i].title, songs[i].artist);
    };
    printf("##############################\n");

    int shouldBack = 0;
    printf("\nIngrese una opcion:\n0. Volver a Menu Principal\n1. Salir\n");
    scanf("%d", &shouldBack);
    fflush(stdin);
    if (shouldBack == 0) {
        showMenu();
    }
    else {
        exit(0);
    }
}

void listenToSong() {
    FILE* library = loadFile("songsLibrary.txt", "r");
    FILE* history = loadFile("history.txt", "a");

    rewind(library);

    char title[100];
    char registeredArtist[100];
    char registeredTitle[100];

    int finded;
    int shouldContinue = 1;

    do {

        title[100] = "";
        registeredArtist[100] = "";
        registeredTitle[100] = "";
        finded = 0;

        printf("\n######BUSCADOR DE CANCIONES######\n");
        printf("Ingrese el titulo de una cancion: ");
        scanf("%s", title);

        int lines = getLinesOfFile(library);
        for (int i = 0; i < lines; i++){
            if (fscanf(library, "%s %s%*[^\n]\n", registeredArtist, registeredTitle) != 2) {
                fprintf(stderr, "Formato incorrecto en línea %d\n", i + 1);
                exit(1);
            }

            if (strcmp(title, registeredTitle) == 0) {
                fprintf(history, "%s\n", title);
                printf("Listo.\n");
                printf("#################################\n");
                finded = 1;
                break;
            }
        };

        if (!finded) {
            printf("La cancion no esta en el reproductor.\n");
        }

        printf("\nIngresa una opcion:\n0. Volver a Menu Principal\n1. Escuchar otra cancion\n");
        scanf("%d", &shouldContinue);
        fflush(stdin);
    } while (shouldContinue == 1);

    fclose(library);
    fclose(history);
    showMenu();
}

void showHistory() {
    FILE* history = loadFile("history.txt", "r");

    rewind(history);

    int lines = getLinesOfFile(history);

    printf("\n####HISTORIAL####\n");
    if (lines == 0) {
        printf("No se han escuchado canciones.\n");
    }
    else {
        char title[100];
        for (int i = 0; i < lines; i++) {
            if (fscanf(history, "%s", title) != 1) {
                fprintf(stderr, "Formato incorrecto de linea %d\n", i + 1);
                exit(1);
            }
            printf("%d.  %s\n", i + 1, title);
        };
    }
    printf("#################\n");
    fflush(stdin);
    fclose(history);

    int shouldBack;
    printf("\nIngrese una opcion:\n0. Volver a Menu Principal\n1. Salir\n");
    scanf("%d", &shouldBack);
    fflush(stdin);
    if (shouldBack == 0) {
        showMenu();
    }
    else {
        exit(0);
    }
}

//Visualizacion de menu principal
void showMenu() {
    printf("##################################\n");
    printf("#        REPRODUCTOR MP3         #\n");
    printf("##################################\n");

    // Opciones
    printf("# 1. Ingresar Cancion            #\n");
    printf("# 2. Listar Canciones            #\n");
    printf("# 3. Escuchar Cancion            #\n");
    printf("# 4. Historial                   #\n");
    printf("# 5. Salir                       #\n");

    printf("##################################\n\n");

    printf("Por favor, ingresa el numero de la opcion deseada: ");
    mainOptionSelector();
}


/*
-------FUNCIONES AUXILIARES-------
*/
//cargar archivo
FILE* loadFile(char* dir, char* mode) {
    FILE* fptr = fopen(dir, mode);
    if (!fptr) {
        fprintf(stderr, "Error: No se pudo abrir %s.\n", dir);
        exit(1);
    }
    return fptr;
}

//Procesamiento de la entrada del usuario en menu principal
void mainOptionSelector() {
    char c;

    while (1) {
        scanf("%c", &c);

        if (c == '1') {
            enterSong();
            break;
        }
        else if (c == '2') {
            listSongs();
            break;
        }
        else if (c == '3') {
            listenToSong();
            break;
        }
        else if (c  == '4') {
            showHistory();
            break;
        }
        else if (c == '5') {
            exit(0);
        }
        else {
            fprintf(stderr, "Entrada invalida. Por favor, ingresa una opcion valida (1-5): ");
            fflush(stdin);
            continue;
        }
    };
}

//Guardar cancion en libreria de canciones
void saveNewSong(struct Song* newSong) {
    FILE* library = loadFile("songsLibrary.txt", "a");

    if (!library) {
        fprintf(stderr, "Error: no se pudo abrir la librería de canciones.\n");
        return 1;
    }

    fprintf(library, "%s %s %s %d\n", newSong->artist, newSong->title, newSong->duration, newSong->year);
    fclose(library);
    fflush(stdin);
}

//Comprobar que se haya ingresado la duracion con el formato 00:00
int checkDurationFormat(char* s) {
    if(strlen(s) != 5) return 0;

    if(s[2] != ':') return 0;

    if(!isdigit((unsigned char)s[0]) ||
       !isdigit((unsigned char)s[1]) ||
       !isdigit((unsigned char)s[3]) ||
       !isdigit((unsigned char)s[4])
       ) return 0;

    int a = (s[0] - '0') * 10 + (s[1] - '0');
    int b = (s[3] - '0') * 10 + (s[4] - '0');

    if ((a < 0 || a > 60) || (b < 0 || b > 60)) return 0;

    return 1;
}

//Obtener numeros de registros de un archivo
int getLinesOfFile(FILE* fp) {
    int c, count = 0;
    for (c = getc(fp); c != EOF; c = getc(fp)) {//se extraen caracteres del archivo hasta llegar al final
        if (c == '\n') count++;
    };

    rewind(fp);
    return count;
}

//Implementacion propia de quick sort basada en https://www.geeksforgeeks.org/c/quick-sort-in-c/
void swap(struct Song* a, struct Song* b) {
    struct Song temp = *a;
    *a = *b;
    *b = temp;
}

//devuelve punto pivote a partir del cual particionar el array
int partition(struct Song songs[], int low, int high) {
    int p = songs[low].year;
    int i = low;
    int j = high;

    while (i < j) {
        while(songs[i].year <= p && i < high) {
            i++;
        }

        while(songs[j].year > p && j > low) {
            j--;
        };
        if (i < j) {
            swap(&songs[i], &songs[j]);
        }
    };

    swap(&songs[low], &songs[j]);
    return j;
}

//divide el array en dos mitades recursivas a partir de un punto pivote
void quickSort(struct Song songs[], int low, int high) {
    if (low < high) {
        int p = partition(songs, low, high);

        quickSort(songs, low, p - 1);
        quickSort(songs, p + 1, high);
    }
}

