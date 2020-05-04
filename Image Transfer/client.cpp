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
        
    fstream f1;
	char ch;
	
	f1.open("MARBLES.BMP", ios::in | ios::binary);
	
	while(!f1.eof())
	{
	
		ch = f1.get();

		//memset(&msg, 0, sizeof(msg));
		//strcpy(msg,str.c_str());
		//msg[1] = '\0';
		bytesWritten +=send(clientSd, &ch, sizeof(ch),0); //strlen(ch)
		
	}
	//cout<<ch;
	//cout<<msg;
	cout<<"\n File sended successfully. \n";

	char exit = '@';	
	//cout<<">";
	//cin>>exit;		//x is the exit 
	bytesWritten +=send(clientSd, &exit, sizeof(exit),0); //strlen(ch)
	//char exit[] = "exit"; 
	//memset(&msg, 0, sizeof(msg));
	//strcpy(msg,exit);
	//send(clientSd, (char*)msg, strlen(msg),0);
	f1.close();
	//f2.close();
        
    gettimeofday(&end1, NULL);
    close(clientSd);
    cout << "********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << 
    " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec- start1.tv_sec) 
      << " secs" << endl;
    cout << "Connection closed" << endl;
    return 0;    
}
