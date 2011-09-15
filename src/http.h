/*
 * http.h
 *
 *  Created on: 15.09.2011
 *      Author: root
 */

#ifndef HTTP_H_
#define HTTP_H_
#include <stdlib.h>
#include "mongoose/mongoose.h"
#include <tr1/memory>
#include <map>
#include <string>
namespace refin {

using namespace std;

using namespace std::tr1;


//-----------------------------------------------------------------------------------------------------


class HttpRequest
{
public:
	string GetPost( const string & name );
	void WriteData (const string &data);

	friend class HttpServer;
private:
	HttpRequest ( mg_connection *conn, const mg_request_info *info );
	map <string, string> m_headers;
	size_t m_contentLength;
	mg_connection *m_conn;
	const mg_request_info *m_info;
};

class HttpServer
{
public:

	typedef void (*RequestCallBack) (HttpRequest &);
	HttpServer (  RequestCallBack callback, const string & listenPort = "8080", const string & documentRoot = "." );

private:
	static void * event_handler (mg_event event, mg_connection *conn, const mg_request_info *info);
public:
	virtual ~HttpServer();
private:
	mg_context * m_ctx;
	RequestCallBack m_callback;

};
} /* namespace refin */
#endif /* HTTP_H_ */
