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
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/core/null_deleter.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using src::severity_logger;
using boost::log::trivial::severity_level;

#define NUMBER_OF_THREADS 100
//std::thread::hardware_concurrency()
#define THIS_string "0000"
#define POS_to_find_THIS_string 60
#define SIZE_OF_SRC_STR 10
#define SIZE_OF_HASH_STR 61
#define MAX_NUMBER_OF_CALCS 20000
#define LOG_FILE_NAME "sample_%N.log"
#define ROTATION_SIZE_A 10
#define ROTATION_SIZE_B 1024
#define ROTATION_SIZE_H 1024


class my_little_hash{
public:
    my_little_hash() {
        alpha =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890";
        auto now = static_cast<unsigned int>(time(nullptr));
        counter = static_cast<uint64_t>(rand_r(&now) % alpha.length());
        std::cout << std::endl << "SPECIAL FOR DIMON!!!!:)" << std::endl;
        std::cout << "Atomic = " << counter << std::endl;
    }
    void init()
    {
        //boost::log::register_simple_formatter_factory
        //    <boost::log::trivial::severity_level, char>("Severity");
        logging::add_file_log
        (
            keywords::file_name = LOG_FILE_NAME,
            keywords::rotation_size = ROTATION_SIZE_A *
                                      ROTATION_SIZE_B * ROTATION_SIZE_H,
            keywords::time_based_rotation =
                sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format =
                 "[%TimeStamp%][%ThreadID%][%Severiti%]: %Message%");
        logging::add_console_log
            (
                    std::cout,
                    logging::keywords::format =
                    "[%TimeStamp%] [%ThreadID%] [%Severity%]: %Message%");
        logging::add_common_attributes();
    }
    static void calc_hash(uint32_t id, std::string alphabet,
                                std::atomic_ulong *magic_number){
        std::mutex door_first;
        while (!door_first.try_lock())
            std::this_thread::sleep_for(std::chrono::milliseconds(id+1));
        auto hex_str = new std::string(SIZE_OF_HASH_STR, '\0');
        auto hash = new std::vector<unsigned char>(picosha2::k_digest_size);
        auto src_str = new std::string;
        door_first.unlock();
        (*src_str).assign(alphabet, 0, SIZE_OF_SRC_STR);
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
            //BOOST_LOG_SEV(trace, lg) <<  "ID: " << id; for file output
            BOOST_LOG_TRIVIAL(trace) <<  "ID: " << id;
            BOOST_LOG_TRIVIAL(trace) << "; string: '" << src_str->c_str();
            BOOST_LOG_TRIVIAL(trace) << "' SHA = " << (*hex_str);
            door_print.unlock();
            if ((*magic_number) > MAX_NUMBER_OF_CALCS)
                break;
        } while (hex_str->rfind(THIS_string) != POS_to_find_THIS_string);
        std::mutex door_last;
        while (!door_last.try_lock())
            std::this_thread::sleep_for(std::chrono::milliseconds(id+1));
        delete hash;
        if (hex_str->rfind(THIS_string) == POS_to_find_THIS_string) {
            BOOST_LOG_TRIVIAL(info) << "FINAL RESULT: " << std::endl;
            BOOST_LOG_TRIVIAL(info) << "ID: " << id;
            BOOST_LOG_TRIVIAL(info) << "; String '" << src_str->c_str();
            BOOST_LOG_TRIVIAL(info) << "' SHA = " << (*hex_str);
        }
        delete hex_str;
        delete src_str;
        door_last.unlock();
    }
    void zaraza() {
        init();
        logging::add_common_attributes();
        src::severity_logger< severity_level > lg;
        auto arr = new std::thread[NUMBER_OF_THREADS];
        for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i) {
            arr[i] = std::thread(calc_hash, i, alpha, &counter);//, lg);
        }
        for (uint32_t i = 0; i < NUMBER_OF_THREADS; ++i) {
            arr[i].join();
        }
        delete [] arr;
    }

private:
    std::string alpha;
    std::atomic_ulong counter;
};

#endif // INCLUDE_HEADER_HPP_
