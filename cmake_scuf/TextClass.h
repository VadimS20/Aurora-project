#pragma once
#include <boost/locale.hpp>
#include <iomanip>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <set>

class TextClass {
public:
	void createPath();
	void convert();
	void textAnalys(std::string imgPath);
	void createSets();
	void getResult();
	void setVector(std::vector<std::vector<int>> N);
	void createCsv();
	int getNumber();
	std::string getPath();

private:
	std::vector<std::vector<int>> vecN;
	std::vector<std::set<std::string>> texts;
	std::string path;
	int tableNum;
	std::string tableNumber;
};