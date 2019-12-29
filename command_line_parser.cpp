#include "command_line_parser.h"


command_line_parser::command_line_parser(int start, int stop, char *argv[])
{
  for(int i=start; i<=stop; ++i)
  {
    args.push_back(argv[i]);
  }
}


bool command_line_parser::exists(std::string flag)
{
  if(std::find(args.begin(), args.end(), "-"+flag)!=args.end())
  {
    return true;
  }
  return false;
}


std::string command_line_parser::get_value(std::string flag)
{
  std::vector< std::string >::iterator pos=std::find(args.begin(), args.end(), "-"+flag);
  if(args.end()-pos<2)
  {
    throw 1;
    return "";
  }
  ++pos;
  if((*pos)[0]=='-')
  {
    throw 1;
    return "";
  }
  return *pos;
}
