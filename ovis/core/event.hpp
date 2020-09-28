#pragma once

#include <functional>
#include <vector>

#include <SDL2/SDL_assert.h>

namespace ovis {

class EventBase {
 public:
  virtual ~EventBase() = default;

 protected:
  EventBase() = default;

 private:
  virtual void Unsubscribe(std::size_t subscription_index) = 0;
};

template <class T>
class Event;

class EventSubscription {
  template <typename T>
  friend class Event;

 public:
  void Unsubscribe();

 private:
  EventSubscription(EventBase* event, std::size_t subscription_index)
      : event_(event), subscription_index_(subscription_index) {}

  EventBase* event_;
  std::size_t subscription_index_;
};

template <typename... ArgumentTypes>
class Event<void(ArgumentTypes...)> : public EventBase {
 public:
  using FunctionType = std::function<void(ArgumentTypes...)>;

  EventSubscription Subscribe(const FunctionType& function) {
    std::size_t subscription_index = 0;
    while (subscription_index < subscriptions_.size()) {
      if (!subscriptions_[subscription_index]) {
        subscriptions_[subscription_index] = function;
        return {this, subscription_index};
      } else {
        ++subscription_index;
      }
    }

    subscriptions_.push_back(function);
    return {this, subscriptions_.size() - 1};
  }

  void Unsubscribe(std::size_t subscription_index) override {
    SDL_assert(subscription_index < subscriptions_.size());
    SDL_assert(subscriptions_[subscription_index]);
    subscriptions_[subscription_index] = nullptr;
  }

  void Invoke(ArgumentTypes&&... arguments) {
    for (const auto& subscription : subscriptions_) {
      subscription(arguments...);
    }
  }

 private:
  std::vector<FunctionType> subscriptions_;
};

}  // namespace ovis