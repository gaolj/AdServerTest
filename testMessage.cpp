#include <boost/test/unit_test.hpp> 
namespace utf = boost::unit_test;

#include "stdafx.h"
#include "Message.pb.h"
#include "AdPlayPolicy.pb.h"
#include "AdManagerImpl.h"

BOOST_AUTO_TEST_CASE(tc_message_test1, *utf::enable_if<enable_message>())
{
	std::cout << "BGN message test" << std::endl;

	Message msg;
	BOOST_CHECK_EQUAL(msg.id(), 0);
	BOOST_CHECK_EQUAL(msg.method(), "");
	BOOST_CHECK_EQUAL(msg.returncode(), 0);
	BOOST_CHECK_EQUAL(msg.returnmsg(), "");

	msg.set_id(1);
	msg.set_method("method");
	msg.set_returncode(1);
	msg.set_returnmsg("returnmsg");
	BOOST_CHECK_EQUAL(msg.id(), 1);
	BOOST_CHECK_EQUAL(msg.method(), "method");
	BOOST_CHECK_EQUAL(msg.returncode(), 1);
	BOOST_CHECK_EQUAL(msg.returnmsg(), "returnmsg");

	std::string str;
	bool b = msg.SerializeToString(&str);
	BOOST_CHECK_EQUAL(b, true);

	Message msg1;
	b = msg1.ParseFromString(str);
	BOOST_CHECK_EQUAL(b, true);
	BOOST_CHECK_EQUAL(msg1.ByteSize(), msg.ByteSize());
	BOOST_CHECK_EQUAL(msg1.id(), msg.id());
	BOOST_CHECK_EQUAL(msg1.method(), msg.method());
	BOOST_CHECK_EQUAL(msg1.returncode(), msg.returncode());
	BOOST_CHECK_EQUAL(msg1.returnmsg(), msg.returnmsg());

	std::cout << "END message test" << std::endl << std::endl;
}

BOOST_AUTO_TEST_CASE(tc_message_test2, *utf::enable_if<enable_message>())
{
	std::cout << "BGN message 中文" << std::endl;

	Message msg;
	msg.set_id(1);
	msg.set_method("method");
	msg.set_returncode(1);
	msg.set_returnmsg("结果");
	BOOST_CHECK_EQUAL(msg.id(), 1);
	BOOST_CHECK_EQUAL(msg.method(), "method");
	BOOST_CHECK_EQUAL(msg.returncode(), 1);
	BOOST_CHECK_EQUAL(msg.returnmsg(), "结果");

	std::string str;
	bool b = msg.SerializeToString(&str);
	BOOST_CHECK_EQUAL(b, true);

	Message msg1;
	b = msg1.ParseFromString(str);
	BOOST_CHECK_EQUAL(b, false);	// 虽然由于returnmsg不是utf8编码导致ParseFromString返回false, 但msg1还是成功Parse的
	BOOST_CHECK_EQUAL(msg1.ByteSize(), msg.ByteSize());
	BOOST_CHECK_EQUAL(msg1.id(), msg.id());
	BOOST_CHECK_EQUAL(msg1.method(), msg.method());
	BOOST_CHECK_EQUAL(msg1.returncode(), msg.returncode());
	BOOST_CHECK_EQUAL(msg1.returnmsg(), msg.returnmsg());	// returnmsg为ansi编码

	std::cout << "END message 中文" << std::endl << std::endl;
}

BOOST_AUTO_TEST_CASE(tc_end_message, *utf::enable_if<enable_message>())
{
	std::cout << "按任意键及回车，进入下一组测试";
	char c;
	std::cin >> c;

	google::protobuf::ShutdownProtobufLibrary();
}
