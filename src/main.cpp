/*
 * main.cpp
 *
 *  Created on: 15.09.2011
 *      Author: roman@ispsystem.com
 */

#include <string>
#include <fstream>
#include "http.h"


using namespace std;

void ServerCallback ( refin::HttpRequest & req )
{

	string html_page = "/root/src/refin/html-template/index.html";

	req.WriteData("HELLO, WORLD");
}

int main()
{

	refin::HttpServer server (ServerCallback);

	getchar();

	return 0;
}
