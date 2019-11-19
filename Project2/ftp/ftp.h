#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>

#define MAX_BUFFER_SIZE 512
#define PORT_COMMANDS 21


/**
 * ... description ... 
 * @param { control_socket_fd }      file descriptor of control socket
 * @param { filename }               path for the file to be transferred 
 */
int ftp_retr(const int control_socket_fd, const char *filename);

/**
 * ... description ... 
 * @param { control_socket_fd }      file descriptor of control socket
 * @param { path }                   path to change directory 
 */
int ftp_cwd(const int control_socket_fd, const char *path);


/**
* ... description ... 
* @param {...} ...
* @param {...} ...
* @return {...} ...
*/
int ftp_write(const int socket_fd, const char *msg);

/**
* ... description ... 
* @param { control_socket_fd }                  file descriptor of control socket
* @param { code_returned }                      first 3 digits of the message
* @return { string_returned }                   reply text without first 3 digit's
* @return { size_of_string_returned_array }     size of reply text without first 3 digit's
*/
int ftp_read(const int control_socket_fd, int *code_returned, 
                char *string_returned, const int size_of_string_returned_array);



/**
* ... description ... 
* @param { socket_control }      file descriptor of control socket
* @param { ip } ...
* @param { port } ...
*/
int ftp_passive_mode(const int socket_control,char *ip,int* port);

/**
* ... description ... 
* @param {...} ...
* @param {...} ...
* @return {...} ...
*/
int ftp_login(const int socket_control, const char *username, const char *password);
/**
 * 
 * 
 * 
 * 
 * 
 * 
*/
int ftp_disc(const int control_socket_fd,const int data_socket_fd);
