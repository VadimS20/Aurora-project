#include "ClientClass.h"

ClientClass::ClientClass(QObject *parent):QObject(parent)
{
}

Q_INVOKABLE void ClientClass::setUrlPort(QString Url, int Port){
    auto temp = Url.toStdString();
    url=temp.c_str();
    port = Port;
}

void ClientClass::upload_file(httplib::Client& client)
{
    std::string filepath = path;
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Cannot open the  file\n";
    }

    auto res = client.Get("/check");
    std::cout << res->body << "\n";
    // Читаем файл
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    httplib::MultipartFormDataItems items = {
        { "file", content, "filename.jpg", "image/jpeg" }
    };
    res = client.Post("/upload", items);
    if (res && res->status == 200) {
        std::cout << "File uploaded successfully\n";
    }
    else {
        std::cerr << "Failed to upload file\n";
        std::cerr<< httplib::to_string(res.error())<<std::endl;
    }
}

bool ClientClass::download_file(httplib::Client& client)
{
    const char* local_file_path = "/home/defaultuser/Documents/schedule.csv";
    const char* server_file_path = "/csv";

    // Запрос файла с сервера
    auto res = client.Get(server_file_path);

    if (res && res->status == 200) {
        // Открытие локального файла для записи
        std::ofstream file(local_file_path, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open local file for writing." << std::endl;
            return false;
        }

        // Запись содержимого файла на локальный диск
        file << res->body;

        std::cout << "File downloaded successfully." << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to download file. Error code: " << (res ? res->status : -1) << std::endl;
        return false;
    }

}

Q_INVOKABLE void ClientClass::start(QString Path)
{
    if (Path[0]=='f'){
        Path.remove(0,7);
    }
    path = Path.toStdString();
    std::cout<<path<<std::endl;
    httplib::Client client(url, port);
    upload_file(client);

    bool check = true;
    while (check) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        auto res = client.Get("/check");
        std::cout << res->body << "\n";
        if (res && res->status == 200 && res->body == "Available") {
            download_file(client);
            check = false;
        }
    }
    client.stop();
}
