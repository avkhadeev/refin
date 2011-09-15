/*
 * main.cpp
 *
 *  Created on: 15.09.2011
 *      Author: roman@ispsystem.com
 */

#include <iostream>
#include <stdio.h>
#include "mongoose/mongoose.h"
#include "tr1/memory"
#include <iostream>
#include <map>

#include <stdlib.h>

using namespace std;

using namespace std::tr1;

class Control
{
public:
	typedef shared_ptr<Control> Ptr;

	virtual string Name() const
	{
		return m_name;
	}

	void AddChild (Control::Ptr child)
	{
		m_childs[child->Name()] = child;
	}


	Control (Ptr parent, const string & name)
		: m_name (name)
		, m_parent (parent)
	{
		parent->AddChild (Ptr(this));
	}

	virtual void OnWriteBefore (ostream & outStream) {}
	virtual void OnWriteAfter (ostream & outStream) {}

	ostream & Write (ostream & outStream)
	{
		OnWriteBefore( outStream );
		for (map <string, Ptr>::iterator i = m_childs.begin(); i != m_childs.end(); ++i )
		{
			i->second->Write (outStream);
		}
		OnWriteAfter (outStream);
		return outStream;
	}
	virtual ~Control() {}

private:
	string m_name;
	Ptr m_parent;
	map <string, Ptr> m_childs;
};

class Window : public Control
{
public:
	Window (Control::Ptr parent, const string & name, const string & header, int left, int right, int top, int bottom)
		: Control (parent, name)
		, m_left (left)
		, m_right (right)
		, m_top (top)
		, m_bottom (bottom)
		, m_header (header)
	{

	}

	void OnWriteBefore (ostream & out)
	{
		out << "<window name=\"" << Name() << "\" header=\"" << m_header << "\" left=\"" << m_left << "\" right=\"" << m_right << "\" top=\"" << m_top << "\" bottom=\"" << m_bottom << "\">";
	}
	void OnWriteAfter (ostream & out)
	{
		out << "</window>";
	}

private:
	int m_left;
	int m_right;
	int m_top;
	int m_bottom;
	string m_header;
};

class Label : public Control
{
public:
	Label (Control::Ptr parent, const string & name, const string & label, int left, int top )
		: Control (parent, name)
		, m_left (left)
		, m_top (top)
		, m_label (label)
	{

	}

	void OnWriteBefore (ostream & out)
	{
		out << "<label name=\"" << Name() << "\" left=\"" << m_left << "\" top=\"" << m_top << "\">" << m_label << "</label>";
	}

private:
	int m_left;
	int m_top;
	string m_label;
};

//-----------------------------------------------------------------------------------------------------


class HttpRequest
{
public:
	string GetPost( const string & name )
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
	friend class HttpServer;
private:
	HttpRequest ( mg_connection *conn, const mg_request_info *info )
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
private:
	map <string, string> m_headers;
	size_t m_contentLength;
	mg_connection m_conn;
	mg_request_info m_info;
};

class HttpServer
{
public:

	typedef void (*RequestCallBack) (HttpRequest &);
	HttpServer (  RequestCallBack callback, const string & listenPort = "8080", const string & documentRoot = "." )
		: m_callback (callback)
	{
		const char *options[] = {
				"document_root", documentRoot.c_str(),
				"listening_ports", listenPort.c_str(),
				NULL
		};

		m_ctx = mg_start(&event_handler, NULL, options);
	}

private:
	static void * event_handler (mg_event event, mg_connection *conn, const mg_request_info *info)
	{
		HttpRequest request;
		m_callback (request);
		return (void*) "yes";
	}
private:
	virtual ~HttpServer()
	{
		mg_stop(m_ctx);
	}

	mg_context * m_ctx;
	RequestCallBack m_callback;

};

string GetValue  ( const char * data, int dataLen, const string & name )
{
	char *buf = new char[dataLen];
	int varLength = mg_get_var( data, dataLen, name.c_str(), buf, dataLen );
	string result;
	if (varLength > 0)
	{
		result.append(buf, buf+varLength);
	}
	return result;
}

static void *event_handler (mg_event event, mg_connection *conn, const mg_request_info *info)
{
	if (event == MG_NEW_REQUEST)
	{
		string inp1Value;
		string inp2Value;
		string content_length ;

		const char * cl =  mg_get_header(conn, "Content-Length") ;
		if (cl)
		{
			content_length = cl;
		}

		int length = 0;
		if (!content_length.empty())
		{
			length = atoi (content_length.c_str());
		}

		if (length)
		{
			char *content = new char [length];
			mg_read(conn, content, length);

			inp1Value = GetValue(content, length, "input1");
			inp2Value = GetValue(content, length, "input2");

		}

		mg_printf(conn, "HTTP/1.1 200 OK\r\n"
				"Cache: no-cache\r\n"
				"Content-Type: text/html\r\n"
				"\r\n"
				"<form action='action' method='post' name='form1'>"
				"<input name='input1'/>"
				"<input name='input2'/>"
				"<input type='submit'/>"
				"</form>");


		mg_printf(conn, "input1='%s', input2='%s'", inp1Value.c_str(), inp2Value.c_str());
	}
	return (void *)"yes";
}


int main()
{
	static const char *options[] = {
			"document_root", ".",
			"listening_ports", "8081",
			NULL
	};

	shared_ptr<mg_context> context(mg_start(&event_handler, NULL, options), mg_stop);

	getchar();

	return 0;
}
