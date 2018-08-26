/**************************************************************************
 * Named Data Networking plugin for xrootd                                *
 * Copyright © 2018 California Institute of Technology                    *
 *                                                                        *
 * Author: Catalin Iordache <catalin.iordache@cern.ch>                    *
 *                                                                        *
 * This program is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU General Public License as published by   *
 * the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                    *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. *
 **************************************************************************/

#ifndef XRDNDN_CONSUMER_HH
#define XRDNDN_CONSUMER_HH

#include <map>
#include <ndn-cxx/face.hpp>

#include "xrdndn-directory-file-handler.hh"

namespace xrdndn {
class Consumer : DFHandler {
  public:
    Consumer();
    ~Consumer();

    virtual int Open(std::string path) override;
    virtual int Close(std::string path) override;
    virtual ssize_t Read(void *buff, off_t offset, size_t blen,
                         std::string path) override;

  private:
    ndn::Face m_face;

    void onNack(const ndn::Interest &interest, const ndn::lp::Nack &nack);
    void onTimeout(const ndn::Interest &interest);

    int getIntegerFromData(const ndn::Data &data);
};

class FileReader {
  public:
    FileReader(ndn::Face &face);
    ~FileReader();

    ssize_t Read(void *buff, off_t offset, size_t blen, std::string path);

  private:
    ndn::Face &m_face;

    std::map<uint64_t, std::shared_ptr<const ndn::Data>> m_bufferedData;
    uint64_t m_nextToCopy;
    off_t m_buffOffset;

    void onNack(const ndn::Interest &interest, const ndn::lp::Nack &nack);
    void onTimeout(const ndn::Interest &interest);

    void saveDataInOrder(void *buff, off_t offset, size_t blen);
};
} // namespace xrdndn

#endif // XRDNDN_CONSUMER_HH