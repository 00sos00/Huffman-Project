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
		if (!head) return;
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
	List finalCodes;
	SortedList* l = new SortedList;
	int freq[256] = { 0 };
	int maxSize = 8000000;
	char* inchars = new char[maxSize];
	int i;
	ifstream inputfile("sample_img.bmp", ifstream::binary);

	if (!inputfile) {
        cout << "Error opening input file!";
        return -1;
    }

	inputfile.seekg (0, inputfile.end);
    int flen = inputfile.tellg();
    inputfile.seekg (0, inputfile.beg);
	for (i = 0; i < flen && i < maxSize; i++)
	{
		if (i == maxSize - 1) break;
		inputfile.read(&inchars[i], 1);
		freq[(unsigned char)(inchars[i] + 128)]++;
	}
	inputfile.close();

	for (i = 0; i < 256; i++)
	{
		if (freq[i] > 0)
		{
			ListNode* lnn = new ListNode;
			lnn->val = freq[i];
			lnn->c = i;
			l->Insert(lnn);
		}
	}
	l->Convert();
	l->head->down->PrintCodes("");

	finalCodes.CreateFromTree(l->head->down);

	string tmp;
	int ci = 0;
	int bitsWritten = 0;
	int maxComp = 8000000;
	char* compressed = new char[maxComp];
	for (i = 0; i < flen && ci < maxComp; i++)
	{
		string code = finalCodes.GetCode(inchars[i]);
		for (int j = 0; j < code.size(); j++)
		{
			tmp += code[j];
			bitsWritten++;
			if (ci >= maxComp - 1) break;
			if (tmp.size() == 8)
			{
				char b = ByteFromString(tmp);
				compressed[ci] = b;
				ci++;
				tmp.clear();
			}
		}
	}
	if (tmp.size() > 0 && tmp.size() <= 8)
	{
		while (tmp.size() < 8)
		{
			tmp += '0';
		}
		char b = ByteFromString(tmp);
		compressed[ci] = b;
		ci++;
	}

	ofstream outfile("compressed_img.bmp", fstream::binary);

	if (!outfile) {
        cout << "Error opening output file!";
        return -1;
    }

	char codesCount = finalCodes.Size() - 1;
	outfile.write(&codesCount, 1);
	ListNode* t = finalCodes.head;
	// For each code:
	// 1. Write code size
	// 2. Write corresponding character
	// 3. Write code itself as chars to make a string in the end
	while (t)
	{
		char codeSize = t->code.size();
		outfile.write(&t->c[0], 1);
		outfile.write(&codeSize, 1);
		for (i = 0; i < t->code.size(); i++)
		{
			outfile.write(&t->code[i], 1);
		}
		t = t->next;
	}

	// Write number of compressed bytes
	char byte1 = ci >> 24;
	char byte2 = ci >> 16;
	char byte3 = ci >> 8;
	char byte4 = ci;

	outfile.write(&byte1, 1);
	outfile.write(&byte2, 1);
	outfile.write(&byte3, 1);
	outfile.write(&byte4, 1);

	// Write number of compressed bits
	byte1 = bitsWritten >> 24;
	byte2 = bitsWritten >> 16;
	byte3 = bitsWritten >> 8;
	byte4 = bitsWritten;
	outfile.write(&byte1, 1);
	outfile.write(&byte2, 1);
	outfile.write(&byte3, 1);
	outfile.write(&byte4, 1);

	for (i = 0; i < ci; i++)
	{
		outfile.write(&compressed[i], 1);
	}

	outfile.close();
	delete[] inchars;
	delete[] compressed;
	return 0;
}