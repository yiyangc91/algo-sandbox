#ifndef BASE_H
#define BASE_H

#include <string>
#include <unordered_map>
#include <vector>

class BaseSearch {
public:
    virtual ~BaseSearch() = 0;
    virtual std::string::size_type operator()(const std::string& needle, const std::string& haystack) = 0;
};

class SearchSingleton {
    std::unordered_map<std::string, BaseSearch*> instances;
public:
    void addInstance(const std::string& s, BaseSearch* inst);
    inline const std::unordered_map<std::string, BaseSearch*>& getInstances() const noexcept {
        return instances;
    }
};

SearchSingleton* getInstance();

template <class T>
T* RegisterAlgorithm(const std::string& algorithmName) {
    auto* sa = new T();
    getInstance()->addInstance(algorithmName, sa);
    return sa;
}

#define ALGO_F(cls, name, needleParam, haystackParam) \
    class name##Search : public cls { \
       ~name##Search() {} \
    public: \
       static name##Search* const ptr; \
       std::string::size_type operator()(const std::string& needle, const std::string& haystack); \
    }; \
    name##Search* const name##Search::ptr = RegisterAlgorithm<name##Search>(#name); \
    std::string::size_type name##Search::operator()(const std::string& needleParam, const std::string& haystackParam)

#define ALGO(name, needleParam, haystackParam) \
    class name##Search : public BaseSearch { \
       ~name##Search() {} \
    public: \
       static name##Search* const ptr; \
       std::string::size_type operator()(const std::string& needle, const std::string& haystack); \
    }; \
    name##Search* const name##Search::ptr = RegisterAlgorithm<name##Search>(#name); \
    std::string::size_type name##Search::operator()(const std::string& needleParam, const std::string& haystackParam)

#endif // BASE_H
