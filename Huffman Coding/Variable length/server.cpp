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
    //char msg[1500];
     
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
   
   struct node Heap[Max];		//array of structures
	int n, i, freq;
	int j;
	char ch;
	//input
	//cout << "\n Enter no of elements: ";
	//cin >> n;

    recv(newSd,&n,sizeof(n),0);
    //cout<<"\n ="<<n;
	for (i = 1; i <= n; i++)
	{
		//cout << "\n Enter character: ";
		recv(newSd,&ch,sizeof(ch),0);

		//cout << "\n Enter frequency; ";
		recv(newSd,&freq,sizeof(freq),0);



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
	//char str[100];
	//int Estr[500];//Estr contains encoded string
    /*
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
	

	cout << "\n Encoded string is: ";
	for(i=0;Estr[i]!='@';i++)		//or i<k
	cout << Estr[i];		//working till here
	cout << endl;*/

    char msg[1000];
    recv(newSd,&msg,sizeof(msg),0);
    cout<<"\n Encoded string is: "<<msg;
    //for(i=0;i<sizeof(en_msg);i++)
      //  cout<<en_msg[i];

    char st[1000];
	cout << "\n Enter string to be Decoded: ";
	cin >> st;

    
	decode(&Heap[1], st);

	
   
   
   
   
   
   /* while(1)
    {
        //receive a message from the client (listen)
        cout << "Awaiting client response..." << endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            cout << "Client has quit the session" << endl;
            break;
        }
        cout << "Client: " << msg << endl;
        cout << ">";
        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            //send to the client that server has closed the connection
            send(newSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        //send the message to client
        bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
    }*/
    //we need to close the socket descriptors after we're all done
    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);
    cout << "\n********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) 
        << " secs" << endl;
    cout << "Connection closed..." << endl;
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

/*struct node Delete(struct node Heap[])
{
	int ptr, left, right;
	struct node temp;
	struct node x;



	//initialisation
	ptr = 1;
	left = 2;
	right = 3;
	x = Heap[ptr];  //deleted element frequency

	//remove last Node
	temp = Heap[ptr];
	Heap[ptr] = Heap[N];
	Heap[N] = temp;

	N = N - 1;

	while (left <= N)
	{
		if (Heap[ptr].freq <= Heap[left].freq && Heap[ptr].freq <= Heap[right].freq)
			return x;

		if ( Heap[right].freq >= Heap[left].freq)
		{
			//swap with left
			temp = Heap[ptr];
			Heap[ptr] = Heap[left];
			Heap[left] = temp;


			ptr = left;
		}

		else
		{
			//swap with right
			temp = Heap[ptr];
			Heap[ptr] = Heap[right];
			Heap[right] = temp;
			ptr = right;

		}
		left = 2 * ptr;		//left child position
		right = left + 1;
	}

	return x;		//return structure

}*/

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

