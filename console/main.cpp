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
#include <iostream>
#include <bitcoin/server.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <bitcoin/blockchain.hpp>
#include <bitcoin/network.hpp>
#include <bitcoin/server/server_node.hpp>
#include <bitcoin/server/settings.hpp>
#include "executor.hpp"

using namespace boost::filesystem;
using namespace boost::program_options;
using namespace bc;
using namespace bc::server;
using namespace bc::blockchain;
using namespace bc::chain;
using namespace bc::config;
using namespace bc::network;
using namespace bc::database;

libbitcoin::server::configuration configured;

BC_USE_LIBBITCOIN_MAIN

/**
 * Invoke this program with the raw arguments provided on the command line.
 * All console input and output streams for the application originate here.
 * @param argc  The number of elements in the argv array.
 * @param argv  The array of arguments, including the process.
 * @return      The numeric result to return via console exit.
 */
int bc::main(int argc, char* argv[])
{
    set_utf8_stdio();

    variables_map variables;
    server::parser sp(configured);

    const auto options = sp.load_options(&configured);
    const auto arguments = sp.load_arguments();
    
    // command_line_parser documentation:
    // https://www.boost.org/doc/libs/1_68_0/doc/html/program_options/tutorial.html
    auto command_parser = command_line_parser(argc, argv).options(options)
    /*.allow_unregistered()*/.positional(arguments);
    
    // Boost.ProgramOptions explained:
    // https://theboostcpplibraries.com/boost.program_options
    boost::program_options::store(command_parser.run(), variables);
    
    notify(variables);

    if (!configured.regtest && !configured.testnet)
    {
        configured.init(config::settings::mainnet);
    }
    else if (configured.testnet)
    {
        configured.init(config::settings::testnet);
    }
    else
    {
        // The regression testing network
        // https://github.com/libbitcoin/libbitcoin-server/wiki/Regtest-Configuration
        configured.init(config::settings::regtest);
    }
    
    try
    {
        auto settings = sp.load_settings(&configured);
        auto bfile = false;
        auto lenv = sp.load_environment(&configured);
        sp.load_environment_variables(variables, BS_ENVIRONMENT_VARIABLE_PREFIX, &lenv);
        
        // Don't load the rest if any of these options are specified.
        if (!sp.get_option(variables, BS_VERSION_VARIABLE) &&
            !sp.get_option(variables, BS_SETTINGS_VARIABLE) &&
            !sp.get_option(variables, BS_HELP_VARIABLE))
        {
            // Returns true if the settings were loaded from a file.
            bfile = sp.load_configuration_variables(variables, BS_CONFIG_VARIABLE, &settings);
        }
        
        // don't parse command-line parameters a second time
        auto command_parser_b = command_line_parser(argc, argv);
        command_parser_b.allow_unregistered().options(settings);
        
        boost::program_options::store(command_parser_b.run(), variables);
        
        notify(variables);
        
        // Clear the config file path if it wasn't used.
        if (!bfile)
            configured.configfile.clear();
    }
    catch (const boost::program_options::error& e)
    {
        // This is obtained from boost, which circumvents our localization.
        cerr << "Exception: " << config::parser::format_invalid_parameter(e.what()) << std::endl;
        return false;
    }
    
    executor host(sp, cin, cout, cerr);
    int ret = host.menu() ? console_result::okay : console_result::failure;
    
    return ret;
}
