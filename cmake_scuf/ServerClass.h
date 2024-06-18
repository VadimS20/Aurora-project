#pragma once
#include<httplib.h>
#include <iostream>
#include <fstream>
#include <mutex>

class ServerClass {
public:
	void setFilePath(std::string path);
	ServerClass(const char* Url, int port);
	void start();
	bool checkUpload();
	void setCheckFile(bool check);
private:
	std::string filePath;
	httplib::Server server;
	int port;
	const char* url;
	bool checkFile=false;
	bool checkUploadFile=false;
};