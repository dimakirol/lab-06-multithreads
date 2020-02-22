// Copyright 2019 dimakirol <your_email>

#include <header.hpp>

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    auto func = []()           //считать строки 14-34 просто необычно заданной функцией f
    {
        std::string hex_str = ""; //в ней будет лежать хэш!!!!
        do {
            int size = rand() % 23 + 5; // в строках 15-20 генерируем случайную строку
            std::cout << "size" << size << std::endl;
            std::string src_str = "";
            for (int j = 0; j < size; ++j) {
                src_str[j] = rand() % 256;
            }
            std::vector<unsigned char> hash(picosha2::k_digest_size); //в строках 21-23 считаем хэш для строки
            picosha2::hash256(src_str.begin(), src_str.end(), hash.begin(), hash.end());
            hex_str = picosha2::bytes_to_hex_string(hash.begin(), hash.end());

            std::cout << "ID: " << std::this_thread::get_id(); //пока вывожу просто сиаутом id потока, сгенерированную
            std::cout << " string: '" << src_str.c_str() << "' SHA= " << hex_str << std::endl; //строку и полученный
        }while(hex_str.rfind("0000") != 60);                                                //хэш, соответственно
        //в данном месте искомый хэш найден и лежит в той же строке после закрытия фигурной скобки на 34 строке
        //данный поток закроется
    };
    auto *arr = new std::thread[NUMBER_OF_THREADS]; //создаем массив потоков
    for(int i = 0; i < NUMBER_OF_THREADS; ++i){
        arr[i] = std::thread(func); //отправляем каждый вновь созданный поток работать в функции f
    }
    for(int i = 0; i < NUMBER_OF_THREADS; ++i){
        arr[i].join(); // на данном месте потоки вышли из функции f и умерли)
    }
}
