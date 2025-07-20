#include "spellChecker.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace seneca {

    SpellChecker::SpellChecker(const char* filename) {
        std::ifstream file(filename);
        if (!file)
            throw "Bad file name!";

        std::string line;
        size_t count = 0;

        while (std::getline(file, line) && count < 6) {
            std::istringstream iss(line);
            iss >> m_badWords[count] >> m_goodWords[count];
            ++count;
        }
    }

    void SpellChecker::operator()(std::string& text) {
        for (size_t i = 0; i < 6; ++i) {
            size_t pos = 0;
            while ((pos = text.find(m_badWords[i], pos)) != std::string::npos) {
                text.replace(pos, m_badWords[i].length(), m_goodWords[i]);
                ++m_replacements[i];
                pos += m_goodWords[i].length();
            }
        }
    }

    void SpellChecker::showStatistics(std::ostream& out) const {
        out << "Spellchecker Statistics\n";
        for (size_t i = 0; i < 6; ++i) {
            out << std::left << std::setw(15) << m_badWords[i]
                << ": " << m_replacements[i] << " replacements\n";
        }
    }

}
