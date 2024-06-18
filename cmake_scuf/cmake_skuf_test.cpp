#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<string>
#include <cmath>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <fstream>
#include <Windows.h> 
#include <string>
#include <boost/locale.hpp>
#include <iomanip>
#include <filesystem>

#include"ServerClass.h"
#include"ScannerClass.h"
#include"TextClass.h"


using namespace cv;
using namespace std;


int main() {
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
	setlocale(LC_ALL, "Rus"); // Подключение русского языка



	remove("received_file.jpg");


	ServerClass server("192.168.175.135", 8080);
	std::thread server_thread([&]() {
		server.start();
		});


	while (true) {
		remove("received_file.jpg");
		server.setCheckFile(false);
		bool start = true;
		while (start) {
			std::fstream file("received_file.jpg");
			if (file.is_open()) {
				file.seekg(0, ios::end);
				cout << file.tellg() << "\n";

				if (file.tellg() > 0) {
					start = false;
					std::cout << "file received\n";
					break;
				}
			}

			file.close();
			std::cout << "file not received" << "\n";
			Sleep(5000);
		}

		string path = "received_file.jpg";
		ScannerClass test(path);

		// TODO


		test.Resize(0.5, 0.5); //изменение размеров
		test.imgPrep(); //серый цвет, размытие, canny
		test.imgContours(); //нахождение контуров
		if (test.correctPoints()) {//изменение точек таблицы в соответствии с ориентацией листа
			cout << "cannot recognise points, please try again";
			continue;
		}
		test.Warp(); //растягивание изображения
		//test.showWarp(); //показать warp
		test.drawPoints(Scalar(0, 0, 255));
		//test.showOriginal();
		test.result();

		test.createCrop();

		std::vector<std::vector<int>> N = test.findN(); //матрица ячеек


		TextClass text;
		text.createPath();//создание пути к csv
		text.textAnalys("result.jpg"); //определяем текст с помощью Tesseract OCR

		text.createSets(); //создание словаря из таблиц-примеров
		text.getResult(); //определение наиболее похожий к нашей таблице

		text.setVector(N); //сеттер матрицы ячеек
		text.createCsv(); //создание итогового csv файла

		std::string newPath = text.getPath(); //getter пути к csv

		server.setFilePath(newPath); //установка к пути к файлу,который необходимо загрузить на серврер
		server.setCheckFile(true); //установка флага, того что файл загружен на сервер
		Sleep(10000);
	}

	server_thread.join();

	return 0;
}
