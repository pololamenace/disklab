//------------------------------------------------------------------------------
/// @file
/// @brief fully-associative cache for rotating disk-based storage devices (HDD)
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2017/05/16 Bernhard Egger created
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

#ifndef __CA_CACHE_H__
#define __CA_CACHE_H__

#include "types.h"

//------------------------------------------------------------------------------
/// @brief cache for rotating disk-based storage devices (HDD)
///
/// The BlockCache class implements a simple fully-associative cache
///
class BlockCache {
  public:
    /// @name constructor/destructor
    /// @{

    /// @brief constructor
    /// @param nblocks number of cache blocks (MUST BE >= 2!)
    /// @param verbose verbose output
    BlockCache(uint32 nblocks,
               bool verbose=false);

    /// @brief destructor
    virtual ~BlockCache(void);

    /// @}
    //


    /// @name properties
    /// @{

    /// @brief retrieve number of cache blocks
    uint32 size(void) const;

    /// @brief retrieve number of cache hits
    uint32 hits(void) const;

    /// @brief retrieve number of cache misses
    uint32 misses(void) const;

    /// @brief retrieve the miss rate
    float miss_rate(void) const;

    /// @brief dump cache contents to stdout
    void dump(void) const;

    /// @}


    /// @name access methods
    /// @{

    /// @brief check whether a block is cached or not
    /// @param block block number
    /// @retval true block exists in cache (cache hit)
    /// @retval false block not cached (cache miss)
    bool has(uint64 block) const;

    /// @brief retrieve a block from the cache. If the block is
    ///        already cached, the block's access timestamp is
    ///        updated. If the block is not in the cache, it is
    ///        brought in.
    /// @param block block number
    /// @retval true block exists in cache (cache hit)
    /// @retval false block not cached (cache miss)
    bool get(uint64 block);

    /// @brief encache a block. If the block is already cached,
    ///        this function updates the block's access timestamp.
    /// @param block block number
    /// @retval true cache hit
    /// @retval false cache miss
    void put(uint64 block);

    /// @}


  protected:
    uint32 _nblocks;                ///< number of blocks in cache
    bool   _verbose;                ///< toggle verbose output

    uint32 _hit;                    ///< number of cache hits
    uint32 _miss;                   ///< number of cache misses

    // TODO add more fields as necessary

    // TODO
    // add more methods as necessary
};

#endif // __CA_CACHE_H__
