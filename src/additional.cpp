#include "additional.h"
#include "src/todo.h"

#include <codecvt>
#include <list>
#include <sstream>

#include <string.h>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include <memory>
#include <filesystem>
#include <src/math/math.h>
#include <regex>
#include <filesystem>
#include <sys/types.h>

#ifdef __unix__
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#else
#include <shlobj.h>
#endif

namespace e172 {
namespace {
#if defined(_MSC_FULL_VER) && !defined(__INTEL_COMPILER)
std::string ucs2ToUtf8(const std::wstring &wstr)
{
    if(wstr.empty()) return std::string();
    const auto size = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), result.data(), size, NULL, NULL);
    return result;
}
#endif
}
}

std::string e172::Additional::constrainPath(const std::string &path) {
    if(path.size() <= 0)
        return "";

    auto results = split(path, separator);

    std::list<std::string> stack;
    for(auto&& r : results) {
        if(r == "..") {
            stack.pop_back();
        } else if(r == ".") {

        } else if(r == "") {

        } else {
            stack.push_back(r);
        }
    }

    std::string result = (path[0] == separator) ? separatorString : "";
    int i = 0;
    for(auto&& s : stack) {
        result += ((i == 0) ? "" : separatorString) + s;
        i++;
    }
    return result;
}

std::vector<std::string> e172::Additional::split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::pair<std::string, std::string> e172::Additional::splitIntoPair(const std::string &s, char delimiter) {
    const auto index = s.find_first_of(delimiter);
    if(index >= 0 && index < s.size())
        return { s.substr(0, index), s.substr(index + 1, s.size() - index) };

    return {};
}

std::string e172::Additional::trim(const std::string &str, char symbol) {
    size_t first = str.find_first_not_of(symbol);
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(symbol);
    return str.substr(first, (last - first + 1));
}

std::string e172::Additional::trim(const std::string &str, const std::vector<char> &symbols) {
    std::string result = str;
    for(auto s : symbols) {
        result = trim(result, s);
    }
    return result;
}

std::string e172::Additional::removeSymbols(const std::string &string, const std::vector<char> &symbols) {
    std::string result;
    result.reserve(string.size());
    for(auto c : string) {
        if(std::find(symbols.begin(), symbols.end(), c) == symbols.end()) {
            result.push_back(c);
        }
    }
    return result;
}

std::string e172::Additional::absolutePath(const std::string &path, const std::string &exe_path) {        
    if(path.size() <= 0) return "";

    if(path[0] == '~') {
        return constrainPath(homeDirectory() + separatorString + path.substr(1, path.size() - 1));
    }

    if(exe_path.size() <= 0) return "";

#ifdef __WIN32__
    if(path.size() > 2 && path[1] == ':' && path[2] == separator) return path;
#else
    if(path[0] == separator) return path;
#endif

    return constrainPath(exe_path + separatorString + ".." + separatorString + path);
}

std::string e172::Additional::homeDirectory() {
#ifdef __unix__
    passwd *pw = getpwuid(getuid());
    if(pw) {
        return pw->pw_dir;
    }
#elif defined(_MSC_FULL_VER) && !defined(__INTEL_COMPILER)
    WCHAR profilePath[MAX_PATH];
    HRESULT result = SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, profilePath);
    if (SUCCEEDED(result)) {
        return ucs2ToUtf8(std::wstring (profilePath));
    }
#endif
    return std::string();
}

std::string e172::Additional::fencedArea(const std::string &string, e172::Additional::Fence fence) {
    char beginFence;
    char endFence;
    if(fence == Brackets) {
        beginFence = '[';
        endFence = ']';
    } else if(fence == Parentheses) {
        beginFence = '(';
        endFence = ')';
    } else if(fence == CurlyBraces) {
        beginFence = '{';
        endFence = '}';
    } else {
        return string;
    }

    size_t fenceCount = 0;
    size_t begin = 0;
    bool beginFound = false;
    for(size_t i = 0; i < string.size(); ++i) {
        if(string[i] == beginFence) {
            fenceCount++;
            if(!beginFound) {
                begin = i;
                beginFound = true;
            }
        } else if(string[i] == endFence) {
            fenceCount--;
            if(fenceCount == 0 && beginFound) {
                return string.substr(begin, i - begin + 1);
            }
        }
    }
    return string;
}

std::string e172::Additional::fencedArea(const std::string &string, size_t beginIndex, size_t *nextIndexPtr) {
    if(beginIndex >= string.size())
        return std::string();

    char endFence;
    char beginFance;

    size_t fenceCount = 0;
    size_t begin = 0;
    bool beginFound = false;
    for(size_t i = beginIndex; i < string.size(); ++i) {
        if(beginFound) {
            if(string[i] == beginFance) {
                ++fenceCount;
            } else if(string[i] == endFence) {
                if(--fenceCount == 0) {
                    if(nextIndexPtr)
                        *nextIndexPtr = i + 1;
                    return string.substr(begin, i - begin + 1);
                }
            }
        } else if(string[i] == '{' || string[i] == '[' || string[i] == '(') {
            begin = i;
            beginFound = true;
            beginFance = string[i];
            endFence = reversedFence(beginFance);
            ++fenceCount;
        }
    }
    return std::string();
}

std::string e172::Additional::jsonTopLevelField(const std::string &string, size_t beginIndex, size_t *nextIndexPtr) {
    if(beginIndex >= string.size())
        return std::string();

    char endFence;
    char beginFance;

    size_t fenceCount = 0;
    bool beginFound = false;
    for(size_t i = beginIndex; i < string.size(); ++i) {
        if(beginFound) {
            if(string[i] == beginFance) {
                ++fenceCount;
            } else if(string[i] == endFence) {
                if(--fenceCount == 0) {
                    beginFound = false;
                }
            }
        } else {
            if(string[i] == ',') {
                if(nextIndexPtr)
                    *nextIndexPtr = i + 1;
                return string.substr(beginIndex, i - beginIndex);
            } else if(string[i] == '{' || string[i] == '[' || string[i] == '(') {
                beginFound = true;
                beginFance = string[i];
                endFence = reversedFence(beginFance);
                ++fenceCount;
            }
        }
    }
    if(nextIndexPtr)
        *nextIndexPtr = string.size();
    return string.substr(beginIndex, string.size() - beginIndex);
}

std::vector<std::string> e172::Additional::fencedAreas(const std::string &string) {
    size_t begin = 0;
    std::vector<std::string> result;
    while (true) {
        const auto r = fencedArea(string, begin, &begin);
        if(r.size() > 0) {
            result.push_back(r);
        } else {
            return result;
        }
    }
}

std::vector<std::string> e172::Additional::jsonTopLevelFields(const std::string &string) {
    size_t begin = 0;
    std::vector<std::string> result;
    while (true) {
        const auto r = jsonTopLevelField(string, begin, &begin);
        if(r.size() > 0) {
            result.push_back(r);
        } else {
            return result;
        }
    }
}

std::string e172::Additional::jsonRemoveSymbols(const std::string &string, const std::vector<char> &symbols) {
    std::string result;
    result.reserve(string.size());
    size_t quotes = 0;
    for(auto c : string) {
        if(c == '"') {
            quotes++;
        }
        if(quotes % 2 == 1 || std::find(symbols.begin(), symbols.end(), c) == symbols.end()) {
            result.push_back(c);
        }
    }
    return result;
}

char e172::Additional::reversedFence(char symbol) {
    switch (symbol) {
    case '{': return '}';
    case '[': return ']';
    case '(': return ')';
    case '<': return '>';
    case '}': return '{';
    case ']': return '[';
    case ')': return '(';
    case '>': return '<';
    default: return symbol;
    }
}

std::string e172::Additional::readFile(const std::string &path) {
    std::ifstream ifile(path);
    if(ifile.is_open()) {
        std::string string((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
        ifile.close();
        return string;
    }
    return std::string();
}

bool e172::Additional::writeFile(const std::string &path, const std::string &content) {
    std::filesystem::create_directories(cutPath(path, 1));
    std::ofstream ofile(path, std::ios::out);
    if(ofile.is_open()) {
        ofile << content;
        ofile.close();
        return true;
    }
    return false;
}

std::vector<std::string> e172::Additional::directoryContent(std::string path) {
#ifdef __unix__
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != nullptr) {
        std::vector<std::string> result;
        while ((ent = readdir (dir)) != nullptr) {
            if(strcmp(ent->d_name, "..") && strcmp(ent->d_name, ".")) {
                result.push_back(ent->d_name);
            }
        }
        closedir (dir);
        return result;
    } else {
        return std::vector<std::string>();
    }
#else
    todo;
#endif
}


bool e172::Additional::isDirectory(std::string path) {
#ifdef __unix__
    struct stat st;
    if(stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
        return true;
    } else {
        return false;
    }
#else
    todo;
#endif
}

std::string e172::Additional::fileSufix(std::string string) {
    return string.substr(string.find_last_of('.'), string.length() - 1);
}

std::string e172::Additional::concatenatePaths(std::string prefix, std::string string) {
    if(string[0] == '.') string.erase(0, 1);
    if(string[0] != '/') string = '/' + string;
    if(prefix[prefix.length() - 1] == '/') prefix.pop_back();

    return prefix + string;
}

std::string e172::Additional::cutPath(std::string string, unsigned level) {
    size_t pos = string.find_last_of(separator);
    std::string result = string.substr(0, pos);
    if(level > 1) {
        return cutPath(result, level - 1);
    }
    return result;
}

std::string e172::Additional::pathTopLevelItem(const std::string &string) {
    const size_t pos = string.find_last_of(separator);
    return string.substr(pos + 1, string.size() - pos - 1);
}

size_t e172::Additional::countOfFiles(std::string path, std::string suffix) {
    if(path[path.length() - 1] == '/') path.pop_back();
    std::vector<std::string> items = e172::Additional::directoryContent(path);
    size_t sum_count = 0;
    for(unsigned long long i = 0, L = items.size(); i < L; i++) {
        std::string item = items[i];
        std::string file = path + '/' + item;
        if(e172::Additional::isDirectory(file)) {
            sum_count += countOfFiles(file, suffix);
        } else {
            if(e172::Additional::fileSufix(file) == suffix) {
                ++sum_count;
            }
        }
    }
    return sum_count;
}

std::map<std::string, std::string> e172::Additional::readAllVof(const std::string &path, char delimiter) {
    std::map<std::string, std::string> result;
    std::ifstream fin;
    fin.open(path, std::ios::in);
    if (fin.is_open()) {
        while(!fin.eof()) {
            std::string line;
            std::getline(fin, line);
            const auto lv = e172::Additional::split(line, delimiter);
            if(lv.size() > 1) {
                result[lv[0]] = lv[1];
            }
        }
        fin.close();
    }
    return result;
}

std::string e172::Additional::readVof(const std::string &path, const std::string &id, char delimiter) {
    std::ifstream fin;
    fin.open(path, std::ios::in);
    if (fin.is_open()) {
        while(!fin.eof()) {
            std::string line;
            std::getline(fin, line);
            const auto lv = e172::Additional::split(line, delimiter);
            if(lv.size() > 1) {
                if (lv[0] == id) {
                    fin.close();
                    return e172::Additional::trim(lv[1]);
                }
            }
        }
        fin.close();
    }
    return std::string();
}

void e172::Additional::writeVof(const std::string &path, const std::string &id, const std::string &value, char delimiter) {
    bool found = false;
    bool endsWithEndline = false;
    std::ifstream fin;
    std::ofstream fout;
    std::list<std::string> tmp;
    fin.open(path, std::ios::in);
    if (fin.is_open() == false) {
        fout.open(path, std::ios::out);
        fout << id << delimiter << value;
        fout.close();
    } else {
        while(!fin.eof()) {
            std::string line;
            std::getline(fin, line);
            const auto lv = e172::Additional::split(line, delimiter);
            if(lv.size() > 1) {
                if (lv[0] == id) {
                    found = true;
                    tmp.push_back(lv[0] + delimiter + value);
                } else {
                    tmp.push_back(lv[0] + delimiter + lv[1]);
                }
            }
        }
        fin.close();
        if(tmp.size() > 0) {
            const auto back = tmp.back();
            if(back.size() > 0) {
                endsWithEndline = (back.back() == '\n');
            }
        }
        if (!found) {
            fout.open(path, std::ios::app);
            if(!endsWithEndline) {
                fout << '\n';
            }
            fout << id + delimiter + value << '\n';
            fout.close();
        } else {
            fout.open(path, std::ios::out);
            for(const auto& line : tmp) {
                fout << line << '\n';
            }
            fout.close();
        }
    }
}

std::vector<std::string> e172::Additional::executeCommand(const std::string &command) {
#ifdef __unix__
    std::array<char, 128> buffer;
    std::vector<std::string> result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result.push_back(buffer.data());
    }
    return result;
#else
    todo;
#endif
}

void e172::Additional::parseForder(std::string path, const std::function<void(const std::string&)> &callback) {
    if(path[path.length() - 1] == '/') path.pop_back();
    std::vector<std::string> items = directoryContent(path);
    for(unsigned long long i = 0, L = items.size(); i < L; i++) {
        std::string item = items[i];
        std::string file = path + '/' + item;
        if(Additional::isDirectory(file)) {
            parseForder(file, callback);
        } else {
            callback(file);
        }
    }
}

std::string e172::Additional::defaultFontDirectory() {
#if defined(_WIN32)
    return "%WINDIR%\fonts";
#elif defined(_APPLE_) && defined(_MACH_)
    return "/Library/Fonts";
#elif defined(linux) || defined(__linux)
    return "/usr/share/fonts";
#else
    return "Unknown operation system";
#endif
}

std::string e172::Additional::defaultFont(const std::string &suffix) {
    const auto dir = defaultFontDirectory();
    std::filesystem::recursive_directory_iterator it(dir), end;
    while (it != end) {
        if(it->is_regular_file() && it->path().string().ends_with(suffix)) {
            return it->path().string();
        }
        ++it;
    }
    return {};
}

std::vector<std::string> e172::Additional::coverArgs(int argc, const char *argv[])
{
    std::vector<std::string> result;
    for(int i = 0; i < argc; i++) {
        result.push_back(argv[i]);
    }
    return result;
}

e172::Option<double> e172::Additional::parseRadians(const std::string &string) {
    if(string.empty())
        return None;

    std::regex regex("(\\-?Pi)?[ ]*([\\/\\*]?)[ ]*(\\-?\\d*\\.?\\d*)");

    double result;
    std::string lastPart = "";

    bool err = false;
    const auto partToNum = [&err](const std::string& str){
        if(str == "Pi") {
            return Math::Pi;
        } else if(str == "-Pi") {
            return -Math::Pi;
        } else {
            try {
                return std::stod(str);
            }  catch (std::invalid_argument) {
                err = true;
                return 0.;
            }
        }
    };

    for(std::sregex_iterator i = std::sregex_iterator(string.begin(), string.end(), regex);
        i != std::sregex_iterator();
        ++i) {
        std::smatch smatch = *i;
        for(size_t j = 1; j < smatch.size(); ++j){
            const auto part = trim(smatch[j].str());
            if(!part.empty()) {
                if(part != "/" && part != "*") {
                    if(lastPart == "") {
                        result = partToNum(part);
                    } else if(lastPart == "/") {
                        result /= partToNum(part);
                    } else {
                        result *= partToNum(part);
                    }
                }
                lastPart = part;
                if(err)
                    return None;

            }
        }
    }
    return result;
}

std::string e172::Additional::sameBeginningSubstring(const std::list<std::string> &list) {
    size_t i = 0;
    std::string result;
#undef min
#undef max
    auto max = std::numeric_limits<size_t>::min();
    for(const auto &l : list) {
        if(l.size() > max) {
            max = l.size();
        }
    }

    while(true) {
        std::string substr;
        for(const auto &l : list) {
            auto s = l.substr(0, i);
            if(!substr.empty() && s != substr) {
                return result;
            }
            substr = s;
        }
        result = substr;
        if(i > max) {
            return result;
        }
        ++i;
    }
}

std::string e172::Additional::compleateString(const std::string &string, const std::list<std::string> &model) {
    return sameBeginningSubstring(filter(model, [string](const std::string& m) {
        return m.find(string) == 0;
    }));
}
