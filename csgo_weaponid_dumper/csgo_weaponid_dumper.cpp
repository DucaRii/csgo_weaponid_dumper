#include <Windows.h>
#include <fstream>
#include <iostream>

#include "external/vdf_parser/vdf_parser.h"
using namespace tyti;

#include "external/android_strings/android_strings.h"

int main()
{
	std::cout << "Hey! I hope you are having a great day!" << std::endl <<
		"I will now try dump the weapon ids for you!" << std::endl;

	std::ifstream items_game( "items_game.txt" );
	if ( !items_game.is_open() )
	{
		std::cout << "ERROR: Failed to open items_game.txt, make sure you placed the program in the right folder";
		return 0;
	}

	/// Parse the .vdf
	auto items_parsed = vdf::read( items_game );

	/// We can now close the filestream
	items_game.close();

	auto weapon_ids = std::vector<std::pair<int, std::string>>{};
	for ( const auto& index : items_parsed.childs[ "items" ]->childs )
	{
		const auto id = std::atoi( index.first.c_str() );

		/// Skip first item since it's a default item
		if ( id == 0 )
			continue;

		const auto inner = index.second;

		auto name = inner->attribs[ "name" ];
		const auto prefab_name = inner->attribs[ "prefab" ];

		/// Only add needed items
		if ( android::base::StartsWith( name, "weapon_" ) ||
			 android::base::StartsWith( prefab_name, "hands" ) )
		{
			/// Capitalize the name
			std::transform( name.begin(), name.end(), name.begin(), ::toupper );
			weapon_ids.push_back( std::make_pair( id, name ) );
		}
	}

	/// Sort weapon ids
	std::sort( weapon_ids.begin(), weapon_ids.end() );

	/// Open output file
	std::ofstream weapon_ids_log( "weapon-id_log.txt" );
	if ( !weapon_ids_log.is_open() )
	{
		std::cout << "ERROR: Failed to create log file";
		return 0;
	}

	weapon_ids_log << "enum WeaponIds \n{" << std::endl;
	for ( const auto& item : weapon_ids )
	{
		weapon_ids_log << "\t" << item.second << " = " << item.first << "," << std::endl;
	}
	weapon_ids_log << "};";
	weapon_ids_log.close();

	std::cout << "Done :)";
}
