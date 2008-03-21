//                  -- url_decode.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_DETAIL_URL_DECODE_HPP_INCLUDED__
#define CGI_DETAIL_URL_DECODE_HPP_INCLUDED__

#include <string>

namespace cgi {
 namespace detail {

   /// Convert a char into a hexadecimal value
   std::string char_to_hex(char const& ch)
   {
     return std::string();      
   }

   /// Convert two characters into a single, hex-encoded character
   char hex_to_char(char const& c1, char const& c2)
   {
     int ret ( ( std::isalpha(c1)
                 ? ((c1 & 0xdf) - 'A') + 10
                 : (c1 - '0')
               ) << 4
             );

     ret += ( std::isalpha(c2)
              ? ((c2 & 0xdf) - 'A') + 10
              : (c2 - '0')
            );

     return static_cast<char>(ret);
   }

   /// Take two characters (a hex sequence) and return a char
   // **DEPRECATED**
   char url_decode( const char& c1, const char& c2 )
   {
     int ret = ( (c1 >= 'A' && c1 <= 'Z') || (c1 >= 'a' && c1 <= 'z')
                   ? ((c1 & 0xdf) - 'A') + 10
                   : (c1 - '0')
                 ) << 4;

     ret += ( (c2 >= 'A' && c2 <= 'Z') || (c2 >= 'a' && c2 <= 'z')
                ? ((c2 & 0xdf) - 'A') + 10
                : (c2 - '0')
            );

     return static_cast<char>(ret);
   }

   /// Workaround for istreams, since the above can't be used directly
   char url_decode(std::istream& is)
   {
     const char c1 = is.get();
     return url_decode(c1, is.get());
   }

   /// URL-decode a string
   std::string url_decode( const std::string& str )
   {
     std::string ret;

     for( std::string::const_iterator iter = str.begin(), end = str.end()
        ; iter != end; ++iter )
     {
       switch( *iter )
       {
         case ' ':
           break;
         case '+':
           ret.append(1, ' ');
           break;
         case '%':
           if (std::isxdigit(*(iter+1))
            && std::isxdigit(*(iter+2))
            && std::distance(iter, end) >= 2)
           {
             char ch = *++iter; // need this because order of function arg 
                                // evaluation is UB.
             ret.append(1, hex_to_char(ch, *++iter));
           }
           else // we're not dealing with a properly encoded hex value.
           {
             ret.append(1, '%');
           }
           break;
         default:
           ret.append(1, *iter);
       }
     }

     return ret;
   }

 } // namespace detail
} // namespace cgi

#endif // CGI_DETAIL_URL_DECODE_HPP_INCLUDED__
