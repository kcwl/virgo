#pragma once
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include "statement.h"

namespace aquarius
{
    class parse
    {
    public:
        parse();

    public:
        bool read_file(const std::string& file_name);

        std::vector<statement> get_statements();

    private:
        int parse_statement(std::fstream& ifs, statement& state);

    private:
        int read_normal(std::fstream& ifs, statement& state);

        int read_normal_bytes(std::fstream& ifs, statement& state);

        int read_enum(std::fstream& ifs, statement& state);

        int read_message(std::fstream& ifs, statement& state, bool is_service = false);

    private:
        template<typename... _Args>
        std::string trip(std::string& str, _Args&&... args)
        {
            if (str.empty())
                return str;

            (str.erase(std::remove(str.begin(), str.end(), args), str.end()), ...);

            return str;
        }

        template <typename... _Args>
        void skip_if(std::fstream& ifs, _Args&&... args)
        {
            while (!ifs.eof())
            {
                auto c = ifs.peek();

                if (c == '\n')
                {
                    column_++;
                    row_ = 1;
                }
                else {
                    row_++;
                }

                if (((c == args) || ...))
                {
                    ifs.get();
                }
                else
                {
                    break;
                }
            }
        }

        void log(const std::string& str, int line);

        int read_util(std::fstream& ifs, char sp, std::string& value);

        int read_util(std::fstream& ifs, char sp);

        int seek_end(std::fstream& ifs);

        int read_keyword(std::fstream& ifs, std::string& keyword);

        int read_value(std::fstream& ifs, std::string& value);

        int read_value_of(std::fstream& ifs, std::string& value);

        int read_sequence(std::fstream& ifs, std::string& sequence);

        int read_value_for(std::fstream& ifs, std::string& value);

        bool check_type(std::string& type);

    private:
        std::vector<statement> statements_;

        int row_;

        int column_;
    };
}
