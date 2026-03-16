#include <client.hpp>

using namespace pg;

int main(int argc, char* argv[]) {
	try {
		auto ctx = std::make_shared<asio::io_context>();
		Client client("127.0.0.1", "8000");
		client.init(ctx);
		request_t req;

		req.method(http::verb::post);
		req.target("/echo");
		req.body() = "some info";

		client.query(req);

		ctx->run();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}