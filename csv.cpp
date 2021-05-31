#include "csv.h"

CSVReader::CSVReader( std::string filename ) : fin( filename ) {
	while ( !fin.eof() )
		table.push_back( getParsedLine() );
}

std::vector< std::string > CSVReader::getParsedLine() {
	std::string plainLine;
	getline( fin, plainLine );
	std::istringstream lineStream( plainLine );
	std::vector< std::string > ret;
	std::string cell;
	while ( lineStream ) {
		if ( lineStream.peek() == '"' ) {
			char trash;
			lineStream >> trash;
			getline( lineStream, cell, '"' );
		}
		else {
			getline( lineStream, cell, ',' );
		}
		// std::cout << cell << "- -";	//just for dubugging
		ret.push_back( cell );
	}
	return ret;
}

std::vector< std::string > CSVReader::getRow( int rowNum ) {
	return table.at( rowNum );
}

std::vector< std::string > CSVReader::getCol( int colNum ) {
	std::vector< std::string > ret;
	for ( auto it : table )
		ret.push_back( it.at( colNum ) );
	return ret;
}

std::string CSVReader::getCell( int rowNum, int colNum ) {
	return table.at( rowNum ).at( colNum );
}

CSVWriter::CSVWriter( const std::string& filename ) : fout( filename ) {}

void CSVWriter::setRow( const int& rowNum, const std::vector< std::string >& row ) {
	table.at( rowNum ).clear();
	for ( auto& it : row )
		table.at( rowNum ).push_back( it );
}

void CSVWriter::setCol( const int& colNum, const std::vector< std::string >& col ) {
	if ( col.size() != table.size() )
		throw std::invalid_argument( "CSVWriter: argument size is not equal to CSVWriter::table." );
	for ( int i = 0; i < table.size(); i++ ) {
		table.at( i ).at( colNum ) = col.at( colNum );
	}
}

void CSVWriter::setCell( const int& rowNum, const int& colNum, const std::string& content ) {
	table.at( rowNum ).at( colNum ) = content;
}

void CSVWriter::writeFile() {
	std::string line;
	for ( auto& row : table ) {
		line = produceLine( row );
		fout << line << std::endl;
	}
}

std::string CSVWriter::produceLine( const std::vector< std::string >& row ) {
	std::string rawLine = "";
	for ( auto& it : row ) {
		if ( !rawLine.empty() )
			rawLine += ",";
		if ( it.find( ',' ) != std::string::npos )
			rawLine = "\"" + it + "\"";
		else
			rawLine += it;
	}
	return rawLine;
}