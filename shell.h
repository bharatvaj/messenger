#ifndef SHELL_H
#define SHELL_H
#define _SH_TAG "SHELL"
/*
 * shell.h
 *
 *  Created on: 23-Oct-2016
 *      Author: root
 */
#ifdef __GNUG__
#include <iostream>
#else
#include <stdio.h>
#endif
#include <string.h>
#include "clog.h"

#define SH_BUFSIZE 1024 //shell read buffer size
#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIM " \t\r\n\a" //shell token delimeter

typedef struct job job;
typedef struct option option;

struct option{
    const char *word;
    const char letter;
    const char *description;
};

struct job{
    const char *command;
    const char *info;
    int (*function)(int, char**);
    int opt_length;
    option *options;
};

int shell_socket = 2;

int sh_process(int jlen, job *jobs, char *line){
int count = 0;
  ssize_t arg_buffsize = 64; //chage variably
    char **args = (char **)calloc(arg_buffsize, SH_BUFSIZE);

	 char *token = strtok(line, SH_TOK_DELIM);
    while(token != NULL){

		args[count++] = token;
		if(count == arg_buffsize){
			arg_buffsize += 64;//change variably
			args = (char **)realloc(args, sizeof(char) * arg_buffsize);
		}
		token = strtok(NULL, SH_TOK_DELIM);
    }
	for (int i = 0; i < jlen; i++) {
        	if (strcmp(*args, jobs[i].command) == 0) {
            		return (jobs[i].function)(count, ++args);
        	}
	}
	printf("you clearly need 'help'\n");
    return -1;//FIXME add custom codes to identify returns and errors
}

int sh_next(int jlen, job *jobs, const char *_shell_name){
    	int bufsize = SH_BUFSIZE;
      printf("%s", _shell_name);

	int position = 0;
	char c;
	char * buffer = (char *) malloc(sizeof(char) * bufsize);

	if (!buffer)
		log_fat(_SH_TAG, "allocation error, not enough memory...closing");

	while (1) {
		//read a character
		c = getchar();
		//if we hit EOF or '\n' update it as null '\0' and return
		if (c == EOF || c == '\n' || c == '\r') {
			buffer[position] = '\0';
			break;
		} else {
			buffer[position] = c;
		}
		position++;

		//if we reached the buffer size, reallocate
		if (position == bufsize) {
			bufsize += SH_BUFSIZE;
			buffer = (char *) realloc(buffer, sizeof(char) * bufsize);//TODO replace with calloc + realloc
			if (!buffer)
				log_fat(_SH_TAG, "allocation error, not enough memory...closing");
		}
	}

    if(*buffer == '\0'){
	    //an empty command was entered
	    return 0;
    }
    return sh_process(jlen, jobs, buffer);
   }


int start_shell(int jlen, job jobs[], const char *_shell_name, int exit_code = 100){
	int status = 0;
	while(status != exit_code){
		status = sh_next(jlen, jobs, _shell_name);
		log_inf("SHELL", "Process exited with status %d", status);
	}
	return 0;
}

/* job1
 *  option 1 [-o]
 *  ...
 *  ...
 */

int sh_help(int jlen, job *jbs, const char *opt = "\0"){
    for(int i = 0; i < jlen; i++){
        printf("%s\t - %s\n", jbs[i].command, jbs[i].info);
        for(int j = 0; j < jbs[i].opt_length; j++){
            printf("  |--%s[-%c] - %s\n", jbs[i].options[j].word, jbs[i].options[j].letter, jbs[i].options[j].description);
        }
    }
    return 0;
}


#endif
