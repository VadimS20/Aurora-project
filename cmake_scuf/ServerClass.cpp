#include "ServerClass.h"



void ServerClass::setFilePath(std::string path)
{
    filePath = path;
}

ServerClass::ServerClass(const char* Url, int Port)
{
    url = Url;
    port = Port;

    server.Post("/upload", [&](const httplib::Request& req, httplib::Response& res) { //загрузка файла с сервера
        auto file = req.get_file_value("file");
        // Сохраняем файл
        checkUploadFile = true;
        std::ofstream ofs("received_file.jpg", std::ios::binary);
        ofs << file.content;
        ofs.close();
        res.set_content("File uploaded successfully", "text/plain");
       // std::cerr << res.status << "\n";
        });

    server.Get("/check", [&](const httplib::Request& req, httplib::Response& res) { //установка флага загруженного файла
        if (checkFile)
        {
            res.set_content("Available", "text/plain");
        }
        else {
            res.set_content("NotAvailable", "text/plain");
        }
        });

    server.Get("/csv", [&](const httplib::Request& req, httplib::Response& res) { //загрузка csv на сервер
        if (checkFile) {
            std::ifstream file(filePath, std::ios::binary);
            if (file) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                res.set_content(buffer.str(), "application/octet-stream");
            }
            else {
                res.status = 404;
                res.set_content("File not found", "text/plain");
            }
        }
        });

}



void ServerClass::start() //стартуем сервер
{
    std::thread server_thread([&]() { 
    server.listen(url, port); 
    int temp = server.bind_to_any_port("0.0.0.0");
    server.listen_after_bind();
        });

    server_thread.join();
}

bool ServerClass::checkUpload()
{
    return checkUploadFile;
}

void ServerClass::setCheckFile(bool check) //установка флага файла
{
    checkFile = check;
}
