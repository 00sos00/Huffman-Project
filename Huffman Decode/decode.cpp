#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class TreeNode
{
public:
	int val;
	string c;
	TreeNode* left;
	TreeNode* right;
	string code;

	TreeNode()
	{
		val = 0;
		left = right = NULL;
	}

	void PrintHuffmanTree(int level)
	{
		for (int i = 0; i < level * 5; i++)
		{
			cout << ' ';
		}
		cout << "'" << c << "'" << "= " << val << ':' << endl;
		if (!left && !right)
		{
			return;
		}
		left->PrintHuffmanTree(level + 1);
		right->PrintHuffmanTree(level + 1);
	}

	void PrintCodes(string prevcode)
	{
		code += prevcode;
		if (!left && !right)
		{
			cout << "'" << c << "': " << code << endl;
		}
		else
		{
			left->PrintCodes(code + '0');
			right->PrintCodes(code + '1');
		}
	}
};

class ListNode
{
public:
	int val;
	string c;
	ListNode* next;
	TreeNode* down;
	string code;

	ListNode()
	{
		val = 0;
		next = NULL;
		down = NULL;
	}
};

class SortedList
{
public:
	ListNode* head;

	SortedList()
	{
		head = NULL;
	}

	void Insert(ListNode* lpnn)
	{
		if (!head)
		{
			head = lpnn;
		}
		else
		{
			ListNode* trav = head;
			ListNode* back = NULL;
			int f = 0;
			while (trav)
			{
				if (lpnn->val <= trav->val)
				{
					if (!back)
					{
						lpnn->next = trav;
						head = lpnn;
					}
					else
					{
						back->next = lpnn;
						lpnn->next = trav;
					}
					f = 1;
					break;
				}
				back = trav;
				trav = trav->next;
			}
			if (!f)
			{
				back->next = lpnn;
			}
		}
	}

	void print()
	{
		ListNode* t = head;
		while (t)
		{
			cout << "'" << t->c << "'" << " = " << t->val << "   ";
			t = t->next;
		}
		cout << endl;
	}

	void Convert()
	{
		while (head->next)
		{
			ListNode* first = head;
			ListNode* second = head->next;
			TreeNode* parent = new TreeNode;
			TreeNode* tnode1;
			if (first->down)
			{
				tnode1 = first->down;
			}
			else
			{
				tnode1 = new TreeNode;
			}
			TreeNode* tnode2;
			if (second->down)
			{
				tnode2 = second->down;
			}
			else
			{
				tnode2 = new TreeNode;
			}
			tnode1->val = first->val;
			tnode1->c = first->c;
			tnode2->val = second->val;
			tnode2->c = second->c;
			parent->val = first->val + second->val;
			parent->c = first->c + second->c;
			parent->left = tnode1;
			parent->right = tnode2;

			ListNode* lpnn = new ListNode;
			lpnn->val = first->val + second->val;
			lpnn->c = first->c + second->c;
			lpnn->down = parent;
			Insert(lpnn);
			head = second->next;
			delete first;
			delete second;
		}
	}

	~SortedList()
	{
		if (head)
		{
			ListNode* trav = head;
			ListNode* pb = NULL;
			while (trav)
			{
				pb = trav;
				trav = trav->next;
				delete pb;
			}
			delete pb;
		}
	}
};

class List
{
public:
	ListNode* head;
	ListNode* tail;

	List()
	{
		head = tail = NULL;
	}

	int Size()
	{
		int cnt = 0;
		ListNode* trav = head;
		while (trav)
		{
			cnt++;
			trav = trav->next;
		}
		return cnt;
	}

	void Insert(ListNode* pnn)
	{
		if (!head) {
			head = tail = pnn;
		}
		else
		{
			tail->next = pnn;
			tail = pnn;
		}
	}

	void CreateFromTree(TreeNode* root)
	{
		if (!root) return;
		if (!root->left && !root->right)
		{
			ListNode* pnn = new ListNode;
			pnn->code = root->code;
			pnn->c = root->c;
			Insert(pnn);
		}
		CreateFromTree(root->left);
		CreateFromTree(root->right);
	}

	string GetCode(char c)
	{
		string o;
		ListNode* t = head;
		while (t)
		{
			if (t->c[0] == c)
			{
				o = t->code;
				break;
			}
			t = t->next;
		}
		return o;
	}
};

char ByteFromString(string& tmp)
{
	char b = 0;
	for (int k = 0; k < 8; k++)
	{
		if (tmp[k] == '1')
		{
			b |= (1 << (7 - k));
		}
	}
	return b;
}

int main()
{
	int i;
	char c;
	List finalCodes;
	ifstream encodedTextFile("encoded_text.bin", ifstream::binary);
	char codesCount;
	encodedTextFile.read(&codesCount, 1);
	for (i = 0; i < codesCount; i++)
	{
		ListNode* pnn = new ListNode;
		encodedTextFile.read(&c, 1);
		pnn->c = c;

		char codeSize;
		encodedTextFile.read(&codeSize, 1);
		for (int j = 0; j < codeSize; j++)
		{
			encodedTextFile.read(&c, 1);
			pnn->code += c;
		}
		finalCodes.Insert(pnn);
	}

	// Read number of compressed bytes
	char byte1;
	char byte2;
	char byte3;
	char byte4;
	encodedTextFile.read(&byte1, 1);
	encodedTextFile.read(&byte2, 1);
	encodedTextFile.read(&byte3, 1);
	encodedTextFile.read(&byte4, 1);
	int numberOfBytes = 0;
	numberOfBytes |= (int)byte1 << 24;
	numberOfBytes |= (int)byte1 << 16;
	numberOfBytes |= (int)byte1 << 8;
	numberOfBytes |= byte4;

	// Read number of compressed bits
	encodedTextFile.read(&byte1, 1);
	encodedTextFile.read(&byte2, 1);
	encodedTextFile.read(&byte3, 1);
	encodedTextFile.read(&byte4, 1);
	int numberOfBits = 0;
	numberOfBits |= (int)byte1 << 24;
	numberOfBits |= (int)byte1 << 16;
	numberOfBits |= (int)byte1 << 8;
	numberOfBits |= byte4;

	char* compressed = new char[numberOfBytes];
	for (i = 0; i < numberOfBytes; i++)
	{
		encodedTextFile.read(&compressed[i], 1);
	}
	
	ofstream outfile("decoded_text.txt", fstream::binary);
	for (int bi = 0; bi < numberOfBits; bi++)
	{
		string current_code;
		ListNode* t = finalCodes.head;
		while (t)
		{
			int f = 1;
			for (int j = 0; j < t->code.size(); j++)
			{
				int bi2 = bi + j;
				int bitState = (compressed[bi2 / 8] & (1 << (7 - bi2 % 8))) > 0;
				int ctoi = t->code[j] - '0';
				if (ctoi != bitState)
				{
					f = 0;
					break;
				}
			}
			if (f)
			{
				outfile.write(&t->c[0], 1);
				bi += t->code.size() - 1;
				break;
			}
			t = t->next;
		}
	}

	return 0;
}