/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_SERVER_PARSER_HPP
#define LIBBITCOIN_SERVER_PARSER_HPP

#include <ostream>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/server/define.hpp>
#include <bitcoin/server/configuration.hpp>

namespace libbitcoin {
namespace server {

/// Parse configurable values from environment variables, settings file, and
/// command line positional and non-positional options.
class BCS_API parser
  : public config::parser
{
public:
    parser(configuration&);

    /// Load command line options (named).
    options_metadata load_options(config::configuration *);

    /// Load command line arguments (positional).
    arguments_metadata load_arguments();

    /// Load configuration file settings.
    options_metadata load_settings(config::configuration *);

    /// Load environment variable settings.
    options_metadata load_environment(config::configuration *pconf);
};

} // namespace server
} // namespace libbitcoin

#endif
