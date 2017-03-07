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

	std::cout << "�������Ӵ���: ";
	std::cin >> count;
	std::cout << std::endl << std::endl;
}


// ģʽ������-�Ͽ�-������-�ٶϿ�.....
// Ŀ�ģ������ڴ�й©��socket���й©
//BOOST_AUTO_TEST_CASE(tc_connect, *utf::enable_if<enable_connect>())
//{
//	std::cout << "BGN ��������" << std::endl;
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
//	std::cout << "END ��������" << std::endl << std::endl;
//}

BOOST_AUTO_TEST_CASE(tc_end, *utf::enable_if<enable_logger>())
{
	std::cout << "��������? y/n";
	char c;
	std::cin >> c;
}
