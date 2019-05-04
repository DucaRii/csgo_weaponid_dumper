#include <Windows.h>
#include <fstream>
#include <iostream>

#include "external/vdf_parser/vdf_parser.h"
using namespace tyti;

int main()
{
    std::cout << "Hey! I hope you are having a great day!" << std::endl <<
		"I will now try dump the weapon ids for you!" << std::endl; 

	std::ifstream items_game( "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo\\scripts\\items\\items_game.txt" );
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

		/// Only add needed items
		if ( name.find( "weapon_" ) != std::string::npos &&
			 name.find( "case" ) == std::string::npos )
		{
			std::transform( name.begin(), name.end(), name.begin(), ::toupper );
			weapon_ids.push_back( std::make_pair( id, name ) );
		}

		//weapon_ids.push_back( std::make_pair( id, name ) );
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

	weapon_ids_log << "enum ClassIds \n{" << std::endl;
	for ( const auto& item : weapon_ids )
	{
		weapon_ids_log << "\t" << item.second << " = " << item.first << std::endl;
	}
	weapon_ids_log << "};";
	weapon_ids_log.close();

	std::cout << "Done :)";

	Sleep( 5000 );
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
