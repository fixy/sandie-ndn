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

#ifndef XRDNDN_UTILS_HH
#define XRDNDN_UTILS_HH

#include <ndn-cxx/name.hpp>

#include "xrdndn-namespace.hh"

namespace xrdndn {
class Utils {
  public:
    /**
     * @brief Get the Name object for a prefix, a file name and a segment number
     *
     * @param prefix The prefix of the Name
     * @param path The file path
     * @param segmentNo The segment number
     * @return ndn::Name The resulting NDN Name
     */
    static ndn::Name getName(ndn::Name prefix, std::string path,
                             uint64_t segmentNo = 0) noexcept {
        return prefix == SYS_CALL_READ_PREFIX_URI
                   ? prefix.append(path).appendSegment(segmentNo)
                   : prefix.append(path);
    }

    /**
     * @brief Get the file name from an Interest Name
     *
     * @param name The Name
     * @return std::string The file name as a std::string
     */
    static std::string getPath(ndn::Name name) noexcept {
        try {
            if (name.at(-1).isSegment())
                return name.getSubName(xrdndn::SYS_CALLS_PREFIX_LEN)
                    .getPrefix(-1)
                    .toUri();
        } catch (ndn::Name::Error &error) {
        }
        return name.getSubName(xrdndn::SYS_CALLS_PREFIX_LEN).toUri();
    }

    /**
     * @brief Get segment number from Data or Interest
     *
     * @tparam Packet Data/Interest
     * @param packet The Name or Interest
     * @return uint64_t The segment number
     */
    template <typename Packet>
    static uint64_t getSegmentNo(const Packet &packet) noexcept {
        ndn::Name name = packet.getName();
        try {
            if (name.at(-1).isSegment())
                return name.at(-1).toSegment();
            else if (name.at(-1).isVersion())
                return name.at(-2).toSegment();
        } catch (ndn::Name::Error &error) {
        }

        return 0;
    }
};
} // namespace xrdndn

#endif // XRDNDN_UTILS_HH