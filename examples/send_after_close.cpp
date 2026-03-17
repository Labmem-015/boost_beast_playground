#include <client.hpp>

using namespace pg;

int main(int argc, char* argv[]) {
	try {
		auto ctx = std::make_shared<asio::io_context>();
		Client client("127.0.0.1", "8000");
		client.init(ctx);
		client.connect();

		request_t req;
		req.method(http::verb::post);
		req.target("/echo");
		req.body() = "some info";

		client.query(req);
		asio::steady_timer timer(*ctx);
		timer.expires_after(6s);
		timer.wait();
		client.query(req);

		ctx->run();
		client.disconnect();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}