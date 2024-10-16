Name: Ionita Alexandra-Mihaela
Personal email: alexandraionita901@gmail.com

                            Library App

*** Overview ***
This project is a command-line client application written in C for interacting
with a remote library server. It supports user registration, login, accessing
the library, viewing books, adding books, deleting books, and logging out. The
application communicates with the server using HTTP requests and processes JSON
responses.

*** File Structure ***
- main.c: The main driver of the application, handling user commands and
interactions with the server.
- helpers.h and helpers.c: Utility functions for building HTTP requests and
processing responses.
- requests.h and requests.c: Functions to create and send specific HTTP
requests.
- command_handler.h and command_handler.c: Handlers for different commands such
as login, register, access library, etc.
- parson.h and parson.c: A lightweight JSON library for C.

*** Code Explanation ***
The main function handles the user input loop, sending appropriate requests to
the server and processing the responses. It uses various helper functions
defined in other files to build and send requests. Using a loop, the program
continuously waits for user input and establishes a connection to the server
using open_connection, than with memset it clears the command buffer, and with
fgets it reads the user's command from stdin.

*** Commands Explanation ***
- exit: For this command, all the allocated memory id freed and the program
stops.

- register and login: For this command the program uses the
handle_register_login function that handles both registration and login. It
constructs the appropriate JSON payload for the request, depending on whether
the user is registering or logging in. It reads the username and password from
the user, checks for spaces (invalid input), and constructs a POST request. If
there are spaces in the username or password, the program will show an error
message without getting to the POST request part. Otherwise, after the POST
request, the message created is sent to the server. Then, the server's response
is saved. If the server sent any error, based on the command (register or login)
an error message will be displaied. If everything works fine, a SUCCESS message
will be displaied and, if the login is successful, the function "get_cookie"
will extract the cookie from the response in order to verify that the user is
logged in for entering the library.

- enter_library: For this command the program firstly checks if the user is
logged in (if the cookie is not null). If it is null, an error message will be
displaied. If the user is logged in, the program uses the handle_library_access
function that constructs a GET request to access the library using the provided
cookie. Then, the send_to_server and receive_from_server functions handle
sending the request to the server and receiving the response. If there are not
any errors in the response, the function get_token extracts the token from the
response to use it for future commands and a SUCCESS message will pe displaied.
Otherwise, an error message will be displaied.

- get_books: For this command, the program verifies if the user has access to
the library (if the token is not null). If it does not, it displays an error
message. If the user has access to the library, the handle_get_books function
constructs a GET request to retrieve the list of books using the provided cookie
and token. hen, the send_to_server and receive_from_server functions handle
sending the request to the server and receiving the response. If there are not
any errors in the response, the function basic_extract_json_response extracts
the JSON text containing the books from the response and displays it. Otherwise,
an error message will be displaied.

- get_book: For this command, the program verifies if the user has access to the
library (if the token is not null). If it does not, it displays an error
message. If the user has access to the library, the handle_get_book function
constructs a GET request to retrieve the details of a specific book using the
provided cookie and token. The user is prompted to enter the book ID. Then, the
send_to_server and receive_from_server functions handle sending the request to
the server and receiving the response. If there are no errors in the response,
the function basic_extract_json_response extracts the JSON text containing the
book details from the response and displays it. Otherwise, an error message will
be displayed.

- add_book: For this command, the program uses the handle_add_book function to
construct a POST request to add a new book using the provided cookie and token.
The user is prompted to enter details such as title, author, genre, publisher,
and page count. If the input is valid (if all the details are completed and the
page count is a number), the request payload is built. Then, the send_to_server
and receive_from_server functions handle sending the request to the server and
receiving the response. If there are no errors in the response, a SUCCESS
message is displayed. Otherwise, an error message will be displayed.

- delete_book: For this command, the program verifies if the user has access to
the library (if the token is not null). If it does not, it displays an error
message. If the user has access to the library, the handle_delete_book function
constructs a DELETE request to delete a specific book using the provided cookie
and token. The user is prompted to enter the book ID. Then, the send_to_server
and receive_from_server functions handle sending the request to the server and
receiving the response. If there are no errors in the response, a SUCCESS
message is displayed. Otherwise, an error message will be displayed.

- logout: For this command, the program first checks if the user is logged in
(if the cookie is not null). If it is null, an error message will be displayed.
If the user is logged in, the handle_logout function constructs a GET request to
log out the user using the provided cookie. Then, the send_to_server and
receive_from_server functions handle sending the request to the server and
receiving the response. If there are no errors in the response, the cookie and
token are set to NULL, and a SUCCESS message is displayed. Otherwise, an error
message will be displayed.

- If the user enters an invalid command, an error message is displayed.

*** Helper Functions ***
- compute_message: This function constructs the message to be sent by appending
lines to the message buffer.

- open_connection: This function opens a socket connection to the specified host
and port.

- send_to_server: This function sends a message to the server using the open
socket connection.

- receive_from_server: This function receives a response from the server and
returns it as a string.

*** Request Handlers ***
- compute_get_delete_request: This function constructs a GET or DELETE HTTP
request. It takes the host, URL, cookies, token, and request type (GET or
DELETE) as parameters. It appends the necessary headers and returns the
constructed message.

- compute_post_request: This function constructs a POST HTTP request. It takes
the host, URL, content type, body data, cookies, and token as parameters. It
appends the necessary headers and body data, and returns the constructed
message.

*** Command Handlers ***
- handle_register_login: This function handles user registration and login. It
constructs the appropriate JSON payload for the request, depending on whether
the user is registering or logging in. It reads the username and password from
the user, checks for spaces (invalid input), and constructs a POST request.

- handle_library_access: This function constructs a GET request to access the
library using the provided cookie.

- handle_get_books: This function constructs a GET request to retrieve the list
of books using the provided cookie and token.

- handle_get_book: This function constructs a GET request to retrieve the
details of a specific book using the provided cookie and token. It prompts the
user to enter the book ID.

- handle_add_book: This function constructs a POST request to add a new book
using the provided cookie and token. It prompts the user to enter details such
as title, author, genre, publisher, and page count.

- handle_delete_book: This function constructs a DELETE request to delete a
specific book using the provided cookie and token. It prompts the user to enter
the book ID.

- handle_logout: This function constructs a GET request to log out the user
using the provided cookie.

*** JSON Handling ***
Parson is a lightweight JSON library used for creating and parsing JSON data.
It is used to build the request payloads and parse the server responses.
