//
// Created by 韩磊 on 20/10/9.
//

#ifndef TEST10_FILECOMMPASS_H
#define TEST10_FILECOMMPASS_H

#include"HuffmanTree.h"

typedef long long LongType;

struct CharInfo
{
    char _ch;
    LongType _Count;
    string  _HUffmanCode;
    //这里不写构造，因为赫夫曼编码刚开始的时候不适合生成  调用默认的构造函数

    bool operator !=(const CharInfo& ch)
    {
        return this->_Count != ch._Count;
    }

    bool operator < (const CharInfo& ch)
    {
        return this->_Count < ch._Count;
    }

    CharInfo operator+(const CharInfo& ch)
    {
        CharInfo tmpInfo;
        tmpInfo._Count = this->_Count + ch._Count;
        return tmpInfo;
    }

};

class FileCommpass {
public:
    typedef HuffmanTreeNode<CharInfo> CharNode;
    FileCommpass();
    void Compass(const char* filename);   //   压缩文件
    void GetHuffmanCodeII(CharNode* root, string code);
    void UnCompass(const char* filename);  //解压文件

    struct ConfigurationInformation                 //配置信息
    {
        char _ch;
        LongType _Count;
    };

private:
    CharInfo Info[256];
};


#endif //TEST10_FILECOMMPASS_H
