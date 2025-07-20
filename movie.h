#ifndef SENECA_MOVIE_H
#define SENECA_MOVIE_H

#include "mediaItem.h"
#include <string>

namespace seneca {
    class Movie : public MediaItem {
        // private constructor
        Movie(const std::string& title, unsigned short year, const std::string& summary);

    public:
        void display(std::ostream& out) const override;
        static Movie* createItem(const std::string& strMovie);
    };
}

#endif
