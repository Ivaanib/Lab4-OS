#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include "list.h"

#define PORT 9001
#define ACK "ACK"
  
int main(int argc, char const* argv[]) 
{ 
  
	  int n, val, idx;
    // create server socket similar to what was done in 
    // client program 
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
  
    // string store data to recv/send to/from client 
    char buf[1024];
		char sbuf[1024];
    char* token;

    // define server address 
    struct sockaddr_in servAddr; 
  
	  // list
		list_t *mylist;


    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 
  
    // bind socket to the specified IP and port 
    bind(servSockD, (struct sockaddr*)&servAddr, 
         sizeof(servAddr)); 
  
    // listen for connections 
    listen(servSockD, 1); 
  
    // integer to hold client socket. 
    int clientSocket = accept(servSockD, NULL, NULL); 

    mylist = list_alloc();  // create the list

    while(1){
        // recvs messages from client socket 
        n = recv(clientSocket, buf, sizeof(buf), 0);
				buf[n] = '\0';

        if(n > 0){
            token = strtok(buf, " ");
    
						if(strcmp(token,"exit") == 0){
							  list_free(mylist);
							  exit(1);
						}
						else if(strcmp(token,"get_length") == 0){
								// Make a Call
								val = list_length(mylist);
								sprintf(sbuf,"%s%d", "Length = ", val);
						}
						else if(strcmp(token,"add_front") == 0){
							  token = strtok(NULL, " ");  // get next token (value)
								val = atoi(token);
								// Make a Call
								sprintf(sbuf,"%s%d", ACK, val);
								list_add_to_front(mylist,val);
						}
						else if(strcmp(token,"remove_position") == 0){
							  token = strtok(NULL, " ");
								idx = atoi(token);
								// Make a Call
								val = list_remove_at_index(mylist,idx);
								sprintf(sbuf,"%s%d", ACK, val);
						}
						else if(strcmp(token,"print") == 0){
							  sprintf(sbuf,"%s", listToString(mylist));
						}
						// ADD THE OTHER OPERATIONS
						else if(strcmp(token,"add_back") == 0){
            token = strtok(NULL, " ");  // Get the value 
            if (token != NULL) {
                val = atoi(token);
                list_add_to_back(mylist,val);
                snprintf(sbuf, sizeof(sbuf), "%s", ACK);
            } else {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing value");
            }
        }
        else if(strcmp(token,"add_position") == 0){
            token = strtok(NULL, " ");  // Get the index 
            if (token != NULL) {
                idx = atoi(token);
                token = strtok(NULL, " ");  // Get the value 
                if (token != NULL) {
                    val = atoi(token);
                    list_add_at_index(mylist,val, idx);
                    snprintf(sbuf, sizeof(sbuf), "%s", ACK);
                } else {
                    snprintf(sbuf, sizeof(sbuf), "Error: Missing value");
                }
            } else {
                snprintf(sbuf, sizeof(sbuf), "Error: Missing index");
            }
        }
        else if(strcmp(token,"remove_back") == 0){
            // Call list_remove_from_back() 
            val = list_remove_from_back(mylist);
            if (val != -1)
                snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
            else
                snprintf(sbuf, sizeof(sbuf), "Error: List is empty");
        }
        else if(strcmp(token,"remove_front") == 0){
            // Call list_remove_from_front() 
            val = list_remove_from_front(mylist);
            if (val != -1)
                snprintf(sbuf, sizeof(sbuf), "Removed = %d", val);
            else
                snprintf(sbuf, sizeof(sbuf), "Error: List is empty");

                // send's messages to client socket 
            send(clientSocket, sbuf, sizeof(sbuf), 0);
        }
				memset(buf, '\0', 1024);
    }
  
    return 0; 
}
