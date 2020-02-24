// Copyright 2019 dimakirol <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <iostream>
#include <thread>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <picosha2.h>
#include <mutex>
#define NUMBER_OF_THREADS 2
//std::thread::hardware_concurrency()

void head () {
    const char alpha[] =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "1234567890";
    auto func = [](const char* alphabet)
    {
        int count = 0;
        std::mutex door_first;
        door_first.lock();
        auto hex_str = new std::string(61, '\0');
        auto hash = new std::vector<unsigned char>(picosha2::k_digest_size);
        auto now = new unsigned;
        auto size = new uint32_t;
        auto src_str = new std::string;
        door_first.unlock();
        do {
            std::mutex door_second;
            door_second.lock();
                *now = static_cast<unsigned int>(time(nullptr));
                *size = static_cast<uint32_t>(rand_r(now) % 50 + 5);
                door_second.unlock();
            for (uint32_t j = 0; j < *size; ++j) {
                (*src_str) += alphabet[(rand_r(now) % 62)];
            }
            picosha2::hash256(src_str->begin(), src_str->end(), hash->begin(),
                              hash->end());
            *hex_str = picosha2::bytes_to_hex_string(hash->begin(),
                              hash->end());

            std::mutex door_print;
            door_print.lock();
                std::cout << "ID: " << std::this_thread::get_id();
                std::cout << " string: '" << src_str->c_str() << std::endl;
                std::cout << "' SHA= " << *hex_str << std::endl;
                *src_str = "";
            door_print.unlock();
            if (count > 10)
                break;
            count++;
        } while (hex_str->rfind("0000") != 60);
        std::mutex door_last;
        door_last.lock();
            delete size;
            delete now;
            delete src_str;
            delete hash;
            std::cout << std::endl << std::endl << std::endl;
            std::cout << "FINAL RESULT:" << std::endl;
            std::cout << "ID: " << std::this_thread::get_id();
            std::cout << "; SHA= " << *hex_str << std::endl;
            delete hex_str;
        door_last.unlock();
    };
    auto *arr = new std::thread[NUMBER_OF_THREADS]; //создаем массив потоков
    for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i) {
        arr[i] = std::thread(func, alpha); //отправляем каждый вновь созданный поток
        //работать в функции f
    }
    for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i) {
        arr[i].join(); // на данном месте потоки вышли из функции f и умерли)
    }
}

#endif // INCLUDE_HEADER_HPP_
