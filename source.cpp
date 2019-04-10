#include"class.h"

void check();//检查表达式是否合法
void formatting();//格式化输入
inline void clv();//变量清空
matrix expression_value();//求一个表达式的值
matrix term_value();//求一个项的值
matrix matrix_value();//求一个合并矩阵的值
matrix factor_value();//求一个因子的值

map<string, matrix> var;//变量
string seq;//表达式
set<char> invalid = { '`','~','\\','|','_',':','\"' ,'?','<','>','{','}','&','$','#','@' };//非法字符集(半角）
set<char> validsym = { ',','.','/','\'',';','*','^','%','!','+','=' };//部分合法字符集（半角）
int p = 0;//运行指针
bool valid = true;//表达式合法性

void check()
{	
	bool ini = false;
	for (int i = 0; seq[i] != '\0'; i++)
	{
		if (seq[i] < 0 || seq[i] > 127 || invalid.find(seq[i]) != invalid.end())
		{
			valid = false;
			cout << "invalid character" << endl;
			break;
		}
		if ((isdigit(seq[i]) || isalpha(seq[i])) && seq[i + 1] == ' ' && !ini)
			ini = true;
		else if ((isdigit(seq[i]) || isalpha(seq[i])) && ini)
			valid = false, ini = false, cout<<"invalid space"<<endl;
		else if(seq[i]!=' ')
			ini = false;
	}
	{
		int l_num = 0, r_num = 0, count = 0, left[10] = { 0 }, right[10] = { 0 };
		for (int i = 0; seq[i] != '\0'; i++)
		{
			if (seq[i] == '(')
				left[l_num++] = i + 1;
			else if (seq[i] == ')')
				right[r_num++] = i + 1;
		}
		for (int r = 0; r < r_num; r++)
			for (int l = l_num - 1; l >= 0; l--)
				if (left[l] < right[r])
				{
					left[l] = INT_MAX, count++;
					break;
				}
		if (count != l_num || count != r_num)
			valid = false, cout << "( , ) not match" << endl;
	}
	{
		int l_num = 0, r_num = 0, count = 0, left[10] = { 0 }, right[10] = { 0 };
		for (int i = 0; seq[i] != '\0'; i++)
		{
			if (seq[i] == '[')
				left[l_num++] = i + 1;
			else if (seq[i] == ']')
				right[r_num++] = i + 1;
		}
		for (int r = 0; r < r_num; r++)
			for (int l = l_num - 1; l >= 0; l--)
				if (left[l] < right[r])
				{
					left[l] = INT_MAX, count++;
					break;
				}
		if (count != l_num || count != r_num)
			valid = false, cout << "[ , ] not match" << endl;
	}
}

void formatting()
{
	for (int i = 0; seq[i] != '\0'; i++)
	{
		if (seq[i] == ' ')
		{
			for (int j = i; seq[j] != '\0'; j++)
				seq[j] = seq[j + 1];
			i--;
		}
	}
}

inline void clv()
{
	p = 0, seq.clear(), valid = true;
}

matrix expression_value()
{
	int indicate = p;//当且仅当第一层递归时，检测"surplus input"
	matrix result = term_value();
	while (seq[p] == '+' || seq[p] == '-')
	{
		char op = seq[p];
		p++;
		if (seq[p] == '\0' || validsym.find(seq[p]) != validsym.end())
			valid = false, cout << "lack of input" << endl;
		if (op == '+')
			result = result + term_value();
		else
			result = result - term_value();
	}
	if (seq[p] != '\0' && indicate == 0)
		valid = false, cout << "surplus input" << endl;
	return result;
}

matrix term_value()
{
	matrix result = matrix_value();
	while (seq[p] == '*' || seq[p] == '/')
	{
		char op = seq[p];
		p++;
		if (seq[p] == '\0' || validsym.find(seq[p]) != validsym.end())
			valid = false, cout << "lack of input" << endl;
		matrix right = matrix_value();
		if (op == '*')
		{
			if (result.c() != 1 && result.c() != 1 && right.c() != 1 && right.r() != 1 && (result.c() != right.r() || result.r() != right.c()))
				valid = false, cout << "invalid multiplier" << endl;
			result = result * right;
		}
		else
		{
			if (right.value() == 0 || right.c() != 1 || right.r() != 1)
				valid = false, cout << "invalid divisor" << endl;
			result = result / right;
		}
	}
	return result;
}

matrix matrix_value()
{
	matrix result = factor_value();
	while (seq[p] == '+'&&seq[p + 1] == '=')
	{
		p += 2;
		if (seq[p] == '\0')
			valid = false, cout << "lack of input" << endl;
		matrix right = factor_value();
		if (right.r() == result.r())
			result += right;
		else
			valid = false, cout << "invalid matrix" << endl;
	}
	return result;
}

matrix factor_value()//表达式中的负数应以（-x）的形式消歧义
{
	matrix result = { 0 };
	stringstream s;
	bool rref = false, rank = false, det = false, inverse = false, ln = false, lg = false, si = false, co = false, ta = false;
	if (seq.substr(p, 4) == "rref")
		rref = true, p += 4;
	else if (seq.substr(p, 4) == "rank")
		rank = true, p += 4;
	else if (seq.substr(p, 3) == "det")
		det = true, p += 3;
	else if (seq.substr(p, 7) == "inverse")
		inverse = true, p += 7;
	else if (seq.substr(p, 2) == "ln")
		ln = true, p += 2;
	else if (seq.substr(p, 2) == "lg")
		lg = true, p += 2;
	else if (seq.substr(p, 3) == "sin")
		si = true, p += 3;
	else if (seq.substr(p, 3) == "cos")
		co = true, p += 3;
	else if (seq.substr(p, 3) == "tan")
		ta = true, p += 3;
	if (seq[p] == '(')
	{
		p++;
		result = expression_value();
		p++;
		if (rref)
			result = result.rref();
		else if (rank)
			result = result.rank();
		else if (det)
		{
			if (result.c() == result.r())
				result = result.det(result);
			else
				valid = false, cout << "not square matrix" << endl;
		}
		else if (inverse)
		{
			if (result.r() == result.c() && result.c() == result.rank())
				result = result.inverse();
			else
				valid = false, cout << "not full-rank square matrix" << endl;
		}
		else if (ln)
		{
			if (result.r() == 1 && result.c() == 1)
				result = log(result.value());
			else
				valid = false, cout << "invalid operate" << endl;
		}
		else if (lg)
		{
			if (result.r() == 1 && result.c() == 1)
				result = log10(result.value());
			else
				valid = false, cout << "invalid operate" << endl;
		}
		else if (si)
		{
			if (result.r() == 1 && result.c() == 1)
				result = sin(result.value());
			else
				valid = false, cout << "invalid operate" << endl;
		}
		else if (co)
		{
			if (result.r() == 1 && result.c() == 1)
				result = cos(result.value());
			else
				valid = false, cout << "invalid operate" << endl;
		}
		else if (ta)
		{
			if (result.r() == 1 && result.c() == 1)
				result = tan(result.value());
			else
				valid = false, cout << "invalid operate" << endl;
		}
	}
	else if (seq[p] == '[')
	{
		string sub = seq.substr(p + 1, seq.find(']', p) - 1 - p);
		s.str(sub);
		result = (s);
		if (!result.status())
			valid = false, cout << "invalid input" << endl;
		p = seq.find(']', p) + 1;
	}
	else
	{
		string name;
		if (isdigit(seq[p]))
		{
			int rp;
			for (rp = p; isdigit(seq[rp]) || seq[rp] == '.'; rp++)
				name += seq[rp];
			string sub = seq.substr(p, rp - p);
			if (sub.find('.') != sub.rfind('.'))
				valid = false, cout << "invalid input" << endl;
			s.str(sub);
			result = (s);
			p = rp;
		}
		if (isalpha(seq[p]))
		{
			while (isalpha(seq[p]) || isdigit(seq[p]))
				name += seq[p++];
			if (seq[p] == '=')
			{
				p++;
				var[name] = expression_value();
			}
			if (var.count(name) == 0 || !var[name].status())
				valid = false, cout << "invalid variable" << endl;
			else
				result = var[name];
		}
	}
	if (seq[p] == '\'')
		result = result.trans(), p++;
	if (seq[p] == '%')
		result = result / 100, p++;
	if (seq[p] == '!')
	{
		p++;
		if (result.c() == 1 && result.r() == 1 && result.value() == int(result.value()))
		{
			for (int i = result.value() - 1; i > 0; i--)
				result = result * i;
		}
		else
			valid = false, cout << "invalid operate" << endl;
	}
	if (seq[p] == '^')
	{
		p++;
		matrix ex = factor_value();
		if (ex.r() == 1 && ex.c() == 1)
		{
			if (result.r() == 1 && result.c() == 1)
				result = pow(result.value(), ex.value());
			else if (ex.value() == int(ex.value()))
			{
				matrix right = result;
				for (int i = 0; i < int(ex.value()) - 1; i++)
					result = result * right;
			}
			else
				valid = false, cout << "invalid operate" << endl;
		}
		else
			valid = false, cout << "invalid operate" << endl;
	}
	return result;
}

int main()
{
	const time_t a = time(0);
	cout << "Matrix Calculator on win32.  " << ctime(&a) << "Type \"help\" or \"copyright\" for more information" << endl;
	while (true)
	{
		cout << ">> ";
		clv();
		getline(cin, seq);
		seq.append("\0\0\0\0\0\0\0\0\0\0");
		check();
		formatting();
		if (!valid)
			cout << "syntax error" << endl;
		else if (seq.substr(0, 4) == "help")
			cout << "\"exit\" to exit the program\n" << "\"cls\" to clear the screen" << endl;
		else if (seq.substr(0, 9) == "copyright")
			cout << "Copyright (c) 2019-2023 TED (OUYANG YUCHENG 1800017823). All Rights Reserved." << endl;
		else if (seq.substr(0, 4) == "exit")
			break;
		else if (seq.substr(0, 3) == "cls")
		{
			system("cls");
			cout << "Matrix Calculator on win32.  " << ctime(&a) << "Type \"help\" or \"copyright\" for more information" << endl;
		}
		else
		{
			matrix output = expression_value();
			if (!valid)
				cout << "calculate error" << endl;
			else
				output.show();
		}
	}
	return 0;
}