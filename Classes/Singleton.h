/*********************************************
Author: Zhang Hao
GitHub: https://github.com/120910383/mota
*********************************************/

#pragma once

#include <cassert>

///    singleton
template<typename T>
class Singleton
{
public:
    static bool HasInstance()
    {
        return (instance != nullptr);
    }

    static T* GetInstance()
    {
        assert(instance != nullptr);
        return instance;
    }

    static void NewInstance()
    {
        assert(instance == nullptr);
        if (instance != nullptr)
            delete instance;
        instance = new T();
    }

    static void DeleteInstance()
    {
        assert(instance != nullptr);
        if (instance != nullptr)
            delete instance;
        instance = nullptr;
    }

protected:
    static T* instance;
};

template<typename T>
T* Singleton<T>::instance = nullptr;