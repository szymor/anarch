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



#include "Core/os/asplib_os.h"

#include <string>


namespace asplib
{
template<class C>
class IException
{
public:
  IException(std::string Filename="", std::string FunctionName="", int LineNumber=-1, std::string ModuleName="")
  {
    m_Filename      = Filename;
    m_FunctionName  = FunctionName;
    m_LineNumber    = LineNumber;
    m_ModuleName    = ModuleName;
  }

  virtual ~IException() {}

  virtual C &what() = 0;

  std::string get_Filename()      { return m_Filename; }
  std::string get_FunctionName()  { return m_FunctionName; }
  int         get_LineNumber()    { return m_LineNumber; }
  std::string get_ModuleName()    { return m_ModuleName; }

protected:
  std::string m_Filename;
  std::string m_FunctionName;
  int         m_LineNumber;
  std::string m_ModuleName;
};
}
