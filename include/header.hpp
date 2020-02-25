// Copyright 2019 dimakirol <file>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <iostream>
#include <thread>
#include <string>
#include <atomic>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <picosha2.h>
#include <mutex>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

#define NUMBER_OF_THREADS 100
//std::thread::hardware_concurrency()

class my_little_hash{
public:
    my_little_hash() {
        alpha =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890";
        auto now = static_cast<unsigned int>(time(nullptr));
        counter = static_cast<uint64_t>(rand_r(&now) % 62);
        std::cout << std::endl << "SPECIAL FOR DIMON!!!!:)" << std::endl;
        std::cout << "Atomic = " << counter << std::endl;
    }
    void init()
    {
        logging::add_file_log
        (
            keywords::file_name = "sample_%N.log",
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation =
                sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format =
                 "[%TimeStamp%][%ThreadID%][%Severiti%] %Message%");



        )
    }
    static void calc_hash(uint32_t id, std::string alphabet,
                                std::atomic_ulong *magic_number,
                                src::severity_logger< severity_level > lg){
        std::mutex door_first;
        while (!door_first.try_lock())
            std::this_thread::sleep_for(std::chrono::milliseconds(id+1));
        auto hex_str = new std::string(61, '\0');
        auto hash = new std::vector<unsigned char>(picosha2::k_digest_size);
        auto src_str = new std::string;
        door_first.unlock();
        (*src_str).assign(alphabet, 0, 10);
        do {
            std::mutex door_second;
            while (!door_second.try_lock())
                std::this_thread::sleep_for(std::chrono::milliseconds(id+1));
            uint64_t temp = (*magic_number) % alphabet.length();
            (*src_str).erase((temp % (*src_str).length()), 1);
            (*src_str).push_back(alphabet[temp]);
            (*magic_number)++;
            door_second.unlock();

            picosha2::hash256(src_str->begin(), src_str->end(), hash->begin(),
                              hash->end());
            (*hex_str) = picosha2::bytes_to_hex_string(hash->begin(),
                                                     hash->end());

            std::mutex door_print;
            while (!door_print.try_lock())
                std::this_thread::sleep_for(std::chrono::milliseconds(id+1));
            BOOST_LOG_SEV(lg, trace) <<  "ID: " << id;
            BOOST_LOG_SEV(lg, trace) << " string: '" << src_str->c_str();
            BOOST_LOG_SEV(lg, trace) << "' SHA = " << (*hex_str);
            door_print.unlock();
            if ((*magic_number) > 20000)
                break;
        } while (hex_str->rfind("0000") != 60);
        std::mutex door_last;
        while (!door_last.try_lock())
            std::this_thread::sleep_for(std::chrono::milliseconds(id+1));
        delete hash;
        if (hex_str->rfind("0000") == 60) {
            BOOST_LOG_SEV(lg, info) << "FINAL RESULT: " << std::endl;
            BOOST_LOG_SEV(lg, info) << "String '" << src_str->c_str();
            BOOST_LOG_SEV(lg, info) << "ID: " << id;
            BOOST_LOG_SEV(lg, info) << "; SHA = " << (*hex_str);
        }
        delete hex_str;
        delete src_str;
        door_last.unlock();
    }
    void zaraza() {
        init();
        logging::add_common_attributes();

        using logging::trivial::src::severity_logger;
        using logging::trivial::src::severity_level;
        //using logging::trivial::src::severity_logger;
        src::severity_logger< severity_level > lg;
        auto arr = new std::thread[NUMBER_OF_THREADS]; //создаем массив потоков
        for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i) {
            arr[i] = std::thread(calc_hash, i, alpha, &counter, lg);
        }
        for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i) {
            arr[i].join();
            // на данном месте потоки вышли из функции f и умерли)
        }
        delete [] arr;
    }

private:
    std::string alpha;
    std::atomic_ulong counter;
};

#endif // INCLUDE_HEADER_HPP_
