#include <boost/test/unit_test.hpp> 
namespace utf = boost::unit_test;

#include "stdafx.h"
#include "Logger.h"
#include "TcpSession.h"
#include "TcpClient.h"
#include "TcpServer.h"
using namespace boost::asio::ip;
using boost::asio::io_service;
using std::shared_ptr;
using boost::thread;

int count = 1;
src::severity_channel_logger<SeverityLevel> logger((keywords::channel = "net"));

BOOST_AUTO_TEST_CASE(tc_init_logger, *utf::enable_if<enable_logger>())
{
	initLogger();
}

// ģʽ������-�Ͽ�-������-�ٶϿ�.....
// Ŀ�ģ�����TcpClient�Ͽ����Զ�����
BOOST_AUTO_TEST_CASE(re_connect, *utf::enable_if<enable_connect>())
{
	std::cout << "�����Զ���������: ";
	std::cin >> count;

	LOG_ERROR(logger) << "BGN re_connect";
	std::cout << "BGN re_connect" << std::endl;

	auto endpoint = tcp::endpoint(address::from_string("139.224.61.179"), 8207);

	io_service ios;
	shared_ptr<io_service::work> work(new io_service::work(ios));
	auto t1 = thread([&ios]() {ios.run(); });
	TcpClient tcpClient(ios);
	if (!tcpClient.syncConnect(endpoint))
	{
		std::cout << "��������ʧ��" << std::endl;
		return;
	}

	for (int i = 0; i < count; i++)
	{
		tcpClient.stop();
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}

	tcpClient.setAutoReconnect(false);
	tcpClient.stop();
	work.reset();
	t1.join();
	std::cout << "END re_connect" << std::endl << std::endl;
	LOG_ERROR(logger) << "END re_connect";
}

// ģʽ������-�Ͽ�-������-�ٶϿ�.....
// Ŀ�ģ�����TcpClient�Ƿ�����ڴ�й©��socket���й©����
BOOST_AUTO_TEST_CASE(single_connect, *utf::enable_if<enable_connect>())
{
	std::cout << "������������: ";
	std::cin >> count;

	LOG_ERROR(logger) << "BGN single_connect";
	std::cout << "BGN single_connect" << std::endl;
	auto endpoint = tcp::endpoint(address::from_string("139.224.61.179"), 8207);

	io_service ios;
	shared_ptr<io_service::work> work(new io_service::work(ios));
	auto t1 = thread([&ios]() {ios.run(); });

	for (int i = 0; i < count; i++)
	{
		auto tcpClient = std::make_shared<TcpClient>(ios);
		tcpClient->setAutoReconnect(false);
		tcpClient->syncConnect(endpoint);
	}

	work.reset();
	t1.join();
	std::cout << "END single_connect" << std::endl << std::endl;
	LOG_ERROR(logger) << "END single_connect";
}

// ģʽ������-����..., �Ͽ�-�Ͽ�...
// Ŀ�ģ�����TcpClient�Ƿ�����ڴ�й©��socket���й©���⣬�����ĵĴ󲢷�����֧��
BOOST_AUTO_TEST_CASE(multi_connect, *utf::enable_if<enable_connect>())
{
	std::cout << "���벢��������: ";
	std::cin >> count;

	LOG_ERROR(logger) << "BGN multi_connect";
	std::cout << "BGN multi_connect" << std::endl;
	auto endpoint = tcp::endpoint(address::from_string("139.224.61.179"), 8207);

	io_service ios;
	shared_ptr<io_service::work> work(new io_service::work(ios));
	auto t1 = thread([&ios]() {ios.run(); });
	std::set<shared_ptr<TcpClient>> tcpClientPool;

	for (int i = 0; i < count; i++)
	{
		auto tcpClient = std::make_shared<TcpClient>(ios);
		tcpClient->setAutoReconnect(false);
		tcpClient->asyncConnect(endpoint);
		tcpClientPool.insert(tcpClient);
	}

	int count = 0;
	boost::this_thread::sleep(boost::posix_time::seconds(3));
	for (auto client : tcpClientPool)
		if (client->isConnected())
			count++;
	std::cout << "�ܹ�������" << count << "������" << std::endl;

	tcpClientPool.clear();
	work.reset();
	t1.join();
	std::cout << "END multi_connect" << std::endl << std::endl;
	LOG_ERROR(logger) << "END multi_connect";
}

BOOST_AUTO_TEST_CASE(tc_end, *utf::enable_if<enable_logger>())
{
	std::cout << "TcpClient��������";
	char c;
	std::cin >> c;
}
