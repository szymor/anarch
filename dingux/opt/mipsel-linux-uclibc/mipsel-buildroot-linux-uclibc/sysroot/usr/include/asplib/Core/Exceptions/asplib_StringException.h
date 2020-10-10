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

#include "Core/Exceptions/asplib_IException.h"

#include <string>
#include <iostream>


namespace asplib
{
class CStringException : public IException<std::string>
{
public:
  CStringException(std::string ExceptionMessage, std::string Filename="", std::string FunctionName="", int LineNumber=-1, std::string ModuleName="");
  ~CStringException();

  virtual std::string &what();

private:
  std::string m_ExceptionMessage;
};
}

// exception macro with error message
#define ASPLIB_STRING_EXCEPTION_HANDLER(ExceptionMessage) asplib::CStringException(ExceptionMessage, __ASPLIB_FILE__, __ASPLIB_FUNCTION__, __ASPLIB_LINE__)
// exception macro with error message
#define ASPLIB_STRING_EXCEPTION_MODULE_HANDLER(ExceptionMessage, ModuleStr) asplib::CStringException(ExceptionMessage, __ASPLIB_FILE__, __ASPLIB_FUNCTION__, __ASPLIB_LINE__, ModuleStr)
// exception macro that prints the whole error message to stdout
#define ASPLIB_EXCEPTION_TO_STDOUT(Exception) { std::string	Filename      = Exception.get_Filename();                   \
                                                std::string	FunctionName  = Exception.get_FunctionName();               \
                                                std::string	ModuleName    = Exception.get_ModuleName();                 \
                                                int         LineNumber    = Exception.get_LineNumber();                 \
                                                                                                                        \
                                                                                                                        \
                                                std::cout << std::endl;                                                 \
                                                std::cout << "-------------- Exception --------------" << std::endl;    \
                                                std::cout << " filename: " << Filename << std::endl;                    \
                                                std::cout << " function name: " << FunctionName << std::endl;           \
                                                std::cout << " line number: " << LineNumber << std::endl;               \
                                                if(Module != "" || !Module.empty())                                     \
                                                {                                                                       \
                                                  std::cout << " module name: " << ModuleName << std::endl;             \
                                                }                                                                       \
                                                std::cout << " exception message: " << Exception.what() << std::endl;   \
                                                std::cout << "---------------------------------------" << std::endl;    \
                                                std::cout << std::endl;	}
