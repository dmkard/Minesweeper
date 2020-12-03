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
    //Constructs empty unorderd map
	ResourceManager();
    //Returns a shared_ptr of resource of type T of the path
	std::shared_ptr<T> GetResource(const std::string& path);
    //Create a static instance of resource manager
	static ResourceManager<T>& Instance();
    //Checks for not used resources
    void ReleaseResource(const std::string& path);

    // Not copyable and not moveable.
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

private:
	//Loads a resource of the path
	std::shared_ptr<T> Load(const std::string& path);

    //Stores resources of resource manager`s instance
    std::unordered_map<std::string, std::shared_ptr<T>> resources_;
};

template<typename T>
ResourceManager<T>::ResourceManager() :
    resources_{}
{}

template<typename T>
std::shared_ptr<T> ResourceManager<T>::GetResource(const std::string& path)
{
    auto it = resources_.find(path);
    if ( it != resources_.end())
        return it->second;
    else
        return Load(path);
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
    //Creating a local instance of resource manager of a specific type
    static auto instance = std::make_unique<ResourceManager<T>>();
    return *instance.get();
}

template<typename T>
void ResourceManager<T>::ReleaseResource(const std::string& path)
{
    resources_.erase(path);
}
