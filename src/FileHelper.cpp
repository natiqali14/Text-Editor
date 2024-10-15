#include "FileHelper.h"

namespace Cold {
    i32 FileHelper::open_file(const std::string &path)
    {
        FileUPtr file = std::make_unique<File>();
        file->path = path;
        file->file_stream = std::fstream(path, std::ios::in | std::ios::out | std::ios::app);
        if (!file->file_stream ||!file->file_stream.is_open()) 
            throw std::runtime_error("Can not open file with path" + path);

        file->file_stream.seekp(0);
        opened_files.insert({unique_id, std::move(file)});
        return unique_id++;
    }

    void FileHelper::push_char(i32 file, Character entry)
    {
    }

    void FileHelper::move_file_index(i32 file, i64 index)
    {
    }

    std::pair<bool, std::string> FileHelper::get_next_line(i32 fd)
    {
        std::string line;
        if (std::getline(opened_files.at(fd)->file_stream, line)) {
            return {true, line};
        }
        return {false, ""};
    }

    void FileHelper::write_string(i32 fd, const std::string &data)
    {
        if (!opened_files.at(fd)->file_stream.is_open()) {
            throw std::runtime_error("ERROR on Write");
        }
        opened_files.at(fd)->file_stream.write(data.c_str(), data.size());
    }

    void FileHelper::reset_flags_for_writing(i32 fd)
    {
        opened_files.at(fd)->file_stream.clear();
        opened_files.at(fd)->file_stream.seekp(0);
    }

    void FileHelper::close_file_and_open_it_again(i32 fd)
    {
        opened_files.at(fd)->file_stream.close();
        auto path = opened_files.at(fd)->path;
        opened_files.at(fd)->file_stream = std::fstream(path, std::ios::out | std::ios::trunc);
    }

    void FileHelper::close()
    {
        for (auto& files : opened_files) {
            files.second->file_stream.flush();
            files.second->file_stream.close();
        }
    }

} // namespace Cold