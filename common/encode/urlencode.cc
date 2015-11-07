#include <string.h>
#include <ctype.h>
#include <string>
#include "common/encode/urlencode.h"
static unsigned char XXX_toHex(const unsigned char &x)
{
  return x > 9 ? x -10 + 'A': x + '0';
}

static unsigned char fromHex(const unsigned char &x)
{
  return isdigit(x) ? x-'0' : x-'A'+10;
}

std::string URLEncode(const char *input)
{
  std::string sOut;
  for( size_t ix = 0; ix < strlen(input); ix++ )
  {
    unsigned char buf[4];
    memset( buf, 0, 4 );
    if( isalnum( (unsigned char)input[ix] ) )
    {
      buf[0] = input[ix];
    }
    else
    {
      buf[0] = '%';
      buf[1] = XXX_toHex( (unsigned char)input[ix] >> 4 );
      buf[2] = XXX_toHex( (unsigned char)input[ix] % 16);
    }
    sOut += (char *)buf;
  }
  return sOut;
};

std::string URLDecode(const char *input)
{
  std::string sOut;
  for( size_t ix = 0; ix < strlen(input); ix++ )
  {
    unsigned char ch = 0;
    if(input[ix]=='%')
    {
      ch = (fromHex(input[ix+1])<<4);
      ch |= fromHex(input[ix+2]);
      ix += 2;
    }
    else if(input[ix] == '+')
    {
      ch = ' ';
    }
    else
    {
      ch = input[ix];
    }
    sOut += (char)ch;
  }

  return sOut;

}

