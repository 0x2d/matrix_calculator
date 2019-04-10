# matrix_calculator
An course assignment of Introduction of Computing

一、	程序简介
程序命名为Matrix Calculator，即支持矩阵间简单的运算与操作。合法输入包括：字母、数字、合法半角字符{ ',','.','/','\'',';','*','^','%','!','+','=' ,’-‘}。表达式中的负数输入应以(-x)的形式消歧义。矩阵应以[x,y;m,n]的形式输入，其中，’,’为同一行而不同列的数据的分隔符，’;’为不同行的数据的分隔符，’[]’为零维方阵，单独的浮点数与整数被视为一维方阵，合法值域与double相同，最高支持十维方阵（受限于递归调用的栈内存限制）。
二、	功能介绍
1.	计算表达式
支持计算包含+,-,*,/,(,)与矩阵（或合法变量）组成的合法表达式，返回并显示计算结果。
2.	可对变量多次赋值
合法变量名为仅包含数字与字母的字符串，合法变量名赋值后即为合法变量。程序可对’=’左侧的合法变量名赋值，可赋值变量数取决于Visual Studio 2015中C++ map的实现。
3.	报错
错误类型包括："invalid character"包含非法字符 "invalid space"包含非法空格 "( , ) not match"小括号不匹配 "[ , ] not match"中括号不匹配 "lack of input"缺少输入 "surplus input"过多输入 "invalid multiplier"非法乘数 "invalid divisor"非法除数 "invalid matrix"非法矩阵 "not square matrix"非方阵 "not full-rank square matrix"非满秩方阵 "invalid input"非法赋值 "invalid variable"非法变量 "invalid operate" 非法操作"syntax error"语法错误 "calculate error"计算错误。
4.	界面及命令
显示启动程序的时间，可获取帮助及版权信息。
命令包括：”cls”清空屏幕计算结果 “exit”退出程序。
5.	整数与浮点数的操作
支持计算百分数’%’、阶乘’!’、指数’^’、三角函数’sin’ ‘cos’ ‘tan’、对数’ln’ ‘lg’
6.	矩阵的操作
支持计算矩阵的简化阶梯型’rref’、秩’rank’、行列式’det’、转置’’’、逆’invers’、右侧合并’+=’以及所有整数与浮点数的操作。
（注：前置操作应以rref(x), sin(x)的形式进行，后置操作应以x^y, x’的形式进行计算。
三、	已知问题
递归调用的栈内存限制，导致计算高维矩阵式出现内存溢出。内置double类型在运算过程中出现精度损失，导致计算误差。
四、	设计思路及特色
本程序的代码使用了面向对象的风格，设计了通用矩阵的类”class matrix”，在支持较多使用功能的同时使得代码文档简洁、易维护。
