//
// Created by zhanglei on 2020/11/16.
//

#include "app/AtelFormat.h"

App::AtelFormat::AtelFormat() {

}

void App::AtelFormat::append(std::string& buffer) {
    atelBuffer.push_back(std::move(buffer));
}

void App::AtelFormat::flush() {
    std::cout << atelBuffer.size() << std::endl;

    if (!atelBuffer.empty()) {
        std::ofstream fileOut;
        fileOut.open(outPath, std::ios::app|std::ios::out);
        std::vector<std::string>::iterator atelIterator;
        if (fileOut.is_open())
        {
            for (atelIterator = atelBuffer.begin(); atelIterator != atelBuffer.end(); ++atelIterator)
            {
                fileOut<<(*atelIterator);
            }
        }
        fileOut.close();
    }
}

App::AtelFormat::~AtelFormat() {
    atelBuffer.clear();
}