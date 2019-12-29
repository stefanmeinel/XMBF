#include "replace_macros.h"

// recursive replacing of macros in content nodes
void replace_macros_recursive(xmlpp::Node* node, std::map<std::string,std::string>& macros)
{
  xmlpp::ContentNode* nodeContent = dynamic_cast<xmlpp::ContentNode*>(node);
  if(nodeContent)
  {
    std::string content=nodeContent->get_content();
    remove_whitespaces(content);
    remove_newlines(content);
    if(macros.find(content)!=macros.end())
    {
      nodeContent->set_content(macros[content]);
    }
    return;
  }
  xmlpp::Element* nodeElement = dynamic_cast<xmlpp::Element*>(node);
  if(nodeElement)
  {
    std::string nodename = node->get_name();
    if(nodename!="macros")
    {
      //recurse through child nodes
      xmlpp::Node::NodeList list = node->get_children();
      for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
      {
        replace_macros_recursive(*iter, macros);
      }
    }
  }
}



bool replace_macros(xmlpp::Node* root_node)
{
  // read macros
  std::map<std::string, std::string> macros;
  xmlpp::NodeSet all_macros_nodeset = root_node->find("macros");
  if(all_macros_nodeset.size()>1)
  {
    std::cerr << "Error: \"macros\" did not return unique node" << std::endl << std::endl;
    return false;
  }
  if(all_macros_nodeset.size()==1)
  {
    xmlpp::NodeSet macros_nodeset=(*all_macros_nodeset.begin())->find("macro");
    for(xmlpp::NodeSet::iterator it=macros_nodeset.begin(); it!=macros_nodeset.end(); ++it)
    {
      std::string name;
      try { name=get_string(*it, "name"); } catch (int e) { return false; }
      std::string value;
      try { value=get_string(*it, "value"); } catch (int e) { return false; }
      if(macros.find(name)!=macros.end())
      {
        std::cerr << "Error: multiple definitions for macro \"" << name << "\"" << std::endl << std::endl;
        return false;
      }
      macros[name]=value;
    }
    // replace macros
    replace_macros_recursive(root_node, macros);
  }
  return true;
}
