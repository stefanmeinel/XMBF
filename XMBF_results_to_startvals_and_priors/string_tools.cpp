#include "string_tools.h"

bool is_newline(char c)
{
  return (c=='\n');
}

void remove_whitespaces(std::string& str)
{
  str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

void remove_newlines(std::string& str)
{
  str.erase(std::remove_if(str.begin(), str.end(), is_newline), str.end());
}

bool string_replace(std::string& s, const std::string& _old, const std::string& _new)
{
  bool replaced=false;
  std::string::size_type pos=s.find(_old);
  if(pos!=std::string::npos)
  {
    replaced=true;
    while( pos!=std::string::npos )
    {
      s.replace(pos, _old.length(), _new);
      pos=s.find(_old, pos+_new.length());
    }
  }
  return replaced;
}

std::string repeat_string(int n, const std::string& st)
{
  std::stringstream sst;
  for(int i=0; i<n; ++i)
  {
    sst << st;
  }
  return sst.str();
}

std::string bool_to_string(bool b)
{
  if(b)
  {
    return "true";
  }
  else
  {
    return "false";
  }
}


std::string double_to_string(double d, unsigned int prec)
{
  std::stringstream sst;
  sst.precision(prec);
  sst << d;
  return sst.str();
}

std::string int_to_string(int i)
{
  std::stringstream sst;
  sst << i;
  return sst.str();
}


bool _in(const std::string& sub, const std::string& s)
{
  if(s.find(sub)!=std::string::npos)
  {
    return true;
  }
  else
  {
    return false;
  }
}
