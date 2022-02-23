// This file is part of the SgCity project.
//
// Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <utility>

//-------------------------------------------------
// SgException
//-------------------------------------------------

namespace sg
{
    class SgException : public std::exception
    {
    public:
        SgException(int t_line, const char* t_file, std::string t_message)
            : m_line{ t_line }
            , m_file{ t_file }
            , m_message{ std::move(t_message) }
        {}

        const char* what() const noexcept override
        {
            std::ostringstream oss;

            oss << "occurred: \n";
            oss << GetOriginString();

            m_whatBuffer = oss.str();

            return m_whatBuffer.c_str();
        }

    protected:

    private:
        int m_line;
        std::string m_file;
        std::string m_message;

        mutable std::string m_whatBuffer;

        std::string GetOriginString() const
        {
            std::ostringstream oss;

            oss << "[Message]: " << m_message << "\n";
            oss << "[File]: "    << m_file << "\n";
            oss << "[Line]: "    << m_line;

            return oss.str();
        }
    };
}

#define SG_EXCEPTION(message) sg::SgException(__LINE__, __FILE__, message)
