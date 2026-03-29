#include <iostream>
#include <fstream>
#include <sstream>
#include "RustLexer.h"

int main()
{
    std::string path;
    std::cout << "请输入 .rs 文件路径: ";
    std::getline(std::cin, path);

    std::ifstream ifs(path.c_str(), std::ios::binary);
    if (!ifs)
    {
        std::cout << "打开文件失败\n";
        return 1;
    }

    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string code = ss.str();

    RustLexer lexer;
    TokenList list;
    lexer.Tokenize(code, list);

    std::istringstream iss(code);
    std::string line;
    int line_num = 1;

    while (std::getline(iss, line))
    {
        size_t first_non_space = line.find_first_not_of(" \t");
        std::string trimmed_line = (first_non_space == std::string::npos) ? "" : line.substr(first_non_space);

        std::cout << trimmed_line << "\n";

        for (int i = 0; i < list.Size(); ++i)
        {
            const Token& tk = list.Get(i);
            if (tk.line == line_num)
            {
                if (tk.type == TT_Comment)
                {
                    std::cout << "    " << tk.lexeme << " : 注释\n";
                }
                else
                {
                    std::cout << "    " << tk.lexeme << " : " << TokenTypeToCN(tk.type) << "\n";
                }
            }
        }

        std::cout << "\n";
        ++line_num;
    }

    std::cout << "总数: " << list.Size() << "\n";
    return 0;
}