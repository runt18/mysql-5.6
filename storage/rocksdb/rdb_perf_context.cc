/*
   Portions Copyright (c) 2015-Present, Facebook, Inc.
   Portions Copyright (c) 2012, Monty Program Ab

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/* This C++ file's header file */
#include "./rdb_perf_context.h"

/* C++ system header files */
#include <string>

/* RocksDB header files */
#include "rocksdb/iostats_context.h"
#include "rocksdb/perf_context.h"

/* MyRocks header files */
#include "./ha_rocksdb_proto.h"

namespace myrocks {

// To add a new metric:
//   1. Update the PC enum in rdb_perf_context.h
//   2. Update sections (A), (B), and (C) below
//   3. Update perf_context.test and show_engine.test

std::string rdb_pc_stat_types[]=
{
  // (A) These should be in the same order as the PC enum
  "USER_KEY_COMPARISON_COUNT",
  "BLOCK_CACHE_HIT_COUNT",
  "BLOCK_READ_COUNT",
  "BLOCK_READ_BYTE",
  "BLOCK_READ_TIME",
  "BLOCK_CHECKSUM_TIME",
  "BLOCK_DECOMPRESS_TIME",
  "INTERNAL_KEY_SKIPPED_COUNT",
  "INTERNAL_DELETE_SKIPPED_COUNT",
  "GET_SNAPSHOT_TIME",
  "GET_FROM_MEMTABLE_TIME",
  "GET_FROM_MEMTABLE_COUNT",
  "GET_POST_PROCESS_TIME",
  "GET_FROM_OUTPUT_FILES_TIME",
  "SEEK_ON_MEMTABLE_TIME",
  "SEEK_ON_MEMTABLE_COUNT",
  "SEEK_CHILD_SEEK_TIME",
  "SEEK_CHILD_SEEK_COUNT",
  "SEEK_IN_HEAP_TIME",
  "SEEK_INTERNAL_SEEK_TIME",
  "FIND_NEXT_USER_ENTRY_TIME",
  "WRITE_WAL_TIME",
  "WRITE_MEMTABLE_TIME",
  "WRITE_DELAY_TIME",
  "WRITE_PRE_AND_POST_PROCESS_TIME",
  "DB_MUTEX_LOCK_NANOS",
  "DB_CONDITION_WAIT_NANOS",
  "MERGE_OPERATOR_TIME_NANOS",
  "READ_INDEX_BLOCK_NANOS",
  "READ_FILTER_BLOCK_NANOS",
  "NEW_TABLE_BLOCK_ITER_NANOS",
  "NEW_TABLE_ITERATOR_NANOS",
  "BLOCK_SEEK_NANOS",
  "FIND_TABLE_NANOS",
  "IO_THREAD_POOL_ID",
  "IO_BYTES_WRITTEN",
  "IO_BYTES_READ",
  "IO_OPEN_NANOS",
  "IO_ALLOCATE_NANOS",
  "IO_WRITE_NANOS",
  "IO_READ_NANOS",
  "IO_RANGE_SYNC_NANOS",
  "IO_LOGGER_NANOS"
};

#define IO_PERF_INIT(_field_) \
  m_value[idx++]= rocksdb::perf_context._field_
#define IO_STAT_INIT(_field_) \
  m_value[idx++]= rocksdb::iostats_context._field_

void Rdb_perf_counters::start()
{
  // (B) These should be in the same order as the PC enum
  size_t idx= 0;
  IO_PERF_INIT(user_key_comparison_count);
  IO_PERF_INIT(block_cache_hit_count);
  IO_PERF_INIT(block_read_count);
  IO_PERF_INIT(block_read_byte);
  IO_PERF_INIT(block_read_time);
  IO_PERF_INIT(block_checksum_time);
  IO_PERF_INIT(block_decompress_time);
  IO_PERF_INIT(internal_key_skipped_count);
  IO_PERF_INIT(internal_delete_skipped_count);
  IO_PERF_INIT(get_snapshot_time);
  IO_PERF_INIT(get_from_memtable_time);
  IO_PERF_INIT(get_from_memtable_count);
  IO_PERF_INIT(get_post_process_time);
  IO_PERF_INIT(get_from_output_files_time);
  IO_PERF_INIT(seek_on_memtable_time);
  IO_PERF_INIT(seek_on_memtable_count);
  IO_PERF_INIT(seek_child_seek_time);
  IO_PERF_INIT(seek_child_seek_count);
  IO_PERF_INIT(seek_min_heap_time);
  IO_PERF_INIT(seek_internal_seek_time);
  IO_PERF_INIT(find_next_user_entry_time);
  IO_PERF_INIT(write_wal_time);
  IO_PERF_INIT(write_memtable_time);
  IO_PERF_INIT(write_delay_time);
  IO_PERF_INIT(write_pre_and_post_process_time);
  IO_PERF_INIT(db_mutex_lock_nanos);
  IO_PERF_INIT(db_condition_wait_nanos);
  IO_PERF_INIT(merge_operator_time_nanos);
  IO_PERF_INIT(read_index_block_nanos);
  IO_PERF_INIT(read_filter_block_nanos);
  IO_PERF_INIT(new_table_block_iter_nanos);
  IO_PERF_INIT(new_table_iterator_nanos);
  IO_PERF_INIT(block_seek_nanos);
  IO_PERF_INIT(find_table_nanos);
  IO_STAT_INIT(thread_pool_id);
  IO_STAT_INIT(bytes_written);
  IO_STAT_INIT(bytes_read);
  IO_STAT_INIT(open_nanos);
  IO_STAT_INIT(allocate_nanos);
  IO_STAT_INIT(write_nanos);
  IO_STAT_INIT(read_nanos);
  IO_STAT_INIT(range_sync_nanos);
  IO_STAT_INIT(logger_nanos);
}

#define IO_PERF_DIFF(_field_)                                        \
  m_value[idx]= rocksdb::perf_context._field_ - m_value[idx];        \
  idx++
#define IO_STAT_DIFF(_field_)                                        \
  m_value[idx]= rocksdb::iostats_context._field_ - m_value[idx];     \
  idx++

void Rdb_perf_counters::harvest_diffs()
{
  // (C) These should be in the same order as the PC enum
  size_t idx= 0;
  IO_PERF_DIFF(user_key_comparison_count);
  IO_PERF_DIFF(block_cache_hit_count);
  IO_PERF_DIFF(block_read_count);
  IO_PERF_DIFF(block_read_byte);
  IO_PERF_DIFF(block_read_time);
  IO_PERF_DIFF(block_checksum_time);
  IO_PERF_DIFF(block_decompress_time);
  IO_PERF_DIFF(internal_key_skipped_count);
  IO_PERF_DIFF(internal_delete_skipped_count);
  IO_PERF_DIFF(get_snapshot_time);
  IO_PERF_DIFF(get_from_memtable_time);
  IO_PERF_DIFF(get_from_memtable_count);
  IO_PERF_DIFF(get_post_process_time);
  IO_PERF_DIFF(get_from_output_files_time);
  IO_PERF_DIFF(seek_on_memtable_time);
  IO_PERF_DIFF(seek_on_memtable_count);
  IO_PERF_DIFF(seek_child_seek_time);
  IO_PERF_DIFF(seek_child_seek_count);
  IO_PERF_DIFF(seek_min_heap_time);
  IO_PERF_DIFF(seek_internal_seek_time);
  IO_PERF_DIFF(find_next_user_entry_time);
  IO_PERF_DIFF(write_wal_time);
  IO_PERF_DIFF(write_memtable_time);
  IO_PERF_DIFF(write_delay_time);
  IO_PERF_DIFF(write_pre_and_post_process_time);
  IO_PERF_DIFF(db_mutex_lock_nanos);
  IO_PERF_DIFF(db_condition_wait_nanos);
  IO_PERF_DIFF(merge_operator_time_nanos);
  IO_PERF_DIFF(read_index_block_nanos);
  IO_PERF_DIFF(read_filter_block_nanos);
  IO_PERF_DIFF(new_table_block_iter_nanos);
  IO_PERF_DIFF(new_table_iterator_nanos);
  IO_PERF_DIFF(block_seek_nanos);
  IO_PERF_DIFF(find_table_nanos);
  IO_STAT_DIFF(thread_pool_id);
  IO_STAT_DIFF(bytes_written);
  IO_STAT_DIFF(bytes_read);
  IO_STAT_DIFF(open_nanos);
  IO_STAT_DIFF(allocate_nanos);
  IO_STAT_DIFF(write_nanos);
  IO_STAT_DIFF(read_nanos);
  IO_STAT_DIFF(range_sync_nanos);
  IO_STAT_DIFF(logger_nanos);
}

#undef IO_PERF_INIT
#undef IO_STAT_INIT
#undef IO_PERF_DIFF
#undef IO_STAT_DIFF


static Rdb_atomic_perf_counters rdb_global_perf_counters;

void rdb_get_global_perf_counters(Rdb_perf_counters *counters)
{
  DBUG_ASSERT(counters != nullptr);

  counters->load(rdb_global_perf_counters);
}


void Rdb_perf_counters::end_and_record(
  Rdb_atomic_perf_counters *atomic_counters)
{
  harvest_diffs();

  for (int i= 0; i < PC_MAX_IDX; i++) {
    if (m_value[i]) {
      if (atomic_counters) {
        atomic_counters->m_value[i] += m_value[i];
      }
      rdb_global_perf_counters.m_value[i] += m_value[i];
    }
  }
}


void Rdb_perf_counters::load(const Rdb_atomic_perf_counters &atomic_counters)
{
  for (int i= 0; i < PC_MAX_IDX; i++) {
    m_value[i]= atomic_counters.m_value[i].load(std::memory_order_relaxed);
  }
}

void Rdb_io_perf::start(uint32_t perf_context_level)
{
  rocksdb::PerfLevel perf_level=
    static_cast<rocksdb::PerfLevel>(perf_context_level);

  rocksdb::SetPerfLevel(perf_level);

  if (perf_level == rocksdb::kDisable)
  {
    return;
  }

  m_block_read_byte= rocksdb::perf_context.block_read_byte;
  m_block_read_count= rocksdb::perf_context.block_read_count;
  m_block_read_time= rocksdb::perf_context.block_read_time;
  m_local_perf_context.start();
}

void Rdb_io_perf::end_and_record(uint32_t perf_context_level)
{
  rocksdb::PerfLevel perf_level=
    static_cast<rocksdb::PerfLevel>(perf_context_level);

  if (perf_level == rocksdb::kDisable)
  {
    return;
  }

  /*
    This seems to be needed to prevent gdb from crashing if it breaks
    or enters this function.
   */
  rocksdb::SetPerfLevel(perf_level);

  m_block_read_byte= rocksdb::perf_context.block_read_byte - m_block_read_byte;
  m_block_read_count= rocksdb::perf_context.block_read_count
                      - m_block_read_count;
  m_block_read_time= rocksdb::perf_context.block_read_time - m_block_read_time;
  m_local_perf_context.end_and_record(m_atomic_counters);

  if (m_block_read_byte + m_block_read_count +
      m_block_read_time != 0)
  {
    my_io_perf_t io_perf_read;

    my_io_perf_init(&io_perf_read);
    io_perf_read.bytes= m_block_read_byte;
    io_perf_read.requests= m_block_read_count;

    /*
      Rocksdb does not distinguish between I/O service and wait time, so just
      use svc time.
     */
    io_perf_read.svc_time_max= io_perf_read.svc_time= m_block_read_time;

    my_io_perf_sum_atomic_helper(m_shared_io_perf_read, &io_perf_read);
    my_io_perf_sum(&m_stats->table_io_perf_read, &io_perf_read);
  }

  if (m_local_perf_context.m_value[PC_KEY_SKIPPED])
  {
    m_stats->key_skipped += m_local_perf_context.m_value[PC_KEY_SKIPPED];
  }

  if (m_local_perf_context.m_value[PC_DELETE_SKIPPED])
  {
    m_stats->delete_skipped += m_local_perf_context.m_value[PC_DELETE_SKIPPED];
  }
}

}  // namespace myrocks
