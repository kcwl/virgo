#include "parse.h"
#include <iostream>

namespace aquarius
{
    parse::parse()
        : statements_()
        , row_(1)
        , column_(1)
    {

    }

    bool parse::read_file(const std::string& file_name)
    {
        std::fstream ifs(file_name, std::ios::in | std::ios::binary);
        if (!ifs.is_open())
        {
            std::cout << "file [" << file_name << "] not found\n";

            return false;
        }

        while (!ifs.eof())
        {
            statements_.push_back({});

            auto& state = statements_.back();

            auto result = parse_statement(ifs, state);

            if (result != success)
                return false;
        }

        return true;
    }

    std::vector<statement> parse::get_statements()
    {
        return statements_;
    }

    int parse::parse_statement(std::fstream& ifs, statement& state)
    {
        skip_if(ifs, ' ', '\t', '\r', '\n');

        auto error = read_keyword(ifs, state.type_str);

        if (error != stop_character || error == success)
            return error;

        auto keyword = md5(state.type_str);

        if (keyword == -1)
        {
            if (check_type(state.type_str))
                keyword = 256;
            else
                return syntax_error;
        }

        switch (keyword)
        {
            case 0:
            case 1:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 256:
            {
                error = read_normal(ifs, state);
            }
            break;
            case 2:
            {
                error = read_enum(ifs, state);
            }
            break;
            case 3:
            case 4:
            {
                error = read_message(ifs, state, true);
            }
            break;
            case 13:
            {
                error = read_normal_bytes(ifs, state);
            }
            break;
            case 14:
            {
                error = read_message(ifs, state);
            }
            break;
            case 15:
            {
                state.sub_type = "repeated";

                error = parse_statement(ifs, state);
            }
            break;
            default:
            {
                error = syntax_error;

                std::cout << "unknown keyword [" << state.type_str << "]\n";
            }
            break;
        }

        return error;
    }

    int parse::read_normal(std::fstream& ifs, statement& state)
    {
        auto error = read_value(ifs, state.name_str);
        if (error != stop_character)
        {
            error = syntax_error;
        }

        return error;
    }

    int parse::read_normal_bytes(std::fstream& ifs, statement& state)
    {
        state.type_str = "std::vector<uint8_t>";

        return read_value(ifs, state.name_str);
    }

    int parse::read_enum(std::fstream& ifs, statement& state)
    {
        auto error = read_value_for(ifs, state.name_str);

        if (error != left_brace_match)
        {
            return syntax_error;
        }

        while (true)
        {
            state.seqs.push_back({});

            auto& s = state.seqs.back();

            error = read_value(ifs, s.name_str);

            if (error == syntax_error)
            {
                break;
            }

            if (error == right_brace_match)
                break;
        }

        if (error == right_brace_match)
        {
            error = seek_end(ifs);
        }

        return error;
    }

    int parse::read_message(std::fstream& ifs, statement& state, bool is_service)
    {
        int error = success;

        if (is_service)
        {
            error = read_value_of(ifs, state.name_str);

            if (error != eqyual_match)
                return error;

            error = read_sequence(ifs, state.sequence);
        }
        else
        {
            error = read_value_for(ifs, state.name_str);
        }

        if (error != left_brace_match)
        {
            return syntax_error;
        }

        while (!ifs.eof())
        {
            state.seqs.push_back({});
            error = parse_statement(ifs, state.seqs.back());

            if (error == right_brace_match)
                break;

            if (error == syntax_error)
                break;
        }

        if (error == right_brace_match)
        {
            ifs.get();
            error = seek_end(ifs);
        }

        return error;
    }

    void parse::log(const std::string& str, int line)
    {
        std::cout << "column: " << column_ << " row: " << row_ << "\t" << str << "\tline:" << line << std::endl;
    }

    int parse::read_util(std::fstream& ifs, char sp, std::string& value)
    {
        int error = syntax_error;

        while (!ifs.eof())
        {
            auto c = ifs.peek();
            if (c == sp)
            {
                error = success;
                break;
            }
            else if (std::isalnum(c))
            {
                value += static_cast<char>(ifs.get());
            }
            else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            {
                ifs.get();

                if (c == '\n')
                {
                    column_ += 1;
                    row_ = 1;
                }
            }
            else if (c == ';' || c == '}' || c == ']')
            {
                error = stop_character;
                break;
            }
        }

        return error;
    }

    int parse::read_util(std::fstream& ifs, char sp)
    {
        int error = syntax_error;

        while (!ifs.eof())
        {
            auto c = ifs.peek();
            if (c == sp)
            {
                error = success;
                break;
            }
            else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            {
                ifs.get();

                if (c == '\n')
                {
                    column_ += 1;
                    row_ = 1;
                }
            }
            else if (c == ';' || c == '}' || c == ']')
            {
                error = stop_character;
                break;
            }
        }

        return error;
    }

    int parse::seek_end(std::fstream& ifs)
    {
        int result = syntax_error;

        while (!ifs.eof())
        {
            auto c = ifs.peek();

            if (c == ';')
            {
                ifs.get();
                result = success;
                break;
            }
            else if (c != '\n' && c != '\t' && c != ' ' && c != '\r')
            {
                break;
            }
        }

        return result;
    }


    int parse::read_keyword(std::fstream& ifs, std::string& keyword)
    {
        int error = success;
        keyword.clear();

        while (!ifs.eof())
        {
            row_ += 1;

            auto c = ifs.peek();

            if (std::isalnum(c))
            {
                keyword += static_cast<char>(ifs.get());
            }
            else if (c == ' ')
            {
                ifs.get();
                error = stop_character;
                break;
            }
            else if (c == '}')
            {
                error = right_brace_match;
                break;
            }
            else
            {
                std::string str = "unexpected character";
                str += " ";
                str += static_cast<char>(c);
                log(str, __LINE__);
                error = syntax_error;
                return error;
            }
        }

        return error;
    }

    int parse::read_value(std::fstream& ifs, std::string& value)
    {
        value.clear();

        int error = 0;

        while (!ifs.eof())
        {
            row_ += 1;

            auto c = ifs.peek();

            if (std::isalnum(c) || c == '_')
            {
                value += static_cast<char>(ifs.get());
            }
            else if (c == ';')
            {
                error = stop_character;
                ifs.get();
                break;
            }
            else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            {
                ifs.get();

                if (c == '\n')
                {
                    column_ += 1;
                    row_ = 1;
                }
            }
            else if (c == '}')
            {
                error = right_brace_match;

                ifs.get();
                break;
            }
            else
            {
                error = 3;
                std::string str = "unexpected character";
                str += " ";
                str += static_cast<char>(c);
                log(str, __LINE__);
                break;
            }
        }

        return error;
    }

    int parse::read_value_of(std::fstream& ifs, std::string& value)
    {
        int error = 0;
        value.clear();

        while (!ifs.eof())
        {
            row_ += 1;

            auto c = ifs.peek();

            if (std::isalnum(c) || c == '_')
            {
                value += static_cast<char>(ifs.get());

            }
            else if (c == '=')
            {
                ifs.get();

                error = eqyual_match;

                break;
            }
            else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            {
                ifs.get();
                if (c == '\n')
                {
                    column_ += 1;
                    row_ = 1;
                }
            }
            else if (c == '}')
            {
                error = right_brace_match;

                ifs.get();
                break;
            }
            else
            {
                std::string str = "unexpected character";
                str += " ";
                str += static_cast<char>(c);
                log(str, __LINE__);
                return 3;
            }
        }

        return error;
    }

    int parse::read_sequence(std::fstream& ifs, std::string& sequence)
    {
        int error = 0;
        sequence.clear();

        while (!ifs.eof())
        {
            row_ += 1;
            auto c = ifs.peek();

            if (c == ';')
            {
                error = stop_character;

                ifs.get();
                break;
            }
            else if (c == '{')
            {
                error = left_brace_match;
                ifs.get();
                break;
            }
            else if (c == '}')
            {
                error = right_brace_match;

                ifs.get();
                std::string str = "unexpected character";
                str += " ";
                str += static_cast<char>(c);
                log(str, __LINE__);
                break;
            }
            else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            {
                ifs.get();
                if (c == '\n')
                {
                    column_ += 1;
                    row_ = 1;
                }
            }
            else if (!(std::isalnum(c) && std::isalpha(c)))
            {
                sequence += static_cast<char>(ifs.get());
            }
            else
            {
                error = syntax_error;
                std::string str = "unexpected character";
                str += " ";
                str += static_cast<char>(c);
                log(str, __LINE__);
                break;
            }
        }

        return error;
    }

    int parse::read_value_for(std::fstream& ifs, std::string& value)
    {
        value.clear();

        int error = 0;

        if (ifs.eof())
            return stop_character;

        while (!ifs.eof())
        {
            row_ += 1;
            auto c = ifs.peek();

            if (std::isalnum(c) || c == '_')
            {
                value += static_cast<char>(ifs.get());

            }
            else if (c == '{')
            {
                error = left_brace_match;
                ifs.get();
                break;
            }
            else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            {
                ifs.get();
                if (c == '\n')
                {
                    column_ += 1;
                    row_ = 1;
                }
            }
            else
            {
                error = syntax_error;
                std::string str = "unexpected character";
                str += " ";
                str += static_cast<char>(c);
                log(str, __LINE__);
                break;
            }
        }

        return error;
    }

    bool parse::check_type(std::string& type)
    {
        auto iter = std::find_if(statements_.begin(), statements_.end(), [&](const statement& s)
            {
                return s.name_str == type;
            });

        return iter != statements_.end();
    }

}