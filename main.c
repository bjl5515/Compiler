#include <stdio.h>
#include <stdlib.h>

errno_t  errno_code;

long fileSize(FILE **file) {
    if (!file) {
        exit(0);
    }
    fpos_t origin = 0;
    /* get the current file position of  the stream, if that does not equal zero something went wrong and exit
     * otherwise continue
     */
    if (fgetpos(*file,&origin) != 0) {
        printf("Error : %i\n", errno);
        exit(0);
    }

    fseek(*file, 0, SEEK_END);     // Seek to the end of the file
    long offset = ftell(*file);                // ftell() returns the length of the file after seeking to the end

    /* sets the file position of the stream to the position given
     */
    if (fsetpos(*file, &origin) != 0) {
        printf("Error : %i\n", errno);
        exit(0);
    }
    return offset;
}

char* fileContent(char *path,FILE **file) {
    errno_code = fopen_s(file,path,"r");
    if (errno_code != 0) {
        printf("Error opening file at: %s", path);
        exit(EXIT_FAILURE);
    }
    long size = fileSize(file);
    char* buffer = malloc(size + 1);
    size_t bytesRead = fread_s(buffer, size,1,size,*file);
    if (bytesRead != size) {
        free(buffer);
        return NULL;
    }
    buffer[size] = '\0';
    return buffer;
}

void displayUse(char **argv){
    printf("TO COMPILE FILE USE : %s <path_to_file>\n", argv[0]);
}

FILE *file;
int main(int argc, char *argv[]) {
    if (argc < 2) {
       displayUse(argv);
        exit(0);
    }

    char* buffer = fileContent(argv[1], &file);

    printf("Contents of File %s:\n%s\n",argv[1], buffer);
    free(buffer);

    return 0;
}
