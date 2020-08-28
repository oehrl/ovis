#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <vector>

#include <SDL2/SDL_assert.h>

namespace ovis {

// template <typename T>
// class ResourcePointer;

// class ResourceBase {
//  public:
//   ResourceBase(std::string id) : id_(std::move(id)) {}
//   ResourceBase(std::string id, std::string filename) : id_(std::move(id)) {
//     filenames_.emplace_back(std::move(filename));
//   }
//   ResourceBase(std::string id, std::vector<std::string> files)
//       : id_(std::move(id)), filenames_(std::move(files)) {}

//   inline const std::string& id() const { return id_; }
//   inline const std::string& filename() const {
//     SDL_assert(filenames_.size() == 1);
//     return filenames_[0];
//   }
//   inline const std::vector<std::string>& filenames() const {
//     return filenames_;
//   }

//   template <typename T>
//   ResourcePointer<T> GetResourcePointer();

//   template <typename T>
//   inline T* GetAs() {
//     return reinterpret_cast<T*>(CheckTypeAndGet(typeid(T)));
//   }

//   virtual void Load() = 0;
//   virtual void Unload() = 0;

//  private:
//   std::string id_;
//   std::vector<std::string> filenames_;

//   virtual std::type_index GetType() = 0;
//   virtual void* CheckTypeAndGet(const std::type_info& type) = 0;
// };

// template <typename T>
// class Resource final : public ResourceBase {
//  public:
//   Resource(const std::string& id, const std::string& filename,
//            std::unique_ptr<T> data)
//       : ResourceBase(id, filename), data_(std::move(data)) {}

//   inline T* data() { return data_.get(); }

//   inline void Load() override {}

//  private:
//   std::unique_ptr<T> data_;

//   virtual std::type_index GetType() override { return typeid(T); }

//   void* CheckTypeAndGet(const std::type_info& type) override {
//     return type == typeid(T) ? data_.get() : nullptr;
//   }
// };

// template <typename T>
// class ResourcePointer {
//  public:
//   explicit inline ResourcePointer(Resource<T>* resource)
//       : resource_(resource) {}

//   operator bool() { return resource_ && resource_->data() != nullptr; }
//   std::shared_ptr<T> Lock();

//  private:
//   Resource<T>* resource_;
// };

// template <typename T>
// ResourcePointer<T> ResourceBase::GetResourcePointer() {
//   if (GetType() == typeid(T)) {
//     return ResourcePointer<T>(static_cast<Resource<T>*>(this));
//   } else {
//     return ResourcePointer<T>();
//   }
// }

template <typename T>
class Resource;

// template <typename T>
// using ResourcePointer = std::shared_ptr<Resource<T>>;

template <typename T>
class ResourcePointer {
  friend class ResourceManager;

 public:
  ResourcePointer() = default;
  ResourcePointer(std::shared_ptr<Resource<T>> resource) : resource_(std::move(resource)) {}
  template <typename Derived, typename = std::enable_if_t<std::is_base_of<T, Derived>::value>>
  ResourcePointer(const ResourcePointer<Derived>& other) : resource_(other.resource_) {}

  inline const T* get() const { return resource_ ? resource_->get() : nullptr; }
  inline T* get() { return resource_ ? resource_->get() : nullptr; }
  inline const T* operator->() const { return resource_->get(); }
  inline T* operator->() { return resource_->get(); }

 private:
  std::shared_ptr<Resource<T>> resource_;
};

class ResourceBase {};

template <typename T>
class Resource final : public ResourceBase {
 public:
  Resource() = default;
  Resource(const Resource<T>&) = delete;
  Resource(Resource<T>&&) = delete;
  inline ~Resource() {
    if (is_loaded_) {
      Delete();
    }
  }

  Resource& operator=(const Resource<T>&) = delete;
  Resource& operator=(Resource<T>&&) = delete;

  inline T* operator->() { return get(); }
  inline const T* operator->() const { return get(); }

  inline bool is_loaded() const { return is_loaded_; }

  inline T* get() { return is_loaded_ ? reinterpret_cast<T*>(data_.data()) : nullptr; }

  template <typename... Args>
  void Create(Args... constructor_arguments) {
    SDL_assert(!is_loaded_);
    new (data_.data()) T(std::forward<Args>(constructor_arguments)...);
    is_loaded_ = true;
  }

  void Delete() {
    SDL_assert(is_loaded_);
    get()->~T();
  }

 private:
  std::array<std::uint8_t, sizeof(T)> data_;
  bool is_loaded_ = false;
};

}  // namespace ovis