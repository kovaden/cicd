/* 
  
*/

#include "server.h"
#include "executor.h"
#include "command.h"

#include <boost/asio.hpp>
#include <cstddef>
#include <memory>
#include <iostream>

using boost::asio::ip::tcp;
using namespace std;

class session : public std::enable_shared_from_this<session>
{
	public:
		using tcp = boost::asio::ip::tcp;
		session(tcp::socket socket, const server_params &server_params) : socket_(std::move(socket)),
			server_params_(server_params) {}

		virtual ~session()
		{
			std::cout << "Session destroyed" << std::endl;
		}

		void run()
		{
			do_read();
		}

	private:
		void do_read();
		tcp::socket socket_;
		boost::asio::streambuf buffer_;

		const server_params &server_params_;

		Executor executor;
};

void server::do_accept()
{
	acceptor_.async_accept(
		[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				std::cout << "New connection on: " 
					<< socket.remote_endpoint().address().to_string() << ":" << socket.remote_endpoint().port()
				 	<< std::endl;
				std::make_shared<session>(std::move(socket), server_params_)->run();
			} else {
				std::cout << "Error: " << ec.message() << std::endl;
			}

			do_accept();
		});
}

void session::do_read()
{
	// since we capture `this` in the callback, we need to call shared_from_this()

	auto self(shared_from_this());

	// and now call the lambda once data arrives
	// we read a string until the null termination character	

	boost::asio::async_read_until(socket_, buffer_, "\n",
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			(void)length;
			if (!ec)
			{
				std::istream is(&buffer_);
				std::string command_line;
				std::getline(is, command_line);				

				std::cout << "Received: " << command_line << std::endl;
					auto result = executor.perform(command_line);

					boost::asio::write(socket_, boost::asio::buffer(result.toString()));
				do_read();
			} else {
				std::cout << "Error: " << ec.message() << std::endl;
			}
		});
}
