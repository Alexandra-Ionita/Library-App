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
#include "parson.h"
#include "command_handler.h"

char *handle_register_login(char *command) {
    char *username = malloc(300);
    char *password = malloc(300);
    char *content_type = malloc(100);
    char *url = malloc(300);
    int len;
    strcpy(content_type, "application/json");

    if (!strcmp(command, "register")) {
        strcpy(url, "/api/v1/tema/auth/register");
    } else {
        strcpy(url, "/api/v1/tema/auth/login");
    }
    
    printf("username=");
    fgets(username, 300, stdin);
    len = strlen(username);
    username[len - 1] = '\0';

    printf("password=");
    fgets(password, 300, stdin);
    len = strlen(password);
    password[len - 1] = '\0';
    if (strstr(password, " ") || strstr(username, " ")) {
        return NULL;
    }

    JSON_Value *payload_val = json_value_init_object();
    JSON_Object *payload_obj = json_value_get_object(payload_val);

    json_object_set_string(payload_obj, "username", username);
    json_object_set_string(payload_obj, "password", password);

    char *body_data = json_serialize_to_string_pretty(payload_val);

    char *message = compute_post_request(
        HOST,
        url,
        content_type,
        body_data,
        NULL,
        0,
        NULL
    );

    free(username);
    free(password);
    free(content_type);
    free(url);
    json_free_serialized_string(body_data);
    json_value_free(payload_val);

    return message;
}

char *handle_library_access(char *cookie) {
    char *url = malloc(300);
    strcpy(url, "/api/v1/tema/library/access");

    char *message = compute_get_delete_request(
        HOST,
        url,
        cookie,
        NULL,
        "GET"
    );

    free(url);

    return message;
}

char *handle_get_books(char *cookie, char *token) {
    char *url = malloc(300);
    strcpy(url, "/api/v1/tema/library/books");

    char *message = compute_get_delete_request(
        HOST,
        url,
        cookie,
        token,
        "GET"
    );

    free(url);

    return message;
}

char *handle_get_book(char *cookie, char *token) {
    char *id = malloc(100);
    printf("id=");
    fgets(id, 100, stdin);
    int len = strlen(id);
    id[len - 1] = '\0';

    char *url = malloc(300);
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, id);

    char *message = compute_get_delete_request(
        HOST,
        url,
        cookie,
        token,
        "GET"
    );

    free(url);

    return message;
}

char *handle_add_book(char *cookie, char *token) {
    char *title = malloc(300);
    char *author = malloc(300);
    char *genre = malloc(300);
    char *page_count = malloc(300);
    char *publisher = malloc(300);
    int len;

    printf("title=");
    fgets(title, 300, stdin);
    len = strlen(title);
    title[len - 1] = '\0';

    printf("author=");
    fgets(author, 300, stdin);
    len = strlen(author);
    author[len - 1] = '\0';

    printf("genre=");
    fgets(genre, 300, stdin);
    len = strlen(genre);
    genre[len - 1] = '\0';

    printf("publisher=");
    fgets(publisher, 300, stdin);
    len = strlen(publisher);
    publisher[len - 1] = '\0';

    printf("page_count=");
    fgets(page_count, 300, stdin);
    len = strlen(page_count);
    page_count[len - 1] = '\0';

    char *numbers = "0123456789";
    int not_ok = 0;
    for (int i = 0; i < strlen(page_count); i++) {
        if (!strchr(numbers, page_count[i])) {
            not_ok = 1;
            break;
        }
    }

    if ((!strlen(title) || atoi(title)) || (!strlen(author) || atoi(author)) ||
        (!strlen(genre) || atoi(genre)) 
        || (!strlen(publisher) || atoi(publisher)) || (!len || not_ok)) {
            free(title);
            free(author);
            free(genre);
            free(page_count);
            free(publisher);
            return NULL;
    }

    char *url = malloc(300);
    strcpy(url, "/api/v1/tema/library/books");

    char *content_type = malloc(100);
    strcpy(content_type, "application/json");

    JSON_Value *payload_val = json_value_init_object();
    JSON_Object *payload_obj = json_value_get_object(payload_val);

    json_object_set_string(payload_obj, "title", title);
    json_object_set_string(payload_obj, "author", author);
    json_object_set_string(payload_obj, "genre", genre);
    json_object_set_string(payload_obj, "publisher", publisher);
    json_object_set_number(payload_obj, "page_count", atoi(page_count));

    char *body_data = json_serialize_to_string_pretty(payload_val);

    char *message = compute_post_request(
        HOST,
        url,
        content_type,
        body_data,
        cookie,
        strlen(cookie),
        token
    );

    free(url);
    free(title);
    free(author);
    free(genre);
    free(page_count);
    free(publisher);

    return message;
}

char *handle_delete_book(char *cookie, char *token) {
    char *id = malloc(100);
    printf("id=");
    fgets(id, 100, stdin);
    int len = strlen(id);
    id[len - 1] = '\0';

    char *url = malloc(300);
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, id);

    char *message = compute_get_delete_request(
        HOST,
        url,
        cookie,
        token,
        "DELETE"
    );

    free(url);

    return message;
}

char *handle_logout(char *cookie) {
    char *url = malloc(300);
    strcpy(url, "/api/v1/tema/auth/logout");

    char *message = compute_get_delete_request(
        HOST,
        url,
        cookie,
        NULL,
        "GET"
    );

    free(url);

    return message;
}