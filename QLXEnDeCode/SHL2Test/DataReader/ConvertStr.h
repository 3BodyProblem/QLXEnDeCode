
#ifndef _CONVERT_STRING_H
#define _CONVERT_STRING_H

#include <vector>
using namespace std;

extern bool MBToUTF8(vector<char>& pu8, const char* pmb, int mLen) ;
extern bool UTF8ToMB(vector<char>& pmb, const char* pu8, int utf8Len);

extern bool MBToUnicode(vector<wchar_t>& pun, const char* pmb, int mLen);
extern bool UnicodeToMB(vector<char>& pmb, const wchar_t* pun, int uLen)  ;

extern bool UTF8ToUnicode(vector<wchar_t>& pun, const char* pu8, int utf8Len);
extern bool UnicodeToUTF8(vector<char>& pu8, const wchar_t* pun, int uLen) ;



#endif