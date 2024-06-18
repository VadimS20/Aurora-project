#include <iostream>
#include <fstream>
#include <httplib.h>

//// Функция для скачивания файла с сервера
bool download_file(httplib::Client& client) {
    const char* local_file_path = "downloaded_file.csv";
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


void upload_file(httplib::Client& client) {
    std::string filepath = "C:/Users/Вадим/Desktop/opencv_test/1n.jpg";
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
    }

    // Читаем файл
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Создаем multipart/form-data
    httplib::MultipartFormDataItems items = {
        { "file", content, "filename.jpg", "image/jpeg" }
    };
    auto res = client.Post("/upload", items);
    if (res && res->status == 200) {
        std::cout << "File uploaded successfully\n";
    }
    else {
        std::cerr << "Failed to upload file\n";
    }
}

int main() {
    httplib::Client client("http://localhost:8080");

    // Скачивание файла с сервера
    upload_file(client);

    bool check = true;
    while (check) {
        Sleep(5000);
        auto res = client.Get("/check");
        std::cout << res->body << "\n";
        if (res && res->status == 200 && res->body == "Available") {
            download_file(client);
            check = false;
        }
    }

    return 0;
}