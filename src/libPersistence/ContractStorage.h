/*
 * Copyright (C) 2019 Zilliqa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CONTRACTSTORAGE_H
#define CONTRACTSTORAGE_H

#include <json/json.h>
#include <leveldb/db.h>

#include "common/Constants.h"
#include "common/Singleton.h"
#include "depends/libDatabase/LevelDB.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "depends/libDatabase/OverlayDB.h"
#pragma GCC diagnostic pop

#include "depends/libTrie/TrieDB.h"

namespace Contract {

Index GetIndex(const dev::h160& address, const std::string& key);

class ContractStorage : public Singleton<ContractStorage> {
  LevelDB m_codeDB;

  LevelDB m_stateIndexDB;
  LevelDB m_stateDataDB;

  std::unordered_map<std::string, bytes> m_stateIndexMap;
  std::unordered_map<std::string, bytes> m_stateDataMap;

  std::unordered_map<std::string, bytes> t_stateIndexMap;
  std::unordered_map<std::string, bytes> t_stateDataMap;

  /// Set the indexes of all the states of an contract account
  bool SetContractStateIndexes(const dev::h160& address,
                               const std::vector<Index>& indexes, bool temp);

  /// Get the raw rlp string of the states of an account
  std::vector<bytes> GetContractStatesData(const dev::h160& address, bool temp);

  ContractStorage()
      : m_codeDB("contractCode"),
        m_stateIndexDB("contractStateIndex"),
        m_stateDataDB("contractStateData"){};

  ~ContractStorage() = default;

  Index GetNewIndex(const dev::h160& address, const std::string& key);

  bool CheckIndexExists(const Index& index);

 public:
  /// Returns the singleton ContractStorage instance.
  static ContractStorage& GetContractStorage() {
    static ContractStorage cs;
    return cs;
  }

  /// Adds a contract code to persistence
  bool PutContractCode(const dev::h160& address, const bytes& code);

  /// Adds contract codes to persistence in batch
  bool PutContractCodeBatch(
      const std::unordered_map<std::string, std::string>& batch);

  /// Get the desired code from persistence
  const bytes GetContractCode(const dev::h160& address);

  /// Delete the contract code in persistence
  bool DeleteContractCode(const dev::h160& address);

  /// Get the indexes of all the states of an contract account
  std::vector<Index> GetContractStateIndexes(const dev::h160& address,
                                             bool temp);

  /// Get the raw rlp string of the state by a index
  std::string GetContractStateData(const Index& index, bool temp);

  /// Put one's contract states in database
  bool PutContractState(const dev::h160& address,
                        const std::vector<StateEntry>& states,
                        dev::h256& stateHash, bool temp);

  bool PutContractState(const dev::h160& address,
                        const std::vector<std::pair<Index, bytes>>& entries,
                        dev::h256& stateHash, bool temp);

  bool CommitStateDB();

  void InitTempState();

  /// Get the json formatted data of the states for a contract account
  bool GetContractStateJson(const dev::h160& address,
                            std::pair<Json::Value, Json::Value>& roots,
                            uint32_t& scilla_version, bool temp);

  /// Get the state hash of a contract account
  dev::h256 GetContractStateHash(const dev::h160& address, bool temp);

  void Reset();
};

}  // namespace Contract

#endif  // CONTRACTSTORAGE_H
