// Copyright 2025 Trossen Robotics
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef LIBTROSSEN_ARM__TROSSEN_ARM_LOGGING_HPP_
#define LIBTROSSEN_ARM__TROSSEN_ARM_LOGGING_HPP_

#include <cstdarg>
#include <cstdio>
#include <string>

// define logging macros
#define TALOG_DEBUG(...) trossen_arm::logging::log( \
    trossen_arm::logging::Level::DEBUG, \
    __VA_ARGS__)
#define TALOG_INFO(...) trossen_arm::logging::log( \
    trossen_arm::logging::Level::INFO, \
    __VA_ARGS__)
#define TALOG_WARN(...) trossen_arm::logging::log( \
    trossen_arm::logging::Level::WARN, \
    __VA_ARGS__)
#define TALOG_ERROR(...) trossen_arm::logging::log( \
    trossen_arm::logging::Level::ERROR, \
    __VA_ARGS__)
#define TALOG_FATAL(...) trossen_arm::logging::log( \
    trossen_arm::logging::Level::FATAL, \
    __VA_ARGS__)

namespace trossen_arm
{

namespace logging
{

enum Level
{
  DEBUG = 0,
  INFO = 1,
  WARN = 2,
  ERROR = 3,
  FATAL = 4
};

// The logging level - anything higher than this level will be logged
static Level _level = Level::INFO;

/**
 * @brief Log a message
 * @param level The level with which to log the message
 * @param fmt The formatted message
 */
void log(Level level, const char * fmt, ...);

/**
 * @brief Set the logging level
 * @param level the new logging level
 */
void set_level(Level level);

/**
 * @brief Set the logging level
 * @param level the new logging level
 */
void set_level(std::string level);

/**
 * @brief Get the logging level
 * @returns The current logging level
 */
Level get_level();

}  // namespace logging

}  // namespace trossen_arm


#endif  // LIBTROSSEN_ARM__TROSSEN_ARM_LOGGING_HPP_
