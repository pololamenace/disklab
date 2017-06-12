//------------------------------------------------------------------------------
/// @file
/// @brief rotating disk-based storage devices (HDD)
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2016/05/22 Bernhard Egger created
/// 2017/05/16 Bernhard Egger modified to incorporate disk cache
///
/// @section license_section License
/// Copyright (c) 2016-2017, Computer Systems and Platforms Laboratory,
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

#ifndef __CA_HDD_H__
#define __CA_HDD_H__

#include "disk.h"
#include "cache.h"
using namespace std;

///@brief struct encoding a byte position on the disk as a surface/track/sector
///       triple.
typedef struct HDD_Position {
  uint32 surface;                   ///< surface
  uint32 track;                     ///< track
  uint64 sector;                    ///< sector
  uint32 max_sectors;               ///< how many sectors can be accessed conse-
                                    ///< cutively until the end of this track
} HDD_Position;

///      struct encoding the element of the cache and the cache itself
///       this a double linked list.
typedef struct HDD_Cache_Ele {
    uint64 cached_block;
    HDD_Cache_Ele* prev;
    HDD_Cache_Ele* next;
}HDD_Cache_Ele;

typedef struct {
    HDD_Cache_Ele* head;
    HDD_Cache_Ele* tail;
}

//------------------------------------------------------------------------------
/// @brief rotating disk-based storage devices (HDD)
///
/// The HDD class implements rotating disks.
///
class HDD : public Disk {
  public:
    /// @name constructor/destructor
    /// @{

    /// @brief constructor
    /// @param surfaces number of surfaces
    /// @param tracks_per_surface number of tracks per surface
    /// @param sectors_innermost_track number of sectors on the innermost track
    /// @param sectors_outermost_track number of sectors on the outermost track
    /// @param rpm rotations per minute
    /// @param sector_size size of one sector, in bytes
    /// @param seek_overhead base overhead of seek operation, in seconds
    /// @param seek_per_track linear seek overhead per track, in seconds
    /// @param cache_blocks number of cache blocks in integraded cache
    /// @param verbose verbose output
    HDD(uint32 surfaces, uint32 tracks_per_surface,
        uint32 sectors_innermost_track, uint32 sectors_outermost_track,
        uint32 rpm, uint32 sector_size,
        double seek_overhead, double seek_per_track,
        uint32 cache_blocks,
        bool verbose=false);

    /// @brief destructor
    virtual ~HDD(void);

    /// @}
    //


    /// @name properties
    /// @{

    /// @brief return the number of bytes per sector
    uint32 bytes_per_sector(void) const;

    /// @brief return the number of tracks/surface
    uint32 tracks_per_surface(void) const;

    /// @brief return the capacity of this disk in bytes
    uint64  capacity(void) const;

    /// @brief return a pointer to the cache
    const BlockCache* cache(void) const;

    /// @brief return the verbose flag
    bool verbose(void) const;

    /// @}


    /// @name access methods
    /// @{

    /// @brief read @a nblocks blocks starting at @a block
    /// @param ts timestamp of the event
    /// @param block logical disk block index of data to read
    /// @param nblocks number of blocks to read
    /// @retval time when the access ends (ts + latency of access)
    virtual double read(double ts, uint64 block, uint64 nblocks);

    /// @brief write @a nblocks blocks starting at @a block
    /// @param ts timestamp of the event
    /// @param block logical disk block index of data to write
    /// @param nblocks number of blocks to write
    /// @retval time when the access ends (ts + latency of access)
    virtual double write(double ts, uint64 block, uint64 nblocks);

    /// @}


    /// @name access latencies
    /// @{

    /// @brief seek time to move the head from @a from_track to @a to_track
    double seek_time(uint32 from_track, uint32 to_track) ;

    /// @brief average rotational latency
    double wait_time(void) ;

    /// @brief time to read @a sectors
    double read_time(uint64 sectors);

    /// @brief time to write @a sectors
    double write_time(uint64 sectors);

    /// @}


  protected:
    uint32 _surfaces;               ///< number of surfaces
    uint32 _tracks_per_surface;     ///< number of tracks per surface
    uint32 _rpm;                    ///< rotations per minute
    uint32 _sector_size;            ///< number of bytes per sector
    double _seek_overhead;          ///< seek overhead
    double _seek_per_track;         ///< seek time per track the head is moved
    bool   _verbose;                ///< toggle verbose output
    BlockCache *_cache;             ///< disk cache
    uint32 _head_pos;               ///< current position (track) of r/w heads.
    // TODO add more fields as necessary
    uint32 _sectors_innermost_track;
    uint32 _sectors_outermost_track;
    uint32 _surface_pos;

    

    /// @brief translate a block index into a position on the HDD
    /// @param block block index
    /// @param pos (output) pointer to result
    /// @retval true if translation was successful, false otherwise
    bool   decode(uint64 block, HDD_Position *pos) const;

    /*Sectors_track: return number of sectors in the track num_track
    num_track is the numero of the track the innermost_track having numero 0 ...*/
    uint64 sectors_track(uint32 num_track) const;
    ///Sectors_surface: return the number of sectors per surface
    uint64 sectors_surface(void) const;

    // add more protected methods as necessary
};

#endif // __CA_HDD_H__
