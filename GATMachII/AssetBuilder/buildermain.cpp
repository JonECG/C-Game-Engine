#include <QtOpenGL\QGLWidget>

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h> 

struct FileHeld
{
	std::string fileName;
	std::string ext;
};

std::vector< FileHeld > getAllFilesInDirectory( std::string directory )
{
	std::vector<FileHeld> result;

	WIN32_FIND_DATA data;
	HANDLE h = FindFirstFile( ( directory + std::string( "*.*" ) ).c_str(),&data );
	
	if( h!=INVALID_HANDLE_VALUE ) 
	{
		do
		{
			char* nPtr = new char [lstrlen( data.cFileName ) + 1];
			for( int i = 0; i < lstrlen( data.cFileName ); i++ )
				nPtr[i] = char( data.cFileName[i] );
			nPtr[ lstrlen( data.cFileName ) ] = '\0';

			std::string full( nPtr );

			if( full.size() > 3 )
			{
				int divider = full.find_first_of( '.' );
				FileHeld held;
				held.fileName = full.substr( 0, divider );
				held.ext  = full.substr( divider+1, full.length()-(divider+1) );

				result.push_back( held );
			}

		} while(FindNextFile(h,&data));
	} 
	else 
		std::cout << "Error: No such folder." << std::endl;
	
	FindClose(h);

	return result;
}

bool needsReplace( std::string sourceFile, std::string destinationFile )
{
	bool result = true;

	struct stat destFileStat;
	if( stat(destinationFile.c_str(), &destFileStat) == 0 )
	{
		struct stat sourceFileStat;
		stat(sourceFile.c_str(), &sourceFileStat);
		result = sourceFileStat.st_mtime > destFileStat.st_mtime;
	} 

	return result;
}

void stringReplaceAll( std::string * source, std::string search, std::string replace )
{
	std::string::size_type n = 0;
	while ( ( n = source->find( search, n ) ) != std::string::npos )
	{
		source->replace( n, search.size(), replace );
		n += replace.size();
	}
}

int main( int argc, char * argv[] )
{
	//TEST
	/*argc = 4;
	char * what[] = {	"D:/Repositories/NewCPPWorkingRepo/GATMachII/AssetBuilder/AssetBuilder.exe",
						"D:/Repositories/NewCPPWorkingRepo/GATMachII/Assets/ ",
						"D:/Repositories/NewCPPWorkingRepo/GATMachII/Temp/Debug/Bin/Assets/ ",
						"D:/Repositories/NewCPPWorkingRepo/GATMachII/../ModelOBJtoBinary/ModelOBJtoBinary/bin/Debug/ModelOBJtoBinary.exe" };
	argv = what;*/
	//ENDTEST

	std::string converted[10];

	//for( int i = 0; i < argc; i++ )
	//{
		//converted[i] = std::string( argv[i] );
		//stringReplaceAll( &converted[i], "\\", "/" );
		//std::cout << "[" << converted[i] << "]" << std::endl;
	//}
	if( argc > 3 )
	{
		int updated = 0;
		std::string inputDir( argv[1] );
		inputDir = inputDir.substr( 0, inputDir.length() - 1  );
		std::string outputDir( argv[2] );
		outputDir = outputDir.substr( 0, outputDir.length() - 1  );
		std::string programLocation( argv[3] );
		auto var = getAllFilesInDirectory( inputDir );
		for( unsigned int i = 0; i < var.size(); i++ )
		{
			FileHeld held = var.at(i);
			QString extQ = QString( held.ext.c_str() ).toLower();

			if( extQ == "png" || extQ == "jpg" || extQ == "jpeg" )
			{
				if( needsReplace( inputDir + held.fileName + "." + held.ext, outputDir + held.fileName + std::string( ".glt" ) ) )
				{
					static QImage image;
					image.load( ( inputDir + held.fileName + "." + held.ext ).c_str() );
					image = QGLWidget::convertToGLFormat( image );

					static std::ofstream fileWrite;
					fileWrite.open( outputDir + held.fileName + std::string( ".glt" ), std::ios::binary | std::ios::out );

					int width = image.width();
					int height = image.height();
					fileWrite.write( reinterpret_cast<char *>(&width), sizeof( width ) );
					fileWrite.write( reinterpret_cast<char *>(&height), sizeof( height ) );
					fileWrite.write( reinterpret_cast<char *>(image.bits()), width*height*4 );

					fileWrite.close();
					updated++;
				}
			}
			else
			if( extQ == "wfo" || extQ == "obj" )
			{
				if( needsReplace( inputDir + held.fileName + "." + held.ext, outputDir + held.fileName + ".glmod" ) )
				{
					system( ( std::string( "call \"" + programLocation + "\" \"" + inputDir + held.fileName + "." + held.ext + "\" \"" + outputDir + held.fileName + ".glmod\"" ) ).c_str() );
					updated++;
				}
			}
			else
			{
				if( needsReplace( inputDir + held.fileName + "." + held.ext, outputDir + held.fileName + "." + held.ext ) )
				{
					CopyFile( ( inputDir + held.fileName + "." + held.ext ).c_str(), ( outputDir + held.fileName + "." + held.ext ).c_str(), false );
					updated++;
				}
			}
		}

		std::cout << "JONASSETBUILDER NUMUPDATED: " + std::to_string( updated ) + "; NUMUPTODATE: " + std::to_string( var.size()-updated ) <<std::endl;
	}

	return 0;
}