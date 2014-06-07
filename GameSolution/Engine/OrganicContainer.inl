template <class O>
OrganicContainer<O>::~OrganicContainer()
{
	for( int i = 0; i < size; i++ )
	{
		delete current[i];
	}
	delete current;
}

template <class O> 
void OrganicContainer<O>::draw( Core::Graphics& g ) const
{
	for( int i = 0; i < size; i++ )
	{
		if( current[i]->isAlive() )
			current[i]->draw( g );
	}
}

template <class O> 
void OrganicContainer<O>::draw( Core::Graphics& g, Matrix4& globalTransform ) const
{
	for( int i = 0; i < size; i++ )
	{
		if( current[i]->isAlive() )
			current[i]->draw( g, globalTransform );
	}
}

template <class O> 
void OrganicContainer<O>::update( float dt )
{
	for( int i = 0; i < size; i++ )
	{
		if( current[i]->isAlive() )
			current[i]->update( dt );
	}
}

template <class O> 
void OrganicContainer<O>::reset()
{
	head = 0;
	for( int i = 0; i < size; i++ )
	{
		current[i]->kill();
	}
}

template <class O> 
O** OrganicContainer<O>::getContained()
{
	return static_cast<O**>(current);
}

template <class O> 
int OrganicContainer<O>::getSize()
{
	return size;
}