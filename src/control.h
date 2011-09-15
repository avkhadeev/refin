/*
 * control.h
 *
 *  Created on: 15.09.2011
 *      Author: root
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <string>
#include <tr1/memory>
#include <ostream>
#include <map>
namespace refin {

class Control
{
public:
	typedef std::tr1::shared_ptr<Control> Ptr;
	virtual std::string Name() const;

	void AddChild (Control::Ptr child);
	Control (Ptr parent, const std::string & name);

	virtual void OnWriteBefore (std::ostream & outStream);
	virtual void OnWriteAfter (std::ostream & outStream);

	std::ostream & Write (std::ostream & outStream);
	virtual ~Control();

private:
	std::string m_name;
	Ptr m_parent;
	std::map <std::string, Ptr> m_childs;
};

class Window : public Control
{
public:
	Window (Control::Ptr parent, const std::string & name, const std::string & header, int left, int right, int top, int bottom);

	void OnWriteBefore (std::ostream & out);
	void OnWriteAfter (std::ostream & out);

private:
	int m_left;
	int m_right;
	int m_top;
	int m_bottom;
	std::string m_header;
};

class Label : public Control
{
public:
	Label (Control::Ptr parent, const std::string & name, const std::string & label, int left, int top );

	void OnWriteBefore (std::ostream & out);

private:
	int m_left;
	int m_top;
	std::string m_label;
};


} /* namespace refin */
#endif /* CONTROL_H_ */
