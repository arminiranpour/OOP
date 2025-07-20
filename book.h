#ifndef SENECA_BOOK_H
#define SENECA_BOOK_H

#include "mediaItem.h"
#include <string>

namespace seneca {
    class Book : public MediaItem {
        std::string m_author{};
        std::string m_country{};
        double m_price{};

        // private constructor
        Book(const std::string& author, const std::string& title,
             const std::string& country, double price,
             unsigned short year, const std::string& summary);

    public:
        void display(std::ostream& out) const override;
        static Book* createItem(const std::string& strBook);
    };
}

#endif
