//#include "List.h"
//
//template<typename T>
//List<T>::List()
//{
//	count = 0;
//	size = 10;
//	data = nullptr;
//	resize();
//}
//
//template<typename T>
//List<T>::~List()
//{
//	delete data;
//}
//
//template<typename T>
//void List<T>::resize()
//{
//	T *newData = new T[size];
//
//	for( int i = 0; i < count; i++ )
//	{
//		newData[i] = data[i];
//	}
//
//	if (data != nullptr)
//		delete data;
//
//	data = newData;
//}
//
//template<typename T>
//void List<T>::add( T addedData )
//{
//	if ( count >= size )
//	{
//		size *= 2;
//		resize();
//	}
//
//	data[count] = addedData;
//	count++;
//}
//
//template<typename T>
//T List<T>::get( int index )
//{
//	return data[index];
//}
//
//template<typename T>
//T List<T>::remove( int index )
//{
//	T result = get( index );
//	for( int i = index; i < count - 1; i++ )
//	{
//		data[i] = data[i+1];
//	}
//	count--;
//	return result;
//}
//
//template<typename T>
//int List<T>::getCount()
//{
//	return count;
//}
//
//template<typename T>
//T& List<T>::operator[] (const int x) 
//{
//	return data[x];
//}
//
//template<typename T>
//void List<T>::clear()
//{
//	count = 0;
//}