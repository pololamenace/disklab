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

#include <cassert>
#include <limits>
#include <cmath>
#include <cstdlib>

#include <iostream>
#include <iomanip>

#include "cache.h"
using namespace std;

//------------------------------------------------------------------------------
// BlockCache
//
BlockCache::BlockCache(uint32 nblocks, bool verbose)
  : _nblocks(nblocks), _verbose(verbose)
{
  assert(nblocks >= 2);

  _hit = _miss = 0;

  // TODO

  //
  // print info
  //
  cout << "BlockCache: " << endl << dec
       << "  # cache blocks:              " << _nblocks << endl
       << endl;
}

BlockCache::~BlockCache(void)
{
  // TODO
}

uint32 BlockCache::size(void) const
{
  return _nblocks;
}

uint32 BlockCache::hits(void) const
{
  return _hit;
}

uint32 BlockCache::misses(void) const
{
  return _miss;
}

float BlockCache::miss_rate(void) const
{
  // TODO
  return 0.0;
}

void BlockCache::dump(void) const
{
  cout.precision(3);
  cout << "BlockCache::dump()" << endl << dec
       << "  #hit/miss:  " << _hit << " / " << _miss << endl
       << "  miss rate:  " << miss_rate()*100 << "%" << endl;
  // TODO: dump contents of cache
}

bool BlockCache::has(uint64 block) const
{
  // TODO
  return false;
}

bool BlockCache::get(uint64 block)
{
  // TODO
  return false;
}

void BlockCache::put(uint64 block)
{
  // TODO
}

