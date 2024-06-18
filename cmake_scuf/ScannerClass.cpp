#include "ScannerClass.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

using namespace cv;

ScannerClass::ScannerClass(std::string path) { //конструктор
	imgOriginal=imread(path);
}

ScannerClass::ScannerClass(const ScannerClass& copy) { // конструктор копировани€
	imgOriginal = copy.imgOriginal;
}

void ScannerClass::Resize(double x, double y) { //уменьшение изображени€
	resize(imgOriginal, imgOriginal, Size(), x, y);
}

void ScannerClass::imgPrep() { 
	cvtColor(imgOriginal, imgNew, COLOR_BGR2GRAY); //мен€ем цвет
	GaussianBlur(imgNew, imgNew, Size(3, 3), 3, 0); //размытие
	Canny(imgNew, imgNew, 25, 75);
}


void ScannerClass::imgContours(){
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	findContours(imgNew, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //ищем контуры

	std::vector<std::vector<Point>> conPoly(contours.size());
	std::vector<Rect> boundRect(contours.size());

	std::vector<Point> biggest;

	int maxArea = 0;

	for (int i = 0; i < contours.size(); i++) { 
		int area = contourArea(contours[i]); //площадь фигуры
		//std::cout << area << std::endl;

		std::string objectType;

		if (area > 1000) { //если площадь фигуры больше определенного значени€
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (area > maxArea && conPoly[i].size() == 4) { //если площадь больше предыдущей и количество углов равно 4, тогда запоминаем
				//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 2);
				biggest = { conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3] };
				maxArea = area;
			}




		}

	}
	points = biggest;
}

int ScannerClass::correctPoints() { //корректируем  угловые точки
	std::vector<Point> newPoints;
	std::vector<int> sum;
	if (points.empty()) { //если не был найден пр€моугольник, тогда выходим из программы
		return 1;
	}
	for (int i = 0; i < 4; i++) {
		sum.push_back(points[i].x + points[i].y);
	}
	int min = 100000;
	int	max = 0;
	int id_1 = -1, id_2 = -1, id_3 = -1, id_4 = -1;
	for (int i = 0; i < 4; i++) { //определение правого нижнего угла
		if (sum[i] > max) {
			max = sum[i];
			id_1 = i;
		}
		if (sum[i] < min) { //определение левого верхнего угла
			min = sum[i];
			id_4 = i;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (i == id_1 || i == id_4) {
			continue;
		}
		if (id_3 == -1)
		{
			id_3 = i;
			continue;
		}
		if (id_2 == -1) {
			id_2 = i;
			continue;
		}
	}
	if (abs(points[id_1].x - points[id_4].x) > abs(points[id_1].y - points[id_4].y)) horizontal = true;
	else horizontal = false;

	//if (abs(points[id_2].x - points[id_2].y) < abs(points[id_3].x - points[id_3].y)) {

	if(points[id_2].x > points[id_3].x) swap(id_2, id_3);

	//if (ScannerClass::horizontal && points[id_2].x>points[id_3].x) swap(id_2,id_3);
	
	if (!ScannerClass::horizontal ) {
		swap(id_4, id_3);
		swap(id_3, id_1);
		swap(id_2, id_1);

	}
	//}

	newPoints.push_back(points[id_1]);
	newPoints.push_back(points[id_2]);
	newPoints.push_back(points[id_3]);
	newPoints.push_back(points[id_4]);
	
	w = 1000;	// ¬џ—ќ“ј
	h = w * (abs(newPoints[id_1].x - newPoints[id_2].x) / abs(newPoints[id_2].y - newPoints[id_3].y));



	points = newPoints;

	return 0;
}

void ScannerClass::showOriginal() {
	imshow("originalImage", imgOriginal); //показать оригинал
}

void ScannerClass::showWarp() {
	cv::imwrite("saved_image.jpg", imgWarp); //показать раст€нутую таблицу
	imshow("warpImage", imgWarp);
}

void ScannerClass::Warp() { //раст€гиваем таблицу по определенным ранее точкам
	Mat matrix;

	Point2f src[4] = { points[3],points[2] ,points[1] ,points[0] };
	Point2f dst[4] = { {0.0f,0.0f}, {h,0.0f}, {0.0f,w},{h,w} };
	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(imgOriginal, imgWarp, matrix, Point(h, w));

	matrix.release();
}

void ScannerClass::drawPoints(cv::Scalar color) { //начертить точки на оригинальном изображении
	for (int i = 0; i < points.size(); i++) {
		circle(imgOriginal, points[i], 10, color, FILLED);
		putText(imgOriginal, std::to_string(i), points[i], FONT_HERSHEY_PLAIN, 2, color, 2);
	}
	cv::imwrite("points.jpg", imgOriginal);
}

ScannerClass::~ScannerClass(){
	imgOriginal.release();
	imgGray.release();
	imgNew.release();
}

void ScannerClass::result() { //результат
	cvtColor(imgWarp, imgWarp, COLOR_BGR2GRAY);
	//GaussianBlur(imgWarp, imgWarp, Size(3, 3), 0);
	//threshold(imgWarp, result, 0, 255, THRESH_BINARY + THRESH_OTSU);
	adaptiveThreshold(imgWarp, imgResult, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 25, 15);
	//threshold(imgWarp, result, 150, 255, THRESH_BINARY);
	cv::imwrite("result.jpg", imgResult);
}

void ScannerClass::createCrop() //обрезать таблицу по €чейкам
{
	Mat imgTemp;
	Mat imgResult = imread("result.jpg");
	cvtColor(imgResult, imgTemp, COLOR_BGR2GRAY);
	GaussianBlur(imgTemp, imgTemp, Size(3, 3), 3, 0);
	Canny(imgTemp, imgTemp, 30, 150);



	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	findContours(imgTemp, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<Point>> conPoly(contours.size());
	std::vector<Rect> boundRect(contours.size());

	std::vector<Point> biggest;

	int smallWidth = 1;
	int smallHeigth = 1;
	int maxArea = 0;
	int minArea = 1000000;

	for (int i = 0; i < contours.size(); i++) { //определ€ем €чейку слева вверху

		int area = contourArea(contours[i]);
		if (area > 600) {
			//std::cout << area << std::endl;

			std::string objectType;

			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (conPoly[i].size() == 4) {

				if (area > maxArea) {
					//drawContours(imgOrig, conPoly, i, Scalar(0, 0, 255), 2);
					biggest = { conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3] };
					maxArea = area;
				}
				if (area < minArea) {
					//drawContours(imgOrig, conPoly, i, Scalar(255, 0, 255), 2);
					smallWidth = max(abs(conPoly[i][0].x - conPoly[i][1].x), abs(conPoly[i][0].x - conPoly[i][2].x));
					smallHeigth = max(abs(conPoly[i][0].y - conPoly[i][1].y), abs(conPoly[i][0].y - conPoly[i][2].y));
					minArea = area;
				}
			}
		}

	}
	Size sz = imgTemp.size();
	//обрезаем таблицу вниз и вправо на высоту левой верхней €чейки

	int startW = abs(biggest[0].x - biggest[1].x);
	int startH = abs(biggest[2].y - biggest[1].y);

	int width = sz.width - startW;
	int height = sz.height - startH;

	line = height / smallHeigth;
	column = width / smallWidth;

	realHeight = height / line;
	realWidth = width / column;

	Mat ROI(imgResult, Rect(startW, startH, width, height));

	ROI.copyTo(imgCrop);

	//imshow("bbb", imgCrop);
	imwrite("cropped.jpg", imgCrop);
	imgTemp.release();
	imgResult.release();
}

int ScannerClass::getColumn()
{
	return column;
}

int ScannerClass::getLine()
{
	return line;
}

std::vector<std::vector<int>> ScannerClass::findN()
{
	Mat imgTemp;
	Mat imgCropped = imread("cropped.jpg");
	cvtColor(imgCropped, imgTemp, COLOR_BGR2GRAY);
	GaussianBlur(imgTemp, imgTemp, Size(3, 3), 3, 1);
	Canny(imgTemp, imgTemp, 100, 200);
	Mat se1 = getStructuringElement(MORPH_RECT, Size(1, 1));

	dilate(imgTemp, imgTemp, se1);

	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	findContours(imgTemp, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<Point>> conPoly(contours.size());
	std::vector<Rect> boundRect(contours.size());

	std::vector<std::vector<Point>> result;

	int smallWidth = 1;
	int maxArea = 0;
	int minArea = 1000000;

	for (int i = 0; i < contours.size(); i++) { //ищем символы, похожие на "н" и по координатам определ€ем, в какой €чейке этот символ

		int area = contourArea(contours[i]);
		std::string objectType;

		float peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);


		if ((conPoly[i].size() > 6)) {
			if (peri > 100 && peri < 500) {
				drawContours(imgCropped, conPoly, i, Scalar(0, 255, 0), 2);
				result.push_back(conPoly[i]);
			}

		}

	}

	imwrite("CannyFindN.jpg", imgTemp);
	imwrite("FindN.jpg", imgCropped);

	std::vector<std::vector<int>> foundN(line, std::vector<int>(column));

	//создаем матрицу €чеек

	for (int i = 0; i < result.size(); i++) {
		int l = result[i][0].y / realHeight;
		int c = result[i][0].x / realWidth;

		float resultX = result[i][0].x;
		float resultY = result[i][0].y;

		float lRate = (resultY / realHeight) - (result[i][0].y / realHeight);
		float cRate = (resultX / realWidth) - (result[i][0].x / realWidth);

		if (lRate > 0.05 && lRate < 0.95 && cRate>0.05 && cRate < 0.95) {
			foundN[l][c] = 1;
		}
	}
	N = foundN;

	imgTemp.release();
	imgCropped.release();
	return foundN;
	
}



