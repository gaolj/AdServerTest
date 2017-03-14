// AdServerTest.cpp : 定义控制台应用程序的入口点。
//

#define BOOST_TEST_MODULE MainTest
#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

#include "stdafx.h"
#include "Logger.h"

BOOST_AUTO_TEST_CASE(tc_init_logger)
{
	initLogger(debug);
}

