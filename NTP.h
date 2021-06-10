/** 
* @file NTP.h
* @author Хиль В.Д.
* @version 1.0
* @date 06.06.2021
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл для класса NTP
*/
#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <chrono>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;
/** 
* @brief Клиент сервера NTP
* @details Метод getTime получает время с NTP сервера и разницу меду системым временем 
* и временем серера. Можно передать в конструктор адрес любого NTP серевера.
*/
class NTP
{
private:
	int remoteAdress;
	int remotePort = htons(123);
	int soket;
public:
	NTP(std::string address = "185.224.145.68");
	~NTP();
	void getTime();
};

/** @brief Класс обработки ошибок ntp_error
* @details Используется для вывода сообщений об ошибках
*/
class ntp_error: public std::invalid_argument
{
public:
	explicit ntp_error (const std::string& what_arg):
		std::invalid_argument(what_arg) {}
	explicit ntp_error (const char* what_arg):
		std::invalid_argument(what_arg) {}
};