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
int pow_of_2(int a);
int isKthBitSet(int n, int k);
//Client side
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0); 
    } //grab the IP address and port number 
    char *serverIp = argv[1]; int port = atoi(argv[2]); 
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
    cout << "Connected to the server!" << endl;
    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    //logic

    int m_bits,i;
    cout<<"\n Enter no of bits in message: ";
    cin>>m_bits;

    int D[m_bits+1];          //data bits array
    cout<<"\n Enter message: ";
    for(i=1;i<=m_bits;i++)
        cin>>D[i];

    cout<<"\n Message: ";
    for(i=1;i<=m_bits;i++)
        cout<<D[i]<<" ";
    
    //caclulate r bits
    int y,r_bits;
    double z;
    for(y=1;;y++)
    {
        z = ceil(log(m_bits+y)/log(2));
        if(y>=z)
        {
            r_bits = y;
            break;
        }   

    }
    cout<<"\n M_bits: "<<m_bits;
    cout<<"\n R_bits: "<<r_bits;

    send(clientSd,&m_bits,sizeof(m_bits),0);
    send(clientSd,&r_bits,sizeof(r_bits),0);
    
    int trans_m[m_bits+r_bits+1];     
    int C[r_bits+1] ={0};      
    
    cout<<"\n Check bit array: ";
    for(i=1;i<=r_bits;i++)
        cout<<C[i]<<" ";
    
    int k=1;
    int l=1;
    for(i=1;i<=(m_bits+r_bits);i++)   
    {
        if(pow_of_2(i))             
        {
            trans_m[i] = C[k];
            k++;
        }
        else
        {
            trans_m[i] = D[l];
            l++;
        }      

    }
    
    int j;
    for(i=1;i<=r_bits;i++)   
    {
        k=i;
        for(j=1;j<=(m_bits+r_bits);j++)  
        {
            if(isKthBitSet(j,k))
            {
                C[i] = C[i] ^ trans_m[j];   
            }
        }
    }
    
    send(clientSd,&C,sizeof(C),0);
    //print check bit array
    cout<<"\n check bit array: ";
    for(i=1;i<=r_bits;i++)
        cout<<C[i]<<" ";

    //adding(putting values of) parity bit to trans_m
    k=1;
    for(i=1;i<=(m_bits+r_bits);i++)   //take r1 and calculate
    {
        if(pow_of_2(i))             //check if i is power of 2
        {
            trans_m[i] = C[k];
            k++;
        }
    }
    
    cout<<"\n Message (m+r): ";
    for(i=1;i<=(m_bits+r_bits);i++)
        cout<<trans_m[i]<<" ";
    cout<<endl;  

    send(clientSd,&trans_m,sizeof(trans_m),0);
    
    
    
    close(clientSd);
    return 0;    
}

int pow_of_2(int a)
{
    float x = log(a)/log(2);

    if(x==(int)x)
        return 1;
    else
        return 0;
}

int isKthBitSet(int n, int k) 
{ 
    if (n & (1 << (k - 1))) 
        return 1;
    else
        return 0; 
} 