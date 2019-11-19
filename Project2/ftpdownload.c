<<<<<<< HEAD
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>

#define MAX_BUFFER_SIZE 512
#define PORT_COMMANDS 21
#define PORT_DATA 20
//State Machine to parse input
enum STATES{

    ST_READ_FTP,
    ST_FAIL,
    ST_READ_USER,
    ST_READ_PASSWORD,
    ST_READ_HOST,
    ST_READ_URL_PATH

};

typedef int STATE;

// Tests
// ftp.up.pt/pub/apache/activemq/activemq-artemis-native/1.0.0/activemq-artemis-native-1.0.0-source-release.tar.gz

//Sets the maximum value the output of buffer can contain


int parseInput(const char * input,char * user,char * password,char * host,char * path,char * filename);

void buffers_cleaner(char * user,char * password,char * host,char * url_path){

    memset(user,0,MAX_BUFFER_SIZE);
    memset(password,0,MAX_BUFFER_SIZE);
    memset(host,0,MAX_BUFFER_SIZE);
    memset(url_path,0,MAX_BUFFER_SIZE);

}

struct hostent* DNS_CONVERT_TO_IP(char* DNS){
    
    struct hostent* ret_value;

    if(DNS==NULL){
        fprintf(stderr,"DNS INVALID REF\n");
        exit(-1);
    }

    if((ret_value=gethostbyname(DNS))==NULL){
        herror("gethostbyname:");
        exit(-1);
    }
        
    return ret_value;
}



int main(int argc, char * argv[]){


    if(argc!=2){
        fprintf(stderr,"Num args invalid\n");
    }

    if(argv[1]==NULL){
        fprintf(stderr,"Null argv pointer");
    }


    char user[MAX_BUFFER_SIZE],password[MAX_BUFFER_SIZE],host[MAX_BUFFER_SIZE],path[MAX_BUFFER_SIZE],filename[MAX_BUFFER_SIZE];
    int socket_control=-1,socket_data=-1;
    struct hostent* ip_info_from_dns=NULL;
    struct sockaddr_in control_server_addr;
    struct sockaddr_in data_server_addr;

    if(parseInput(argv[1],user,password,host,path,filename)!=0){
        fprintf(stderr,"Error in file parsing\n");
        exit(-1);
    }

    //Open both sockets
    if((socket_control=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("Error opening the control socket:");
        return -1;
    }

    if((socket_data=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("Erro in opening the data socket:");
        return -1;
    }
    //Transform DNS to IP ADDRESS
    ip_info_from_dns=DNS_CONVERT_TO_IP(host);

    //Fill sockets connection parameters

    //Erase any possible info inside this structs
    bzero((char*)&control_server_addr,sizeof(control_server_addr));
    bzero((char*)&data_server_addr,sizeof(data_server_addr));
    //Type of connection
    control_server_addr.sin_family = AF_INET;
    data_server_addr.sin_family = AF_INET;

	/*32 bit Internet address network byte ordered*/
    control_server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)ip_info_from_dns->h_addr)));
    data_server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)ip_info_from_dns->h_addr)));
    
    /*server TCP port must be network byte ordered */
	control_server_addr.sin_port = htons(PORT_COMMANDS);		
    data_server_addr.sin_port = htons(PORT_DATA);

    //Establish connection of socket control
    if(connect(socket_control, (struct sockaddr *)&control_server_addr, sizeof(control_server_addr)) < 0){
        perror("connect control socket()");
		exit(0);
	}
    //Establish connection of data control
    if(connect(socket_data, (struct sockaddr *)&data_server_addr, sizeof(data_server_addr)) < 0){
        perror("connect data socket()");
		exit(0);
	}	
    


    
    fprintf(stdout,"Not implemented yet\n");
    return 0;
}


int parseInput(const char * input,char * user,char * password,char * host,char * path,char * filename){

    if(input==NULL||user==NULL||password==NULL||host==NULL||path==NULL||filename==NULL){
        fprintf(stderr,"Invalid References in parse input\n");
        return -1;
    }

    //Const to avoid erros
    const int size_of_input=strlen(input);
    //In the worst case we could store a password and also a path here
    char path_and_filename[2*MAX_BUFFER_SIZE];
    char *pointer_aux=NULL;
    
    STATE current_state=ST_READ_FTP;

    int iterator_insert=0;

    for(int i=0;i<size_of_input;i++){

        switch (current_state)
        {
        case ST_READ_FTP:

            if(i==0){
                
                if(input[i]!='f')
                    current_state=ST_FAIL;
            }
            else if(i==1){
                
                if(input[i]!='t')
                    current_state=ST_FAIL;

            }
            else if(i==2){
                
                if(input[i]!='p')
                    current_state=ST_FAIL;

            }
            else if(i==3){
                
                if(input[i]!=':')
                    current_state=ST_FAIL;

            }
            else if(i==4){
                
                if(input[i]!='/')
                    current_state=ST_FAIL;

            }      
            else if(i==5){
                
                if(input[i]!='/')
                    current_state=ST_FAIL;
                else
                    current_state=ST_READ_USER;

            }

            break;
  
        case ST_READ_USER:

            if(i==size_of_input-1)
                current_state=ST_FAIL;
            
            //Checks if the max buffer_size is targeted, prevent memory faults
            if(iterator_insert==MAX_BUFFER_SIZE)
                current_state=ST_FAIL;

            if(input[i]==':'){
                current_state=ST_READ_PASSWORD;
                //Reseta o iterador auxiliar
                iterator_insert=0;
            }else{
                //Insere caracter a caracter na string de output
                user[iterator_insert]=input[i];
                iterator_insert++;
            }


            break;
        
        case ST_READ_PASSWORD:

            if(i==size_of_input-1)
                current_state=ST_FAIL;
            
            //Checks if the max buffer_size is targeted, prevent memory faults
            if(iterator_insert==MAX_BUFFER_SIZE)
                current_state=ST_FAIL;

            if(input[i]=='@'){
                current_state=ST_READ_HOST;
                //Reseta o iterador auxiliar
                iterator_insert=0;
            }else{
                //Insere caracter a caracter na string de output
                password[iterator_insert]=input[i];
                iterator_insert++;
            }

            break;
        
        case ST_READ_HOST:

            if(i==size_of_input-1)
                current_state=ST_FAIL;
            
            //Checks if the max buffer_size is targeted, prevent memory faults
            if(iterator_insert==MAX_BUFFER_SIZE)
                current_state=ST_FAIL;

            if(input[i]=='/'){
                current_state=ST_READ_URL_PATH;
                //Reseta o iterador auxiliar
                iterator_insert=0;
            }else{
                //Insere caracter a caracter na string de output
                host[iterator_insert]=input[i];
                iterator_insert++;
            }



            break;

        case ST_READ_URL_PATH:
                
            //Checks if the max buffer_size is targeted, prevent memory faults
            if(iterator_insert==MAX_BUFFER_SIZE)
                current_state=ST_FAIL;

            //Insere caracter a caracter na string de output
            path_and_filename[iterator_insert]=input[i];
            iterator_insert++;


            break;
        
        case ST_FAIL:
            
            fprintf(stderr,"ST_FAIL WAS REACHED\n");
            return(-1);

            break;
        
        default:
            fprintf(stderr,"Default was reached\n");
            current_state=ST_FAIL;
            break;
        }

    }

    //Diferenciate path from password
    // 213.13.65.217
    // pointer_aux=strrchr(path_and_filename,'/');
    pointer_aux=path_and_filename;
    
    //There is no path specified
    if(pointer_aux==NULL){
        path=NULL;
        strcpy(password,pointer_aux);
    }
    else{
        //++ to skip the /
        pointer_aux++;
        strcpy(filename,pointer_aux);
        pointer_aux--;
        //Place a \0 in the 
        *pointer_aux='\0';
        strcpy(path,pointer_aux);
        fprintf(stdout,"Nao sei se leva o ultimo / ou nao. Pus que nao precisava\n");

    }






    return 0;
}
=======

#include "ftp/ftp.h"

//State Machine to parse input
enum STATES
{

    ST_READ_FTP,
    ST_FAIL,
    ST_READ_USER_OR_HOST,
    ST_READ_PASSWORD,
    ST_READ_HOST,
    ST_READ_URL_PATH

};

typedef int STATE;

// Tests
// ftp.up.pt/pub/apache/activemq/activemq-artemis-native/1.0.0/activemq-artemis-native-1.0.0-source-release.tar.gz

//Sets the maximum value the output of buffer can contain

int parseInput(const char *input, char *user, char *password, char *host, char *path, char *filename);
int downloadFile(const int socket_data, const char *filename);
struct hostent *DNS_CONVERT_TO_IP(char *DNS);

void buffers_cleaner(char *user, char *password, char *host, char *url_path, char *filename, char *ip_data)
{

    memset(user, 0, MAX_BUFFER_SIZE);
    memset(password, 0, MAX_BUFFER_SIZE);
    memset(host, 0, MAX_BUFFER_SIZE);
    memset(url_path, 0, MAX_BUFFER_SIZE);
    memset(filename, 0, MAX_BUFFER_SIZE);
    memset(ip_data, 0, MAX_BUFFER_SIZE);
}

struct hostent *DNS_CONVERT_TO_IP(char *DNS)
{

    struct hostent *ret_value;

    if (DNS == NULL)
    {
        fprintf(stderr, "DNS INVALID REF\n");
        exit(-1);
    }

    if ((ret_value = gethostbyname(DNS)) == NULL)
    {
        herror("gethostbyname:");
        exit(-1);
    }

    return ret_value;
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Num args invalid\n");
    }

    if (argv[1] == NULL)
    {
        fprintf(stderr, "Null argv pointer");
    }

    char user[MAX_BUFFER_SIZE], password[MAX_BUFFER_SIZE], host[MAX_BUFFER_SIZE], path[MAX_BUFFER_SIZE], filename[MAX_BUFFER_SIZE];
    char reply_str[MAX_BUFFER_SIZE];
    char ip_data[MAX_BUFFER_SIZE];
    int code_returned = -1;
    int socket_control = -1, socket_data = -1;
    int port_data;
    struct hostent *ip_info_from_dns = NULL;
    struct sockaddr_in control_server_addr;
    struct sockaddr_in data_server_addr;
    buffers_cleaner(user, password, host, user, filename, ip_data);

    if (parseInput(argv[1], user, password, host, path, filename) != 0)
    {
        fprintf(stderr, "Error in file parsing\n");
        exit(-1);
    }

    //Open both sockets
    if ((socket_control = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error opening the control socket:");
        return -1;
    }

    //Transform DNS to IP ADDRESS
    ip_info_from_dns = DNS_CONVERT_TO_IP(host);

    //Fill sockets connection parameters

    //Erase any possible info inside this structs
    bzero((char *)&control_server_addr, sizeof(control_server_addr));
    //Type of connection
    control_server_addr.sin_family = AF_INET;

    /*32 bit Internet address network byte ordered*/
    control_server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)ip_info_from_dns->h_addr)));

    /*server TCP port must be network byte ordered */
    control_server_addr.sin_port = htons(PORT_COMMANDS);

    //Establish connection of socket control
    if (connect(socket_control, (struct sockaddr *)&control_server_addr, sizeof(control_server_addr)) < 0)
    {
        perror("connect control socket()");
        exit(-1);
    }

    if (ftp_read(socket_control, &code_returned, reply_str, sizeof(reply_str)) < 0)
    {
        fprintf(stderr, "Erro a receber resposta de boas vinda do server\n");
        return -1;
    }

    if (ftp_login(socket_control, user, password) < 0)
    {
        fprintf(stderr, "Erro em ftp_user\n");
        return -1;
    }

    if (ftp_passive_mode(socket_control, ip_data, &port_data) < 0)
    {
        fprintf(stderr, "FTP_PASSIVE_MODE\n");
        return -1;
    }

    //Creation + Config + Connect Socket Dados
    if ((socket_data = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Erro in opening the data socket:");
        return -1;
    }
    bzero((char *)&data_server_addr, sizeof(data_server_addr));
    data_server_addr.sin_family = AF_INET;
    data_server_addr.sin_addr.s_addr = inet_addr(ip_data);
    data_server_addr.sin_port = htons(port_data);

    //Establish connection of socket data
    if (connect(socket_data, (struct sockaddr *)&data_server_addr, sizeof(data_server_addr)) < 0)
    {
        perror("connect data socket()");
        exit(-1);
    }

    if (ftp_cwd(socket_control, path) < 0)
    {
        fprintf(stderr, "Erro while changing directory\n");
        return -1;
    }

    if (ftp_retr(socket_control, filename) < 0)
    {
        fprintf(stderr, "Error in retr command\n");
        return -1;
    }

    if (downloadFile(socket_data, filename) < 0)
    {
        fprintf(stderr, "Error in downloading file\n");
        return -1;
    }

    if (ftp_disc(socket_control, socket_data) < 0)
    {
        fprintf(stderr, "Error in ftp_disc\n");
        return -1;
    }

    return 0;
}

int parseInput(const char *input, char *user, char *password, char *host, char *path, char *filename)
{

    if (input == NULL || user == NULL || password == NULL || host == NULL || path == NULL || filename == NULL)
    {
        fprintf(stderr, "Invalid References in parse input\n");
        return -1;
    }

    //Const to avoid erros
    const int size_of_input = strlen(input);
    //In the worst case we could store a password and also a path here
    char path_and_filename[2 * MAX_BUFFER_SIZE];
    char user_or_host_str[MAX_BUFFER_SIZE];
    //Ensure 0 on this aux buffer
    memset(user_or_host_str, 0, MAX_BUFFER_SIZE);

    char *pointer_aux = NULL;

    STATE current_state = ST_READ_FTP;

    int iterator_insert = 0;

    for (int i = 0; i < size_of_input; i++)
    {

        switch (current_state)
        {
        case ST_READ_FTP:

            if (i == 0)
            {

                if (input[i] != 'f')
                    current_state = ST_FAIL;
            }
            else if (i == 1)
            {

                if (input[i] != 't')
                    current_state = ST_FAIL;
            }
            else if (i == 2)
            {

                if (input[i] != 'p')
                    current_state = ST_FAIL;
            }
            else if (i == 3)
            {

                if (input[i] != ':')
                    current_state = ST_FAIL;
            }
            else if (i == 4)
            {

                if (input[i] != '/')
                    current_state = ST_FAIL;
            }
            else if (i == 5)
            {

                if (input[i] != '/')
                    current_state = ST_FAIL;
                else
                    current_state = ST_READ_USER_OR_HOST;
            }

            break;

        case ST_READ_USER_OR_HOST:

            if (i == size_of_input - 1)
                current_state = ST_FAIL;

            //Checks if the max buffer_size is targeted, prevent memory faults
            if (iterator_insert == MAX_BUFFER_SIZE)
                current_state = ST_FAIL;

            if (input[i] == ':')
            {
                //WAS AN USER
                current_state = ST_READ_PASSWORD;
                strncpy(user, user_or_host_str, MAX_BUFFER_SIZE);
                //Reseta o iterador auxiliar
                iterator_insert = 0;
            }
            else if (input[i] == '/')
            {
                //WAS AN HOST
                current_state = ST_READ_URL_PATH;
                strncpy(host, user_or_host_str, MAX_BUFFER_SIZE);
                //IN THIS CASE THE USER IS ANNONYMOUS
                strcpy(user, ("anonymous"));
                //AND PASSWORD IS 0
                memset(password, 0, MAX_BUFFER_SIZE);
                //Reseta o iterador auxiliar
                iterator_insert = 0;
            }
            else
            {
                //Insere caracter a caracter na string de output
                user_or_host_str[iterator_insert] = input[i];
                iterator_insert++;
            }

            break;

        case ST_READ_PASSWORD:

            if (i == size_of_input - 1)
                current_state = ST_FAIL;

            //Checks if the max buffer_size is targeted, prevent memory faults
            if (iterator_insert == MAX_BUFFER_SIZE)
                current_state = ST_FAIL;

            if (input[i] == '@')
            {
                current_state = ST_READ_HOST;
                //Reseta o iterador auxiliar
                iterator_insert = 0;
            }
            else
            {
                //Insere caracter a caracter na string de output
                password[iterator_insert] = input[i];
                iterator_insert++;
            }

            break;

        case ST_READ_HOST:

            if (i == size_of_input - 1)
                current_state = ST_FAIL;

            //Checks if the max buffer_size is targeted, prevent memory faults
            if (iterator_insert == MAX_BUFFER_SIZE)
                current_state = ST_FAIL;

            if (input[i] == '/')
            {
                current_state = ST_READ_URL_PATH;
                //Reseta o iterador auxiliar
                iterator_insert = 0;
            }
            else
            {
                //Insere caracter a caracter na string de output
                host[iterator_insert] = input[i];
                iterator_insert++;
            }

            break;

        case ST_READ_URL_PATH:

            //Checks if the max buffer_size is targeted, prevent memory faults
            if (iterator_insert == MAX_BUFFER_SIZE)
                current_state = ST_FAIL;

            //Insere caracter a caracter na string de output
            path_and_filename[iterator_insert] = input[i];
            iterator_insert++;

            break;

        case ST_FAIL:

            fprintf(stderr, "ST_FAIL WAS REACHED\n");
            return (-1);

            break;

        default:
            fprintf(stderr, "Default was reached\n");
            current_state = ST_FAIL;
            break;
        }
    }

    //Diferenciate path from password
    // 213.13.65.217
    pointer_aux = strrchr(path_and_filename, '/');

    //There is no path specified
    if (pointer_aux == NULL)
    {
        path = NULL;
        strcpy(filename, path_and_filename);
    }
    else
    {
        //++ to skip the /
        pointer_aux++;
        strcpy(filename, pointer_aux);
        pointer_aux--;
        //Place a \0 in the
        *pointer_aux = '\0';
        strcpy(path, path_and_filename);
        fprintf(stdout, "Nao sei se leva o ultimo / ou nao. Pus que nao precisava\n");
    }

    return 0;
}

int downloadFile(const int socket_data, const char *filename)
{

    if (filename == NULL || socket_data < 0)
    {
        fprintf(stderr, "Parameters invalid in download File\n");
        return -1;
    }

    FILE *FP = fopen(filename, "w+");

    if (FP == NULL)
    {
        fprintf(stderr, "Error while creating the file\n");
        return -1;
    }
    //The bigger the fastest :)
    char buf_bytes[5 * MAX_BUFFER_SIZE];
    size_t n_bytes_lidos = -1;

    while ((n_bytes_lidos = read(socket_data, buf_bytes, sizeof(buf_bytes))) > 0)
    {

        if (n_bytes_lidos < 0)
        {
            fprintf(stderr, "Error while reading file\n");
            return -1;
        }

        if (fwrite(buf_bytes, n_bytes_lidos, 1, FP) < 0)
        {
            fprintf(stderr, "Error writing to file\n");
            return -1;
        }

        //Resets the content of the buffer to 0
        memset(buf_bytes, 0, sizeof(buf_bytes));
    }

    fclose(FP);
    fprintf(stdout, "File Read\n");

    return 0;
}
