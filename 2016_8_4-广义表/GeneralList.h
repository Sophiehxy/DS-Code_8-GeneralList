#pragma once
#include<cassert>

enum TYPE //��������
{
	HEAD,  //ͷ���
	VALUE, //ֵ���
	SUB	   //�ӱ���
};

struct GeneralListNode
{
	GeneralListNode* _next;
	TYPE _type;

	union  //_value��_sublink����typeֻ�ܴ�����һ 
	{
		char _value;
		GeneralListNode* _sublink;
	};

	GeneralListNode(TYPE type=VALUE,char value=0)
		:_next(NULL)
		, _type(type)
	{
		if (type == VALUE)//ֵ���
		{
			_value = value;
		}
		else if (_type == SUB)//�ӱ���
		{
			_sublink = NULL;
		}
	}
};

class GeneralList
{
public:
	GeneralList()
		:_head(NULL)
	{}
	GeneralList(char* s)
		:_head(NULL)
	{
		_head = _CreateGeneralList(s);
	}
	GeneralList(const GeneralList& g)
	{
		_head = _Copy(g._head);
	}
	GeneralList& operator=(GeneralList g)
	{
		swap(_head, g._head);
		return *this;
	}
	~GeneralList()
	{
		_Destroy(_head);
	}
.
public:
	size_t Size()//����������ܽ����
	{
		size_t size = _Size(_head);
		cout << "Size:" << size << endl;
		return size;
	}

	//�����������
	size_t Depth()
	{
		size_t depth = _Depth(_head);
		cout << "Depth:" << depth << endl;
		return depth;
	}

	void Print()
	{
		_Print(_head);
		cout << endl;
	}

protected:
	//�ж�*s�Ƿ�Ϊ��ֵ
	bool IsValue(char c)
	{
		if ((c >= '0'&&c <= '9') || (c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z'))
		{
			return true;
		}
		return false;
	}


	//�����ӱ���
	GeneralListNode* _CreateGeneralList(char*& s)
	{
		//���ʽ�ĵ�һ���ַ�һ����"(",������ǣ���������
		assert(*s == '(');
		++s;//��ȡ�����ź���ַ�

		//����ͷ���
		GeneralListNode* head = new GeneralListNode(HEAD);

		GeneralListNode* cur = head;
		while (*s)
		{
			//1.������(��,�����ӱ�
			if (*s == '(')//�ݹ�ʵ��
			{
				//���ٴ�������(��ʱ����ʾ��Ҫ�����ӱ�
				GeneralListNode* subNode = new GeneralListNode(SUB);
				cur->_next = subNode;
				cur = cur->_next;
				subNode->_sublink = _CreateGeneralList(s);
			}
			//2.������)������ʾ�ñ����
			else if (*s == ')')
			{
				++s;
				break;
			}
			//3.������ֵ������ֵ���
			else if (IsValue(*s))
			{
				GeneralListNode* valueNode = new GeneralListNode(VALUE,*s);
				cur->_next = valueNode;
				cur = cur->_next;

				++s;
			}
			else
			{
				++s;
			}
		}
		return head;
	}

	GeneralListNode* _Copy(GeneralListNode* head)
	{
		assert(head);
		GeneralListNode* cur = head;
		GeneralListNode* newHead = new GeneralListNode(HEAD);
		GeneralListNode* newCur = newHead;
		while(cur)
		{
			if (cur->_type == VALUE)
			{
				newCur->_next = new GeneralListNode(VALUE, cur->_value);
				newCur = newCur->_next;
			}
			else if (cur->_type == SUB)
			{
				newCur->_next = new GeneralListNode(SUB);
				newCur = newCur->_next;
				newCur->_sublink = _Copy(cur->_sublink);
			}
			cur = cur->_next;
		}
		return newHead;
	}

	void _Destroy(GeneralListNode* head)
	{
		GeneralListNode* cur = head;
		while (cur)
		{
			GeneralListNode* del = cur;
			cur = cur->_next;
			if (del->_type == SUB)
			{
				_Destroy(del->_sublink);
			}
			delete del;
		}
	}

	size_t _Size(GeneralListNode* head)
	{
		GeneralListNode* cur = head;
		size_t size = 0;
		while (cur)
		{
			if (cur->_type == VALUE)
			{
				size++;
			}
			else if (cur->_type == SUB)
			{
				size += _Size(cur->_sublink);
			}
			cur = cur->_next;
		}
		return size;
	}

	size_t _Depth(GeneralListNode* head)
	{
		GeneralListNode* cur = head;
		size_t depth = 1;
		while (cur)
		{
			if (cur->_type == SUB)
			{
				size_t subDepth = _Depth(cur->_sublink);
				if (subDepth + 1 > depth)//�����ӱ���ȴ���ǰһ�μ������ȣ����滻Ϊ������ֵ
				{
					depth = subDepth + 1;
				}
			}
			cur = cur->_next;
		}
		return depth;
	}
	void _Print(GeneralListNode* head)
	{
		GeneralListNode* cur = head;
		while (cur)
		{
			if (cur->_type == HEAD)
			{
				cout << "(";
			}
			else if (cur->_type == VALUE)
			{
				cout << cur->_value;
				if (cur->_next)
				{
					cout << ",";
				}
			}
			else //�ݹ�
			{
				_Print(cur->_sublink);
				if (cur->_next)
				{
					cout << ",";
				}
			}
			cur = cur->_next;
		}
		cout << ")";
	}


private:
	GeneralListNode* _head;
	
};

void Test()
{
	char* s1 = "()";
	char* s2 = "(a,b)";
	char* s3 = "(a,b,(c,d))";
	char* s4 = "(a,b,(c,d),(e,(f),h))";
	GeneralList g1(s1);
	GeneralList g2(s2);
	GeneralList g3(s3);
	GeneralList g4(s4);

	g1.Print();
	g1.Size();
	g1.Depth();
	g2.Print();
	g2.Size();
	g2.Depth(); 
	g3.Print();
	g3.Size();
	g3.Depth(); 
	g4.Print();
	g4.Size();
	g4.Depth();

	GeneralList g5(g4);
	g5.Print();

	g5 = g3;
	g5.Print();

}