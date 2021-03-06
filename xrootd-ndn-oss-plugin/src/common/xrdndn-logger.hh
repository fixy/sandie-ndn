/******************************************************************************
 * Named Data Networking plugin for xrootd                                    *
 * Copyright © 2018 California Institute of Technology                        *
 *                                                                            *
 * Author: Catalin Iordache <catalin.iordache@cern.ch>                        *
 *                                                                            *
 * This program is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#ifndef XRDNDN_LOGGER_HH
#define XRDNDN_LOGGER_HH

#ifdef __MACH__
#pragma GCC diagnostic ignored "-Wunneeded-internal-declaration"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#include <ndn-cxx/util/logger.hpp>
#include <ndn-cxx/util/logging.hpp>

namespace xrdndnconsumer {
/**
 * @brief Application is using ndn-log as logging module. This is the name of
 * the logging module for XRootD NDN Consumer. More information can be found at
 * https://named-data.net/doc/ndn-cxx/current/manpages/ndn-log.html
 *
 */
#define CONSUMER_LOGGER_PREFIX "xrdndnconsumer"

/**
 * @brief Initialize ndn-log module xrdndnconsumer
 *
 */
NDN_LOG_INIT(xrdndnconsumer);
} // namespace xrdndnconsumer

namespace xrdndnproducer {
/**
 * @brief Application is using ndn-log as logging module. This is the name of
 * the logging module for XRootD NDN Producer. More information can be found at
 * https://named-data.net/doc/ndn-cxx/current/manpages/ndn-log.html
 *
 */
#define PRODUCER_LOGGER_PREFIX "xrdndnproducer"

/**
 * @brief Initialize ndn-log module xrdndnproducer
 *
 */
NDN_LOG_INIT(xrdndnproducer);
} // namespace xrdndnproducer

#endif // XRDNDN_LOGGER_HH