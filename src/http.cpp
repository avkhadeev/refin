/*
 * http.cpp
 *
 *  Created on: 15.09.2011
 *      Author: root
 */

#include "http.h"
#include "mongoose/mongoose.h"
#include <tr1/memory>
#include <string>

namespace refin {

using namespace std;

using namespace std::tr1;



//-----------------------------------------------------------------------------------------------------



string HttpRequest::GetPost( const string & name )
{
	if (!m_contentLength) return "";
	char *content = new char [m_contentLength];
	mg_read(m_conn, content, m_contentLength);

	char *value = new char [m_contentLength];

	int varLength = mg_get_var( content, m_contentLength, name.c_str(), value, m_contentLength );
	string result;
	if (varLength > 0)
	{
		result.append(value, value+varLength);
	}

	delete [] content;
	delete [] value;

	return result;
}

void HttpRequest::WriteData (const string &data)
{
	mg_printf(m_conn, "%s", data.c_str());
}

HttpRequest::HttpRequest ( mg_connection *conn, const mg_request_info *info )
: m_contentLength (0)
, m_conn  (conn)
, m_info (info)
{
	//Fill headers
	for (int i = 0; i < info->num_headers; ++ i)
	{
		m_headers[info->http_headers[i].name] = info->http_headers[i].value;
	}

	if (m_headers.find("Content-Length") != m_headers.end())
	{
		m_contentLength = atol (m_headers["Content-Length"].c_str());
	}

}

HttpServer::HttpServer (  RequestCallBack callback, const string & listenPort, const string & documentRoot )
: m_callback (callback)
{
	const char *options[] = {
			"document_root", documentRoot.c_str(),
			"listening_ports", listenPort.c_str(),
			NULL
	};

	m_ctx = mg_start(&event_handler, this, options);
}

void * HttpServer::event_handler (mg_event event, mg_connection *conn, const mg_request_info *info)
{
	HttpRequest request (conn, info);

	((HttpServer *) info->user_data )->m_callback (request);
	return (void*) "yes";
}
HttpServer::~HttpServer()
{
	mg_stop(m_ctx);
}

} /* namespace refin */
