#ifndef REPLACE_MACROS_H
#define REPLACE_MACROS_H

#include <map>
#include <string>
#include <iostream>
#include <libxml++/libxml++.h>
#include "xml_tools.h"
#include "string_tools.h"

bool replace_macros(xmlpp::Node* root_node);

#endif
