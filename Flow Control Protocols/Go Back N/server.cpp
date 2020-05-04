#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
using namespace std;
//Server side
int main(int argc, char *argv[])
{
    //for the server, we only need to specify a port number
    if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    //grab the port number
    int port = atoi(argv[1]);
    //buffer to send and receive messages with
    char msg[1500];
     
    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    cout << "Waiting for a client to connect..." << endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    //receive a request from client using accept
    //we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    cout << "Connected with client!" << endl;
    //lets keep track of the session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    //also keep track of the amount of data sent as well
    int bytesRead, bytesWritten = 0;
    
    string data = "";
    string ack="";
    int i;

    while(1)
    {
        strcpy(msg,"");
        
        recv(newSd,&msg,sizeof(msg),0);
        cout<<"\n msg: "<<msg;
        //int ackno=0;

            if(strcmp(msg,"exit")!=0)
            {
                for(i=0;msg[i]!='\0';i++)
                {
                    
                    cout<<"\n Enter ACK for "<<msg[i]<<": ";
                    cin>>ack;
                    //ackno++;

                
                
                    if(ack=="1")
                    {
                        //put msg in data or array of strings or result
                        cout<<"\n ACK for "<<msg[i]<<" is: "<<ack;
                        data+=msg[i];  // strcat()
                        // cout<<"\n message received: "<<msg;
                        strcpy(msg,ack.c_str());
                        cout<<"\n Sending ACK: "<<msg;
                        send(newSd,&msg,sizeof(msg),0);
                        cout<<"\n sended. "<<endl;

                    }
                    else
                    {
                        cout<<"\n ACK for "<<msg[i]<<" is: "<<ack<<endl;

                        int fail = data.size();     //failed index. send this index again
                        strcpy(msg,ack.c_str());
                        send(newSd,&msg,sizeof(msg),0);
                        cout<<"\n Sending ACK: "<<msg<<endl;
                        send(newSd,&fail,sizeof(fail),0);
                        break;

                        
                    }

                }

            
            }
            else
            {
                cout<<"\n Msg received: "<<msg;     //should print exit
                break;

            }

        
        
        


        
    }
    cout<<"data received: "<<data;

    //we need to close the socket descriptors after we're all done
    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);
    cout << "\n\n ********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) 
        << " secs" << endl;
    cout << "Connection closed..." << endl;
    return 0;   
}
