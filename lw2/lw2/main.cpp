/*
11. В  символьной  строке  записано  выражение  из  цифр  и
операций в постфиксной форме (знак операции  после операндов).
Проверить правильность записи и найти значение  выражения. При
наличии ошибок указать место первой из них.
   Пример: выражение (2+3)*4-5*6  записывается  в  постфиксной
форме как 23+4*56*- (7).

Житинкина Анна, ПС-21
*/

#include "main.hpp"

int main()
{
	string str;
	getline(cin, str);

	try
	{
		auto tokens = TokenizeString(str);
	
		const auto result = CalculateRPN(tokens);
		cout << " = " << fixed << setprecision(3) << result << endl;
	}
	catch (const exception & ex)
	{
		cerr << ex.what() << endl;
	}
}
