#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include <cmath>
#include <fstream>
#include <Windows.h> 
#include <string>




class ScannerClass {
public:
	ScannerClass(std::string path);
	~ScannerClass();
	ScannerClass(const ScannerClass& copy);
	void Resize(double x, double y);
	void imgPrep();
	void imgContours();
	void Warp();
	int correctPoints();
	void showOriginal();
	void showWarp();
	void drawPoints(cv::Scalar color);
	void result();
	void createCrop();
	int getColumn();
	int getLine();
	std::vector<std::vector<int>> findN();

private:
	int realHeight; //������� ������ ������
	int realWidth; //������� ����� ������
	int line; //���������� ����� ��� "�"
	int column; //���������� �������� ��� "�"
	float w; //������
	float h; // ������
	std::vector<std::vector<int>> N; //������� ����� � N
	std::vector<cv::Point> points; //������ ����� ��� warp
	cv::Mat imgGray, imgOriginal, imgNew,imgWarp,imgResult,imgCrop;
	bool horizontal;
};