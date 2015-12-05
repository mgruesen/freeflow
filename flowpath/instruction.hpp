// Copyright (c) 2015 Flowgrammable.org
// All rights reserved

#ifndef FP_INSTRUCTION_HPP
#define FP_INSTRUCTION_HPP

#include "types.hpp"

#include <vector>


namespace fp
{

struct Context;


// Defined address spaces for fields.
constexpr int Packet_memory   = 0;
constexpr int Metadata_memory = 1;


// A field defines the offset and length of a value in
// memory within some address space. Currently, there
// are only two address spaces: packet and metadata.
//
// If the field refers to packet memory, the offset is
// relative to the current header.
struct Field
{
  std::uint8_t  address;
  std::uint16_t offset;
  std::uint16_t length;
};


// -------------------------------------------------------------------------- //
// Actions
//
// TODO: Define actions for TTL operations, pushing, and
// popping headers.


// Copies a value into the given field. Note that
// value + field.length must be within the range
// of memory designated by field.address.
struct Set_action
{
  Field       field;
  Byte const* value;
};


// Copies a field from a source address space to
// an offset in the other address space.
//
// TODO: What if we have >2 address spaces?
struct Copy_action
{
  Field        field;
  std::uint16_t offset;
};


// Set the output port for the current packet.
struct Output_action
{
  std::uint32_t port;
};


// Sets the output queue for the current packet.
struct Queue_action
{
  std::uint32_t queue;
};


// Sets the group action to apply to this packet.
struct Group_action
{
  std::uint32_t group;
};


// Immediately stop processing the packet and
// do not forward it.
struct Drop_action
{
  std::uint32_t group;
};


// Represents one of a set of actions. Abstactly:
//
//    action ::= getfield <field>
//               setfield <field> <value>
//               copyfield <field> <location>
//               output <port>
//               queue <queue>
//               group <group>
//               drop
struct Action
{
  enum Type : std::uint8_t
  {
    SET, COPY, OUTPUT, QUEUE, GROUP, ACTION, DROP
  };
  union Value
  {
    Set_action    set;
    Copy_action   copy;
    Output_action output;
    Queue_action  queue;
    Group_action  group;
    Drop_action   drop;
  } value;
  std::uint8_t type;
};


// A list of actions.
using Action_list = std::vector<Action>;


// -------------------------------------------------------------------------- //
// Evaluation

void evaluate(Context&, Action);
void evaluate(Context&, Action_list const&);

} // namespace fp


#endif
