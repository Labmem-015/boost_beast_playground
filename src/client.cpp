#include "client.hpp"

namespace pg {

Client::Client(std::string_view hostname, std::string_view port):
	m_hostname(hostname),
	m_port(port) { }

auto
Client::init(std::shared_ptr<asio::io_context> ctx) -> void {
	m_ctx = ctx;
	m_strand = std::make_unique<strand_t>(asio::make_strand(m_ctx->get_executor()));
    boost::system::error_code ec;
	for (int i = 0; i < 3; ++i) {
		m_endpoint = tcp_t::resolver(*ctx).resolve(m_hostname, m_port, ec);
		if (!ec) break;
	}
	if (ec) throw boost::system::system_error(ec);
    m_socket = std::make_unique<tcp_stream_t>(*m_strand);
}

auto Client::query(request_t& request) -> response_t
{
    request.set(http::field::host, m_hostname);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.set(http::field::content_type, "application/json; charset=UTF-8");
    request.prepare_payload();  // this sets content_length
    request.version(11);
    request.keep_alive(true);
    //request.insert("Connection", "keep-alive");
    //request.insert("Keep-Alive", "timeout=70, max=200");
    request.set(http::field::keep_alive, "timeout=60, max=200");


    boost::system::error_code ec;
    m_socket->connect(m_endpoint, ec);
    if (ec) throw boost::system::system_error(ec);
    http::write(*m_socket, request, ec);
    if (ec) throw boost::system::system_error(ec);
    
    response_t response;
    http::read(*m_socket, m_buffer, response, ec);
    if (ec == http::error::end_of_stream) {
        std::cout << "Closing socket" << std::endl;
    }
    else if (ec) throw boost::system::system_error(ec);
    
    m_socket->close();
    return response;
}

} // namespace pg