#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "inc/networking.h"

// Creates a Hash Table of 10 users
// userTable = (struct hash *) calloc(10, sizeof(struct hash));

int GenerateUsers() {
     // Verify file exists, and open it
     FILE * file = fopen("Authentication.txt", "r");
     if (file == NULL) {
          printf("Failed to locate authentication.txt. Please check filepath.\n");
          return 1;
     }

     char * line = NULL;
     int currentLine = 0;
     size_t length = 0;
     int key = 0;
     char * user = NULL;
     char * pass = NULL;


     // Loop through .txt line by line
     while ((getline(&line, &length, file)) != -1) {
          currentLine++;

          char * token = strtok(line, " \t\n\r");
          int tokenCounter = 0;

          // Split on TAB delimiter until end of line
          while (token != NULL) {
               if (tokenCounter == 0) {
                    user = malloc(strlen(token) * sizeof(char));
                    strcpy(user, token);
               }
               else if (tokenCounter == 1) {
                    pass = malloc(strlen(token) * sizeof(char));
                    strcpy(pass, token);
               }
               token = strtok(NULL, "\t\n\r");
               tokenCounter++;
          }

          // Catch error in file and terminate program
          if (user == NULL || pass == NULL) {
               printf("Invalid format Line %d.\n. The program will now terminate.", currentLine + 1);
               fclose(file);
               return 0;
          }
          if (user != "Username"){
               printf("U: %s|\n P: %s|\n", user, pass);
          }

     }
     printf("%s, %d, %ld, %d\n", line, currentLine, length, key);
     fclose(file);
     return 1;
}

int count_users() {
     FILE * file = fopen("Authentication.txt", "r");
     if (file == NULL) {
          printf("Failed to locate authentication.txt. Please check filepath.\n");
          return CODE_ERROR;
     }

     int ch=0;
     int lines=-1;
     while(!feof(file)) {
          ch = fgetc(file);
          if(ch == '\n') {
               lines++;
          }
     }

     return lines;
}

int main()
{
     printf("%d", count_users());
     //int loaded = GenerateUsers();
}
