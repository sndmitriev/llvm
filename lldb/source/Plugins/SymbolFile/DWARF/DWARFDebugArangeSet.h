//===-- DWARFDebugArangeSet.h -----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLDB_SOURCE_PLUGINS_SYMBOLFILE_DWARF_DWARFDEBUGARANGESET_H
#define LLDB_SOURCE_PLUGINS_SYMBOLFILE_DWARF_DWARFDEBUGARANGESET_H

#include "lldb/Core/dwarf.h"
#include <cstdint>
#include <vector>

class DWARFDebugArangeSet {
public:
  struct Header {
    uint32_t length;    // The total length of the entries for that set, not
                        // including the length field itself.
    uint16_t version;   // The DWARF version number
    uint32_t cu_offset; // The offset from the beginning of the .debug_info
                        // section of the compilation unit entry referenced by
                        // the table.
    uint8_t addr_size;  // The size in bytes of an address on the target
                        // architecture. For segmented addressing, this is the
                        // size of the offset portion of the address
    uint8_t seg_size; // The size in bytes of a segment descriptor on the target
                      // architecture. If the target system uses a flat address
                      // space, this value is 0.
  };

  struct Descriptor {
    dw_addr_t address;
    dw_addr_t length;
    dw_addr_t end_address() const { return address + length; }
  };

  DWARFDebugArangeSet();
  void Clear();
  void SetOffset(uint32_t offset) { m_offset = offset; }
  llvm::Error extract(const lldb_private::DWARFDataExtractor &data,
                      lldb::offset_t *offset_ptr);
  dw_offset_t FindAddress(dw_addr_t address) const;
  size_t NumDescriptors() const { return m_arange_descriptors.size(); }
  const Header &GetHeader() const { return m_header; }

  const Descriptor &GetDescriptorRef(uint32_t i) const {
    return m_arange_descriptors[i];
  }

protected:
  typedef std::vector<Descriptor> DescriptorColl;
  typedef DescriptorColl::iterator DescriptorIter;
  typedef DescriptorColl::const_iterator DescriptorConstIter;

  uint32_t m_offset;
  Header m_header;
  DescriptorColl m_arange_descriptors;
};

#endif // LLDB_SOURCE_PLUGINS_SYMBOLFILE_DWARF_DWARFDEBUGARANGESET_H
