#ifndef CATA_UTILS_H_
#define CATA_UTILS_H_

#include <vector>
#include <string>

class Utils {
public:
    static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch) {
        strs.clear();

        int pos = txt.find(ch);
        int initialPos = 0;
        
        while (pos != std::string::npos) {
            strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
            initialPos = pos + 1;

            pos = txt.find(ch, initialPos);
        }
        strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1))));

        return strs.size();
    }
};

#endif // CATA_UTILS_H_
