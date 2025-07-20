#ifndef SENECA_TVSHOW_H
#define SENECA_TVSHOW_H

#include "mediaItem.h"
#include <string>
#include <sstream> 
#include <vector>
#include <list>

namespace seneca {

    struct TvEpisode {
        const class TvShow* m_show{};
        unsigned short m_numberOverall{};
        unsigned short m_season{};
        unsigned short m_numberInSeason{};
        std::string m_airDate{};
        unsigned int m_length{};
        std::string m_title{};
        std::string m_summary{};
    };

    class TvShow : public MediaItem {
        std::string m_id{};
        std::vector<TvEpisode> m_episodes{};

        TvShow(const std::string& id, const std::string& title,
               unsigned short year, const std::string& summary);

    public:
        void display(std::ostream& out) const override;
        static TvShow* createItem(const std::string& str);

        template<typename Collection_t>
        static void addEpisode(Collection_t& col, const std::string& str);

        double getEpisodeAverageLength() const;
        std::list<std::string> getLongEpisodes() const;
    };

    template<typename Collection_t>
    void TvShow::addEpisode(Collection_t& col, const std::string& str) {
        if (str.empty() || str[0] == '#')
            throw "Not a valid episode.";

        std::stringstream ss(str);
        std::string id, numOverallStr, seasonStr, inSeasonStr, airDate, lengthStr, title, summary;

        std::getline(ss, id, ',');
        std::getline(ss, numOverallStr, ',');
        std::getline(ss, seasonStr, ',');
        std::getline(ss, inSeasonStr, ',');
        std::getline(ss, airDate, ',');
        std::getline(ss, lengthStr, ',');
        std::getline(ss, title, ',');
        std::getline(ss, summary);

        trim(id); trim(numOverallStr); trim(seasonStr); trim(inSeasonStr);
        trim(airDate); trim(lengthStr); trim(title); trim(summary);

        if (seasonStr.empty()) seasonStr = "1";

        // Remove quotes
        if (!id.empty() && id.front() == '"' && id.back() == '"')
            id = id.substr(1, id.size() - 2);
        if (!title.empty() && title.front() == '"' && title.back() == '"')
            title = title.substr(1, title.size() - 2);
        if (!summary.empty() && summary.front() == '"' && summary.back() == '"')
            summary = summary.substr(1, summary.size() - 2);

        for (size_t i = 0; i < col.size(); ++i) {
            auto* ptr = dynamic_cast<TvShow*>(col[i]);
            if (ptr && ptr->m_id == id) {
                TvEpisode ep;
                ep.m_show = ptr;
                ep.m_numberOverall = static_cast<unsigned short>(std::stoi(numOverallStr));
                ep.m_season = static_cast<unsigned short>(std::stoi(seasonStr));
                ep.m_numberInSeason = static_cast<unsigned short>(std::stoi(inSeasonStr));
                ep.m_airDate = airDate;
                ep.m_length = static_cast<unsigned int>(std::stoi(lengthStr));
                ep.m_title = title;
                ep.m_summary = summary;

                ptr->m_episodes.push_back(ep);
                return;
            }
        }
    }

}

#endif
