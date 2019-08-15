#include "resolve.h"
#include "strings.h"
#include <cstdio>

namespace resolve
{
std::string GetRequestType(const std::string& source)
{
    if (source.find("HTTP/") != std::string::npos)
        return "HTTP";
    return "";
}

std::string GetHTTPRequestType(const std::string& source)
{
    auto head = Split(source, "\r\n\r\n")[0];
    auto lines = Split(head, "\r\n");
    for (auto l : lines)
    {
        auto units = Split(l, " ");
        if (units.size() >= 3)
            if (units[2].substr(0, 4) == "HTTP")
                return units[0];
    }
    return "";
}

std::string GetHTTPRequestDir(const std::string& source)
{
    auto head = Split(source, "\r\n\r\n")[0];
    auto lines = Split(head, "\r\n");
    for (auto l : lines)
    {
        auto units = Split(l, " ");
        if (units.size() >= 3)
            if (units[2].substr(0, 4) == "HTTP")
                return units[1];
    }
    return "";
}

std::map<std::string, std::string> GetHTTPRequestPost(const std::string& source)
{
    std::map<std::string, std::string> result;
    auto line = Split(source, "\r\n\r\n")[1];
    auto pairs = Split(line, "&");
    for (auto p : pairs)
    {
        auto kv = Split(p, "=");
        if (kv.size() > 1)
            result[kv[0]] = kv[1];
    }
    return result;
}
}
