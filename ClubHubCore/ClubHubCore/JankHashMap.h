#pragma once
template <class TKey, class TValue>
class JankHashMap
{
	TValue * arr;
	int * insertionLog;
	int * insertionIndex;
	int bucketCount;
	int numInserted;
	int toIndex( TKey key );
public:
	inline JankHashMap( int bucketCount = 97 );
	inline ~JankHashMap();
	inline bool has( TKey key );
	inline TValue get( TKey key );
	inline TValue getInserted( int index );
	inline int count();
	inline void put( TKey key, TValue val );
	inline TValue remove( TKey key );
	inline void clear();

};

template <class TKey, class TValue>
JankHashMap<TKey,TValue>::JankHashMap(int bucketCount)
{
	this->bucketCount = bucketCount;
	arr = new TValue[bucketCount];
	insertionLog = new int[bucketCount];
	insertionIndex = new int[bucketCount];
	numInserted = 0;
	for( int i = 0; i < bucketCount; i++ )
	{
		insertionIndex[i] = -1;
	}
}

template <class TKey, class TValue>
JankHashMap<TKey,TValue>::~JankHashMap(void)
{
	delete [] arr;
	delete [] insertionLog;
	delete [] insertionIndex;
}

template <class TKey, class TValue>
int JankHashMap<TKey,TValue>::toIndex(TKey key)
{
	int n = (int)key;
	int result = std::abs( n ) % bucketCount;
	if ( n < 0 && result != 0 )
		result = bucketCount - result;
	return result;
}

template <class TKey, class TValue>
bool JankHashMap<TKey,TValue>::has( TKey key )
{
	return ( insertionIndex[ toIndex( key ) ] != -1 );
}
template <class TKey, class TValue>
TValue JankHashMap<TKey,TValue>::get( TKey key )
{
	return arr[ toIndex( key ) ];
}
template <class TKey, class TValue>
TValue JankHashMap<TKey,TValue>::getInserted( int index )
{
	return arr[ insertionLog[ index ] ];
}
template <class TKey, class TValue>
int JankHashMap<TKey,TValue>::count()
{
	return numInserted;
}
template <class TKey, class TValue>
void JankHashMap<TKey,TValue>::put( TKey key, TValue val )
{
	int ind = toIndex( key );
	//std::cout << ind << std::endl;
	arr[ ind ] = val;
	if( insertionIndex[ ind ] == -1 )
	{
		insertionIndex[ ind ] = numInserted;
		insertionLog[ numInserted ] = ind;
		numInserted++;
	}
	else
		std::cout << "COLLISION ON PUT";
}
template <class TKey, class TValue>
TValue JankHashMap<TKey,TValue>::remove( TKey key )
{
	int ind = toIndex( key );
	if( insertionIndex[ ind ] != -1 )
	{
		numInserted--;
		insertionLog[ insertionIndex[ ind ] ] = insertionLog[ numInserted ];
		insertionIndex[ ind ] = -1;
	}
	else
		std::cout << "REMOVING SOMETHING NOT EXISTING";
	return arr[ind];
}

template <class TKey, class TValue>
void JankHashMap<TKey,TValue>::clear()
{
	numInserted = 0;
}
