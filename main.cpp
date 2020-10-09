#include <iostream>
#include "FileCommpass.h"

int main() {

    FileCommpass fcps;
    fcps.Compass("../TestCottom/TestInput.txt");

    FileCommpass fcps2;
    fcps2.UnCompass("../output/TestInput.txt.huffman");     //配置信息已经写入
    return 0;
}