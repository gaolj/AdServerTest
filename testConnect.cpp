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

static int count = 1;
static src::severity_channel_logger<SeverityLevel> logger((keywords::channel = "net"));
static auto endpoint = tcp::endpoint(address::from_string("139.224.61.179"), 8207);

// ģʽ������-���Ͽ�-�Զ�������-���Ͽ�-�Զ�������......
// Ŀ�ģ�����TcpClient�Ͽ����Զ���������
BOOST_AUTO_TEST_CASE(auto_reconnect, *utf::enable_if<enable_connect>())
{
	std::cout << "�����Զ���������: ";
	std::cin >> count;

	LOG_INFO(logger) << "BGN auto_reconnect";

	io_service ios;
	shared_ptr<io_service::work> work(new io_service::work(ios));
	auto t1 = thread([&ios]() {ios.run(); });

	TcpClient tcpClient(ios);
	if (!tcpClient.syncConnect(endpoint))
	{
		LOG_INFO(logger) << "��������ʧ��";
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
	LOG_INFO(logger) << "END auto_reconnect" << "\n";
}

// ģʽ������-�Ͽ�-����-�Ͽ�.....
// Ŀ�ģ�����TcpClient�Ƿ�����ڴ�й©��socket���й©����
BOOST_AUTO_TEST_CASE(sync_connect, *utf::enable_if<enable_connect>())
{
	std::cout << "����ͬ�����Ӵ���: ";
	std::cin >> count;

	LOG_INFO(logger) << "BGN sync_connect";

	io_service ios;
	shared_ptr<io_service::work> work(new io_service::work(ios));
	auto t1 = thread([&ios]() {ios.run(); });

	int ok = 0;
	for (int i = 0; i < count; i++)
	{
		auto tcpClient = std::make_shared<TcpClient>(ios);
		tcpClient->setAutoReconnect(false);
		if (tcpClient->syncConnect(endpoint))
			ok++;
	}
	LOG_INFO(logger) << "�ɹ�����" << ok << "��";

	work.reset();
	t1.join();
	LOG_INFO(logger) << "END sync_connect" << "\n";
}

// ģʽ������-����..., �Ͽ�-�Ͽ�...
// Ŀ�ģ�����TcpClient�Ƿ�����ڴ�й©��socket���й©���⣬�����ĵĴ󲢷�����֧��
BOOST_AUTO_TEST_CASE(multi_connect, *utf::enable_if<enable_connect>())
{
	std::cout << "���벢��������: ";
	std::cin >> count;

	LOG_INFO(logger) << "BGN multi_connect";

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
	LOG_INFO(logger) << "ͬʱ������" << count << "������";

	tcpClientPool.clear();
	work.reset();
	t1.join();
	LOG_INFO(logger) << "END multi_connect" << "\n";
}

BOOST_AUTO_TEST_CASE(tc_end_connect, *utf::enable_if<enable_connect>())
{
	LOG_INFO(logger) << "����������س���������һ�����";
	char c;
	std::cin >> c;
}
