namespace pg {

using request_t = http::request<http::string_body>;
using response_t = http::response<http::string_body>;
using strand_t = asio::strand<asio::io_context::executor_type>;

class Client {
    using tcp_t = asio::ip::tcp;
    using tcp_stream_t = beast::tcp_stream;

public:
    explicit Client(std::string_view hostname, std::string_view port);
    void init(std::shared_ptr <asio::io_context> ctx);
    response_t query(request_t& request);
private:
    std::shared_ptr<asio::io_context> m_ctx;
    std::unique_ptr<strand_t> m_strand;
    std::string m_hostname;
    std::string m_port;
    tcp_t::resolver::results_type m_endpoint;

    std::unique_ptr<tcp_stream_t> m_socket;
    beast::flat_buffer m_buffer;
};

} // namespace pg