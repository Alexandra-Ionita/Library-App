#define HOST "34.246.184.49"
#define PORT 8080

char *handle_register_login(char *command);

char *handle_library_access(char *cookie);

char *handle_get_books(char *cookie, char *token);

char *handle_get_book(char *cookie, char *token);

char *handle_add_book(char *cookie, char *token);

char *handle_delete_book(char *cookie, char *token);

char *handle_logout(char *cookie);