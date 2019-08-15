#include "strings.h"

namespace resolve
{
std::vector<std::string> Split(const std::string& source, const std::string& tag)
{
    std::vector<std::string> result;
    std::string s = source;
    while (1)
    {
        auto p = s.find(tag);
        if (p == std::string::npos)
        {
            result.push_back(s);
            break;
        }
        else
        {
            result.push_back(s.substr(0, p));
            s = s.substr(p + tag.size());
        }
    }
    return result;
}
}
