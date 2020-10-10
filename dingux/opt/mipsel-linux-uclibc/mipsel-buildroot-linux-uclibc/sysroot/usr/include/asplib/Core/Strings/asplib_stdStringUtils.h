#pragma once

/*
 * Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
 * https://github.com/AchimTuran/asplib
 *
 * This file is part of asplib (Achim's Signal Processing LIBrary)
 *
 * asplib (Achim's Signal Processing LIBrary) is free software:
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * asplib (Achim's Signal Processing LIBrary) is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with asplib (Achim's Signal Processing LIBrary).
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



#include <string>
#include <sstream>
#include <list>
#include <vector>

template<typename T>
std::string toString(T Val)
{
  std::ostringstream oss;
  oss << Val;
  std::string str(oss.str());
  
  return str;
}

template<typename T>
T stringToVal(std::string Str)
{
  T val;
  std::istringstream(Str) >> val;
  return val;
}

inline unsigned int strTokenizer(std::string Str, std::string Delimiter, std::vector<std::string> &StrTokens)
{
  unsigned int lastPos = Str.find_first_not_of(Delimiter, 0);
  unsigned int pos = Str.find(Delimiter, lastPos);

  while(std::string::npos != pos || std::string::npos != lastPos)
  {
    StrTokens.push_back(Str.substr(lastPos, pos - lastPos));
    lastPos = Str.find_first_not_of(Delimiter, pos);
    pos = Str.find_first_of(Delimiter.c_str(), lastPos);
  }

  return StrTokens.size();
}
