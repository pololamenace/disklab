//------------------------------------------------------------------------------
/// @file
/// @brief test driver for storage simulation
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
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string.h>
#include <libgen.h>

#include "disk.h"
#include "hdd.h"
#include "cache.h"
using namespace std;

/// @brief trim whitespace in string s at both ends
///        Warning: modifies string in-place!
/// @retval trimmed string
static char* trim(char *s)
{
  if (s == NULL) return NULL;

  //
  // run through entire 0-terminated string and set
  // - start: to the first non-whitespace character
  // - end:   to the last non-whitespace character
  //
  char *start = NULL;
  char *end = NULL;
  char *p = s;

  while (*p != '\0') {
    bool white = (*p == ' ') || (*p == '\t');

    if ((start == NULL) && !white) start = p;
    if (!white) end = p;
    p++;
  }

  //
  // terminate string in-place
  //
  if (start == NULL) start = s;
  if (end != NULL) *(++end) = '\0';
  else *start = '\0';

  return start;
}

/// @brief read disk configuration parameters from configuration file
///        and return HDD disk instance
/// @param cfg path to configuration file
/// @retval HDD instance or NULL on failure
HDD* create_disk(const char *cfg)
{
  uint32 surfaces, tracks_per_surface, sectors_innermost, sectors_outermost,
         rpm, bytes_per_sector, cache_size;
  double seek_overhead, seek_per_track;
  bool   verbose;

  //
  // open HDD configuration file
  //
  ifstream in(cfg);
  if (!in.good()) {
    cout << "Cannot open configuration file '" << cfg << "'." << endl;
    return NULL;
  }

  //
  // read HDD parameters
  //
  in >> surfaces;
  in >> tracks_per_surface;
  in >> sectors_innermost;
  in >> sectors_outermost;
  in >> rpm;
  in >> bytes_per_sector;
  in >> seek_overhead;
  in >> seek_per_track;
  in >> cache_size;
  in >> verbose;

  if (!in.good()) {
    cout << "Error reading HDD parameters from configuration file." << endl;
    return NULL;
  }

  //
  // create new instance of HDD
  //
  return new HDD(
      surfaces, tracks_per_surface,
      sectors_innermost, sectors_outermost,
      rpm, bytes_per_sector,
      seek_overhead, seek_per_track,
      cache_size,
      verbose);
}

/// @brief print usage information. Does not return (exit with @retstat)
/// @param program program name (argv[0])
/// @param retstat program exit status
void help(char *program, int retstat)
{
  char *bn = basename(program);
  cout << "Usage: " << bn
         << " -c/--config <CONFIG FILE> [-t/--trace <TRACE FILE>]" << endl
       << endl
       << "Run disk simulation on TRACE FILE using the HDD configuration "
       << "specified in CONFIG FILE." << endl
       << "While the configuration must be specified, the trace is optional"
       << " (trace read from stdin if no file given)." << endl
       << endl
       << "Example: " << bn << " -c hdd.16tb.cfg -t trace.dat" << endl
       << endl;

  exit(retstat);
}

/// @brief parse command line arguments
/// @param argc number of command line parameters
/// @param argv array containing command line parameters
/// @param cfg [output] pointer to character array to hold path to config. file
/// @param trace [output] pointer to character array to hold path to trace file
void parse_arguments(int argc, char *argv[], char **cfg, char **trace)
{
  int i;
  *cfg = *trace = NULL;

  while (i < argc) {
    if ((strcmp(argv[i], "-c") == 0) || (strcmp(argv[i], "--config") == 0)) {
      i++;
      *cfg = argv[i];
    } else
    if ((strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--trace") == 0)) {
      i++;
      *trace = argv[i];
    } else
    if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
      help(argv[0], EXIT_SUCCESS);
    }
    if (i == argc) {
      cout << "Error: missing filename after " << argv[i-1] << " argument."
        << endl;
      help(argv[0], EXIT_FAILURE);
    }
    i++;
  }

  if (*cfg == NULL) {
    cout << "Error: missing configuration file." << endl;
    help(argv[0], EXIT_FAILURE);
  }
}

/// @brief program entry point
int main(int argc, char *argv[])
{
  //
  // parse command line and create HDD instance
  //
  char *config_fn, *trace_fn;

  parse_arguments(argc, argv, &config_fn, &trace_fn);

  HDD *hdd = create_disk(config_fn);
  if (hdd == NULL) return EXIT_FAILURE;

  //
  // standard tests
  //
  cout.precision(7);
  cout << "avg. seek time:    " << dec << fixed
       << hdd->seek_time(0, hdd->tracks_per_surface()/2) << endl
       << "seek 1 track:      " << dec << fixed << hdd->seek_time(0, 1) <<endl
       << "avg. rot. latency: " << dec << fixed << hdd->wait_time() << endl
       << "read 1 sector:     " << dec << fixed << hdd->read_time(1) << endl
       << "write 1 sector:    " << dec << fixed << hdd->write_time(1) << endl
       << "(all units in milliseconds)" << endl
       << endl;

  //
  // process requests from trace file
  //
  istream *in = &cin;
  if (trace_fn != NULL) in = new ifstream(trace_fn);
  else cout << "reading trace from stdin..." << endl << endl;

  #define CMT_SIZE 2048   ///< max. length of comment
  char comment[CMT_SIZE], *trimmed, rw;
  double t_in, t_out, t_tot;
  bool verbose = hdd->verbose();
  uint32 bps = hdd->bytes_per_sector(), rop = 0, wop = 0;
  uint64 address, length, block, nblocks;

  while (in->good()) {
    //
    // get next line from input trace
    //
    (*in) >> t_in >> rw >> address >> length;
    in->getline(comment, CMT_SIZE, '\n');
    trimmed = trim(comment);

    if (!in->good()) break;

    //
    // convert to address to block number, length to #blocks
    //
    block = address / bps;
    nblocks = (length + bps-1) / bps;

    //
    // print access info
    //
    cout.precision(7);
    if (*trimmed != '\0') cout << trim(comment) << endl;
    switch (rw) {
      case 'r': cout << "read "; break;
      case 'w': cout << "write"; break;
      default : cout << "error in input trace";
    }
    cout << "(" << setw(8) << block << ", " << setw(4) << nblocks << ") = ";
    cout.flush();

    //
    // access hdd
    //
    switch (rw) {
      case 'r': rop++; t_out = hdd->read(t_in, block, nblocks); break;
      case 'w': wop++; t_out = hdd->write(t_in, block, nblocks); break;
    }
    t_tot += t_out - t_in;

    //
    // print result
    //
    cout.precision(7);
    cout << t_out-t_in << " ms" << endl;
    if (verbose || (*trimmed != '\0')) cout << endl;
  }

  //
  // print summary
  //
  cout << endl << dec
       << "total time for " << rop+wop << " (read: " << rop << ", write: "
       << wop << ") operations: " << t_tot << " sec" << endl;
  const BlockCache* cache = hdd->cache();
  if (cache != NULL) {
    cout.precision(3);
    cout << "  cache (" << cache->size() << " blocks): " << cache->hits()
         << " hits, " << cache->misses() << " misses, miss rate: "
         << cache->miss_rate()*100 << "%" << endl;
  }
  cout << endl;

  //
  // cleanup & exit
  //
  delete hdd;
  if (in != &cin) delete in;

  return EXIT_SUCCESS;
}

