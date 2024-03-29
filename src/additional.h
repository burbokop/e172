// Copyright 2023 Borys Boiko

#pragma once

#include "utility/option.h"
#include <algorithm>
#include <functional>
#include <limits>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace e172 {

struct Additional {
#ifdef __WIN32__
    static constexpr char separator = '\\';
    static inline const std::string separatorString = "\\";
#else
    static constexpr char separator = '/';
    static inline const std::string separatorString = "/";
#endif

    static std::string stripMargins(const std::string &str, char prefix = '|');

    static std::string constrainPath(const std::string &path);
    static std::vector<std::string> split(const std::string &s, char delimiter);
    static std::pair<std::string, std::string> splitIntoPair(const std::string &s, char delimiter);
    static std::string trim(const std::string& str, char symbol = ' ');
    static std::string trim(const std::string& str, const std::vector<char> &symbols);
    static std::string removeSymbols(const std::string &string, const std::vector<char> &symbols);
    static std::string absolutePath(const std::string &path, const std::string &exe_path);
    static std::string homeDirectory();

    enum Fence { Brackets, Parentheses, CurlyBraces };

    static std::string fencedArea(const std::string &string, Fence fence);

    static std::string fencedArea(const std::string &string,
                                  size_t beginIndex = 0,
                                  size_t *nextIndexPtr = nullptr);

    static std::string jsonTopLevelField(const std::string &string,
                                         size_t beginIndex = 0,
                                         size_t *nextIndexPtr = nullptr);

    static std::vector<std::string> jsonTopLevelFields(const std::string &string);

    static std::string jsonRemoveSymbols(const std::string &string,
                                         const std::vector<char> &symbols);

    static std::vector<std::string> fencedAreas(const std::string &string);

    static char reversedFence(char symbol);

    template<typename T>
    static typename T::mapped_type value(
        const T &map,
        const typename T::key_type &key,
        const typename T::mapped_type &defaultValue = typename T::mapped_type())
    {
        const auto it = map.find(key);
        if (it == map.end()) {
            return defaultValue;
        } else {
            return it->second;
        }
    }

    static std::string readFile(const std::string &path);
    static bool writeFile(const std::string &path, const std::string &content);
    static std::vector<std::string> directoryContent(std::string path);
    static bool isDirectory(std::string path);
    static std::string fileSufix(std::string string);
    static std::string concatenatePaths(std::string prefix, std::string string);
    static std::string cutPath(std::string string, unsigned level);
    static std::string pathTopLevelItem(const std::string &string);

    static size_t countOfFiles(std::string path, std::string suffix);

    static std::map<std::string, std::string> readAllVof(const std::string &path,
                                                         char delimiter = '=');

    static std::string readVof(const std::string &path, const std::string &id, char delimiter = '=');

    static void writeVof(const std::string &path,
                         const std::string &id,
                         const std::string &value,
                         char delimiter = '=');

    static std::vector<std::string> executeCommand(const std::string &command);

    static void parseForder(std::string path, const std::function<void(const std::string&)> &callback);

    template<typename T>
    static std::string toFixedSizeString(const T &value)
    {
        const auto string = std::to_string(value);
        return string + std::string(std::to_string(std::numeric_limits<T>::max()).size() - string.size(), ' ');
    }

    static std::string defaultFontDirectory();
    static std::string defaultFont(const std::string &suffix = ".ttf");


    static Option<double> parseRadians(const std::string& string);

    template<typename T>
    static T filter(const T &container, const std::function<bool(const typename T::value_type &)> cb)
    {
        T result;
        std::copy_if(container.begin(), container.end(), std::back_inserter(result), cb);
        return result;
    }

    static std::string sameBeginningSubstring(const std::list<std::string> &list);
    static std::string compleateString(const std::string& string, const std::list<std::string> &model);
};

} // namespace e172
