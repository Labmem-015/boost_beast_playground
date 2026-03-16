#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <cstdint>
#include <memory>
#include <chrono>
#include <type_traits>

// playground
namespace pg {
	using namespace std::literals;

	namespace asio = boost::asio;
	namespace beast = boost::beast;
	namespace http = boost::beast::http;
} // namespace pg