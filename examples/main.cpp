#include <iostream>

#include <basicvariant/basicvariant.h>

int main(int argc, char** argv)
{
    extension::core::variant_t var("123");

    std::cout << "Hello, World!" << var.which() <<std::endl;


    return 0;
}