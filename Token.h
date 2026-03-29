//coding: GBK
//@author: Cairenbin
//Version: 1.0
//Only use for homework: Compilers
//fileName: Token.h

#pragma once
#include <string>

//Rust记号类型
//作用: 定义Rust记号类型TokenType
enum TokenType {
    TT_Keyword, //关键字
    TT_Identifier, //标识符
    TT_MacroIdentifier, //宏标识符
    TT_IntegerLiteral, //整数常量
    TT_FloatLiteral, //浮点数常量
    TT_StringLiteral, //字符串常量
    TT_Comment, //注释
    TT_Delimiter, //分隔符
    TT_Operator, //运算符
    TT_Unknown, //未知记号
}; //Rust记号类型TokenType定义结束


//Rust单个记号结构体
//作用: 定义Rust单个记号结构体Token
struct Token {
    TokenType type; //记号类型
    std::string lexeme; //记号的文本内容
    int line; //行号(从1开始)
    int column; //列号(从1开始)
}; //Rust记号结构体Token定义结束


//类型转中文
//类型转换, 将TokenType转化中文字符串返回
//作用: 定义类型转换函数TokenTypeToCN, 将TokenType转化中文字符串返回
inline const char* TokenTypeToCN(TokenType t) {
    switch (t)
    {
    case TT_Keyword:         return "关键字";
    case TT_Identifier:      return "标识符";
    case TT_MacroIdentifier: return "宏调用名";
    case TT_IntegerLiteral:  return "字面量（整数）";
    case TT_FloatLiteral:    return "字面量（浮点数）";
    case TT_StringLiteral:   return "字符串字面量";
    case TT_Comment:         return "注释";
    case TT_Delimiter:       return "分隔符";
    case TT_Operator:        return "操作符";
    default:                 return "未知";
    }
} //TokenTypeToCN类型转换定义结束