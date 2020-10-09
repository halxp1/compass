//
// Created by 韩磊 on 20/10/9.
//

#include "FileCommpass.h"

FileCommpass::FileCommpass() {
    for (size_t i = 0; i < 256; i++)
    {
        Info[i]._ch = i;
        Info[i]._Count = 0;
    }
}

void FileCommpass::Compass(const char *filename) {
    FILE* fout = fopen(filename, "r");        //   二进制读取

    //统计字符出现的次数
    size_t chcount = 0;
    char ch = fgetc(fout);                 //fgetc的用法
    while (!feof(fout))                      //EOF：文件结束标志
    {
        Info[(unsigned char)ch]._Count++;
        chcount++;
        ch = fgetc(fout);
    }
    cout << "原文件的字符总数：" << chcount << endl;

    //搭建出现字母的赫夫曼树，将文件中出现的字母进行建堆，没有出现就不需要建堆了 所以需要用的invalid
    CharInfo _invalidInfoNode;
    _invalidInfoNode._Count = 0;                          //出现次数为0的字母包
    HuffmanTree<CharInfo> _h(Info, 256,_invalidInfoNode);

    string code;
    GetHuffmanCodeII(_h.GetRoot(),code);


    //写配置信息（二进制方式）然后再压缩
    string compassfile = "";                   //单参数构造函数支持强转

    string name = string(filename);
    int pos = name.find_last_of('/');
    compassfile = "../output/"+name.substr(pos+1)+".huffman";

    FILE* fIn = fopen(compassfile.c_str(), "w");


    //写入二进制信息
    ConfigurationInformation cinfo;
    for (size_t i = 0; i < 256; i++)
    {
        if (Info[i]._Count)
        {
            cinfo._ch = Info[i]._ch;
            cinfo._Count = Info[i]._Count;
            size_t size=fwrite(&cinfo, sizeof(ConfigurationInformation), 1, fIn);            //写入
            //cout << size << endl;
        }
    }
    cinfo._Count = 0;                                                        //这里相当于写一个结束标志方便后面解压的时候配合
    size_t size=fwrite(&cinfo, sizeof(ConfigurationInformation), 1, fIn);   //返回值是：size*count


    char value = 0;
    int count = 0;                 //要注意这里的count的刚开始的值和最后结束的值
    int compasschcount = 0;
    fseek(fout, 0, SEEK_SET);     //fseek函数的返回值为整形，不是一个文件指针  fout指针指向文件首字母
    char ch1 = fgetc(fout);       //fgetc操作函数
    while (!feof(fout))
    {
        string& code = Info[(unsigned char)ch1]._HUffmanCode;                 //使用unsigned char 强转的目的是为了什么？
        for (size_t i = 0; i < code.size(); i++)
        {
            value <<= 1;                      //注意移位并不会改变变量本身的值
            if (code[i] == '1')                //string支持了[]的重载
            {
                value |= 1;
            }
            else
            {
                value |= 0;
            }
            count++;

            if (count == 8)                   //满8位就写到压缩文件里面去  然后重新给8位0
            {
                fputc(value, fIn);            //fputc的使用      //文本方式写入？可以吗？打开的时候是用二进制打开的
                value = 0;
                count = 0;
            }
        }
        ch1 = fgetc(fout);
        compasschcount++;
    }

    if (count != 0)                 //未满八位的位处理
    {
        value <<= (8 - count);
        fputc(value, fIn);
    }
    fclose(fIn);
    fclose(fout);
}

void FileCommpass::GetHuffmanCodeII(CharNode *root, string code) {
    if (root == NULL)
        return;
    if (root->_left == NULL&&root->_right == NULL)
    {
        Info[(unsigned char)root->_data._ch]._HUffmanCode = code;     //注意�@里不需要反转
        return;
    }
    GetHuffmanCodeII(root->_left, code + '0');     //注意：string是支持+的
    GetHuffmanCodeII(root->_right, code + '1');

}

void FileCommpass::UnCompass(const char *filename) {
    string uncompassfile = filename;
    size_t pos = uncompassfile.rfind('.');
    uncompassfile = uncompassfile.substr(0, pos);    //substr的用法
    FILE* fin = fopen(uncompassfile.c_str(), "w");
    FILE* fout = fopen(filename, "r");        //压缩文件二进制写，则解压的时候使用二进制方式读
    //读取配置信息然后传件原文件里面相同的Info结构体最后建树
    while (1)
    {
        ConfigurationInformation cinfo;
        size_t size = fread(&cinfo, sizeof(ConfigurationInformation), 1, fout);          //从fout读到cinfo结构体里面
        if (cinfo._Count)
        {
            Info[(unsigned char)cinfo._ch]._ch = cinfo._ch;
            Info[(unsigned char)cinfo._ch]._Count = cinfo._Count;
        }
        else
        {
            break;            //循环终止
        }
    }

    //重建huffman树
    CharInfo _invalidInfoNode;
    _invalidInfoNode._Count = 0;
    HuffmanTree<CharInfo> _h(Info, 256, _invalidInfoNode);

    CharNode* root = _h.GetRoot();
    size_t charCount = root->_data._Count;          //赫夫曼树根结点的权值的含义是   可以记录原文件里面一共有多少字符


    //按位读取进行
    char value = fgetc(fout);
    CharNode* cur = root;
    size_t cCount = 0;
    while (!feof(fout))                    //如果这里是EOF  EOF的值相当于-1
    {
        for (int pos = 7; pos >= 0; pos--)         //别用size_t
        {
            if (value&(1 << pos))      //1       //位操作
            {
                cur = cur->_right;
            }
            else                       //0
            {
                cur = cur->_left;
            }
            if (cur->_left == NULL&&cur->_right == NULL)
            {
                fputc(cur->_data._ch, fin);
                cCount++;
                cur = root;
                if (cCount == charCount)               //该解压到多少字符就该停止了
                {
                    break;
                }
            }
        }
        value = fgetc(fout);
    }
    fclose(fin);
    fclose(fout);
}