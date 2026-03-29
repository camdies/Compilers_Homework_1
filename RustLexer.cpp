//coding: GBK
//@author: Cairenbin
//Version: 1.0
//Only use for homework: Compilers
//fileName: RustLexer.cpp

#include "RustLexer.h"
#include <cctype>
#include <set>
//-------------------------------------------------------------------------------------------

// Rust keywords
//作用: 定义 Rust 语言的关键字列表，用于词法分析器识别和区分标识符与关键字。
//输入: 无
//输出: 一个字符串数组 g_keywords，包含 Rust 语言的所有关键字，以及一个整数 g_kw_count，表示关键字的数量。
static const char* g_keywords[] = {
    "as","break","const","continue","crate","else","enum","extern","false","fn",
    "for","if","impl","in","let","loop","match","mod","move","mut","pub","ref",
    "return","self","Self","static","struct","super","trait","true","type","unsafe",
    "use","where","while","async","await","dyn"
}; // Rust 语言的关键字列表定义结束
//-------------------------------------------------------------------------------------------

static const int g_kw_count = sizeof(g_keywords) / sizeof(g_keywords[0]); // 计算关键字数量

// 判断是否为关键字
//作用: 判断给定的字符串是否是 Rust 语言的关键字，返回 true 表示是关键字，false 表示不是。
//输入: 一个字符串 s
//输出: 返回 true 表示 s 是 Rust 语言的关键字，返回 false 表示 s 不是 Rust 语言的关键字。
bool RustLexer::IsKeyword(const std::string& s) const {
    for (int i = 0; i < g_kw_count; ++i)
    {
        if (s == g_keywords[i]) return true;
    }
    return false;
} //IsKeyword 函数定义结束
//--------------------------------------------------------------------------------------------

// 判断是否为标识符的起始字符
//作用: 判断给定的字符是否可以作为 Rust 语言标识符的起
//输入: 一个字符 c
//输出: 返回 true 表示 c 是一个有效的标识符起始字符（字母或下划线），
//      返回 false 表示 c 不是一个有效的标识符起始字符。
bool RustLexer::IsIdStart(char c) const {
    return std::isalpha((unsigned char)c) || c == '_'; // 标识符的起始字符必须是字母或下划线
} //IsIdStart 函数定义结束
//--------------------------------------------------------------------------------------------

// 判断是否为标识符的非起始字符
//作用: 判断给定的字符是否可以作为 Rust 语言标识符的非起始字符
//输入: 一个字符 c
//输出: 返回 true 表示 c 是一个有效的标识符非起始字符（字母、数字或下划线），
//      返回 false 表示 c 不是一个有效的标识符非起始字符。
bool RustLexer::IsIdPart(char c) const {
    return std::isalnum((unsigned char)c) || c == '_'; // 标识符的非起始字符可以是字母、数字或下划线
} //IsIdPart 函数定义结束
//--------------------------------------------------------------------------------------------

// 判断是否为数字字符
//作用: 判断给定的字符是否是一个数字字符
//输入: 一个字符 c
//输出: 返回 true 表示 c 是一个数字字符（0-9），返回 false 表示 c 不是一个数字字符。
bool RustLexer::IsDigit(char c) const {
    return std::isdigit((unsigned char)c) != 0; // 数字字符必须是 0-9
} //IsDigit 函数定义结束
//--------------------------------------------------------------------------------------------

// 将一个 Token 对象添加到 TokenList 中
//作用: 创建一个 Token 对象并将其添加到给定的 TokenList 中
//输入: 一个 TokenList 对象 out，一个 TokenType 枚举值 t，一个字符串 lex，一个整数 line 和一个整数 col
//输出: 无返回值，但会将一个新的 Token 对象添加到 out 中，Token 对象的类型为 t，
//      文本内容为 lex，行号为 line，列号为 col。
void RustLexer::PushToken(TokenList& out, TokenType t, const std::string& lex, int line, int col) {
    Token tk;
    tk.type = t;
    tk.lexeme = lex;
    tk.line = line;
    tk.column = col;
    out.Add(tk);
} //PushToken 函数定义结束
//--------------------------------------------------------------------------------------------

// 匹配分隔符
//作用: 判断给定位置的字符是否是 Rust 语言的分隔符，并返回匹配结果
//输入: 一个字符串 code，一个位置 pos，一个字符串引用 out 和一个整数引用 len
//输出: 返回 true 表示在 code 的 pos 位置匹配到了一个分隔
//      符，并将匹配到的分隔符存储在 out 中，匹配到的分隔符长度存储在 len 中；
//      返回 false 表示在 code 的 pos 位置没有匹配到分隔符。
bool RustLexer::MatchDelimiter(const std::string& code, size_t pos, std::string& out, size_t& len) const {
    const char c = code[pos];
    const char* delims = "(){}[];,:"; // Rust 语言的分隔符列表
    for (const char* p = delims; *p; ++p) {
        if (c == *p) {
            out.assign(1, c); // 匹配成功，设置输出字符串为匹配到的分隔符
            len = 1;
            return true; // 匹配成功，返回 true
        }
    }
    return false; // 没有匹配到分隔符，返回 false
} //MatchDelimiter 函数定义结束
//--------------------------------------------------------------------------------------------

// 匹配操作符
//作用: 判断给定位置的字符是否是 Rust 语言的操作符，并返回匹配结果
//输入: 一个字符串 code，一个位置 pos，一个字符串引用 out 和一个整数引用 len
//输出: 返回 true 表示在 code 的 pos 位置匹配到了一个操作符，
//      并将匹配到的操作符存储在 out 中，匹配到的操作符长度存储在 len 中；
//      返回 false 表示在 code 的 pos 位置没有匹配到操作符。
bool RustLexer::MatchOperator(const std::string& code, size_t pos, std::string& out, size_t& len) const {
    static const char* ops[] = { // Rust 语言的操作符列表
        ">>=","<<=","..=","==","!=",">=","<=","+=","-=","*=","/=","%=","&&","||","::","..",
        "=","+","-","*","/","%","!","<",">","&","|","^","."
    };
    const int n = sizeof(ops) / sizeof(ops[0]); // 计算操作符数量
    for (int i = 0; i < n; ++i) {
        std::string op = ops[i];
        if (pos + op.size() <= code.size() && code.compare(pos, op.size(), op) == 0) {
            out = op; // 匹配成功，设置输出字符串为匹配到的操作符
            len = op.size();
            return true; // 匹配成功，返回 true
        }
    }
    return false; // 没有匹配到操作符，返回 false
} //MatchOperator 函数定义结束
//--------------------------------------------------------------------------------------------

// 词法分析器入口函数
//作用: 将输入的源代码字符串分解成一个个 Token 对象，并将这些 Token 对象存储在一个 TokenList 对象中
//输入: 一个字符串 code 和一个 TokenList 对象 outTokens
//输出: 无返回值，但会将 code 中的所有 Token 对象添加到 outTokens 中，供后续的语法分析使用。
void RustLexer::Tokenize(const std::string& code, TokenList& outTokens) {
    outTokens.Clear(); // 清空输出的 TokenList，准备存储新的 Token 对象

    size_t  i = 0;
    int line = 1, col = 1;
    while (i < code.size()) {
        char ch = code[i];

        if (ch == ' ' || ch == '\t' || ch == '\r') { ++i; ++col; continue; } // 跳过空格、制表符和回车符
        if (ch == '\n') { ++i; ++line; col = 1; continue; } // 跳过换行符，并更新行号和列号

        int sLine = line, sCol = col; // 记录当前 Token 的起始行号和列号

        //处理注释
        if (ch == '/') {
            if (i + 1 < code.size() && code[i + 1] == '/') { // 单行注释
                size_t st = i;
                i += 2; col += 2;
                while (i < code.size() && code[i] != '\n') { ++i; ++col; } // 跳过单行注释内容
                PushToken(outTokens, TT_Comment, code.substr(st, i - st), sLine, sCol); // 将单行注释作为一个 Token 添加到输出列表中
                continue;
            }
            if (i + 1 < code.size() && code[i + 1] == '*') { // 多行注释
                size_t st = i;
                i += 2; col += 2;
                while (i + 1 < code.size())
                {
                    if (code[i] == '\n') { ++line; col = 1; ++i; continue; } // 跳过换行符，并更新行号和列号
                    if (code[i] == '*' && code[i + 1] == '/') { i += 2; col += 2; break; } // 匹配到多行注释的结束标志，跳出循环
                    ++i; ++col; // 继续扫描多行注释内容
                }
                PushToken(outTokens, TT_Comment, code.substr(st, i - st), sLine, sCol); // 将多行注释作为一个 Token 添加到输出列表中
                continue;
            }
        }

        //处理字符串
        if (ch == '"') {
            size_t st = i;
            ++i; ++col;
            bool esc = false;
            while (i < code.size()) {
                char c = code[i];
                if (c == '\n') { ++line; col = 1; ++i; esc = false; continue; } // 跳过换行符，并更新行号和列号)
                if (!esc && c == '"') { ++i; col += 1; break; } // 匹配到字符串的结束标志，跳出循环
                if (!esc && c == '\\') esc = true; else esc = false; // 处理转义字符
                ++i; ++col; // 继续扫描字符串内容
            }
            PushToken(outTokens, TT_StringLiteral, code.substr(st, i - st), sLine, sCol); // 将字符串作为一个 Token 添加到输出列表中
            continue;
        } //

        //处理数字
        if (IsDigit(ch)) {
            size_t st = i;
            bool isFloat = false;

            if (ch == '0' && i + 1 < code.size() && //处理数字的不同进制表示
                (code[i + 1] == 'X' || code[i + 1] == 'x') ||
                (code[i + 1] == 'O' || code[i + 1] == 'o') ||
                (code[i + 1] == 'B' || code[i + 1] == 'b'))
            {
                i += 2; col += 2; // 跳过进制前缀
                while (i < code.size() && (std::isalnum((unsigned char)code[i]) || code[i] == '_'))
                {
                    ++i; ++col;
                } // 继续扫描数字内容，允许字母、数字和下划线
                PushToken(outTokens, TT_IntegerLiteral, code.substr(st, i - st), sLine, sCol); // 将整数常量作为一个 Token 添加到输出列表中
                continue;
            }

            while (i < code.size() && (IsDigit(code[i]) || code[i] == '_')) { ++i; ++col; } // 继续扫描数字内容，允许数字和下划线

            if (i < code.size() && code[i] == '.' && !(i + 1 < code.size() && code[i + 1] == '.')) { // 处理浮点数
                isFloat = true;
                ++i; ++col; // 跳过小数点
                while (i < code.size() && (IsDigit(code[i]) || code[i] == '_')) { ++i; ++col; } // 继续扫描浮点数内容，允许数字和下划线
            }

            if (i < code.size() && (code[i] == 'e' || code[i] == 'E')) { // 处理科学计数法表示的浮点数
                isFloat = true;
                ++i; ++col;
                if (i < code.size() && (code[i] == '+' || code[i] == '-')) { ++i; ++col; }
                while (i < code.size() && (IsDigit(code[i]) || code[i] == '_')) { ++i; ++col; }
            }

            PushToken(outTokens, isFloat ? TT_FloatLiteral : TT_IntegerLiteral, code.substr(st, i - st), sLine, sCol);
            continue;
        }

        //处理标识符, 关键字, 宏标识
        if (IsIdStart(ch)) {
            size_t st = i;
            ++i; ++col;
            while (i < code.size() && IsIdPart(code[i])) { ++i; ++col; } // 继续扫描标识符内容，允许字母、数字和下划线

            std::string word = code.substr(st, i - st); // 提取标识符文本

            if (i < code.size() && code[i] == '!') { //处理宏标识符
                ++i; ++col; // 跳过感叹号
                word.push_back('!'); // 将感叹号添加到标识符文本中，形成宏标识符
                PushToken(outTokens, TT_MacroIdentifier, word, sLine, sCol); // 将宏标识符作为一个 Token 添加到输出列表中
                continue;
            }
            PushToken(outTokens, IsKeyword(word) ? TT_Keyword : TT_Identifier, word, sLine, sCol);
            // 根据是否是关键字将标识符作为一个 Token 添加到输出列表中
            continue;
        }

        //处理分隔符
        {
            std::string d; size_t len = 0;
            if (MatchDelimiter(code, i, d, len)) { //处理分隔符
                PushToken(outTokens, TT_Delimiter, d, sLine, sCol); // 将分隔符作为一个 Token 添加到输出列表中
                i += len; col += (int)len; // 跳过匹配到的分隔符
                continue;
            }
        }

        //处理操作符
        {
            std::string op; size_t len = 0;
            if (MatchOperator(code, i, op, len)) { //处理操作符
                PushToken(outTokens, TT_Operator, op, sLine, sCol); // 将操作符作为一个 Token 添加到输出列表中
                i += len; col += (int)len; // 跳过匹配到的操作符
                continue;
            }
        }

        // 未知
        PushToken(outTokens, TT_Unknown, std::string(1, ch), sLine, sCol);
        ++i; ++col;
    }
} //Tokenize函数定义结束
//--------------------------------------------------------------------------------------------