#ifndef VIRGIL_UTILS_QUUID_UNORDERED_MAP_SPEC_H_INCLUDED
#define VIRGIL_UTILS_QUUID_UNORDERED_MAP_SPEC_H_INCLUDED

#include <QUuid>
#include <algorithm>
#include <string>

namespace std {

template <> struct hash<QUuid> {
    size_t
    operator()(const QUuid &k) const {
        return std::hash<std::string>()(k.toString().toStdString());
    }
};

} // namespace std


#endif /* VIRGIL_UTILS_QUUID_UNORDERED_MAP_SPEC_H_INCLUDED */
