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
//Server side
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    int port = atoi(argv[1]);

    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    cout << "Waiting for a client to connect..." << endl;
    listen(serverSd, 5);
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
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
    int m_bits, r_bits;
    int i,j;

    recv(newSd,&m_bits,sizeof(m_bits),0);
    recv(newSd,&r_bits,sizeof(r_bits),0);

    cout<<"\n M_bits: "<<m_bits;
    cout<<"\n R_bits: "<<r_bits;

    //receiving old parity bit array
    int C[r_bits+1]={0};
    recv(newSd,&C,sizeof(C),0);
    cout<<"\n old check bit array: ";
    for(i=1;i<=r_bits;i++)
        cout<<C[i]<<" ";
    cout<<endl;


    int rec_m[m_bits+r_bits+1]={0};
    recv(newSd,&rec_m,sizeof(rec_m),0);
    

    cout<<"\n received message: ";
    for(i=1;i<=(m_bits+r_bits);i++)
        cout<<rec_m[i]<<" ";
    cout<<endl;

    //giving error at pos 6
    rec_m[6] = !rec_m[6];

    //calculate new parity bits for received message
    int new_c[r_bits+1]={0};      //new check bits
    int k;
    for(i=1;i<=r_bits;i++)   //take check bit
    {
        k=i;
        for(j=1;j<=(m_bits+r_bits);j++)  //and with every bit
        {
            if(!pow_of_2(j) && isKthBitSet(j,k))
            {
                new_c[i] = new_c[i] ^ rec_m[j];   //xor
                
            }
        }
    }
    cout<<"\n new check bit array: ";
    for(i=1;i<=r_bits;i++)
        cout<<new_c[i]<<" ";
    cout<<endl;

    
    //xor of old and new parity bits
    int XOR[r_bits+1]={0};
    for(i=1;i<=r_bits;i++)
    {
        if(C[i]==new_c[i])
            XOR[i] = 0;
        else
            XOR[i] = 1;
    }

    cout<<"\n XOR output: ";
    for(i=1;i<=r_bits;i++)
        cout<<XOR[i];

    int base = 1;
    int N=0;
    //convert to decimal and check different cases 
    for(i=1;i<=r_bits;i++)
    {
        N += (base*XOR[i]);
        base = base *2;
    }

    cout<<"\n N = "<<N;
    if(N==0)
        cout<<"\n No error. ";
    else
        cout<<"\n error is at position: "<<N;

   
    close(newSd);
    close(serverSd);
    
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