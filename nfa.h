#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include<map>
#include<set>

using namespace std;
class node
{
public:
	int id;
	bool start;
	bool accept;

	node();
	node(int id, bool start, bool accept);
	node(bool start, bool accept);
	void setid(int id);
	~node();

private:
};
node::node()
{
}

node::node(int id, bool start, bool accept)
{
	this->id = id;
	this->accept = accept;
	this->start = start;
}

node::node(bool start, bool accept)
{
	this->accept = accept;
	this->start = start;
}
void node::setid(int id)
{
	this->id = id;
}
node::~node()
{
}
class edge
{
public:
	node* source;
	node* target;
	char tr;//- ������ epsilon
	edge();
	edge(node* sr, node* tg, char tr);
	~edge();

private:
};

edge::edge()
{
}

edge::edge(node* sr, node* tg, char tr)
{
	source = sr;
	target = tg;
	this->tr = tr;
}

edge::~edge()
{
}
class NFA
{
public:
	vector<node*> state;
	vector<edge*> transition;
	node* start;	//��ǰnfa��������
	node* accept;
	NFA();
	NFA(char in);
	void add(NFA* next);//��������
	void orlink(NFA* next);//ѡ����������
	void clolink();//�ظ���������

	~NFA();

private:
};

NFA::NFA()
{
}

NFA::NFA(char in)
{
	node* bg = new node(0, 1, 0);
	node* end = new node(1, 0, 1);
	start = bg;
	accept = end;
	state.push_back(bg);
	state.push_back(end);
	transition.push_back(new edge(bg, end, in));
}
void NFA::add(NFA* next) //�������㡣
{
	NFA in = *next;
	vector<node*>::iterator i;
	accept->accept = 0;
	for (i = in.state.begin(); i != in.state.end(); i++)
	{
		if ((*i)->start == 0) {
			node* tmp = *i;
			tmp->id += accept->id;
			state.push_back(tmp);
		}
	}

	vector<edge*>::iterator ie;
	for (ie = in.transition.begin(); ie != in.transition.end(); ie++)
	{
		if ((*ie)->source == in.start)
		{
			(*ie)->source = accept;
		}
		transition.push_back((*ie));
	}
	accept = state.back();
}
void NFA::orlink(NFA* next)
{
	node* bgn = new node(0, 1, 0);
	node* end = new node(next->state.size() + state.size() + 1, 0, 1);
	//��ͷ
	start->start = 0;
	transition.insert(transition.begin(), new edge(bgn, *(next->state.begin()), '-'));
	transition.insert(transition.begin(), new edge(bgn, *state.begin(), '-'));


	NFA in = *next;
	vector<node*>::iterator i;
	accept->accept = 0;
	in.start->start = 0;
	in.accept->accept = 0;
	for (i = state.begin(); i != state.end(); i++)
	{
		(*i)->id++;
	}
	for (i = in.state.begin(); i != in.state.end(); i++)
	{
		(*i)->id += accept->id + 1;
		state.push_back(*i);
	}

	vector<edge*>::iterator ie;
	/*for (ie = in.transition.begin(); ie != in.transition.end(); ie++)
	{
		if ((*ie)->source == in.start)
		{
			(*ie)->source = accept;
		}
		transition.push_back(*ie);
	}*/
	accept->accept = 0;
	//��β
	transition.insert(transition.end(), in.transition.begin(), in.transition.end());
	transition.insert(transition.end(), new edge(in.accept, end, '-'));
	transition.insert(transition.end(), new edge(accept, end, '-'));
	accept = next->accept;

	//���´�������β���Ž�״̬��
	state.insert(state.begin(), bgn);
	state.push_back(end);
	start = bgn;
	accept = end;
}
void NFA::clolink()//�ظ�����
{
	//�½���β״̬
	node* bgn = new node(0, 1, 0);
	node* end = new node(state.size() + 1, 0, 1);
	//���״̬
	vector<node*>::iterator it;
	for (it = state.begin(); it != state.end(); it++)
	{
		(*it)->id++;
	}
	state.insert(state.begin(), bgn);
	state.push_back(end);
	//��ӱ�
	transition.push_back(new edge(accept, start, '-'));
	transition.push_back(new edge(bgn, start, '-'));
	transition.push_back(new edge(accept, end, '-'));
	transition.push_back(new edge(bgn, end, '-'));
	//����start��acc
	start->start = 0;
	accept->accept = 0;
	start = bgn;
	accept = end;
}
NFA::~NFA()
{
}
class dfanode :public node
{
public:
	vector<edge*> transition;
	set<dfanode*> closure_e; //��ŵ�ǰ����epsilon�հ����
	dfanode();
	dfanode(node* a);
	~dfanode();

private:

};

dfanode::dfanode()
{
}
inline dfanode::dfanode(node* a)
{
	id = a->id;
	start = a->start;
	accept = a->accept;
}
dfanode::~dfanode()
{
}
class DFA
{
public:
	set<dfanode*> state;
	vector<edge*> transition;
	DFA();
	DFA(vector<edge*> in);
	~DFA();

private:

};

DFA::DFA()
{
}

inline DFA::DFA(vector<edge*> in)
{

}

DFA::~DFA()
{
}