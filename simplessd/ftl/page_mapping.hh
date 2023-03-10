/*
 * Copyright (C) 2017 CAMELab
 *
 * This file is part of SimpleSSD.
 *
 * SimpleSSD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimpleSSD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SimpleSSD.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __FTL_PAGE_MAPPING__
#define __FTL_PAGE_MAPPING__

#include <cinttypes>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <deque>

#include "ftl/abstract_ftl.hh"
#include "ftl/common/block.hh"
#include "ftl/ftl.hh"
#include "pal/pal.hh"

#include "ftl/error_modeling.hh"

#include "sim/engine.hh"

extern Engine engine;

namespace SimpleSSD {

namespace FTL {

class PageMapping : public AbstractFTL {
 private:
  PAL::PAL *pPAL;

  ConfigReader &conf;

  std::unordered_map<uint64_t, std::vector<std::pair<uint32_t, uint32_t>>>
      table;
  std::unordered_map<uint32_t, Block> blocks;
  std::list<Block> freeBlocks;
  uint32_t nFreeBlocks;  // For some libraries which std::list::size() is O(n)
  std::vector<uint32_t> lastFreeBlock;

  Bitset lastFreeBlockIOMap;
  uint32_t lastFreeBlockIndex;

  bool bReclaimMore;
  bool bRandomTweak;
  uint32_t bitsetSize;

  struct {
    uint64_t hotGcCount;
    uint64_t reclaimedHotBlocks;
    uint64_t hotValidSuperPageCopies;
    uint64_t hotValidPageCopies;

    uint64_t coldGcCount;
    uint64_t reclaimedColdBlocks;
    uint64_t coldValidSuperPageCopies;
    uint64_t coldValidPageCopies;

    uint64_t refreshCount;
    uint64_t refreshedBlocks;
    uint64_t refreshSuperPageCopies;
    uint64_t refreshPageCopies;
    uint64_t refreshCallCount;
    uint64_t layerCheckCount;
  } stat;

  uint64_t lastRefreshed;

  ErrorModeling errorModel;

  uint64_t refresh_period;

  Bitset insertedBlockCheck;
  std::vector< std::deque<uint32_t> > refreshQueues;
  std::vector< std::deque<uint32_t> > checkedQueues;

  // WARM
  std::unordered_map<uint32_t, Block> hotPoolBlocks;
  std::unordered_map<uint32_t, Block> coldPoolBlocks;
  std::unordered_map<uint32_t, Block> coolDownBlocks;

  std::list<Block> hotFreeBlocks;
  std::list<Block> coldFreeBlocks;
  
  uint32_t nHotFreeBlocks;
  uint32_t nColdFreeBlocks;
  uint32_t nCooldownBlocks;

  uint32_t hotBlocksLimit;
  uint32_t coldBlocksLimit;

  std::deque<uint32_t> hotWindow;
  std::deque<uint32_t> coolDownWindow;

  std::vector<uint32_t> lastHotFreeBlock;
  std::vector<uint32_t> lastColdFreeBlock;
  std::vector<uint32_t> lastCoolFreeBlock;

  float coldRatio;

  Bitset lastHotFreeBlockIOMap;
  Bitset lastColdFreeBlockIOMap;
  Bitset lastCoolFreeBlockIOMap;
  uint32_t lastHotFreeBlockIndex;
  uint32_t lastColdFreeBlockIndex;
  uint32_t lastCoolFreeBlockIndex;

  
  
  uint32_t cooldownIndex;   //Starting index of cooldown window in cold window

  std::deque< uint32_t > refreshTargetQueue; // For refresh mode 1
  
  //std::ofstream refreshStatFile;
  //std::ofstream gcStatFile;
  string gcStatFileName;
  string refreshStatFileName;

  void gcStatRecord(string, bool);
  void refreshStatRecord(string, bool);
  void gcStatRecord(string);
  void refreshStatRecord(string);

  // Refresh
  void refresh_event(uint64_t);
  void setRefreshPeriod(uint32_t, uint32_t, uint32_t);

  // WARM
  void hotWriteInternal(Request &, uint64_t &, bool = true);
  void coldWriteInternal(Request &, uint64_t &, bool = true);
  float hotFreeBlockRatio();
  float coldFreeBlockRatio();
  uint32_t getLastHotFreeBlock();
  uint32_t getLastColdFreeBlock();
  uint32_t getLastCoolFreeBlock();

  uint32_t getHotFreeBlock();
  uint32_t getColdFreeBlock(bool queueInsert);

  uint32_t getLastHotFreeBlock(Bitset &);
  uint32_t getLastColdFreeBlock(Bitset &);
  uint32_t getLastCoolFreeBlock(Bitset &);

  uint32_t getHotFreeBlock(uint32_t);
  uint32_t getColdFreeBlock(uint32_t, bool);

  void calculateColdVictimWeight(std::vector<std::pair<uint32_t, float>> &,
                             const EVICT_POLICY);
  void selectColdVictimBlock(std::vector<uint32_t> &, uint64_t &);
  void selectHotVictimBlock(std::vector<uint32_t> &, uint64_t &);


  float freeBlockRatio();
  uint32_t convertBlockIdx(uint32_t);
  uint32_t getFreeBlock(uint32_t);
  uint32_t getLastFreeBlock(Bitset &);
  void calculateVictimWeight(std::vector<std::pair<uint32_t, float>> &,
                             const EVICT_POLICY, uint64_t);
  void selectVictimBlock(std::vector<uint32_t> &, uint64_t &, std::vector<uint32_t> &);
  void doGarbageCollection(std::vector<uint32_t> &, uint64_t &, blockPoolType);

  float calculateWearLeveling();
  void calculateTotalPages(uint64_t &, uint64_t &);

  void readInternal(Request &, uint64_t &);
  void writeInternal(Request &, uint64_t &, bool = true);
  void trimInternal(Request &, uint64_t &);
  void eraseInternal(PAL::Request &, uint64_t &);

  // Old refresh
  void doRefresh(uint64_t &);
  void selectRefreshVictim(std::vector<uint32_t> &, uint64_t &);
  void calculateRefreshWeight(std::vector<std::pair<uint32_t, float>> &,
                            const REFRESH_POLICY, uint64_t);

  void refreshBlock(uint32_t, uint64_t &);
  
  float calculateAverageError();

 public:
  PageMapping(ConfigReader &, Parameter &, PAL::PAL *, DRAM::AbstractDRAM *);
  ~PageMapping();

  bool initialize() override;

  void read(Request &, uint64_t &) override;
  void write(Request &, uint64_t &) override;
  void trim(Request &, uint64_t &) override;

  void format(LPNRange &, uint64_t &) override;

  Status *getStatus(uint64_t, uint64_t) override;

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace FTL

}  // namespace SimpleSSD

#endif
