#include <boost/test/unit_test.hpp> 
namespace utf = boost::unit_test;

#include "stdafx.h"
#include "AdManager.h"
#include "AdManagerImpl.h"
#include "TcpClient.h"

using namespace boost::asio::ip;
using boost::asio::io_service;
using std::shared_ptr;
using boost::thread;

static int count;
static src::severity_channel_logger<SeverityLevel> logger((keywords::channel = "net"));
static auto endpoint = tcp::endpoint(address::from_string("139.224.61.179"), 8207);

AdManager::AdManagerImpl impl;
BOOST_AUTO_TEST_CASE(connection, *utf::enable_if<enable_AdManager>())
{
	impl.setConfig("139.224.61.179", 8207, 123456, true, 18888, "debug");
	if (!impl._tcpClient->syncConnect(endpoint))
		LOG_INFO(logger) << "连接中心失败" << "\n";
	else
		LOG_INFO(logger) << "连接中心成功" << "\n";

	std::cout << "输入向中心请求次数: ";
	std::cin >> count;
}


// 向中心取策略
BOOST_AUTO_TEST_CASE(getAdPlayPolicy, *utf::enable_if<enable_AdManager>())
{
	LOG_INFO(logger) << "BGN getAdPlayPolicy";
	for (int i = 0; i < count; i++)
	{
		int id = htonl(123456);
		char bufId[4] = {};
		memcpy(bufId, &id, 4);

		Message msgReq;
		msgReq.set_content(bufId, 4);
		msgReq.set_method("getAdPlayPolicy");

		auto fut = impl._tcpClient->session()->request(msgReq);
		auto msgRsp = fut.get();
		if (msgRsp.returncode() == 0 && impl._policy.ParseFromString(msgRsp.content()) == true)
			LOG_INFO(logger) << "请求广告策略成功";
		else
		{
			utf8ToGB2312(msgRsp);
			LOG_ERROR(logger) << "请求广告策略失败:" << msgRsp.returnmsg() << msgRsp.returncode();
		}
	}

	LOG_INFO(logger) << "END getAdPlayPolicy" << "\n";
}

// 向中心取广告列表
BOOST_AUTO_TEST_CASE(getAdList, *utf::enable_if<enable_AdManager>())
{
	LOG_INFO(logger) << "BGN getAdList";
	for (int i = 0; i < count; i++)
	{
		Message msgReq;
		msgReq.set_method("getAdList");

		auto fut = impl._tcpClient->session()->request(msgReq);
		auto msgRsp = fut.get();
		Result ads;
		if (msgRsp.returncode() == 0 && ads.ParseFromString(msgRsp.content()) == true)
		{
			LOG_INFO(logger) << "请求广告列表成功";
			for (int i = 0; i < ads.ads_size(); i++)
			{
				auto ad = *ads.mutable_ads(i);
				utf8ToGB2312(ad);
				impl._mapAd.insert(std::make_pair(ad.id(), ad));
			}
		}
	}
	LOG_INFO(logger) << "END getAdList" << "\n";
}

// 下载广告
BOOST_AUTO_TEST_CASE(downloadAds, *utf::enable_if<enable_AdManager>())
{
	// 下载广告文件
	for (int i = 0; i < count; i++)
	{
		impl.downloadAds();
		impl._bufImages.clear();
	}

	LOG_INFO(logger) << "END downloadAds" << "\n";
}

// 向中心请求不存在的接口
//BOOST_AUTO_TEST_CASE(error_test, *utf::enable_if<enable_AdManager>())
//{
//	LOG_INFO(logger) << "BGN error_test";
//
//	Message msgReq;
//	msgReq.set_method("noSuchMethod");
//
//	// 中心没有此方法
//	auto fut = impl._tcpClient->session()->request(msgReq);
//	auto msgRsp = fut.get();
//	utf8ToGB2312(msgRsp);
//	LOG_INFO(logger) << "noSuchMethod 返回:" << msgRsp.returnmsg() << msgRsp.returncode();
//
//	// 中心解析Message成功，但没有此方法
//	msgReq.set_method("是utf8字符串");
//	gb2312ToUTF8(msgRsp);
//	fut = impl._tcpClient->session()->request(msgReq);
//	msgRsp = fut.get();
//	utf8ToGB2312(msgRsp);
//	LOG_INFO(logger) << "是utf8字符串 返回:" << msgRsp.returnmsg() << msgRsp.returncode();
//
//	// 中心解析Message会失败
//	msgReq.set_method("不是utf8字符串");
//	fut = impl._tcpClient->session()->request(msgReq);
//	msgRsp = fut.get();
//	utf8ToGB2312(msgRsp);
//	LOG_INFO(logger) << "noSuchMethod 返回:" << msgRsp.returnmsg() << msgRsp.returncode();
//
//	LOG_INFO(logger) << "END error_test" << "\n";
//}

BOOST_AUTO_TEST_CASE(tc_end_AdManager, *utf::enable_if<enable_AdManager>())
{
	LOG_INFO(logger) << "按任意键及回车，进入下一组测试";
	char c;
	std::cin >> c;
}
