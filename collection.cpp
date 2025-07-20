#include "collection.h"
#include <algorithm>

namespace seneca {

    Collection::Collection(const std::string& name) : m_name(name) {}

    Collection::~Collection() {
        for (auto item : m_items)
            delete item;
    }

    const std::string& Collection::name() const {
        return m_name;
    }

    size_t Collection::size() const {
        return m_items.size();
    }

    void Collection::setObserver(void (*observer)(const Collection&, const MediaItem&)) {
        m_observer = observer;
    }

    Collection& Collection::operator+=(MediaItem* item) {
        auto it = std::find_if(m_items.begin(), m_items.end(),
            [item](MediaItem* existing) {
                return existing->getTitle() == item->getTitle();
            });

        if (it != m_items.end()) {
            delete item;
        } else {
            m_items.push_back(item);
            if (m_observer)
                m_observer(*this, *item);
        }
        return *this;
    }

    MediaItem* Collection::operator[](size_t idx) const {
        if (idx >= m_items.size()) {
            throw std::out_of_range("Bad index [" + std::to_string(idx) +
                "]. Collection has [" + std::to_string(m_items.size()) + "] items.");
        }
        return m_items[idx];
    }

    MediaItem* Collection::operator[](const std::string& title) const {
        auto it = std::find_if(m_items.begin(), m_items.end(),
            [title](MediaItem* item) {
                return item->getTitle() == title;
            });
        return (it != m_items.end()) ? *it : nullptr;
    }

    void Collection::removeQuotes() {
        std::for_each(m_items.begin(), m_items.end(), [](MediaItem* item) {
            std::string t = item->getTitle();
            std::string s = item->getSummary();

            if (!t.empty() && t.front() == '"' && t.back() == '"')
                item->setTitle(t.substr(1, t.size() - 2));

            if (!s.empty() && s.front() == '"' && s.back() == '"')
                item->setSummary(s.substr(1, s.size() - 2));
        });
    }

    void Collection::sort(const std::string& field) {
        if (field == "title") {
            std::sort(m_items.begin(), m_items.end(), [](MediaItem* a, MediaItem* b) {
                return a->getTitle() < b->getTitle();
            });
        } else if (field == "year") {
            std::sort(m_items.begin(), m_items.end(), [](MediaItem* a, MediaItem* b) {
                return a->getYear() < b->getYear();
            });
        }
    }

    std::ostream& operator<<(std::ostream& out, const Collection& col) {
        for (size_t i = 0; i < col.size(); ++i)
            out << *col[i];
        return out;
    }
}
