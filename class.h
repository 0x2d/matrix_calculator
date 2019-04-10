#pragma once
#include<iostream>
#include<iomanip>
#include<map>
#include<ctime>
#include<string>
#include<sstream>
#include<set>
#include<cmath>
using namespace std;

class matrix
{
private:
	double m[10][10] = { 0 };//局部变量栈限制1MB
	int row = 0, col = 0;
	bool flag = true;
	bool isZero(int i);
	void swap(int i, int j);
	void mult(int i, double k);
	void plus(int i, int j, double k);

public:
	matrix() {};
	matrix(double mm) { m[0][0] = mm, row = 1, col = 1; };
	matrix(stringstream &sub);
	matrix(double mm[][10], int r, int c);
	~matrix() {};
	void show();
	matrix rref();
	matrix trans();
	double det(matrix temp);//当且仅当this为方阵
	matrix inverse();//当且仅当this为方阵
	int rank();
	int r() { return row; };
	int c() { return col; };
	double value() { return m[0][0]; };
	bool status() { return flag; };
	matrix operator* (matrix a);
	matrix operator+ (matrix a);
	matrix operator- (matrix a);
	matrix operator/ (matrix a);//当且仅当a为一维方阵
	matrix operator+= (matrix a);//右侧合并，优先级高于乘法，当且仅当a与this行相等
};

bool matrix::isZero(int i)
{
	bool flag = true;
	for (int j = 0; j < col; j++)
		if (m[i][j] != 0)
			flag = false;
	return flag;
}

void matrix::swap(int i, int j)
{
	double temp;
	for (int k = 0; k < col; k++)
	{
		temp = m[i][k];
		m[i][k] = m[j][k];
		m[j][k] = temp;
	}
}

void matrix::mult(int i, double k)
{
	for (int j = 0; j < col; j++)
		m[i][j] *= k;
}

void matrix::plus(int i, int j, double k)
{
	for (int h = 0; h < col; h++)
		m[i][h] += m[j][h] * k;
}

matrix::matrix(stringstream &sub)
{
	int r = 0, c = 0;
	char temp;
	for (r = 0; sub.peek() != EOF; r++)
	{
		temp = '\0';
		for (c = 0; sub.peek() != EOF&&temp != ';'; c++)
		{
			if (isdigit(sub.peek()) || sub.peek() == '-')
				sub >> m[r][c];
			else
				flag = false;
			if (sub.peek() != EOF)
			{
				sub >> temp;
				if (temp != ','&&temp != ';')
					flag = false;
			}
		}
	}
	row = r, col = c;
}

matrix::matrix(double mm[][10], int r, int c)
{
	row = r, col = c;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			m[i][j] = mm[i][j];
	}
}

void matrix::show()
{
	if (row == 1 && col == 1)
		cout << m[0][0] << endl;
	else
	{
		for (int r = 0; r < row; r++)
		{
			for (int c = 0; c < col; c++)
			{
				if (m[r][c] == -0)
					m[r][c] = 0;
				cout << setw(7) << m[r][c] << ' ';
			}
			cout << endl;
		}
	}
}

matrix matrix::rref()
{
	matrix res = { m,row,col };
	for (int i = 0; i < col&&i < row; i++)
	{
		for (int j = i; j < row; j++)
		{
			if (res.m[j][i] != 0)
			{
				res.mult(j, 1 / res.m[j][i]);
				for (int h = 0; h < row; h++)
				{
					if (h != j)
						res.plus(h, j, -res.m[h][i]);
				}
				break;
			}
		}
	}
	for (int i = 0; i < col&&i < row; i++)
	{
		for (int j = i; j < row; j++)
		{
			if (res.m[j][i] != 0)
			{
				res.swap(j, i);
				break;
			}
		}
	}
	return res;
}

matrix matrix::trans()
{
	matrix res;
	double mm[10][10] = { 0 };
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
			mm[i][j] = m[j][i];
	}
	res = { mm,col,row };
	return res;
}

double matrix::det(matrix temp)
{
	if (temp.col == 1)
		return temp.m[0][0];
	double res = 0;
	for (int i = 0; i < temp.row; i++)
	{
		matrix n = temp;
		for (int j = 0; j < n.row; j++)
		{
			for (int h = 0; h < n.col; h++)
				n.m[j][h] = n.m[j][h + 1];
		}
		for (int j = i; j < n.row; j++)
		{
			for (int h = 0; h < n.col; h++)
				n.m[j][h] = n.m[j + 1][h];
		}
		n.col--, n.row--;
		res += pow(-1, i)*temp.m[i][0] * det(n);
	}
	return res;
}

matrix matrix::inverse()
{
	matrix res, i, r;
	double mm[10][10] = { 0 };
	double mi[10][10] = { 0 };
	for (int i = 0; i < row; i++)
		mi[i][i] = 1;
	i = { mi,row,row };
	r = (*this += i).rref();
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < row; j++)
			mm[i][j] = r.m[i][row + j];
	}
	res = { mm,row,row };
	return res;
}

int matrix::rank()
{
	int r = 0;
	matrix res = rref();
	for (int i = 0; i < row; i++)
	{
		if (!res.isZero(i))
			r++;
	}
	return r;
}

matrix matrix::operator* (matrix a)
{
	matrix res;
	double mm[10][10] = { 0 };
	if (a.col == 1 && a.row == 1)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
				mm[i][j] = m[i][j] * a.value();
		}
		res = { mm, row, col };
	}
	else if (col == 1 && row == 1)
	{
		for (int i = 0; i < a.row; i++)
		{
			for (int j = 0; j < a.col; j++)
				mm[i][j] = a.m[i][j] * value();
		}
		res = { mm, a.row, a.col };
	}
	else
	{		
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < a.col; j++)
			{
				for (int h = 0; h < col; h++)
					mm[i][j] += m[i][h] * a.m[h][j];
			}
		}
		res = { mm, row, a.col };
	}
	return res;
}

matrix matrix::operator+ (matrix a)
{
	matrix res;
	double mm[10][10] = { 0 };
	if (a.col == 1 && a.row == 1)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
				mm[i][j] = m[i][j] + a.value();
		}
		res = { mm, row, col };
	}
	else if (col == 1 && row == 1)
	{
		for (int i = 0; i < a.row; i++)
		{
			for (int j = 0; j < a.col; j++)
				mm[i][j] = a.m[i][j] + value();
		}
		res = { mm, a.row, a.col };
	}
	else
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
				mm[i][j] = a.m[i][j] + m[i][j];
		}
		res = { mm, row, col};
	}
	return res;
}

matrix matrix::operator- (matrix a)
{
	matrix res;
	double mm[10][10] = { 0 };
	if (a.col == 1 && a.row == 1)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
				mm[i][j] = m[i][j] - a.value();
		}
		res = { mm, row, col };
	}
	else if (col == 1 && row == 1)
	{
		for (int i = 0; i < a.row; i++)
		{
			for (int j = 0; j < a.col; j++)
				mm[i][j] = value() - a.m[i][j];
		}
		res = { mm, a.row, a.col };
	}
	else
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
				mm[i][j] = m[i][j] - a.m[i][j];
		}
		res = { mm, row, col };
	}
	return res;
}

matrix matrix::operator/ (matrix a)
{
	double mm[10][10] = { 0 };
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			mm[i][j] = m[i][j] / a.value();
	}
	matrix res = { mm, row, col };
	return res;
}

matrix matrix::operator+= (matrix a)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < a.col; j++)
			m[i][col + j] = a.m[i][j];
	}
	col += a.col;
	return *this;
}