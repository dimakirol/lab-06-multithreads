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
#define NUMBER_OF_THREADS 5 //std::thread::hardware_concurrency()

void head() {
  auto func = []() //считать строки 7-32 просто необычно заданной функцией f
  {
    std::mutex door_first;
    door_first.lock();
    auto hex_str = new std::string(61, '\0');
    auto hash = new std::vector<unsigned char>(picosha2::k_digest_size);
    auto now = new unsigned;
    auto size = new uint32_t;
    auto src_str = new std::string(55, '\0');
    door_first.unlock();
    do {
      //в строках 12-17 генерируем случайную строку от 5 до 50 символов
      std::mutex door_second;
      door_second.lock();
      srand(time(nullptr));
      *now = static_cast<unsigned int>(time(0));
      *size = static_cast<uint32_t>(rand_r(now) % 50 + 5);
      door_second.unlock();
      for (uint32_t j = 0; j < *size; ++j) {
        (*src_str)[j] = static_cast<char>(rand_r(now) % 256);
      }
      //в строках 19-22 считаем хэш для строки
      picosha2::hash256(src_str->begin(), src_str->end(), hash->begin(),
                        hash->end());
      *hex_str = picosha2::bytes_to_hex_string(hash->begin(), hash->end());
      //пока вывожу просто сиаутом id потока, сгенерированную строку и
      //полученный хэш, соответственно
      std::mutex door_print;
      door_print.lock();
      //std::cout << "ID: " << std::this_thread::get_id();
      //std::cout << " string: '" << src_str->c_str();
      //std::cout << "' SHA= " << *hex_str << std::endl;
      door_print.unlock();
    } while (hex_str->rfind("0000") != 60);
    std::mutex door_last;
    door_last.lock();
    delete size;
    delete now;
    delete src_str;
    delete hash;
    std::cout << "' SHA= " << *hex_str << std::endl;
    delete hex_str;
    //в данном месте искомый хэш найден и лежит в той же строке после
    //закрытия фигурной скобки на 32 строке данный поток закроется
    //так что лучше именно здесь логировать успешный результат
    door_last.unlock();
  };
  auto *arr = new std::thread[NUMBER_OF_THREADS]; //создаем массив потоков
  for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i) {
    arr[i] = std::thread(func); //отправляем каждый вновь созданный поток
                                //работать в функции f
  }
  for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i) {
    arr[i].join(); // на данном месте потоки вышли из функции f и умерли)
  }
}

#endif // INCLUDE_HEADER_HPP_
