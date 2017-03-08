#include <boost/test/unit_test.hpp> 
namespace utf = boost::unit_test;

#include "stdafx.h"
#include "AdManager.h"
#include "AdManagerImpl.h"
#include "TcpClient.h"
#include "md5.hh"
#include <boost/algorithm/string.hpp>	// boost::to_upper_copy

using namespace boost::asio::ip;
using boost::asio::io_service;
using std::shared_ptr;
using boost::thread;

static int count;
static src::severity_channel_logger<SeverityLevel> logger((keywords::channel = "net"));
static auto locolhost = tcp::endpoint(address::from_string("127.0.0.1"), 18888);

// 测试向中心取策略、广告列表、下载广告的完整业务
BOOST_AUTO_TEST_CASE(AdServerTest, *utf::enable_if<enable_AdServer>())
{
	LOG_INFO(logger) << "BGN AdServerTest";

	AdManager& adManager = AdManager::getInstance();
	adManager.setConfig("139.224.61.179", 8207, 123456, true, 18888);
	adManager.bgnBusiness();

	LOG_INFO(logger) << "END AdServerTest" << "\n";
}

BOOST_AUTO_TEST_CASE(tc_end_AdServer, *utf::enable_if<enable_AdServer>())
{
	LOG_INFO(logger) << "按任意键及回车，进入下一组测试";
	char c;
	std::cin >> c;
}

