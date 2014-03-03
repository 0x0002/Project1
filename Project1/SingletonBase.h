#ifndef SINGLETON_BASE_H
#define SINGLETON_BASE_H

#include <cstddef>			// intptr_t, NULL

/*------------------------------------------------------------------------------
classes derived from this type become singletons
usage:
class Example : public SingletonBase< Example > { ...
------------------------------------------------------------------------------*/
template < typename T >
class SingletonBase {
public:
	// return a pointer to the singleton instance
	inline static T* GetSingleton() {
		Assert( s_singletonInstance, "singleton doesn't exist" );
		return s_singletonInstance;
	}

	// true if a singleton instance exists
	inline static bool DoesSingletonExist() { return s_singletonInstance != NULL; }

protected:
	SingletonBase() {
		Assert( !s_singletonInstance, "singleton already exists" );
		
		// find the relative address of the subclass instance
		// cast a non-existent object at address 0x1 to types T and iSingleton<T>
		intptr_t addressT = reinterpret_cast< intptr_t >( reinterpret_cast< T* >( 0x1 ) );
		intptr_t addressSingletonT = reinterpret_cast< intptr_t >( static_cast< SingletonBase< T >* >( reinterpret_cast< T* >( 0x1 ) ) );

		// the difference might be non-zero if the subclass derives from more than just iSingleton
		intptr_t offset = addressT - addressSingletonT;
		
		// assign the singleton instance
		s_singletonInstance = reinterpret_cast< T* >( reinterpret_cast< intptr_t >( this ) + offset );
	}

	~SingletonBase(){
		// free the instance pointer
		s_singletonInstance = NULL;
	}

private:
	static T* s_singletonInstance;		// unique instance
};

template < typename T > T* SingletonBase< T >::s_singletonInstance = NULL;

#endif // ISINGLETON_H