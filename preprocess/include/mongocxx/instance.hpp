// Copyright 2014 MongoDB Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <memory>

#include <mongocxx/config/prelude.hpp>

namespace mongocxx {
MONGOCXX_INLINE_NAMESPACE_BEGIN

class logger;

///
/// Class representing an instance of the MongoDB driver.
///
/// Life cycle: A unique instance of the driver *MUST* be kept around.
///
class MONGOCXX_API instance {
   public:
    ///
    /// Creates an instance of the driver.
    ///
    instance();

    ///
    /// Creates an instance of the driver with a user provided log handler.
    ///  @param logger The logger that the driver will direct log messages to.
    ///
    /// @throws mongocxx::logic_error if an instance already exists.
    ///
    instance(std::unique_ptr<logger> logger);

    ///
    /// Move constructs an instance of the driver.
    ///
    instance(instance&&) noexcept;

    ///
    /// Move assigns an instance of the driver.
    ///
    instance& operator=(instance&&) noexcept;

    ///
    /// Destroys an instance of the driver.
    ///
    ~instance();

    ///
    /// Returns the current unique instance of the driver. If an instance was explicitly created,
    /// that will be returned. If no instance has yet been created, a default instance will be
    /// constructed and returned. If a default instance is constructed, its destruction will be
    /// sequenced according to the rules for the destruction of static local variables at program
    /// exit (see http://en.cppreference.com/w/cpp/utility/program/exit).
    ///
    /// Note that, if you need to configure the instance in any way (e.g. with a logger), you cannot
    /// use this method to cause the instance to be constructed. You must explicitly create an
    /// properly configured instance object. You can, however, use this method to obtain that
    /// configured instance object.
    ///
    /// @note This method is intended primarily for test authors, where managing the lifetime of the
    /// instance w.r.t. the test framework can be problematic.
    ///
    static instance& current();

   private:
    class MONGOCXX_PRIVATE impl;
    std::unique_ptr<impl> _impl;
};

MONGOCXX_INLINE_NAMESPACE_END
}  // namespace mongocxx

#include <mongocxx/config/postlude.hpp>