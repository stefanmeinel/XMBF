#ifndef XML_TOOLS_H
#define XML_TOOLS_H

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <libxml++/libxml++.h>
#include <boost/lexical_cast.hpp>
#include "string_tools.h"

xmlpp::Node::NodeList keep_text_nodes(const xmlpp::Node::NodeList& list);

bool is_element(const xmlpp::Node* node);

int get_int(xmlpp::Node* node, const std::string& name);

int get_positive_int(xmlpp::Node* node, const std::string& name);

int get_non_negative_int(xmlpp::Node* node, const std::string& name);

double get_double(xmlpp::Node* node, const std::string& name);

double get_positive_double(xmlpp::Node* node, const std::string& name);

std::string get_string(xmlpp::Node* node, const std::string& name);

bool get_bool(xmlpp::Node* node, const std::string& name);

bool name_exists(xmlpp::Node* node, const std::string& name);

xmlpp::Node* get_node(xmlpp::NodeSet node_set, const std::string& property_name, const std::string& property_value);

xmlpp::Node* get_node_two_properties(xmlpp::NodeSet node_set, const std::string& property_name1, const std::string& property_value1, const std::string& property_name2, const std::string& property_value2);

void set_double(xmlpp::Node* node, const std::string& name, double value, const std::string& indentation, bool last);

void set_string(xmlpp::Node* node, const std::string& name, const std::string& value, const std::string& indentation, bool last);

template<class T>
void addchild(xmlpp::Element* elem, const std::string& name, T content)
{
  xmlpp::Element* child_elem = elem -> add_child(name);
  child_elem->set_child_text(boost::lexical_cast<std::string>(content));
}

#endif
