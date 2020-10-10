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
#include <vector>

typedef std::pair<uint32_t, std::string> loggerTagPair_t;
typedef std::vector<loggerTagPair_t> loggerTags_t;

#define ASPLIB_DEFAULT_LOGGER_FILE  "asplib.log"

typedef enum ASPLIB_DEFAULT_LOG_TAGS
{
  ASPLIB_MIN_LOG_TAG = -1,
  ASPLIB_DEBUG_LOG,
  ASPLIB_INFO_LOG,
  ASPLIB_WARNING_LOG,
  ASPLIB_ERROR_LOG,
  ASPLIB_MAX_LOG_TAG
};

typedef enum ASPLIB_LOGGER_TYPES
{
  ASPLIB_INVALID_LOGGER_TYPE = -1,
  ASPLIB_FILE_LOGGER,
  ASPLIB_STDOUT_LOGGER,
  ASPLIB_MAX_LOGGER_TYPE
};

extern const char *defaultLoggerTags[];
