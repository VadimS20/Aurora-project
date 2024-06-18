#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include "TextClass.h"

void TextClass::createPath() {
	auto t = time(nullptr);
	auto tm = *localtime(&t);

	std::stringstream ss;
	ss << std::put_time(&tm, "%d%m%Y");

	path = ss.str();

	path += ".csv";
}

void TextClass::convert() {
	std::ifstream inputFile("temp.txt"); // Файл с текущей кодировкой (UTF-8)

	std::ofstream outputFile(path); // Файл, куда будет записано содержимое с новой кодировкой (ANSI)

	if (!inputFile.is_open()) {
		std::cerr << "Error: open for reading" << std::endl;
	}

	if (!outputFile.is_open()) {
		std::cerr << "Error: open for writing" << std::endl;
	}

	std::string line;
	while (std::getline(inputFile, line)) {
		// Преобразование строки из UTF-8 в кодировку ANSI
		std::string convertedLine = boost::locale::conv::from_utf<char>(line, "CP1251");

		outputFile << convertedLine << std::endl;
	}

	inputFile.close();
	outputFile.close();
}

void TextClass::textAnalys(std::string imgPath)
{
	char* outText;
	FILE* f;

	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with Russian, without specifying tessdata path
	if (api->Init(NULL, "rus", tesseract::OEM_LSTM_ONLY)) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	Pix* image = pixRead(imgPath.c_str());
	api->SetVariable("tessedit_char_blacklist", "|/_(&$»>[]{}%:>");
	api->SetImage(image);

	// Get OCR result
	outText = api->GetUTF8Text();

	std::ofstream out;      // поток для записи
	out.open("temp.txt");      // открываем файл для записи
	if (out.is_open())
	{
		out << outText << std::endl;
	}
	out.close();

	// Destroy used object and release memory
	api->End();
	delete api;
	delete[] outText;
	pixDestroy(&image);
}

void TextClass::createSets()
{
	for (int i = 0; i < 4; i++) {
		std::set<std::string> temp_set;
		std::string line;
		std::string path = std::to_string(i + 1) + ".csv";
		std::ifstream in(path);
		if (in.is_open()) {
			while (getline(in, line)) {
				std::string temp;
				temp = "";
				for (auto c = line.begin(); c != line.end(); c++) {
					if (*c != ' ' && *c != ';') {
						temp += *c;
					}
					else {
						if (temp != "") {
							temp_set.insert(temp);
						}
						temp = "";
					}
				}
			}
		}
		texts.push_back(temp_set);
		in.close();
	}
}

void TextClass::getResult()
{
	std::vector<int> result;
	std::string line;
	result.resize(texts.size(), 0);
	std::ifstream text("temp.txt");
	if (text.is_open()) {
		while (getline(text, line)) {
			std::string temp;
			for (auto c = line.begin(); c != line.end(); c++) {
				if (*c != ' ' && *c != ';') {
					temp += *c;
				}
				else {
					if (temp != "") {
						for (int i = 0; i < texts.size(); i++) {
							if (texts[i].count(temp)) {
								result[i] += 1;
							}
						}
					}
					temp = "";
				}
			}

		}
	}

	int max = 0;
	int res = -1;

	for (int i = 0; i < texts.size(); i++) {
		if (result[i] > max) {
			max = result[i];
			res = i + 1;
		}
	}

	tableNum = res;
	tableNumber = std::to_string(res);

	//std::cout << "Ответ: " << res << ".csv \n";
}

void TextClass::setVector(std::vector<std::vector<int>> N)
{
	vecN = N;
}

void TextClass::createCsv()
{
	std::string line;
	std::string inPath = tableNumber + ".csv";
	std::ifstream in(inPath);

	std::ofstream out(path);

	if (in.is_open() && out.is_open()) {
		getline(in, line);
		out << line << "\n";

		int i = 0;

		while (getline(in, line)) {
			int j = 0;
			std::string temp;
			temp = "";
			for (auto c = line.begin(); c != line.end(); c++) {
				if (*c != ';') {
					temp += *c;
				}
				else {
					out << temp;
					temp = "";
					if (vecN[i][j] == 1) {
						out << ";Н";
					}
					else {
						out << ";";
					}
					j++;
				}
			}
			out << "\n";
			i++;
		}
	}
}

int TextClass::getNumber()
{
	return tableNum;
}

std::string TextClass::getPath()
{
	return path;
}


