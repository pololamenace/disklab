//------------------------------------------------------------------------------
/// @file
/// @brief disk-based storage devices HUGAHUGA
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2016/05/22 Bernhard Egger created
///
/// @section license_section License
/// Copyright (c) 2017, Computer Systems and Platforms Laboratory,
/// Seoul National University. All rights reserved.
///
/// Redistribution and use in source and binary forms,  with or without modifi-
/// cation, are permitted provided that the following conditions are met:
///
/// - Redistributions of source code must retain the above copyright notice,
///   this list of conditions and the following disclaimer.
/// - Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY  AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER  OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT,  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSE-
/// QUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE
/// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
/// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT
/// LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY
/// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
/// DAMAGE.
//------------------------------------------------------------------------------

#ifndef __CA_DISK_H__
#define __CA_DISK_H__

#include "types.h"

//------------------------------------------------------------------------------
/// @brief base class for disk-based storage devices
///
/// Disk provides an abstract base class for all disk-based storage devices
/// (rotating HDDs, SDD, flash-disks, etc).
///
class Disk {
  public:
    /// @name constructor/destructor
    /// @{

    /// @brief constructor
    Disk(void) {};

    /// @brief destructor
    virtual ~Disk(void) {};

    /// @}


    /// @name access methods
    /// @{

    /// @brief read @a nblocks blocks starting at @a block
    /// @param ts timestamp of the event
    /// @param block logical disk block index of data to read
    /// @param nblocks number of blocks to read
    /// @retval time when the access ends (ts + latency of access)
    virtual double read(double ts, uint64 block, uint64 nblocks) = 0;

    /// @brief write @a nblocks blocks starting at @a block
    /// @param ts timestamp of the event
    /// @param block logical disk block index of data to write
    /// @param nblocks number of blocks to write
    /// @retval time when the access ends (ts + latency of access)
    virtual double write(double ts, uint64 block, uint64 nblocks) = 0;

    /// @}
};

#endif // __CA_DISK_H__
