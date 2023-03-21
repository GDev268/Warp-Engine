#include <iostream>
#include "include/lol.hpp"

std::string lol::name = "Hello world!";

int main(char argc, char **argv){
    std::cout << lol::name << std::endl;
}