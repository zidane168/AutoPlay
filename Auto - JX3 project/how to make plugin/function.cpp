bool GetConfigFileName ( char* pszName, size_t nBuffSize )
{
	char szConfigFilename[MAX_PATH] = "C:\\Helloworld";	
	
	// Read install dir from registry... we'll assume it succeeds.

	// Add on a backslash if it wasn't present in the registry value.
	// First, get a pointer to the terminating zero.
	char* pLastChar = strchr ( szConfigFilename, '\0' );

	// Now move it back one character.
	pLastChar--;  

	if ( *pLastChar != '\\' )
		strcat ( szConfigFilename, "\\" );

	// Add on the name of the config file.
	strcat ( szConfigFilename, "config.bin" );

	// If the caller's buffer is big enough, return the filename.
	if ( strlen ( szConfigFilename ) >= nBuffSize )
		return false;
	else
	{
		strcpy ( pszName, szConfigFilename );
		return true;
	}
}

bool GetConfigFileName ( wchar_t* pszName, size_t nBuffSize )
{
	wchar_t szConfigFilename[MAX_PATH] = L"C:\\你好吗\\Chúc em may mắn";	

	// Read install dir from registry... we'll assume it succeeds.

	// Add on a backslash if it wasn't present in the registry value.
	// First, get a pointer to the terminating zero.
	wchar_t* pLastChar = wcschr( szConfigFilename, '\0' );

	// now move it back one double-byte character
	// pLastChar = CharPrev(szConfigFilename, pLastChar);

	pLastChar--;  
	
	if ( *pLastChar != '\\' )
		wcscat ( szConfigFilename, L"\\" );

	// Add on the name of the config file.
	wcscat( szConfigFilename, L"config.bin" );

	// If the caller's buffer is big enough, return the filename.
	if ( wcslen( szConfigFilename ) >= nBuffSize )
		return false;
	else
	{
		wcscpy( pszName, szConfigFilename );
		return true;
	}
}