#include <boost/test/unit_test.hpp> 
namespace utf = boost::unit_test;

#include "stdafx.h"
#include "Logger.h"
#include "TcpSession.h"
#include "TcpClient.h"
#include "TcpServer.h"
using namespace boost::asio::ip;

int count = 1;

BOOST_AUTO_TEST_CASE(tc_init_logger, *utf::enable_if<enable_logger>())
{
	initLogger();
}

BOOST_AUTO_TEST_CASE(tc_repeat_times, *utf::enable_if<enable_repeat_times>())
{

	std::cout << "输入连接次数: ";
	std::cin >> count;
	std::cout << std::endl << std::endl;
}


// 模式：连接-断开-再连接-再断开.....
// 目的：测试内存泄漏，socket句柄泄漏
//BOOST_AUTO_TEST_CASE(tc_connect, *utf::enable_if<enable_connect>())
//{
//	std::cout << "BGN 连接中心" << std::endl;
//	auto endpoint = tcp::endpoint(address::from_string("139.224.61.179"), 8207);
//
//	boost::asio::io_service ios;
//	std::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(ios));
//	auto thread = boost::thread([&ios]() {ios.run(); });
//
//	for (int i = 0; i < count; i++)
//	{
//		auto tcpClient = std::make_shared<TcpClient>(ios);
//		tcpClient->session()->_afterNetError = []() {};
//		tcpClient->syncConnect(endpoint);
//	}
//
//	work.reset();
//	thread.join();
//	std::cout << "END 连接中心" << std::endl << std::endl;
//}

BOOST_AUTO_TEST_CASE(tc_end, *utf::enable_if<enable_logger>())
{
	std::cout << "结束测试? y/n";
	char c;
	std::cin >> c;
}
