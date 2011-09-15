/*
 * control.cpp
 *
 *  Created on: 15.09.2011
 *      Author: root
 */

#include "control.h"
#include <string>
#include <tr1/memory>
#include <map>

using namespace std;
using namespace std::tr1;


namespace refin {



string Control::Name() const
{
	return m_name;
}

void Control::AddChild (Control::Ptr child)
{
	m_childs[child->Name()] = child;
}


Control::Control (Ptr parent, const string & name)
: m_name (name)
, m_parent (parent)
{
	parent->AddChild (Ptr(this));
}

void Control::OnWriteBefore (ostream & outStream) {}
void Control::OnWriteAfter (ostream & outStream) {}

ostream & Control::Write (ostream & outStream)
{
	OnWriteBefore( outStream );
	for (map <string, Ptr>::iterator i = m_childs.begin(); i != m_childs.end(); ++i )
	{
		i->second->Write (outStream);
	}
	OnWriteAfter (outStream);
	return outStream;
}
Control::~Control() {}


/**
 * 	Window
 */



Window::Window (Control::Ptr parent, const string & name, const string & header, int left, int right, int top, int bottom)
: Control (parent, name)
, m_left (left)
, m_right (right)
, m_top (top)
, m_bottom (bottom)
, m_header (header)
{

}

void Window::OnWriteBefore (ostream & out)
{
	out << "<window name=\"" << Name() << "\" header=\"" << m_header << "\" left=\"" << m_left << "\" right=\"" << m_right << "\" top=\"" << m_top << "\" bottom=\"" << m_bottom << "\">";
}
void Window::OnWriteAfter (ostream & out)
{
	out << "</window>";
}


/**
 * class Label
 */

Label::Label (Control::Ptr parent, const string & name, const string & label, int left, int top )
: Control (parent, name)
, m_left (left)
, m_top (top)
, m_label (label)
{

}

void Label::OnWriteBefore (ostream & out)
{
	out << "<label name=\"" << Name() << "\" left=\"" << m_left << "\" top=\"" << m_top << "\">" << m_label << "</label>";
}


} /* namespace refin */
