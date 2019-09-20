#include<iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <iomanip>

#include <clog/clog.h>
#include "network_utils.h"
#include "init.h"
#include "config.h"

using namespace std;

bool isServer = false;

pthread_t readThread;

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

void *startServer(void *opt){
    buddyfd = start_server(DEFAULT_PORT);
    return NULL;
}

void print(string str, int servfd = 0){
    if(servfd != 0){
        write(servfd, (str).c_str(), str.size());
        return;
    }
    // cout << getColor() << str << COLOR_RESET << endl;
    cout << str << endl;
}

void exit_handler(int sig){
    char c = (char)32;
    print("Are you sure, you want to exit, y/N?");
    cin >> c;
    if(c == 'Y' || c == 'y'){
        write(buddyfd, ESCSTR, 1);
        exit(EXIT_SUCCESS);
    }
}

bool isWriting = true;
char *writeBuffer = (char *)malloc(BUFFER_SIZE);
void *reader(void *arg){
    while (1) {
        char *buffer = (char *)malloc(BUFFER_SIZE);
        char c = '\0';
        int count = 0;
        while(c != ESCCHAR){
            read(buddyfd, &c, 1);
            buffer[count++] = c;
        }
        if(c == ESCSTR[0]){
            print("Buddy exited from chat!");
            print(ESCSTR, buddyfd);
            close(buddyfd);
            exit(EXIT_SUCCESS);
        }
        string buf = string(writeBuffer);
        if(isWriting){
          // cout << '\r' << getColor() << BUDDYNAME << ": " << COLOR_RESET << buffer;
            cout << '\r' << BUDDYNAME << ": " << buffer
            << '\n' << "me: " << writeBuffer << std::flush;
            continue;
        }
        // cout << getColor() << BUDDYNAME << ": " << COLOR_RESET << buf << endl;
        cout << BUDDYNAME << ": " << buf << endl;
    }
    return NULL;
}

void writer(){
    while (1) {
        memset(writeBuffer, '\0', BUFFER_SIZE);
        // cout << COLOR_RESET << "me: " << COLOR_RESET;
        cout << "me: ";
        isWriting = true;
        char c = '\0';
        int writePtr = -1;
        while((c = getchar()) != '\n'){
            writeBuffer[++writePtr] = c;
        }
	writeBuffer[++writePtr]=ESCCHAR;
        //getline(cin, writeBuffer);
        isWriting = false;
        print(writeBuffer, buddyfd);
    }
}

void cleanerThread(void *arg){
    print("bye!");
    close(buddyfd);
    exit(EXIT_SUCCESS);

}

void start_communication(){
    //pthread_cleanup_push(cleanerThread, NULL);
    pthread_create(&readThread, NULL, &reader, NULL);
    writer();
    //pthread_join(readThread, NULL);
}


int main(int argc, char *argv[]){
    init();
    if(argc < 2){
        print("Destination address is required");
        return -1;
    }
    signal(SIGINT, exit_handler);
    if(*argv[1] == 's'){
        if((buddyfd = start_server(DEFAULT_PORT)) < 0){
            print("Unexpected error occured");
            return -1;
        }
        start_communication();
        return 0;
    }
    string buddy = argv[1];
    if((buddyfd = connect_server(buddy.c_str(), DEFAULT_PORT)) < 0){
        print("Cannot reach host, try again later");
        return -1;
    }
    print("Connection established succesfully");
    start_communication();
	return 0;
}
