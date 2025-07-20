#include "book.h"
#include "settings.h"
#include <sstream>
#include <iomanip>

namespace seneca {

    Book::Book(const std::string& author, const std::string& title,
               const std::string& country, double price,
               unsigned short year, const std::string& summary)
        : MediaItem(title, summary, year),
          m_author(author), m_country(country), m_price(price) { }

    void Book::display(std::ostream& out) const {
        if (g_settings.m_tableView) {
            out << "B | ";
            out << std::left << std::setfill('.');
            out << std::setw(50) << this->getTitle() << " | ";
            out << std::right << std::setfill(' ');
            out << std::setw(2) << this->m_country << " | ";
            out << std::setw(4) << this->getYear() << " | ";
            out << std::left;
            if (g_settings.m_maxSummaryWidth > -1) {
                if ((short)this->getSummary().size() <= g_settings.m_maxSummaryWidth)
                    out << this->getSummary();
                else
                    out << this->getSummary().substr(0, g_settings.m_maxSummaryWidth - 3) << "...";
            } else {
                out << this->getSummary();
            }
            out << std::endl;
        } else {
            size_t pos = 0;
            out << this->getTitle() << " [" << this->getYear() << "] ["
                << m_author << "] [" << m_country << "] [" << m_price << "]\n";
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << "" << '\n';
            while (pos < this->getSummary().size()) {
                out << "    " << this->getSummary().substr(pos, g_settings.m_maxSummaryWidth) << '\n';
                pos += g_settings.m_maxSummaryWidth;
            }
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << ""
                << std::setfill(' ') << '\n';
        }
    }

    Book* Book::createItem(const std::string& strBook) {
        if (strBook.empty() || strBook[0] == '#')
            throw "Not a valid book.";

        std::stringstream ss(strBook);
        std::string author, title, country, priceStr, yearStr, summary;

        std::getline(ss, author, ',');
        std::getline(ss, title, ',');
        std::getline(ss, country, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, yearStr, ',');
        std::getline(ss, summary);

        trim(author);
        trim(title);
        trim(country);
        trim(priceStr);
        trim(yearStr);
        trim(summary);

        double price = std::stod(priceStr);
        unsigned short year = static_cast<unsigned short>(std::stoi(yearStr));

        return new Book(author, title, country, price, year, summary);
    }

}
