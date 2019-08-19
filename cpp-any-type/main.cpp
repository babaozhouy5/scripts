#include "any.h"
#include <map>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    int ret = 0;

    gl::any i_a = 3;
    int i_b = gl::any_cast<int>(i_a);
    std::cout << i_a << "\t" << i_b << std::endl;

    gl::any s_a = std::string("demo");
    std::string s_b = gl::any_cast<std::string>(s_a);
    std::cout << s_a << "\t" << s_b << std::endl;

    std::map<std::string, gl::any> map_any;
    map_any.emplace("a", i_a);
    map_any.emplace("b", s_b);

    std::cout << i_a << "\t" << gl::any_cast<int>(map_any["a"]) << std::endl;
    std::cout << s_a << "\t" << gl::any_cast<std::string>(map_any["b"]) << std::endl;

    return ret;
}