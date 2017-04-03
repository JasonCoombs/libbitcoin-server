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
#include <bitcoin/server/messages/route.hpp>

#include <string>
#include <boost/functional/hash_fwd.hpp>
#include <bitcoin/bitcoin.hpp>

namespace libbitcoin {
namespace server {

route::route()
  : secure(false), delimited(false)
{
}

std::string route::display() const
{
    return "[" + encode_base16(address) + "]" +
        (delimited ? "[]" : "") + (secure ? " (secure)" : "");
}

bool route::operator==(const route& other) const
{
    return secure == other.secure
        && delimited == other.delimited
        && address == other.address;
}

} // namespace server
} // namespace libbitcoin
