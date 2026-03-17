#include <iostream>

int main(int argc, char* argv[]) {
    boost::system::error_code ec;
    ec = boost::asio::error::interrupted;
    std::cout << "Reading response interruped by system call" << std::endl;
    boost::system::system_error err(ec);
    std::cout << err.what() << std::endl;

    return 0;
}