#include "tvShow.h"
#include "settings.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace seneca {

    TvShow::TvShow(const std::string& id, const std::string& title,
                   unsigned short year, const std::string& summary)
        : MediaItem(title, summary, year), m_id(id) { }

    TvShow* TvShow::createItem(const std::string& str) {
        if (str.empty() || str[0] == '#')
            throw "Not a valid show.";

        std::stringstream ss(str);
        std::string id, title, yearStr, summary;
        std::getline(ss, id, ',');
        std::getline(ss, title, ',');
        std::getline(ss, yearStr, ',');
        std::getline(ss, summary);

        trim(id);
        trim(title);
        trim(yearStr);
        trim(summary);

        // Remove quotes from id and title
        if (!id.empty() && id.front() == '"' && id.back() == '"')
            id = id.substr(1, id.size() - 2);
        

        unsigned short year = static_cast<unsigned short>(std::stoi(yearStr));
        return new TvShow(id, title, year, summary);
    }

    void TvShow::display(std::ostream& out) const {
        if (g_settings.m_tableView) {
            out << "S | ";
            out << std::left << std::setfill('.');
            out << std::setw(50) << this->getTitle() << " | ";
            out << std::right << std::setfill(' ');
            out << std::setw(2) << m_episodes.size() << " | ";
            out << std::setw(4) << this->getYear() << " | ";
            out << std::left;
            if (g_settings.m_maxSummaryWidth > -1) {
                std::string printedSummary = this->getSummary();
                if (!printedSummary.empty() && printedSummary.front() == '"' &&
                    printedSummary.back() == '"') {
                    printedSummary = printedSummary.substr(1, printedSummary.size() - 2);
                }
                if ((short)printedSummary.size() > g_settings.m_maxSummaryWidth) {
                    size_t trimLen = g_settings.m_maxSummaryWidth - 3;
                    printedSummary = printedSummary.substr(0, trimLen);

                    size_t lastSpace = printedSummary.find_last_of(' ');
                    if (lastSpace != std::string::npos && lastSpace > 0)
                        printedSummary = printedSummary.substr(0, lastSpace + 1);  // include the space


                   
                    printedSummary += "...";
                } else {
                    // Ensure the summary is trimmed properly
                    while (!printedSummary.empty() && printedSummary.back() == ' ')
                        printedSummary.pop_back();

                }
                if ((short)this->getSummary().size() > g_settings.m_maxSummaryWidth) {
                    out << "\"" << printedSummary; // NO closing quote
                } else {
                    out << "\"" << printedSummary << "\""; // Include closing quote
                }

            } else {
                std::string printedSummary = this->getSummary();
                if (!printedSummary.empty() && printedSummary.front() == '"' &&
                    printedSummary.back() == '"') {
                    printedSummary = printedSummary.substr(1, printedSummary.size() - 2);
                }
                out << "\"" << printedSummary << "\"";            }


            out << std::endl;
        } else {
            size_t pos = 0;
            out << this->getTitle() << " [" << this->getYear() << "]\n";
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << "" << '\n';
            while (pos < this->getSummary().size()) {
                out << "    " << this->getSummary().substr(pos, g_settings.m_maxSummaryWidth) << '\n';
                pos += g_settings.m_maxSummaryWidth;
            }
            for (const auto& ep : m_episodes) {
                out << std::setfill('0') << std::right;
                out << "    S" << std::setw(2) << ep.m_season
                    << "E" << std::setw(2) << ep.m_numberInSeason << " ";
                if (!ep.m_title.empty())
                    out << ep.m_title << '\n';
                else
                    out << "Episode " << ep.m_numberOverall << '\n';

                pos = 0;
                while (pos < ep.m_summary.size()) {
                    out << "            " << ep.m_summary.substr(pos, g_settings.m_maxSummaryWidth - 8) << '\n';
                    pos += g_settings.m_maxSummaryWidth - 8;
                }
            }
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << ""
                << std::setfill(' ') << '\n';
        }
    }

    double TvShow::getEpisodeAverageLength() const {
        if (m_episodes.empty()) return 0.0;

        unsigned total = 0;
        std::for_each(m_episodes.begin(), m_episodes.end(),
            [&total](const TvEpisode& ep) {
                total += ep.m_length;
            });
        return static_cast<double>(total) / m_episodes.size();
    }

    std::list<std::string> TvShow::getLongEpisodes() const {
        std::list<std::string> result;
        std::for_each(m_episodes.begin(), m_episodes.end(),
            [&result](const TvEpisode& ep) {
                if (ep.m_length >= 3600)
                    result.push_back(ep.m_title);
            });
        return result;
    }

}
