#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CMD_LEN 512
#define MAX_EMAILS 100

typedef struct {
    char from[100];
    char to[100];
    char subject[200];
    char date[50];
} Email;

int method;

int email_compare(const void* a, const void* b) 
{
    const Email* email1 = a;
    const Email* email2 = b;
    int result;

    if (method == 1) 
    {
        result = strcmp(email1->from, email2->from);
    }
    else 
    {
        result = strcmp(email1->date, email2->date);
    }

    return result;
}

int read_emails(Email* emails, int max_emails) 
{
    char cmd[MAX_CMD_LEN];
    sprintf(cmd, "mail -p");

    FILE* fp = popen(cmd, "r");
    if (fp == NULL) 
    {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char line[1024];
    while (fgets(line, sizeof(line), fp) != NULL) 
    {
        if (count >= max_emails) break;
        
        char token[100];
        Email email;
        while (fgets(token, sizeof(token), fp)) 
        {
            if (strncmp(token, "From:", 5) == 0) {
                sscanf(token, "From:%99s", email.from);
            } else if (strncmp(token, "Subject:", 8) == 0) {
                sscanf(token, "Subject:%199s", email.subject);
            } else if (strncmp(token, "To:", 3) == 0) {
                sscanf(token, "To:%99s", email.to);
            } else if (strncmp(token, "Date:", 5) == 0) {
                sscanf(token, "Date:%49s", email.date);
                emails[count++] = email;
            }
        }
    }    
    pclose(fp);

    return count;
}

void send_email(const char* to, const char* subject, const char* body) 
{
    char cmd[MAX_CMD_LEN];
    sprintf(cmd, "echo \"%s\" | mail -s \"%s\" \"%s\"", body, subject, to);
    FILE* mail = popen(cmd, "w");
    if (mail == NULL) {
        fprintf(stderr, "Error: could not open mail program\n");
        exit(EXIT_FAILURE);
    }
    pclose(mail);
}

int main(int argc, char** argv) 
{
    if (argc == 2) {
        // użytkownik podał jeden argument - sortujemy wg. adresu e-mail lub daty
        if (strcmp(argv[1], "nadawca") == 0)  method = 1;
        else if (strcmp(argv[1], "data") == 0) method = 2;
        else {
            fprintf(stderr, "Invalid arguments\n");
            exit(EXIT_FAILURE);
        }

        Email* emails = malloc(MAX_EMAILS * sizeof(Email));
        int email_count = read_emails(emails, MAX_EMAILS);
        qsort(emails, email_count, sizeof(Email), email_compare);

        for (int i = 0; i < email_count; i++) 
        {
            if (strcmp(emails[i].subject, "") != 0) 
            {
                printf("From: %s, ", emails[i].from);
                printf("To: %s, ", emails[i].to);
                printf("Subject: %s, ", emails[i].subject);
                printf("Date: %s\n", emails[i].date);
                printf("\n");
            }
        }
        free(emails);

    }

    else if (argc == 4) {
        // użytkownik podał trzy argumenty - wysyłamy e-mail
        send_email(argv[1], argv[2], argv[3]);
    }

    else{
        perror("Use one arg: <data>/<nadawca> \nor three args: <EmailAdress> <Title> <Message>");
        return 1;
    }
    return 0;
}