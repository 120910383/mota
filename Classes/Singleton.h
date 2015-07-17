#pragma once

#include <cassert>

///	单体模板
template<typename T>
class Singleton 
{
public:
	/// 是否存在？
	static bool HasInstance() 
	{
        return (instance != nullptr);
	}

	/// 获取
	static T* GetInstance() 
	{
		assert(instance != nullptr);
		return instance;
	}
	
	/// 创建
	static void NewInstance() 
	{
        assert(instance == nullptr);
        if (instance != nullptr)
			delete instance;
		instance = new T();
	}
	
	/// 删除
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
