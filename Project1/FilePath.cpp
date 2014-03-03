#include "Precompiled.h"
#include <cctype>       // tolower
#include <string>       //string container

// convert a file path to use Windows naming conventions
//void ToWindowsPath( std::string *path );

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
FilePath::FilePath( char const *path ) {
    // convert the path to standard windows format
    strcpy_s( m_path, path );
    ToWindowsPath( m_path );

    std::string stdPath = m_path;

    // extract directory and name with extension
	size_t lastBackslash = stdPath.rfind( '\\' );
	size_t directoryEnd = lastBackslash != -1 ? lastBackslash + 1 : 0;

	strcpy_s( m_directory, stdPath.substr( 0, directoryEnd ).c_str() );
	std::string nameWithExtension = stdPath.substr( directoryEnd, stdPath.size() - directoryEnd ).c_str();

	// separate the name from the extension
    size_t lengthWithExtensionLength = nameWithExtension.size();
	size_t lastPeriod = nameWithExtension.rfind( '.' );
	size_t nameEnd = ( lastPeriod != -1 ? lastPeriod : lengthWithExtensionLength );

	strcpy_s( m_name, nameWithExtension.substr( 0, nameEnd ).c_str() );
	strcpy_s( m_extension, nameWithExtension.substr( nameEnd, lengthWithExtensionLength - nameEnd ).c_str() );
}

/*------------------------------------------------------------------------------
copy constructor
------------------------------------------------------------------------------*/
FilePath::FilePath( FilePath const &filePath ) {
    memcpy( m_path, filePath.m_path, kMaxPathLength );
    memcpy( m_name, filePath.m_name, kMaxNameLength );
    memcpy( m_directory, filePath.m_directory, kMaxDirectoryLength );
    memcpy( m_extension, filePath.m_extension, kMaxExtensionLength );
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
FilePath& FilePath::operator=( FilePath const &filePath ) {
    memcpy( m_path, filePath.m_path, kMaxPathLength );
    memcpy( m_name, filePath.m_name, kMaxNameLength );
    memcpy( m_directory, filePath.m_directory, kMaxDirectoryLength );
    memcpy( m_extension, filePath.m_extension, kMaxExtensionLength );
    return *this;
}

/*------------------------------------------------------------------------------
convert a file path to use Windows naming conventions
------------------------------------------------------------------------------*/
void ToWindowsPath( char *path ) {
    char *pPath = path;
    char ch = *pPath;
    while( ch ) {
        *pPath = ( ch == '/' ? '\\' : (char)tolower( ch ) );
        ++pPath;
        ch = *pPath;
    }
}