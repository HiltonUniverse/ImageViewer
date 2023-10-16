#pragma once

template<typename T>
class SingletonPattern
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }
private:
    SingletonPattern() = delete;
    ~SingletonPattern() = delete;
    SingletonPattern(const SingletonPattern&) = delete;
    SingletonPattern(SingletonPattern&&) = delete;

    SingletonPattern& operator=(const SingletonPattern&) = delete;
    SingletonPattern& operator=(const SingletonPattern&&) = delete;
};
