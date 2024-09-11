#ifndef SENECA_UTILITIES_H
#define SENECA_UTILITIES_H

namespace seneca
{
    /* --- Suports the parsing of input files --- */
    class Utilities
    {
        size_t m_widthField = 1; // Length of token
        static char m_delimiter; // Token separator

    public:
        void setFieldWidth(size_t newWidth);
        size_t getFieldWidth() const;
        std::string extractToken(const std::string &str, size_t &next_pos, bool &more);

        // Class functions
        static void setDelimiter(char newDelimiter);
        static char getDelimiter();

        void trim(std::string &str);
    };
} // seneca

#endif