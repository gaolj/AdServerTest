// AdServerTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AdManager.h"


int main()
{
	AdManager& adManager = AdManager::getInstance();
	adManager.setConfig("139.224.61.179", 8207, 1, true, 18888);
	adManager.bgnBusiness();
	
	int i;
	std::cin >> i;
	
	return 0;
}

