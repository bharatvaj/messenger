#include<iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <iomanip>

#define ENABLE_LOG
#define ENABLE_LOG_COLOR
#include "clog.h"
#include "network_utils.h"
#include "init.h"

using namespace std;

bool isServer = false;

pthread_t readThread;

int buddyfd = -1;

#define DEFAULT_PORT 27000

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 256
#endif

#define ESCCHAR ((char)16)
#define NEWCHAR ((char)15)

char ESCSTR[] = {ESCCHAR};
char NEWSTR[] = {NEWCHAR};

int i = 0;
string getColor(){
    i++;
    if(i % 6 == 0)return COLOR_RED;
    else if(i % 6 == 1)return COLOR_RED;
    else if(i % 6 == 2)return COLOR_GREEN;
    else if(i % 6 == 3)return COLOR_CYAN;
    else if(i % 6 == 4)return COLOR_YELLOW;
    else return COLOR_MAGENTA;
}

void *startServer(void *opt){
    buddyfd = start_server(DEFAULT_PORT);
    return NULL;
}

void print(string str, int servfd = 0){
    if(servfd != 0){
        write(servfd, (str + NEWCHAR).c_str(), str.size() + 1);
        return;
    }
    cout << getColor().c_str() << str << COLOR_RESET << endl;
}

void exit_handler(int sig){
    char c = (char)32;
    print("popaa po en ponath thaandi po, Y/N?");
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
        while(c != NEWCHAR && c != ESCCHAR){
            read(buddyfd, &c, 1);
            buffer[count++] = c;
        }
        if(c == ESCCHAR){
            print("bye da !");
            write_data(buddyfd, ESCSTR, 1);
            close(buddyfd);
            exit(EXIT_SUCCESS);
        }
        string buf = string(writeBuffer);
        if(isWriting){
            cout << '\r' << getColor() << "pangu: " << COLOR_RESET << buffer;
            cout << '\n' << "me: " << writeBuffer << std::flush;
            continue;
        }
        cout << getColor() << "pangu: " << COLOR_RESET << buf << endl;
    }
    return NULL;
}

void writer(){
    while (1) {
        memset(writeBuffer, '\0', BUFFER_SIZE);
        cout << COLOR_RESET << "me: " << COLOR_RESET;
        isWriting = true;
        char c = '\0';
        int writePtr = -1;
        while((c = getchar()) != '\n'){
            writeBuffer[++writePtr] = c;
        }
        //getline(cin, writeBuffer);
        isWriting = false;
        print(writeBuffer, buddyfd);
    }
}

void cleanerThread(void *arg){
    print("bye da !");
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
        print("ip address kudra venna");
        return -1;
    }
    signal(SIGINT, exit_handler);
    if(*argv[1] == 's'){
        if((buddyfd = start_server(DEFAULT_PORT)) < 0){
            print("etho error daw close pantu open pannu");
            return -1;
        }
        start_communication();
        return 0;
    }
    string buddy = argv[1];
    if((buddyfd = connect_server(buddy.c_str(), DEFAULT_PORT)) < 0){
        print("target reach panna mudila bha, net modhala connect panniyaa!?");
        return -1;
    }
    print("connet aaiduchu... pesuraa ippo pesu");
    start_communication();
	return 0;
}
