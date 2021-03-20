#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#define BUFFER_MAX 2048

// driver
int main(int argc, char *argv[])
{
    char buffer[BUFFER_MAX];
    char command[BUFFER_MAX];
    char* str = NULL;
    FILE* w;
    FILE* fp;
    DIR* dp;
    struct dirent* sd;
    //-------------------------------------------------------------------------------------------
    if(argc != 4)
    {
        fprintf(stderr, "extcopy [source] [destination] extension\n\nfatal: cannot execute command\n\n");
        fprintf(stderr, "e.g.:\nextcopy /home/user/source/ /home/user/destination/ .extension\n");
        exit(-3);
    }

    //-------------------------------------------------------------------------------------------
    fp = fopen("copy.tmp", "w");
    if(fp==NULL)
    {
        fprintf(stderr, "Unable to open file.\n");
        exit(-1);
    }

    dp = opendir(argv[1]); //put source dir here
    if(dp==NULL)
    {
        fprintf(stderr, "Unable to open directory.\n");
        exit(-2);
    }

    while((sd=readdir(dp))!=NULL)
        fprintf(fp, ">> %s\n", sd->d_name);
    fclose(fp);
    closedir(dp);
    //------------------------------------------------------------------------------------------
    fp = fopen("copy.tmp", "r");
    if(fp==NULL)
    {
        fprintf(stderr, "Unable to open file.\n");
        exit(-1);
    }

    w = fopen("copy1.tmp", "w");
    if(w==NULL)
    {
        fprintf(stderr, "Unable to open file.\n");
        exit(-1);
    }

    while(fscanf(fp, ">> %s\n", buffer)!=EOF)
    {
        str = strstr(buffer, argv[3]); // put extension here
        if(str != NULL)
            fprintf(w, "%s\n", buffer);
    }
    fclose(w);
    fclose(fp);
    remove("copy.tmp");
    //--------------------------------------------------------------------------------------------
    fp = fopen("copy1.tmp", "r");
    if(fp==NULL)
    {
        fprintf(stderr, "Unable to open the file.\n");
        exit(-1);
    }

    while(fscanf(fp, "%s\n", buffer)!=EOF)
    {
        strcpy(command, "cp \"");
        strcat(command, argv[1]);
        strncat(command, buffer, strlen(buffer)+1);
        strcat(command, "\" \"");
        strcat(command, argv[2]); // put dest dir here
        strcat(command, "\"");
        system(command);
    }
    fclose(fp);
    remove("copy1.tmp");
    //--------------------------------------------------------------------------------------------
    return 0;
}
