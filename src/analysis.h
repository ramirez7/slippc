
#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include <iostream>
#include <fstream>
#include <unistd.h>  //usleep
#include <math.h>    //sqrt

#include "enums.h"
#include "util.h"

const unsigned MAX_PUNISHES  = 255;    //Maximum number of punishes per player per game (increase later if needed)

namespace slip {

//Struct for storing basic punish infomations
struct Punish {
  unsigned num_moves = 0;  //WARNING: need to initialize this to avoid some awful OOB memory accesses
  unsigned start_frame;
  unsigned end_frame;
  float    start_pct;
  float    end_pct;
  uint8_t  last_move_id;
  int      opening;   // TODO: make an enum for this
  int      kill_dir;  //-1 = no kill, other directions as specified in Dir enum
};

//Struct for holding analysis data for a particular player within a game
struct AnalysisPlayer {
  unsigned     port;
  std::string  tag_player;
  std::string  tag_css;
  unsigned     char_id;
  std::string  char_name;
  unsigned     airdodges;
  unsigned     spotdodges;
  unsigned     rolls;
  unsigned     dashdances;
  unsigned     l_cancels_hit;
  unsigned     l_cancels_missed;
  unsigned     techs;
  unsigned     walltechs;
  unsigned     walljumps;
  unsigned     walltechjumps;
  unsigned     missed_techs;
  unsigned     ledge_grabs;
  unsigned     air_frames;
  unsigned     end_stocks;
  unsigned     end_pct;
  unsigned     wavedashes;
  unsigned     wavelands;
  unsigned     neutral_wins;
  unsigned     pokes;
  unsigned     counters;
  unsigned     dyn_counts[Dynamic::__LAST];  //Frame counts for player interaction dynamics
  Punish*      punishes;

  AnalysisPlayer() {
    punishes = new Punish[MAX_PUNISHES];
  }
  ~AnalysisPlayer() {
    delete punishes;
  }
};

//Struct for holding all analysis data within a game
struct Analysis {
  std::string     game_time;
  std::string     slippi_version;
  std::string     parser_version;
  std::string     analyzer_version;
  std::string     stage_name;
  unsigned        stage_id;
  unsigned        winner_port;
  unsigned        game_length;
  AnalysisPlayer* ap;

  bool           success;
  unsigned*      dynamics;  //Interaction dynamics on a per_frame basis

  Analysis() {
    ap = new AnalysisPlayer[2];
  }
  ~Analysis() {
    delete ap;
    delete dynamics;
  }

  std::string asJson();
  void save(const char* outfilename);
};

}

#endif /* ANALYSIS_H_ */
