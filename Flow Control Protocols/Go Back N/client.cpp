/*cp ~/OS/LAB\ exercises/FORK/Q8.cpp ~/OS/shell.cpp*/
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
//Client side
int main(int argc, char *argv[])
{
    //we need 2 things: ip address and port number, in that order
    if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0); 
    } //grab the IP address and port number 
    char *serverIp = argv[1]; int port = atoi(argv[2]); 
    //create a message buffer 
    char msg[1500]; 
    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = 
        inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(clientSd,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout<<"Error connecting to socket!"<<endl; 
		exit(0);
    }
    cout << "Connected to the server!" << endl;
    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
   
    //logic
    bool canSend = true;

    //A is array of bits or array of strings;
    string A="";
    
    cout<<"\n Enter msg: ";
    cin>>A;

    //cout<<"\n msg: "<<A;



    int nbits = A.size();
    // cout<<nbits;
    
    int bit_sended = 0;
    int i  = 0;     //i is current frame;
    string m="";

    int N;
    //enter window size
    cout<<"\n Enter window size: ";
    cin>>N;
    int j;


    while(nbits != bit_sended )      //&& i<=nbits //run this loop until all bits are sended
    {   
       // cout<<"\n enter in while: ";
        //if(i != nbits && canSend==true)         //event request to send ---see if array of bits still have data to be sended
        
            //make frame of 1 bit
            //send frame
                m="";
            
            for(j=0;j<N;j++)
            {
                m+= A[i];
                i++;
    
            }
            strcpy(msg,m.c_str());

            send(clientSd, &msg ,sizeof(msg),0);
            cout<<"\n sending: "<<msg<<endl;
            

                
            for(int k=0;k<N;k++)
            {
                recv(clientSd, &msg, sizeof(msg),0); //ACK received in msg
                cout<<"ack received: "<<msg<<endl;

                 if(strcmp(msg,"1")==0)
                {
                    //print ACK
                    cout<<"\n Message received: "<<msg<<endl;
                    bit_sended = bit_sended+1;

                    //i++;       //send next bit -- increment curr_frame
                    //canSend = true;
                } 
                else
                {
                    int fail;
                    recv(clientSd, &fail, sizeof(fail),0); //failed index received;
                    cout<<"\n failed index: "<<fail;

                    i=fail;
                    break;
                }
            }
                
                    

            
            

            //canSend = false;

        
        // else
        //     break;       


        
               

    }

    //send exit
    strcpy(msg,"exit");
    send(clientSd,&msg,sizeof(msg),0);


    gettimeofday(&end1, NULL);
    close(clientSd);
    cout << "\n\n********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << 
    " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec- start1.tv_sec) 
      << " secs" << endl;
    cout << "Connection closed" << endl;
    return 0;    
}
