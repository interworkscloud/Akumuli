/*
 * Copyright (c) 2013 Eugene Lazin <4lazin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */


#include <cstdio>
#include <cstdlib>
#include <string>
#include <memory>

#include "akumuli.h"
#include "storage.h"

using namespace Akumuli;

/** 
 * Object that extends a Database struct.
 * Can be used from "C" code.
 */
struct DatabaseImpl : public aku_Database
{
    std::string path_to_file_;
    bool debug_mode_;
    Storage storage_;

    // private fields
    DatabaseImpl(const aku_Config& config)
        : path_to_file_(config.path_to_file)
        , debug_mode_(config.debug_mode != 0)
        , storage_(config.path_to_file)
    {
    }

    void flush() {
        storage_.commit();
    }

    void add_sample(int32_t param_id, int32_t unix_timestamp, aku_MemRange value) {
        TimeStamp ts;
        ts.t.object = unix_timestamp;
        ts.t.server = 0;
        auto entry = Entry2(param_id, ts, value);
        storage_.write(entry);
    }

};

void aku_flush_database(aku_Database* db) {
    auto dbi = reinterpret_cast<DatabaseImpl*>(db);
    dbi->flush();
}


void aku_add_sample(aku_Database* db, int32_t param_id, int32_t unix_timestamp, aku_MemRange value) {
    auto dbi = reinterpret_cast<DatabaseImpl*>(db);
    dbi->add_sample(param_id, unix_timestamp, value);
}


aku_Database* aku_open_database(aku_Config config)
{
    aku_Database* ptr = new DatabaseImpl(config);
    return static_cast<aku_Database*>(ptr);
}

void aku_close_database(aku_Database* db)
{
    delete db;
}