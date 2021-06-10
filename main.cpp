/** 
* @file main.cpp
* @brief Главный модуль NTP клиента
*/
#include "NTP.h"
using namespace std;
int main()
{
	try {
		string opt;

		cout << "Получить NTP время и разницу между ним и системным временем введите 1" << endl;
		cout << "Ввести адрес NTP сервера вручную, получить время и разницу для него 2" << endl;
		cout << "Чтобы выйти, введите 0" << endl;
		cin >> opt;
		if(opt == "1") {
			NTP t;
			t.getTime();
		} else if(opt == "2") {
			cout << "Введите адрес в виде 185.224.145.68" << endl;
			string addr;
			cin >> addr;

			NTP t(addr);
			t.getTime();
		} else {
			cout << "Выход..." << endl;
		}
	}
	catch (const ntp_error & e) {
		cerr << e.what() << endl;
	}

	return 0;
}
