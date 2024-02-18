#pragma once

#include "nuis/eventinput/IEventSource.h"

namespace nuis {

// ABC for EventSource wrappers that want to give access to the underlying
// low level event source
//  e.g. You want to apply a preselection and a caching layer between the
//  ananlysis and the IO via conposition, but need to give callers the ability
//  to check the type of the lowest level event source.
class IEventSourceWrapper {
protected:
  std::shared_ptr<IEventSource> wrapped_ev_source;

public:
  IEventSourceWrapper(std::shared_ptr<IEventSource> evs)
      : wrapped_ev_source(evs) {}

  template <typename T> std::shared_ptr<T> as() {
    return std::dynamic_pointer_cast<T>(wrapped_ev_source);
  }

  std::shared_ptr<IEventSource> unwrap() { return wrapped_ev_source; }
};

using IWrappedEventSourcePtr = std::shared_ptr<IEventSourceWrapper>;

} // namespace nuis