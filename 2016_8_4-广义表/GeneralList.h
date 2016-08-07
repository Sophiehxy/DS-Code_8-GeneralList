#pragma once
#include<cassert>

enum TYPE //结点的类型
{
	HEAD,  //头结点
	VALUE, //值结点
	SUB	   //子表结点
};

struct GeneralListNode
{
	GeneralListNode* _next;
	TYPE _type;

	union  //_value和_sublink根据type只能存在其一 
	{
		char _value;
		GeneralListNode* _sublink;
	};

	GeneralListNode(TYPE type=VALUE,char value=0)
		:_next(NULL)
		, _type(type)
	{
		if (type == VALUE)//值结点
		{
			_value = value;
		}
		else if (_type == SUB)//子表结点
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
	size_t Size()//计算广义表的总结点数
	{
		size_t size = _Size(_head);
		cout << "Size:" << size << endl;
		return size;
	}

	//计算广义表深度
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
	//判断*s是否为数值
	bool IsValue(char c)
	{
		if ((c >= '0'&&c <= '9') || (c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z'))
		{
			return true;
		}
		return false;
	}


	//创建子表函数
	GeneralListNode* _CreateGeneralList(char*& s)
	{
		//表达式的第一个字符一定是"(",如果不是，则有问题
		assert(*s == '(');
		++s;//读取左括号后的字符

		//创建头结点
		GeneralListNode* head = new GeneralListNode(HEAD);

		GeneralListNode* cur = head;
		while (*s)
		{
			//1.遇到“(”,创建子表
			if (*s == '(')//递归实现
			{
				//当再次遇到“(”时，表示需要创建子表
				GeneralListNode* subNode = new GeneralListNode(SUB);
				cur->_next = subNode;
				cur = cur->_next;
				subNode->_sublink = _CreateGeneralList(s);
			}
			//2.遇到“)”，表示该表结束
			else if (*s == ')')
			{
				++s;
				break;
			}
			//3.遇到数值，创建值结点
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
				if (subDepth + 1 > depth)//若该子表深度大于前一次计算的深度，则替换为大的深度值
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
			else //递归
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