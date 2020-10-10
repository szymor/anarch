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
#include "Core/Constants_Typedefs/asplib_Typedefs.h"

#include "Logger/Logger_types.h"

#include <string>
#include <stdarg.h>

namespace asplib
{
class ILogger
{
public:
  ILogger(loggerTags_t &LoggerTags);
  virtual ~ILogger();

  ASPLIB_ERR Log(const uint32_t TagID, const std::string Message, ...);
  std::string get_LoggerTagStr(const uint32_t TagID);

protected:
  virtual ASPLIB_ERR Open() = 0;
  virtual ASPLIB_ERR Close() = 0;
  virtual ASPLIB_ERR LogWrite(const uint32_t TagID, const std::string DateStr, std::string Message, va_list VarArgs) = 0;

  loggerTags_t          m_LoggerTags;
  std::vector<int32_t>  m_LoggerTagIDTable;
};
}
