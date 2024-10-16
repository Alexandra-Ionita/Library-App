#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

#include "command_handler.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    char *command = malloc(100);
    char *cookie;
    char *token;
    int sockfd;

    while (1) {
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        memset(command, 0, 100);
        fgets(command, 100, stdin);
        int len = strlen(command);
        command[len - 1] = '\0';

        if (!strcmp(command, "exit")) {
            if (response) {
                free(response);
            }
            if (message) {
                free(message);
            }
            if (command) {
                free(command);
            }
            if (cookie) {
                free(cookie);
            }
            if (token) {
                free(token);
            }
                break;
            }

        if (!strcmp(command, "register") || !strcmp(command, "login")) {
            message = handle_register_login(command);

            if (!message) {
                printf("ERROR: user and password must not contain spaces.\n");
                continue;
            }

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);  
            close(sockfd);

            if (strstr(response, "error")) {
                if (!strcmp(command, "register")) {
                    printf("ERROR at register: Username already exists.\n");
                } else {
                    printf("ERROR at login: Incorrect username or password.\n");
                }
            } else {
                if (!strcmp(command, "register")) {
                    printf("SUCCESS: User created successfully!\n");
                } else {
                    cookie = get_cookie(response);
                    printf("SUCCESS: 200 - OK - Welcome!\n");
                }
            }

            continue;
        }

        if (!strcmp(command, "enter_library")) {
            if (!cookie) {
                printf("ERROR at entering the library: you are not logged in.\n");
                continue;
            }
            message = handle_library_access(cookie);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);  
            close(sockfd);

            if (strstr(response, "error")) {
                printf("ERROR at entering the library: Access denied.\n");
            } else {
                token = get_token(response);
                printf("SUCCESS: You have entered the library!\n");
            }

            continue;
        }

        if (!strcmp(command, "get_books")) {
            if (!token) {
                printf("ERROR at getting books: you do not have access to the library.\n");
                continue;
            }
            message = handle_get_books(cookie, token);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);  
            close(sockfd);

            if (strstr(response, "error")) {
                printf("ERROR at getting books: Access denied.\n");
            } else {
                char *body = basic_extract_json_response(response);
                printf("[%s\n", body);
            }

            continue;
        }

        if (!strcmp(command, "get_book")) {
            if (!token) {
                printf("ERROR at getting book: you do not have access to the library.\n");
                continue;
            }
            message = handle_get_book(cookie, token);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);  
            close(sockfd);

            if (strstr(response, "error")) {
                printf("ERROR at getting book: The id book provided is invalid.\n");
            } else {
                char *body = basic_extract_json_response(response);
                printf("%s\n", body);
            }

            continue;
        }

        if (!strcmp(command, "add_book")) {
            message = handle_add_book(cookie, token);

            if (!message) {
                printf("ERROR at adding book: The informations are not correct.\n");
                continue;
            }

            if (!token) {
                printf("ERROR at adding book: you do not have access to the library.\n");
                continue;
            }

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);  
            close(sockfd);

            if (strstr(response, "error")) {
                printf("ERROR at adding book: The book provided is invalid.\n");
            } else {
                printf("SUCCESS: Book added successfully!\n");
            }

            continue;
        }

        if (!strcmp(command, "delete_book")) {
            if (!token) {
                printf("ERROR at deleting book: you do not have access to the library.\n");
                continue;
            }
            message = handle_delete_book(cookie, token);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);  
            close(sockfd);

            if (strstr(response, "error")) {
                printf("ERROR at deleting book: The book provided is invalid.\n");
            } else {
                printf("SUCCESS: Book deleted successfully!\n");
            }

            continue;
        }

        if (!strcmp(command, "logout")) {
            if (!cookie) {
                printf("ERROR at logging out: you are not logged in.\n");
                continue;
            }

            message = handle_logout(cookie);
            cookie = NULL;
            token = NULL;

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);  
            close(sockfd);

            if (strstr(response, "error")) {
                printf("ERROR at logging out: You could not log out.\n");
            } else {
                printf("SUCCESS: Logged out successfully!\n");
            }

            continue;
        }

        printf("ERROR: Invalid command.\n");
    }

    return 0;
}
