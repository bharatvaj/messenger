#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#include <clog.h>
#include "network_utils.h"
#include "config.h"

uint8_t is_server = 1;

pthread_t read_thread;

int buddyfd = -1;

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 256
#endif

#define ESCCHAR ((char)16)
#define BUDDYNAME "someone"
#define ESCSTRD ((char)2)
char ESCSTR[] = {ESCSTRD};


int i = 0;
// const char *getColor(){
//     i++;
//     if(i % 6 == 0)return COLOR_RED;
//     else if(i % 6 == 1)return COLOR_RED;
//     else if(i % 6 == 2)return COLOR_GREEN;
//     else if(i % 6 == 3)return COLOR_CYAN;
//     else if(i % 6 == 4)return COLOR_YELLOW;
//     else return COLOR_MAGENTA;
// }

void print(const char* str, int servfd){
    if(servfd != 0){
        // TODO This will probably cause segfault, check this out later.
        write(servfd, str, strlen(str));
        return;
    }
    // cout << getColor() << str << COLOR_RESET << endl;
    printf("%s\n", str);
}

void exit_handler(int sig){
    char c;
    fprintf(stderr, "Are you sure, you want to exit, y/N?");
    c = *fgets(&c, 1, stdin);
    if(c == 'Y' || c == 'y'){
        write(buddyfd, ESCSTR, 1);
        exit(EXIT_SUCCESS);
    }
}

int8_t is_writing = 1;
char *write_buffer = NULL;
void *reader(void *arg){
    if (write_buffer == NULL) {
        write_buffer = (char *)malloc(BUFFER_SIZE);
    }
    while (1) {
        char *buffer = (char *)malloc(BUFFER_SIZE);
        char c = '\0';
        int count = 0;
        while(c != ESCCHAR){
            read(buddyfd, &c, 1);
            buffer[count++] = c;
        }
        if(c == ESCSTR[0]){
            fprintf(stderr, "Buddy exited from chat!");
            print(ESCSTR, buddyfd);
            close(buddyfd);
            exit(EXIT_SUCCESS);
        }
        if(is_writing){
          // cout << '\r' << getColor() << BUDDYNAME << ": " << COLOR_RESET << buffer;
            printf("\r%s: %s\n"
                   "me: %s", BUDDYNAME, buffer, write_buffer);
            continue;
        }
        // cout << getColor() << BUDDYNAME << ": " << COLOR_RESET << buf << endl;
        printf("%s: %s\n", BUDDYNAME, write_buffer);
    }
    return NULL;
}

void writer(){
    while (1) {
        memset(write_buffer, '\0', BUFFER_SIZE);
        // cout << COLOR_RESET << "me: " << COLOR_RESET;
        fprintf(stderr, "me: ");
        is_writing = 1;
        char c = '\0';
        int writePtr = -1;
        while((c = getchar()) != '\n'){
            write_buffer[++writePtr] = c;
        }
	write_buffer[++writePtr]=ESCCHAR;
        //getline(cin, write_buffer);
        is_writing = 0;
        print(write_buffer, buddyfd);
    }
}

void cleanerThread(void *arg){
    fprintf(stderr, "bye!");
    close(buddyfd);
    exit(EXIT_SUCCESS);

}

void start_communication(){
    //pthread_cleanup_push(cleanerThread, NULL);
    pthread_create(&read_thread, NULL, &reader, NULL);
    writer();
    //pthread_join(read_thread, NULL);
}


int main(int argc, char *argv[]){
    if(argc < 2){
        fputs("Destination address is required\n", stderr);
        return -1;
    }
    signal(SIGINT, exit_handler);
    if(*argv[1] == 's'){
        if((buddyfd = start_server(DEFAULT_PORT)) < 0){
            fputs("Unexpected error occured\n", stderr);
            return -1;
        }
        start_communication();
        return 0;
    }
    const char* buddy = argv[1];
    if((buddyfd = connect_server(buddy, DEFAULT_PORT)) < 0){
        fputs("Cannot reach host, try again later\n", stderr);
        return -1;
    }
    fputs("Connection established succesfully\n", stderr);
    start_communication();
	return 0;
}
