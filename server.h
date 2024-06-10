#ifndef SERVER_H
#define SERVER_H

#include <cstdint>
#include <iostream>
#include <boost/asio.hpp>

struct server_params
{
	int port;
};

class server
{
public:
	using tcp = boost::asio::ip::tcp;
	server(boost::asio::io_context &io_context, const server_params &sp)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), sp.port)),
		server_params_(sp)
	{
		do_accept();
	}

private:
	void do_accept();
	tcp::acceptor acceptor_;
	const server_params &server_params_;
};


#endif