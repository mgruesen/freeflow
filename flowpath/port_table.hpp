#ifndef FP_PORT_TABLE_HPP
#define FP_PORT_TABLE_HPP

#include "port.hpp"
#include "port_udp.hpp"
#include "port_tcp.hpp"

#include "thread.hpp"

#include <string>
#include <vector>
#include <climits>

namespace fp
{

// This is a special port used to flood a packet to all ports
// except for the one that received it.
class Port_flood : public Port_udp
{
public:
  using Port_udp::Port_udp;
  // Constructor/Destructor.
  Port_flood(std::string const& addr)
    : Port_udp(0xffff, addr)
  { }

  ~Port_flood() { }

  int   send();
};

// Global port table type.
class Port_table
{
  using store_type = std::vector<Port*>;
  using value_type = Port*;
  using iter_type  = std::vector<Port*>::iterator;
public:

  // Constructors.
  //
  // Default.
  Port_table();
  // Initial size.
  Port_table(int);

  // Destructor.
  ~Port_table();

  // Allocator.
  Port* alloc(Port::Type, std::string const&);
  // Deallocator.
  void  dealloc(Port::Id);

  // Accessors.
  //
  // Search functions.
  value_type find(Port::Id);
  value_type find(std::string const&);

  // Lists all ports.
  store_type list();

  // Logical ports.
  value_type flood_port() const { return flood_port_; }
  value_type broad_port() const { return broad_port_; }
  value_type drop_port()  const { return drop_port_; }


private:
  // Port management thread.
  Thread thread_;

  // List of all ports allocated in the system.
  store_type data_;

  // Reserved ports.
  value_type flood_port_;
  value_type broad_port_;
  value_type drop_port_;
};

extern Port_table port_table;
extern void port_mgr_work(void);

} // end namespace fp

#endif
