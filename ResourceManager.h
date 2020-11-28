#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>
#include <iterator>
template <typename T>
class ResourceManager
{
public:

	explicit ResourceManager();
	std::shared_ptr<T> GetResource(const std::string& path);
	static ResourceManager<T>& Instance();
	void Update();

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

private:
	
	std::shared_ptr<T> Load(const std::string& path);
    std::unordered_map<std::string, std::shared_ptr<T>> resources_;
};

template<typename T>
ResourceManager<T>::ResourceManager() :
    resources_{}
{
    std::cout << resources_.size();
}

template<typename T>
std::shared_ptr<T> ResourceManager<T>::GetResource(const std::string& path)
{

    auto it = resources_.find(path);
    if ( it != resources_.end())
    {
        return it->second;
    }
    else
    {
        std::cout << "We have to load " << path << std::endl;
        return Load(path);
    }
}

template<typename T>
std::shared_ptr<T> ResourceManager<T>::Load(const std::string& path)
{
    T resouce;
    resouce.loadFromFile(path);
    resources_.emplace(path, std::make_shared<T>(resouce));
    return resources_[path];
}

template<typename T>
ResourceManager<T>& ResourceManager<T>::Instance()
{
    static auto instance = std::make_unique<ResourceManager<T>>();
    return *instance.get();
}

template<typename T>
void ResourceManager<T>::Update()
{
    for (auto it = resources_.begin(); it != resources_.end();)
    {
        std::cout << it->first << " has " << it->second.use_count() << " examples\n";
        if (it->second.use_count() == 1)
        {
            std::cout << "we dont need " << it->first << " anymore\n";
            it = resources_.erase(it);
        }
        else
            ++it;

    }
}

