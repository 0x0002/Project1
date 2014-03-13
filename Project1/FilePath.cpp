#include "Precompiled.h"
#include <cctype>       // tolower
#include <string>       // string container

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
    strcpy_s( m_path, filePath.m_path );
    strcpy_s( m_name, filePath.m_name );
    strcpy_s( m_directory, filePath.m_directory );
    strcpy_s( m_extension, filePath.m_extension );
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

/*------------------------------------------------------------------------------
convert to wchar_t. this returns a temporary buffer only valid until the next call
------------------------------------------------------------------------------*/
wchar_t* ToWchar( char const *str ) {
    static wchar_t buffer[ kTempTextLength ];
    mbstowcs( buffer, str, kTempTextLength - 1 );
    return buffer;
}