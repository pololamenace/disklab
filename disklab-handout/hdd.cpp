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

#include <cassert>
#include <limits>
#include <cmath>
#include <cstdlib>

#include <iostream>
#include <iomanip>

#include "hdd.h"
using namespace std;

//------------------------------------------------------------------------------
// HDD
//
HDD::HDD(uint32 surfaces, uint32 tracks_per_surface,
         uint32 sectors_innermost_track, uint32 sectors_outermost_track,
         uint32 rpm, uint32 sector_size,
         double seek_overhead, double seek_per_track,
         uint32 cache_blocks,
         bool verbose)
  : _surfaces(surfaces), _tracks_per_surface(tracks_per_surface), _rpm(rpm),
    _sector_size(sector_size), _seek_overhead(seek_overhead),
    _seek_per_track(seek_per_track), _verbose(verbose)
{
  // TODO
 /*surfaces;               
  _tracks_per_surface;     
  _rpm;             
  _sector_size;
  _seek_overhead;          ///< seek overhead
  _seek_per_track;         ///< seek time per track the head is moved
  _verbose;                ///< toggle verbose output
  _cache;             ///< disk cache*/
  _head_pos=0; // it is assumed that the head starts being above the track 0
  _surface_pos=0;
  _sectors_innermost_track=sectors_innermost_track;
  _sectors_outermost_track=sectors_outermost_track;


  //
  // print info
  //
  cout.precision(3);
  cout << "HDD: " << endl
       << "  surfaces:                  " << _surfaces << endl
       << "  tracks/surface:            " << _tracks_per_surface << endl
       << "  sect on innermost track:   " << sectors_innermost_track << endl
       << "  sect on outermost track:   " << sectors_outermost_track << endl
       << "  rpm:                       " << _rpm << endl
       << "  sector size:               " << _sector_size << endl
       << "  cache blocks:              " << cache_blocks << endl
       << endl;
       if (verbose) cout<<"capacity "<<dec<<(double)capacity()/pow(2.0,20.0)<< endl;
}

HDD::~HDD(void)
{
  // TODO
}

uint32 HDD::bytes_per_sector(void) const
{
  return _sector_size;
}

uint32 HDD::tracks_per_surface(void) const
{
  return _tracks_per_surface;
}

/**********************************************************************************/
/*
 */


   /*Sectors_track: return number of sectors in the track num_track
    num_track is the numero of the track the innermost_track having numero 0 ...*/
uint64 HDD::sectors_track(uint32 num_track) const
{
  return (_sectors_innermost_track) + num_track * (_sectors_outermost_track - _sectors_innermost_track)/(_tracks_per_surface-1);
}

/**********************************************************************************/
/*
 */
    ///Sectors_surface: return the number of sectors per surface
uint64 HDD::sectors_surface(void) const
{
  int i=0,sum=0;

  for(i=0;i<(_tracks_per_surface-1);i++){
    sum+=sectors_track(i);
  }

  return sum;
}

/**********************************************************************************/
/*
 */
uint64 HDD::capacity(void) const
{
  return _surfaces * sectors_surface()*_sector_size;
}

/**********************************************************************************/
/*
 */

const BlockCache* HDD::cache(void) const
{
  return _cache;
}

/**********************************************************************************/
/*
 */

bool HDD::verbose(void) const
{
  return _verbose;
}


/**********************************************************************************/
/*
 */
double HDD::seek_time(uint32 from_track, uint32 to_track) 
{
  //return the seek overhead plus the absolute value of from_track-to_track multiply by the time to move on one track.
  //since the difference between those two tracks is the number of tracks the head will pass on. 

  if(from_track==to_track)
  {
     return 0.0;
  }
  if(from_track>to_track)
  {
    return _seek_overhead + ((double)to_track - (double)from_track)*_seek_per_track;
  }
  return _seek_overhead + ((double)to_track - (double)from_track)*_seek_per_track;
}

/**********************************************************************************/
/*
 */
  //return the time for half a rotation 
  //rpm is a number of rotations per minutes, hence 60/rpm is the time in second to make a tour. 
 // this is a time in second, but it is equal to the display of disk-lab reference
double HDD::wait_time(void) 
{
  return ((60.0)/_rpm)/2.0;
}
/**********************************************************************************/
/*
 */
//Return the time to read a number of "sectors" sectors
// used head_pos as the track where those sectors are read 
//return the time necessary to do the portion of rotation we have to make in order to pass all those sectors under the head 

double HDD::read_time(uint64 sectors)
{
  
  uint64 nb_sectors_track=sectors_track(_head_pos);
  uint32 surface_buf;

  surface_buf=_surface_pos;
  _surface_pos=(sectors+_surface_pos)%_surfaces;

  return ((ceil((double)(sectors+surface_buf)/(double)_surfaces))/(double)nb_sectors_track )*60.0/(double)_rpm;

}

double HDD::write_time(uint64 sectors)
{
  // same than read_time
  uint64 nb_sectors_track=sectors_track(_head_pos);
  uint32 surface_buf;

 

  surface_buf=_surface_pos;
  _surface_pos=(sectors+_surface_pos)%_surfaces;
  
  return ((ceil((double)(sectors+surface_buf)/(double)_surfaces))/(double)nb_sectors_track )*60.0/(double)_rpm;

}

bool HDD::decode(uint64 block, HDD_Position *pos) const
{
  //parameters checking
  if(pos==NULL)
  {
    cout<<" HDD::decode; non allocated HDD_Position struct gave in argument"<<endl;
    
    return false;
  } 
  //block negative or greater than the number of blocks in the disk drive
  if(block<0 || block >=(sectors_surface()*_surfaces) )
  {
     cout<<" block is too big or negative"<<dec<<block<<endl; 
    return false;
  }




  //surface 
  pos->surface=block%_surfaces; 
 
  //to the find the track containing this block
  // It is equivalent to find the track for which the block is smaller or equal than the maximum block number of this track
  int i=1, sum=sectors_track(0);

  while(block>((sum*_surfaces)-1) && i<_tracks_per_surface)
  {

    sum+= sectors_track(i);
    i++;
  }

  if(i>=_tracks_per_surface) // useless since checking has already been done /////////////////////////////////////////////////////
  {
    cout<<"debug: block is too big"<<dec<<block<<endl; 
    return false;
  }
  pos->track=i-1;

  //block
  //find the block on the track with the same number 
  int cursor=(sum-sectors_track(i))*_surfaces+pos->surface; //this is the first sector of the track where the sector is.
  i=0;
  cout<<"debug: cursor is"<<dec<<cursor<<endl;
  while(block!=cursor && i< sectors_track(pos->track))
  {
    cursor+=_surfaces;
    i++;
  }
  if(i>=sectors_track(pos->track)) // useless since checking has already been done /////////////////////////////////////////////////////
  {
    cout<<"debug: pb with track found, nb block is"<<dec<<block<<endl; 
    return false;
  }
  pos->sector=i;

  //max sectors is the number of sectors between the sectors given in parameter and the end of the track
  // it is the number of sectors in the track minus the position of the given sector( which is count), and then multiply by the number of surfaces ;
  //actually we must add minus the surface of the block since, for example we are at the first block of surface 2 then the first block of surfaces 1-2 cannot be read
  pos->max_sectors=((sectors_track(pos->track)-pos->sector)*_surfaces)-pos->surface;


  //printing
  cout<< "HDD::decode("<<dec<<block<<")= surface "<<dec<<pos->surface
  <<"/ track "<<dec<<pos->track
  <<"/ sector "<<dec<<pos->sector
  <<"/ max_sectors "<<dec<<pos->max_sectors<<endl;
  return true;
}

/**********************************************************************************/
/*
 */
/*-get the position using decode(block)
  -modify head_pos to the position track;
  - return the timestamp + all the result of the previous functions returning a time ( except write_time)   call read_time with sectors, seek_time with head_pos and the position track 
 */

double HDD::read(double ts, uint64 block, uint64 nblocks)
{
  HDD_Position pos;
  uint64 read_track, sum=0;
  double seek_tim=0, read_tim=0, nb_track=0;

  if(!decode(block, &pos)) return -1.1; // a print is done is decode in case of return value is false
  /*init surface position */
  _surface_pos=pos.surface;

  seek_tim=seek_time(_head_pos, pos.track);
  if(nblocks-sum<pos.max_sectors) read_tim=read_time(nblocks); 
  else
  { 
    read_tim=read_time(pos.max_sectors);
  }
  //since we don't know the number of track that are going to be read, a loop is necessary
  while(nblocks-sum>pos.max_sectors)
  {
    //udpates 
    _head_pos=pos.track+nb_track;
    pos.track++;
    pos.sector=0;
     sum+=pos.max_sectors;
    pos.max_sectors= sectors_track(pos.track)*_surfaces;
   
    cout<<"whoooooooooo max sec"<<dec<<pos.max_sectors<<"et sum"<<dec<<sum<<"et nblock -sum" << dec<<nblocks-sum<<endl;


    if(nblocks-sum>pos.max_sectors) read_track=pos.max_sectors;// we read all we can in this track
    else {read_track=nblocks-sum;} // we read what we need on this track and the loop stop after this iteration

    seek_tim+=seek_time(_head_pos, pos.track);
    read_tim+=read_time(read_track);

    

    if(pos.track > _tracks_per_surface)
    {
      cout<<"DEBUG: HDD::read : you want to read too much"<<endl;
      return -1.2;
    }

  }
 /* cout<<"DEBUG: read_time"<<dec<<read_tim<<endl;*/

  return ts+seek_tim+read_tim+wait_time();
}


/**********************************************************************************/
/*
 */

double HDD::write(double ts, uint64 block, uint64 nblocks)
{
  // TODO
  return ts;
}

