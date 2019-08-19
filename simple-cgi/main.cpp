#include "getpost.h"
#include <map>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    int ret = 0;

    std::map<std::string, std::string> GetData;
    std::map<std::string, std::string> PostData;
    initializeGet(GetData);
    initializePost(PostData);

    // do something you want
    // std::string url_param1 = Get["param1"];
    // std::string form_param1 = Post["param1"];

    // must write head to stdout first
    std::cout << "Content-type: text/html" << std::endl << std::endl;

    return ret;
}