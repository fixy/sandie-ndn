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

#include <iostream>
#include <stdlib.h>

#include <boost/filesystem.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "../common/xrdndn-logger.hh"
#include "../common/xrdndn-throughput.hh"
#include "xrdndn-consumer.hh"

namespace xrdndnconsumer {

uint64_t bufferSize(262144);

int copyFileOverNDN(std::string filePath) {
    Consumer consumer;
    try {
        int retOpen = consumer.Open(filePath);
        if (retOpen) {
            std::cerr << "ERROR: Unable to open file: " << filePath
                      << std::endl;
            return -1;
        }

        struct stat info;
        int retFstat = consumer.Fstat(&info, filePath);
        if (retFstat) {
            std::cerr << "ERROR: Unable to get fstat for file: " << filePath
                      << std::endl;
            return -1;
        }

        off_t offset = 0;
        std::string buff(bufferSize, '\0');

        xrdndnconsumer::ThroughputComputation pThroughputComputation;
        pThroughputComputation.start();

        int retRead = consumer.Read(&buff[0], offset, bufferSize, filePath);
        while (retRead > 0) {
            offset += retRead;
            retRead = consumer.Read(&buff[0], offset, bufferSize, filePath);
        }

        pThroughputComputation.stop();
        consumer.Close(filePath);

        pThroughputComputation.printSummary(consumer.getNoSegmentsReceived(),
                                            consumer.getDataSizeReceived(),
                                            filePath);
    } catch (const std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    return 0;
}

int runTest(std::string dirPath, std::string filePath) {
    int ret = 1;

    if (boost::filesystem::is_directory(dirPath)) {
        for (auto &entry : boost::make_iterator_range(
                 boost::filesystem::directory_iterator(dirPath), {})) {
            auto absolutePath =
                boost::filesystem::canonical(entry.path(),
                                             boost::filesystem::current_path())
                    .string();
            ret &= copyFileOverNDN(absolutePath);
        }
    }

    try {
        auto absolutePath =
            boost::filesystem::canonical(boost::filesystem::path(filePath),
                                         boost::filesystem::current_path())
                .string();
        ret &= copyFileOverNDN(absolutePath);
    } catch (const boost::filesystem::filesystem_error &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 2;
    }

    return ret;
}

static void usage(std::ostream &os, const std::string &programName,
                  const boost::program_options::options_description &desc) {
    os << "Usage: " << programName
       << " [options]\nYou must specify one of dir/file parameters. \n\n"
       << desc;
}

int main(int argc, char **argv) {
    std::string programName = argv[0];

    std::string filePath;
    std::string dirPath;

    boost::program_options::options_description description("Options");
    description.add_options()(
        "bsize,b",
        boost::program_options::value<uint64_t>(&bufferSize)
            ->default_value(bufferSize),
        "Maximum size of the read buffer. The default is 262144. Specify any "
        "value between 8KB and 1GB in bytes.")(
        "dir,d", boost::program_options::value<std::string>(&dirPath),
        "Path to a directory of files to be copied via NDN.")(
        "file,f", boost::program_options::value<std::string>(&filePath),
        "Path to the file to be copied via NDN.")(
        "help,h", "Print this help message and exit");

    boost::program_options::variables_map vm;
    try {
        boost::program_options::store(
            boost::program_options::command_line_parser(argc, argv)
                .options(description)
                .run(),
            vm);
        boost::program_options::notify(vm);
    } catch (const boost::program_options::error &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 2;
    } catch (const boost::bad_any_cast &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 2;
    }

    if (vm.count("help") > 0) {
        usage(std::cout, programName, description);
        return 0;
    }

    if ((vm.count("file") == 0) && (vm.count("dir") == 0)) {
        std::cerr << "ERROR: Specify path to a file or a directory if files to "
                     "be copied over NDN."
                  << std::endl;
        usage(std::cerr, programName, description);
        return 2;
    }

    if (bufferSize < 8192 || bufferSize > 1073741824) {
        std::cerr << "ERROR: Buffer size must be between 8KB and 1GB."
                  << std::endl;
        return 2;
    }

    return runTest(dirPath, filePath);
}
} // namespace xrdndnconsumer

int main(int argc, char **argv) { return xrdndnconsumer::main(argc, argv); }