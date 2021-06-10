/**
 * @file NTP.cpp
 */
#include "NTP.h"
/**
* @brief Конструктор класса NTP.
* @param string address адрес NTP сервера в формате 127.0.0.1 по умолчанию: 185.224.145.68
* @details Создаёт сокет и привязывает его к локальному адресу
* @warning При передаче не верного адреса в верном формате подключение произойдёт
* @throw ntp_error, при ошибке
*/
NTP::NTP(string address)
{
	if(address.empty())
		throw ntp_error("Недопустимый адрес");
	for(auto s : address)
		if(s < 48 && s != 46 || s > 57)
			throw ntp_error("Недопустимый адрес");
	size_t n = count(address.begin(), address.end(), '.');
	if(n != 3)
		throw ntp_error("Недопустимый формат адреса");
	
	string check_addr = address;	
	
	// Преобразуем адресс
	struct in_addr addr;
	int ad_addr = inet_aton(address.c_str(), &addr);
	if(ad_addr != 1)
		throw ntp_error("Ошибка преобразования адреса");
	remoteAdress = addr.s_addr;

	// Таймаут на прием
	struct timeval tm;
	tm.tv_sec = 2;
	tm.tv_usec = int(std::fmod(2, 1.0) * 1000000);

	// Сокет
	soket = socket(AF_INET, SOCK_DGRAM, 0);
	if(soket == -1) 
		throw ntp_error("Ошибка создания сокета");

	if(setsockopt(soket, SOL_SOCKET, SO_RCVTIMEO, &tm, sizeof(tm)) == -1)
		throw ntp_error(std::string("Время истекло"));

	sockaddr_in selfAddr;
	selfAddr.sin_family = AF_INET;
	selfAddr.sin_port = 0;
	selfAddr.sin_addr.s_addr = 0;

	// Бинд
	if(bind(soket, (struct sockaddr *) &selfAddr, sizeof(selfAddr)) == -1)
		throw ntp_error("Ошибка бинда");
}
/**
* @brief Деструктор класса NTP.
* @details Закрывает сокет, открытый в конструкторе.
*/
NTP::~NTP()
{
	close(soket);
}
/**
* @brief Запрос к NTP серверу и получение разницы во времени
* @throw ntp_error, если произошла ошибка.
*/
void NTP::getTime()
{
	unsigned char msg[48]= {010,0,0,0,0,0,0,0,0};
	uint64_t resp[1024];

	// Подготовка структуры с адресом программы
	sockaddr_in remoteAddr;
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = remotePort;
	remoteAddr.sin_addr.s_addr = remoteAdress;

	// Отправка запроса на запись
	if (sendto(soket, msg, 48, 0, (struct sockaddr *) &remoteAddr, sizeof(remoteAddr)) == -1)
		throw ntp_error("Ошибка отправки запроса на запись");

	// Получение ответа от сервера
	socklen_t rlen = sizeof(rlen);
	if (recv(soket, resp, sizeof(resp), 0) == -1)
		throw ntp_error("Ошибка получения ответа от сервера");
	else {
		int tmit = ntohl((time_t)resp[4]);
		tmit -= 2208988800U;
		time_t temp = tmit;

		int i = time(0); // Системное время
		cout << "Время NTP сервера:" << ctime(&temp);
		cout << "Разница между системным временем и NTP " << (i - tmit) << "сек." << endl;
	}
}
