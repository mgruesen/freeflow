#ifndef FREEFLOW_KQUEUE_HPP
#define FREEFLOW_KQUEUE_HPP

#include <sys/event.h>
#include <sys/time.h>
#include <stdint>
#include <vector>
#include <algorithm>

namespace ff
{


struct KQueue_event : struct kevent
{
  KQueue_event() = default;

  inline uint32_t id()        const { return ident); }

  inline bool     is_read()   const { return flags & EV_FILTREAD; }
  inline bool     is_write()  const { return flags & EV_FILTWRITE; }
  inline bool     is_error()  const { return flags & EV_ERROR; }

  inline uint32_t filter()    const { return filter; }
  inline uint32_t flags()     const { return flags; }
  inline int64_t  data()      const { return data; }
  inline void*    udata()     const { return (void*)udata; }
};

struct KQueue : std::vector<KQueue_event>
{
  KQueue(int);

  // Mutators.
  void add(int);
  void del(int);
  void clear();

  // Accessors.
  bool can_read(int) const;
  bool can_write(int) const;
  bool has_error(int) const;

  // Returns the kqueue fd.
  int fd() const { return kqfd_; }

  // Data members.
  //
  // Master kqueue event.
  KQueue_event master_;
  // Number of events after a call to kevent.
  int num_events_;
  // The kqueue fd.
  int kqfd_;
};


// KQueue ctor. Initializes the underlying vector and the kqueue fd.
KQueue::KQueue(int size)
  : std::vector<KQueue_event>(size), master_(), num_events_(0), kqfd_(kqueue())
{ }


// Adds the file descriptor to the queue.
inline void
KQueue::add(int fd)
{
  EV_SET(&master_, fd, EV_FILTREAD, EV_ADD, 0, 0, 0);
}


// Removes the file descriptor from the queue.
inline void
KQueue::del(int fd)
{
  EV_SET(&master_, fd, EV_FILTREAD, EV_DELETE, 0, 0, 0);
}


// Removes all file descriptors from the queue.
inline void
KQueue::clear()
{
  master_ = KQueue_event();
}


// Returns true if the given file descriptor can read.
inline bool
can_read(int fd) const
{
  return std::find_if(begin(), end(), [fd](KQueue_event& kqe){
    return kqe.fd() == fd && kqe.is_read();
  });
}


// Returns true if the given file descriptor can write.
inline bool
can_write(int fd) const
{
  return std::find_if(begin(), end(), [fd](KQueue_event& kqe){
    return kqe.fd() == fd && kqe.is_write();
  });
}


// Returns true if the given file descriptor has an error.
inline bool
has_error(int fd) const
{
  return std::find_if(begin(), end(), [fd](KQueue_event& kqe){
    return kqe.fd() == fd && kqe.is_error();
  });
}

// KQueue Operations.
//

// Returns the number of events registered with a given Kqueue that occurred
// in the given duration in milliseconds.
inline int
monitor(Kqueue& kq, int timeout)
{
  struct timespec ts = { 0, timeout * 1000; };
  kq.num_events_ = kevent(kq.fd(), kq.master(), kq.size(), kq.data(), kq.size(),
    &ts);
  return kq.num_events_;
}

} // end namspace ff

#endif
