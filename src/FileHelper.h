#pragma once

#include <includes.h>
#include <unordered_map>
#include <fstream>
#include <string>
#include <memory>
// TODO to be changed with more complex algo
namespace Cold
{
    struct File {
        std::fstream file_stream;
        i64 current_index {0};
        std::string path;
    };

    class FileHelper
    {
    public:
        static i32 open_file(const std::string& path);
        static void push_char(i32 file, Character entry);
        static void move_file_index(i32 file, i64 index);
        static std::pair<bool, std::string> get_next_line(i32 fd);
        static void write_string(i32 fd, const std::string& data);
        static void reset_flags_for_writing(i32 fd);
        static void close_file_and_open_it_again(i32 fd);
        static void close();
    private:
        static inline std::unordered_map<i32, std::unique_ptr<File>> opened_files;
        static inline i32 unique_id {0};

    };
    
    using FileUPtr = std::unique_ptr<File>;
    
} // namespace Cold
