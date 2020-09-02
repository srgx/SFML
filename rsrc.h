#ifndef RSRC__H__
#define RSRC__H__
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder{
private:
  std::map<Identifier,std::unique_ptr<Resource>> mResourceMap;
public:
  void load(Identifier, const std::string &);
  template <typename Parameter>
  void load(Identifier, const std::string &, const Parameter &);
  Resource & get(Identifier id);
  const Resource & get(Identifier id) const;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource,Identifier>::load(Identifier id, const std::string & filename){
  std::unique_ptr<Resource> resource(new Resource());
  if(!resource->loadFromFile(filename)){
    throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
  }

  auto inserted = mResourceMap.insert(std::make_pair(id,std::move(resource)));
  assert(inserted.second);
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource,Identifier>::load(Identifier id, const std::string & filename, const Parameter & secondParam){
  std::unique_ptr<Resource> resource(new Resource());
  if(!resource->loadFromFile(filename, secondParam)){
    throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
  }

  auto inserted = mResourceMap.insert(std::make_pair(id,std::move(resource)));
  assert(inserted.second);
}

template <typename Resource, typename Identifier>
Resource & ResourceHolder<Resource,Identifier>::get(Identifier id){
  auto found = mResourceMap.find(id);
  assert(found != mResourceMap.end());
  return *found->second;
}

template <typename Resource, typename Identifier>
const Resource & ResourceHolder<Resource,Identifier>::get(Identifier id) const{
  auto found = mResourceMap.find(id);
  assert(found != mResourceMap.end());
  return *found->second;
}

#endif