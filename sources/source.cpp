// Copyright 2019 dimakirol <your_email>

#include <header.hpp>

int main()
{
    auto func = []()//считать строки 7-32 просто необычно заданной функцией f
    {
        std::string hex_str = ""; //в ней будет лежать хэш!!!!
        do {
            //в строках 12-17 генерируем случайную строку от 5 до 50 символов
            unsigned now = static_cast<unsigned int>(time(0);
            uint32_t size = static_cast<char>(rand_r(&now) % 50 + 5);
            std::string src_str = "";
            for (uint32_t j = 0; j < size; ++j) {
                src_str[j] = static_cast<char>(rand_r(&now) % 256);
            }
            //в строках 19-22 считаем хэш для строки
            std::vector<unsigned char> hash(picosha2::k_digest_size);
            picosha2::hash256(src_str.begin(), src_str.end(), hash.begin(),
                                                               hash.end());
            hex_str = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
            //пока вывожу просто сиаутом id потока, сгенерированную строку и
            //полученный хэш, соответственно
            std::cout << "ID: " << std::this_thread::get_id();
            std::cout << " string: '" << src_str.c_str();
            std::cout<< "' SHA= " << hex_str << std::endl;
        }while(hex_str.rfind("0000") != 60);
        //в данном месте искомый хэш найден и лежит в той же строке после
        //закрытия фигурной скобки на 32 строке данный поток закроется
        //так что лучше именно здесь логировать успешный результат
    };
    auto *arr = new std::thread[NUMBER_OF_THREADS]; //создаем массив потоков
    for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i){
        arr[i] = std::thread(func); //отправляем каждый вновь созданный поток
                                                    //работать в функции f
    }
    for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i){
        arr[i].join(); // на данном месте потоки вышли из функции f и умерли)
    }
}
