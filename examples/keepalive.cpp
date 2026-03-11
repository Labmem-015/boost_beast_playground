namespace asio = boost::asio;
namespace beast = boost::beast;

using tcp_t = asio::ip::tcp;
using tcp_stream_t = beast::tcp_stream;

int main(int argc, char* argv[]) {
	asio::io_context ctx;
	tcp_t::resolver(ctx).resolve("127.0.0.1", "80");
	return 0;
}