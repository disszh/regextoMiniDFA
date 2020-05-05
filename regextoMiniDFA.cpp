#include"nfa.h"
using namespace std;


bool isLetter(char i) {
	if (i >= 'a' && i <= 'z')
		return true;
	else
	{
		return false;
	}
}
bool isOperator(char i) {
	if (i == '*' || i == '.' || i == '|' || i == '(' || i == ')')
		return true;
	else
	{
		return false;
	}
}
bool isToken(char i) {
	if (isOperator(i) || isLetter(i))
		return true;
	else
	{
		return false;
	}
}
//检查输入是否只包括a-z,*,|,.,是否符合表达式语法
int inputcheck(string in) {
	string input = in;
	string::iterator cur;
	int flg_fst = 0;
	char tmp;
	for (cur = input.begin(); cur != input.end(); cur++)
	{
		if (!isToken(*cur)) {
			//表达式不符合字母表
			cout << "inputcheck(): " << *cur << "不是操作符" << endl;
			return -1;
		}
		if (flg_fst == 0) {
			if (*input.begin() == '(' || isLetter(*input.begin()))	//首字符是否为字母或（，否则不符合语法
			{
				flg_fst = 1;
				tmp = *cur;//tmp为首字符
				continue;
			}
			else
			{
				return -1;
			}
		}
		//todo 语法识别  没写完
		if (isOperator(tmp) && tmp != ')')
		{
			if (isLetter(*cur))
			{
			}
		}
	}
	return 0;
}
int typeofchar(char in) {//1a 2. 3| 4* 5( 6)
	if (isLetter(in))
	{
		return 1;
	}
	else if (in == '.')
		return 2;
	else if (in == '|')
		return 3;
	else if (in == '*')
		return 4;
	else if (in == '(')
		return 5;
	else if (in == ')')
		return 6;
}
queue<char> addOptr(string in) {
	queue<char> out;
	string::iterator cur;
	int flg = 0;//1a 2. 3| 4* 5( 6)
	for (cur = in.begin(); cur != in.end(); cur++)
	{
		//cout << "cur is " << *cur<<"flg is "<<flg << endl;
		switch (flg)
		{
		case 1:
			if (typeofchar(*cur) == 1 || typeofchar(*cur) == 5)
			{
				out.push('.');
			}break;
		case 4:if (typeofchar(*cur) == 1 || typeofchar(*cur) == 5)
		{
			out.push('.');
		}break;

		case 6:if (typeofchar(*cur) == 1 || typeofchar(*cur) == 5)
		{
			out.push('.');
		}break;
		}
		flg = typeofchar(*cur);
		out.push(*cur);
	}

	return out;
}
//比较.,|,*的优先级，a>b就返回true
bool bigpri(char a, char b) {
	map<char, int> pri;
	pri['|'] = 1;
	pri['.'] = 2;
	pri['*'] = 3;
	if (pri[a] >= pri[b])
	{
		return true;
	}
	else
	{
		return false;
	}
}
string toPRN(queue<char> in) {
	stack<char> exp, optr;
	string out;
	bool flgop = 1;	//操作符栈被清空且输入in为空，flgop=0
	char temwp;
	temwp = in.front();
	while (!in.empty())
	{
		temwp = in.front();
		//cout << "cur is " << in.front() << endl;
		if (isLetter(in.front()))
		{
			exp.push(in.front());
		}
		//压操作符
		else
		{
			if (!optr.empty()) {
				if (in.front() == ')')
				{
					char tmp = optr.top();
					while (tmp != '(')
					{
						exp.push(tmp);
						optr.pop();
						tmp = optr.top();
					}
					optr.pop();
				}
				else if (in.front() == '(')
				{
					optr.push(in.front());
				}
				else
				{
					while (bigpri(optr.top(), in.front()))//栈顶操作符优先级>=，就弹出栈顶
					{
						exp.push(optr.top());
						optr.pop();
						if (optr.empty())
						{
							break;
						}
					}

					optr.push(in.front());
				}
			}
			else
			{
				optr.push(in.front());
			}
		}

		//cout << in.front();
		in.pop();
		if (in.empty())
		{
			while (!optr.empty())
			{
				exp.push(optr.top());
				optr.pop();
			}
		}
	}
	while (!exp.empty())
	{
		out.push_back(exp.top());
		exp.pop();
	}
	reverse(out.begin(), out.end());
	return out;
}
NFA strtoNFA(string a) {
	string in = a;
	string::iterator st;
	stack<NFA*> st_nfa;
	for (st = in.begin(); st != in.end(); st++)
	{
		switch (*st) {
		case '*': {
			NFA* tmp = st_nfa.top();
			st_nfa.pop();
			tmp->clolink();
			st_nfa.push(tmp);
			break; }
		case '.': {
			NFA* next = st_nfa.top();
			st_nfa.pop();
			NFA* former = st_nfa.top();
			st_nfa.pop();
			former->add(next);
			st_nfa.push(former);
			break; }
		case '|': {
			NFA* next = st_nfa.top();
			st_nfa.pop();
			NFA* former = st_nfa.top();
			st_nfa.pop();
			former->orlink(next);
			st_nfa.push(former);
			break; }
		default:
			st_nfa.push(new NFA(*st));
		}
	}
	NFA tmp1 = *st_nfa.top();
	vector<node*>::iterator iter;
	st_nfa.pop();
	int b = 0;
	/*for (iter=tmp1.state.begin();iter!=tmp1.state.end();iter++  )
	{
		(*iter)->setid(b);
		b++;
	}*/
	return tmp1;
}
void outgraphviz(NFA a) {
	string out = "digraph dfa{\n\"\" [shape=none]\n";
	NFA  in = a;
	vector<node*>::iterator iter;
	vector<edge*>::iterator ite;
	for (iter=in.state.begin(); iter!=in.state.end(); iter++)
	{
		string tmp="\"id:";
		tmp.append(to_string((*iter)->id));
		tmp += "\" [shape=circle]\n";
		out.append(tmp);
	}
	out.append("\n\"\"-> \"id:0\"");
	for (ite = in.transition.begin(); ite != in.transition.end(); ite++)
	{
		string tmp= "\"id:";
		tmp = tmp + (to_string((*ite)->source->id));
		tmp+="\" -> \"id:";
		tmp=tmp+to_string((*ite)->target->id) + "\" [label=\"" + (*ite)->tr + "\"]\n";
		out.append(tmp);
	}
	out.append("}");
	cout << out;
}
map<node*, vector<edge*>> todfanode(vector<edge*> ine) {
	map<node*, vector<edge*>> out;
	vector<edge*>::iterator ite;
	for ( ite = ine.begin(); ite !=ine.end(); ite++)
	{
			out[(*ite)->source].push_back(*ite);
	}
	return out;
}
int main() {
	string test = "ab|c(d*|a)";
	string out1;
	queue<char> out;
	if (inputcheck(test) > -1)
	{
		out = addOptr(test);

		cout << endl;
		out1 = toPRN(out);
		cout << out1;
		NFA out2 = strtoNFA(out1);
		//outgraphviz(out2);
		auto mapdfa = todfanode(out2.transition);
	}
	return 0;
}