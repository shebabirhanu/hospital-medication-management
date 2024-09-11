#include <iomanip>
#include <cstddef>
#include <string>
#include <algorithm>
#include "Utilities.h"

namespace seneca
{
    char Utilities::m_delimiter;

    void Utilities::setDelimiter(char newDelimiter)
    {
        m_delimiter = newDelimiter;
    }
    char Utilities::getDelimiter()
    {
        return m_delimiter;
    }
    void Utilities::setFieldWidth(size_t newWidth)
    {
        m_widthField = newWidth;
    }
    size_t Utilities::getFieldWidth() const
    {
        return m_widthField;
    }

    /* Extract token from string */
    std::string Utilities::extractToken(const std::string &str, size_t &next_pos, bool &more)
    {
        std::string token = ""; // Store extracted token
        size_t first_pos = next_pos; // Store current position as token's starting position
        next_pos = str.find(m_delimiter, first_pos); // Find the position of the next delimiter in the string

        if (first_pos == next_pos) 
        {
            more = false;
            throw std::logic_error("Delimiter found at next_pos. No token.");
        }
        else if (next_pos == std::string::npos) // end of string reached
        {
            token = str.substr(first_pos); // Extract remaining token
            next_pos = str.length();       // Update next_pos to end of the string
            more = false;                  // No more tokens to extract
        }
        else // Token found
        {
            token = str.substr(first_pos, next_pos - first_pos); // Extract token
            next_pos++;                                          // Make next_pos character after delimiter
            more = true;                                         // Indicates more tokens to extract
        }

        trim(token); // Remove trailing and leading whitespaces

        if (m_widthField < token.length())
        {
            m_widthField = token.length();
        }

        return token;
    }
    
    // Trim whitespace characters from the beginning and end of a string
    void Utilities::trim(std::string &str)
    {
        // Trim from the beginning (left)
        str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), [](unsigned char ch)
                                                { return std::isspace(ch); }));

        // Trim from the end (right)
        str.erase(std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch)
                                   { return std::isspace(ch); })
                      .base(),
                  str.end());
    }
}