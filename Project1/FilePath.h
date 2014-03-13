#ifndef FILE_PATH_H
#define FILE_PATH_H

// convert a file path to use Windows naming conventions
void ToWindowsPath( char *path );

// convert to wchar_t. this returns a temporary buffer only valid until the next call
wchar_t* ToWchar( char const *str );

/*------------------------------------------------------------------------------
file path wrapper
------------------------------------------------------------------------------*/
class FilePath {
public:
    // constructors
    FilePath() {}
	FilePath( char const *path );
    FilePath( FilePath const &filePath );

    // accessors
    inline char const* GetName() const { return m_name; }
	inline char const* GetExtension() const { return m_extension; }
	inline char const* GetPath() const { return m_path; }
	inline char const* GetDirectory() const { return m_directory; }

public:

    enum{
        kMaxPathLength = 260,
        kMaxNameLength = 260,
        kMaxDirectoryLength = 260,
        kMaxExtensionLength = 32, 
    };

private:
    char    m_path[ kMaxPathLength ];           // file path    
    char    m_name[ kMaxNameLength ];           // filename without extension
    char    m_directory[ kMaxDirectoryLength ]; // file directory
    char    m_extension[ kMaxExtensionLength ]; // extension (includes .)
};

#endif // FILE_PATH_H