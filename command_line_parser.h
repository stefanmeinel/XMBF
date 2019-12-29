#ifndef COMMAND_LINE_PARSER
#define COMMAND_LINE_PARSER

#include <vector>
#include <algorithm>
#include <string>

class command_line_parser
{
  public:
     command_line_parser(int start, int stop, char *argv[]);

     bool exists(std::string flag);

     std::string get_value(std::string flag);

  private:

    std::vector< std::string > args;
};

#endif
