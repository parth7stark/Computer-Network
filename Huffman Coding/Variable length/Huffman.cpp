#include<iostream>
#include<string>

#define Max 100
int N = 0;
using namespace std;

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
int main()
{
	struct node Heap[Max];		//array of structures
	int n, i, freq;
	int j;
	char ch;
	//input
	cout << "\n Enter no of elements: ";
	cin >> n;

	for (i = 1; i <= n; i++)
	{
		cout << "\n Enter character: ";
		cin >> ch;

		cout << "\n Enter frequency; ";
		cin >> freq;


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


	//delete and print
	/*int j = N;
	int Min1;

	for(i=1; i<=j;i++)
	{
		Min1 = Delete(Heap);
		cout<<"\n deleted freq : "<<Min1;
		cout<<endl;
		for(int k=1;k<=N;k++)
		{
			cout<<Heap[k].left<<" "<<Heap[k].ch<<" "<<Heap[k].freq<<" "<<Heap[k].right<<" ";
			cout<<endl;
		}

	}	*/	//working till here



	//struct node Min2;
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
	

	cout << "\n Encoded string is: ";
	for(i=0;Estr[i]!='@';i++)		//or i<k
	cout << Estr[i];		//working till here
	cout << endl;

	//Decoding 
	char st[1000];
	cout << "\n Enter string to be Decoded: ";
	cin >> st;

	decode(&Heap[1], st);
	



	system("pause");
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

