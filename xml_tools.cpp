#include "xml_tools.h"


xmlpp::Node::NodeList keep_text_nodes(const xmlpp::Node::NodeList& list)
{
  xmlpp::Node::NodeList text_node_list;
  for(xmlpp::Node::NodeList::const_iterator it = list.begin(); it != list.end(); ++it)
  {
    if(dynamic_cast<const xmlpp::TextNode*>(*it))
    {
      text_node_list.push_back(*it);
    }
  }
  return text_node_list;
}


bool is_element(const xmlpp::Node* node)
{
  return dynamic_cast<const xmlpp::Element*>(node);
}


int get_int(xmlpp::Node* node, const std::string& name)
{
  xmlpp::NodeSet set = node->find(name);
  if(set.size()!=1)
  {
    std::cerr << " Error: \"" << node->get_path() << "/" << name << "\" did not return unique node" << std::endl << std::endl;
    throw 1;
    return 0;
  }
  xmlpp::Node::NodeList list = keep_text_nodes((*set.begin())->get_children());
  if(list.size()!=1)
  {
    std::cerr << " Error: \"" << (*set.begin())->get_path() << "\" did not return integer value" << std::endl << std::endl;
    throw 1;
    return 0;
  }
  const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*list.begin());
  std::string text=nodeText->get_content();
  remove_whitespaces(text);
  int i;
  try
  {
    i=boost::lexical_cast<int>(text);
  }
  catch (boost::bad_lexical_cast &)
  {
    std::cerr << " Error: \"" << (*set.begin())->get_path() << "\" did not return integer value" << std::endl << std::endl;
    throw 1;
    return 0;
  }
  return i;
}


int get_positive_int(xmlpp::Node* node, const std::string& name)
{
  int i;
  try
  {
    i=get_int(node, name);
  }
  catch (int e)
  {
    throw 1;
    return 0;
  }
  if(!(i>0))
  {
    std::cerr << " Error: \"" << node->get_path() << "/" << name << "\" did not return positive integer" << std::endl << std::endl;
    throw 1;
    return 0;
  }
  return i;
}


int get_non_negative_int(xmlpp::Node* node, const std::string& name)
{
  int i;
  try
  {
    i=get_int(node, name);
  }
  catch (int e)
  {
    throw 1;
    return 0;
  }
  if(i<0)
  {
    std::cerr << " Error: \"" << node->get_path() << "/" << name << "\" did not return non-negative integer" << std::endl << std::endl;
    throw 1;
    return 0;
  }
  return i;
}


double get_double(xmlpp::Node* node, const std::string& name)
{
  xmlpp::NodeSet set = node->find(name);
  if(set.size()!=1)
  {
    std::cerr << " Error: \"" << node->get_path() << "/" << name << "\" did not return unique node" << std::endl << std::endl;
    throw 1;
    return 0.0;
  }
  xmlpp::Node::NodeList list = keep_text_nodes((*set.begin())->get_children());
  if(list.size()!=1)
  {
    std::cerr << " Error: \"" << (*set.begin())->get_path() << "\" did not return floating point value" << std::endl << std::endl;
    throw 1;
    return 0.0;
  }
  const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*list.begin());
  std::string text=nodeText->get_content();
  remove_whitespaces(text);
  double d;
  try
  {
    d=boost::lexical_cast<double>(text);
  }
  catch (boost::bad_lexical_cast &)
  {
    std::cerr << " Error: \"" << (*set.begin())->get_path() << "\" did not return floating point value" << std::endl << std::endl;
    throw 1;
    return 0.0;
  }
  return d;
}

double get_positive_double(xmlpp::Node* node, const std::string& name)
{
  double d;
  try
  {
    d=get_double(node, name);
  }
  catch (int e)
  {
    throw 1;
    return 0;
  }
  if(!(d>0.0))
  {
    std::cerr << " Error: \"" << node->get_path() << "/" << name << "\" did not return positive floating point value" << std::endl << std::endl;
    throw 1;
    return 0;
  }
  return d;
}


std::string get_string(xmlpp::Node* node, const std::string& name)
{
  xmlpp::NodeSet set = node->find(name);
  if(set.size()!=1)
  {
    std::cerr << " Error: \"" << node->get_path() << "/" << name << "\" did not return unique node" << std::endl << std::endl;
    throw 1;
    return "";
  }
  xmlpp::Node::NodeList list = keep_text_nodes((*set.begin())->get_children());
  if(list.size()!=1)
  {
    std::cerr << " Error: \"" << (*set.begin())->get_path() << "\" did not return string value" << std::endl << std::endl;
    throw 1;
    return "";
  }
  const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*list.begin());
  std::string text=nodeText->get_content();
  remove_whitespaces(text);
  if(text.empty())
  {
    std::cerr << " Error: \"" << (*set.begin())->get_path() << "\" did not return string value" << std::endl << std::endl;
    throw 1;
    return "";
  }
  return text;
}


bool get_bool(xmlpp::Node* node, const std::string& name)
{
  xmlpp::NodeSet set = node->find(name);
  if(set.size()!=1)
  {
    std::cerr << " Error: \"" << node->get_path() << "/" << name << "\" did not return unique node" << std::endl << std::endl;
    throw 1;
    return "";
  }
  xmlpp::Node::NodeList list = keep_text_nodes((*set.begin())->get_children());
  if(list.size()!=1)
  {
    std::cerr << " Error: \"" << (*set.begin())->get_path() << "\" did not return boolean value (true/false)" << std::endl << std::endl;
    throw 1;
    return "";
  }
  const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(*list.begin());
  std::string text=nodeText->get_content();
  remove_whitespaces(text);
  if(text=="true")
  {
    return true;
  }
  else if(text=="false")
  {
    return false;
  }
  else
  {
    std::cerr << " Error: \"" << (*set.begin())->get_path() << "\" did not return boolean value (true/false)" << std::endl << std::endl;
    throw 1;
    return false;
  }
}


bool name_exists(xmlpp::Node* node, const std::string& name)
{
  xmlpp::NodeSet set = node->find(name);
  if(set.size()>0)
  {
    return true;
  }
  else
  {
    return false;
  }
}


xmlpp::Node* get_node(xmlpp::NodeSet node_set, const std::string& property_name, const std::string& property_value)
{
  std::vector<xmlpp::Node*> matching_nodes;
  for(xmlpp::NodeSet::iterator node_set_it=node_set.begin(); node_set_it!=node_set.end(); ++node_set_it)
  {
    std::string value; try { value=get_string(*node_set_it, property_name); } catch (int e) { goto next; }
    if(value==property_value)
    {
      matching_nodes.push_back(*node_set_it);
    }
    next:;
  }
  if(matching_nodes.size()!=1)
  {
    throw 1;
    return NULL;
  }
  else
  {
    return matching_nodes[0];
  }
}

xmlpp::Node* get_node_two_properties(xmlpp::NodeSet node_set, const std::string& property_name1, const std::string& property_value1, const std::string& property_name2, const std::string& property_value2)
{
  std::vector<xmlpp::Node*> matching_nodes;
  for(xmlpp::NodeSet::iterator node_set_it=node_set.begin(); node_set_it!=node_set.end(); ++node_set_it)
  {
    std::string value1;
    std::string value2;
    try { value1=get_string(*node_set_it, property_name1); } catch (int e) { goto next; }
    try { value2=get_string(*node_set_it, property_name2); } catch (int e) { goto next; }
    if( (value1==property_value1) && (value2==property_value2) )
    {
      matching_nodes.push_back(*node_set_it);
    }
    next:;
  }
  if(matching_nodes.size()!=1)
  {
    throw 1;
    return NULL;
  }
  else
  {
    return matching_nodes[0];
  }
}



void set_double(xmlpp::Node* node, const std::string& name, double value, const std::string& indentation, bool last)
{
  xmlpp::NodeSet set = node->find(name);
  if(set.size()==0)
  {
    xmlpp::Element* child_node = node->add_child(name);
    child_node->set_child_text(boost::lexical_cast<std::string>(value));
    // add indentation
    xmlpp::Node* previous = child_node->get_previous_sibling();
    xmlpp::TextNode* previous_tn = dynamic_cast<xmlpp::TextNode*>(previous);
    if(previous_tn && (previous_tn->is_white_space()))
    {
      previous_tn->set_content("\n  "+indentation);
    }
    xmlpp::Element* element = dynamic_cast<xmlpp::Element*>(node);
    element -> add_child_text("\n");
    if(last)
    {
      element -> add_child_text(indentation);
    }
    return;
  }
  else if (set.size()==1)
  {
    xmlpp::Node::NodeList list = keep_text_nodes((*set.begin())->get_children());
    if(list.size()==0)
    {
      xmlpp::Element* element = dynamic_cast<xmlpp::Element*>(*set.begin());
      element->add_child_text(boost::lexical_cast<std::string>(value));
    }
    else if(list.size()==1)
    {
      xmlpp::TextNode* nodeText = dynamic_cast<xmlpp::TextNode*>(*list.begin());
      nodeText->set_content(boost::lexical_cast<std::string>(value));
    }
    else
    {
      node->remove_child(*(set.begin()));
      xmlpp::Element* child_node = node->add_child(name);
      child_node->set_child_text(boost::lexical_cast<std::string>(value));
    }
    return;
  }
  else
  {
    std::cerr << " Error: \"" << node->get_path() << "/" << name << "\" did not return unique node" << std::endl << std::endl;
    throw 1;
    return;
  }
}



void set_string(xmlpp::Node* node, const std::string& name, const std::string& value, const std::string& indentation, bool last)
{
  xmlpp::NodeSet set = node->find(name);
  if(set.size()==0)
  {
    xmlpp::Element* child_node = node->add_child(name);
    child_node->set_child_text(value);
    // add indentation
    xmlpp::Node* previous = child_node->get_previous_sibling();
    xmlpp::TextNode* previous_tn = dynamic_cast<xmlpp::TextNode*>(previous);
    if(previous_tn && (previous_tn->is_white_space()))
    {
      previous_tn->set_content("\n  "+indentation);
    }
    xmlpp::Element* element = dynamic_cast<xmlpp::Element*>(node);
    element -> add_child_text("\n");
    if(last)
    {
      element -> add_child_text(indentation);
    }
    return;
  }
  else if (set.size()==1)
  {
    xmlpp::Node::NodeList list = keep_text_nodes((*set.begin())->get_children());
    if(list.size()==0)
    {
      xmlpp::Element* element = dynamic_cast<xmlpp::Element*>(*set.begin());
      element->add_child_text(value);
    }
    else if(list.size()==1)
    {
      xmlpp::TextNode* nodeText = dynamic_cast<xmlpp::TextNode*>(*list.begin());
      nodeText->set_content(value);
    }
    else
    {
      node->remove_child(*(set.begin()));
      xmlpp::Element* child_node = node->add_child(name);
      child_node->set_child_text(value);
    }
    return;
  }
  else
  {
    std::cerr << " Error: \"" << node->get_path() << "/" << name << "\" did not return unique node" << std::endl << std::endl;
    throw 1;
    return;
  }
}
