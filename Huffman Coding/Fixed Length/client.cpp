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
#include <bits/stdc++.h>
using namespace std;
string addBinary(string a, string b);
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
    //char msg[1500]; 
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

    int n,i;
    cout<<"\n Enter no of characters: ";
    cin>>n;

    char C[n];

    for(i=0;i<n;i++)
    {
        cout<<"\n Enter character "<<i+1<<": ";
        cin>>C[i];
    }
    send(clientSd,&n,sizeof(n),0);
    send(clientSd,&C,sizeof(C),0);
    //no of bits
     double n_bits = log(n)/log(2) ;
     n_bits = ceil(n_bits);

    string str;
    for(i=0;i<n_bits;i++)       //creating string of n_bits zeros 000....00  
        str += '0';

    unordered_map<char,string> m;
    m[C[0]] = str;
    string b = "1";
    for(i=1;i<n;i++)
    {
        m[C[i]] = addBinary(m[C[i-1]], b);
    }
    
    //print lookup table
    for(auto x:m)
    {
        cout<<x.first<<" "<<x.second;
        cout<<endl;
    }    

    //encode message
    string msg,en_msg=" ";
    cout<<"\n Enter message to be encoded: ";
    cin>>msg;

    for(i=0;i<msg.size();i++)       //take a character and replace
    {
        en_msg += m[msg[i]]; 
    }
    cout<<"encoded message: "<<en_msg;

    char text[100];
    strcpy(text,en_msg.c_str());
    bytesWritten +=send(clientSd, &text, sizeof(text),0);  //send encoded msg
    //recv(clientSd,&text,sizeof(text),0);
    //cout<<text;
    //bytesWritten +=send(clientSd,&m,sizeof(m),0);      //send lookup table
    /*while(1)
    {
        cout << ">";
        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg));//clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            send(clientSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
        cout << "Awaiting server response..." << endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            cout << "Server has quit the session" << endl;
            break;
        }
        cout << "Server: " << msg << endl;
    }*/
    gettimeofday(&end1, NULL);
    close(clientSd);
    cout << "\n********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << 
    " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec- start1.tv_sec) 
      << " secs" << endl;
    cout << "Connection closed" << endl;
    return 0;    
}

string addBinary(string a, string b) 
{ 
    string result = ""; // Initialize result 
    int s = 0;          // Initialize digit sum 
  
    // Traverse both strings starting from last 
    // characters 
    int i = a.size() - 1, j = b.size() - 1; 
    while (i >= 0 || j >= 0 || s == 1) 
    { 
        // Comput sum of last digits and carry 
        s += ((i >= 0)? a[i] - '0': 0); 
        s += ((j >= 0)? b[j] - '0': 0); 
  
        // If current digit sum is 1 or 3, add 1 to result 
        result = char(s % 2 + '0') + result; 
  
        // Compute carry 
        s /= 2; 
  
        // Move to next digits 
        i--; j--; 
    } 
    return result; 
} 
