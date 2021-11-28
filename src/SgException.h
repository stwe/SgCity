#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <utility>

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
