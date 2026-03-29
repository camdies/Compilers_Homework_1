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
        // 查找行首第一个非空白字符（这里同时忽略了空格、制表符以及可能在Windows下遗留的回车符 \r）
        size_t first_non_space = line.find_first_not_of(" \t\r");

        // 如果整行都是空白字符（或完全为空行），这行没有需要分析的代码，直接跳过
        if (first_non_space == std::string::npos)
        {
            ++line_num;
            continue;
        }

        // 截取掉前面的空白字符
        std::string trimmed_line = line.substr(first_non_space);

        std::cout << trimmed_line << "\n";

        // 逐个输出属于当前行的 token，缩进 4 个空格
        for (int i = 0; i < list.Size(); ++i)
        {
            const Token& tk = list.Get(i);
            if (tk.line == line_num)
            {
                if (tk.type == TT_Comment)
                {
                    // 注释：保证注释符号（// 或 /* */）在前，后面显示 ": 注释"
                    std::cout << "    " << tk.lexeme << " : 注释\n";
                }
                else
                {
                    std::cout << "    " << tk.lexeme << " : " << TokenTypeToCN(tk.type) << "\n";
                }
            }
        }

        // 分隔不同源代码行的分析输出
        std::cout << "\n";
        ++line_num;
    }
    return 0;
}