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
#define Max 100
int N = 0;

struct node
{
	char ch;
	int freq;
	struct node *left;
	struct node *right;
};
struct node H[Max];	//contains deleted array
int LookUP[128][20];

void insert(struct node Heap[], int freq, char ch, struct node *left_address, struct node *right_address);
struct node Delete(struct node Heap[]);
void preorder(struct node *ptr);
void printCodes(struct node *root, int arr[], int top);
void decode(struct node *root, char s[]);

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
    
    struct node Heap[Max];		//array of structures
	int n, i, freq;
	int j;
	char ch;
	//input
	cout << "\n Enter no of elements: ";
	cin >> n;

    send(clientSd,&n,sizeof(n),0);

	for (i = 1; i <= n; i++)
	{
		cout << "\n Enter character: ";
		cin >> ch;
        send(clientSd,&ch,sizeof(ch),0);

		cout << "\n Enter frequency; ";
		cin >> freq;
        send(clientSd,&freq,sizeof(freq),0);


		insert(Heap, freq, ch, NULL, NULL);		//return structure

	}
    

	for (i = 1; i <= N; i++)
	{
		cout << Heap[i].left << " " << Heap[i].ch << " " << Heap[i].freq << " " << Heap[i].right << " ";
		cout << endl;
	}
	
	//initialise lookip table with @

	for (i = 0; i < 128; i++)
		for (j = 0; j < 20; j++)
			LookUP[i][j] = '@';

    i = 1;
	while (N > 1)
	{
		H[i] = Delete(Heap);	//min1
		struct node *left_address = &H[i];
		i++;

		
		H[i] = Delete(Heap);
		struct node *right_address = &H[i];
		i++;


		insert(Heap, (H[i-2].freq + H[i-1].freq), '#', left_address, right_address);		//deleted elements are moved back. pass their address for left and right child
		//another way
		//insert(Heap, (H[i - 2].freq + H[i - 1].freq), '#', &H[i-2], &H[i-1]);
	}

	for (i = 1; i <= N; i++)
	{
		cout << Heap[i].left << " " << Heap[i].ch << " " << Heap[i].freq << " " << Heap[i].right << " ";
		cout << endl;
	}

	// tree formed or not
	int arr[Max];
	preorder(&Heap[1]);
	printCodes(&Heap[1], arr, 0);



	cout << "\n Look-up table: \n";
	for (i = 97; i <= 101; i++)			//print form 97 to end (till z)
	{	
		cout<<(char)i << ":";
		for (j = 0; LookUP[i][j] != '@'; j++)
			cout<< LookUP[i][j];
		cout << endl;
	}

	//encoding
	char str[100];
	int Estr[500];//Estr contains encoded string
	cout << "\n Enter string to be encoded: ";
	cin >> str;		
	int k = 0;
	for (i = 0; str[i] != '\0'; i++)
	{
		for (j = 0; LookUP[(int)str[i]][j] != '@'; j++)
		{
			Estr[k] = (char)LookUP[(int)str[i]][j];
			k++;
		}
	}
	Estr[k] = '@';
	
    string en_msg=" ";
	cout << "\n Encoded string is: ";
	for(i=0;Estr[i]!='@';i++)		//or i<k
	{
        cout << Estr[i];		//working till here
        if(Estr[i] == 1)
            en_msg+='1';
        else if(Estr[i] == 0)
        {
            en_msg+='0';
        }
        
       // en_msg[i]= Estr[i];
    
    }
   cout<<"\n En_msg: "<<en_msg;

	cout << endl;
    
    strcpy(msg,en_msg.c_str());
    cout<<"\n msg: "<<msg;

    send(clientSd,&msg,sizeof(msg),0);
    
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

void insert(struct node Heap[], int freq, char ch, struct node *left_address, struct node *right_address)
{
	int pos, par;
	struct node temp;
	N = N + 1;
	pos = N;
	Heap[N].ch = ch;
	Heap[N].freq = freq;
	Heap[N].left = left_address;
	Heap[N].right = right_address;

	while (pos > 1)
	{
		par = pos / 2;
		if (Heap[pos].freq >= Heap[par].freq)		//min heap
			return;
		else
		{
			temp = Heap[pos];		//swap structure
			Heap[pos] = Heap[par];
			Heap[par] = temp;

			pos = par;
		}
	}
	return;

}

struct node Delete(struct node Heap[])		//sanfoundary 
{ 
	int minElement, child, i;
	struct node x = Heap[1];
	struct node lastElement = Heap[N--];
	
	for (i = 1; i * 2 <= N; i = child) {
		child = i * 2;
		/*child!=heapSize beacuse heap[heapSize+1] does not exist, which means it has only one child */
		 
		if (child != N && Heap[child + 1].freq < Heap[child].freq) {
			child++;
		}
		//swapping with minimum element
		if (lastElement.freq > Heap[child].freq) {
			Heap[i] = Heap[child];
		}
		else 
		{
			break;		//swapping not possible 
		}
	}
	Heap[i] = lastElement;
	return x;
} //this delete working properly


void preorder(struct node *ptr)
{
	if (ptr != NULL)
	{
		cout << ptr->ch << " " << ptr->freq << "\n ";
		preorder(ptr->left);
		preorder(ptr->right);
	}
}

void printCodes(struct node *root, int arr[], int top)

{

	// Assign 0 to left edge and recur 
	if (root->left) {

		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}

	// Assign 1 to right edge and recur 
	if (root->right) {

		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}

	if (root->left==NULL && root->right==NULL)	//checking if leaf node
	{

		cout << root->ch << ": ";
		int i;
		for ( i = 0; i < top; i++)
		{
			
			cout << arr[i];
			LookUP[(int)root->ch][i] = arr[i];	//store in look-up table
		}
		LookUP[(int)root->ch][i] = '@';			//giving terminating character

		cout << endl;
	}
}

void decode(struct node *root, char s[])
{
	string ans = "";
	struct node *curr = root;
	for (int i = 0; s[i]!='\0'; i++)
	{
		if (s[i] == '0')
			curr = curr->left;
		else
			curr = curr->right;

		// reached leaf node 
		if (curr->left == NULL and curr->right == NULL)
		{
				//print char or store in string
			cout << curr->ch;
			curr = root;
		}
	}
}


