#include "GameEdit.h"



bool PropData::getString( FString& str )
{
	if ( !mData )
		return false;

	switch( mType )
	{
	case PROP_INT:
		{
			int32 value;
			switch( mDataSize )
			{
			case 1: value = castValueT< int8  >(); break;
			case 2: value = castValueT< int16 >(); break;
			case 4: value = castValueT< int32 >(); break;
			}
			str.format( "%d" , value );
		}
		return true;
	case PROP_UINT:
		{
			uint32 value;
			switch( mDataSize )
			{
			case 1: value = castValueT< uint8  >(); break;
			case 2: value = castValueT< uint16 >(); break;
			case 4: value = castValueT< uint32 >(); break;
			}
			str.format( "%u" , value );
		}
		return true;
	case PROP_BOOL:  
		str = castValueT< bool >() ? "true" : "false"; setValue( str ); 
		return true;
	case PROP_FLOAT: 
		{
			str.format( "%f" , castValueT< float >() );
			//cut zero
			int len = strlen( str );
			if ( len )
			{
				char* c = &str[0] + len - 1;
				while( &str[0] != c )
				{ 
					if ( *c != '0' )
					{
						if ( *c == '.' )
							*c = 0;
						break;
					}
					--c; 
				}
			}
		}
		break;
	case PROP_STRING: 
		str = castValueT< String >().c_str();
		return true;
	case PROP_CTRL:   
		return static_cast< IPropCtrl* >( mData )->input( str );
	}

	return false;
}

bool PropData::setValue( char const* str )
{
	if ( !mData )
		return false;

	switch( mType )
	{

	case PROP_INT:
		{
			int value = ::atoi( str );
			switch( mDataSize )
			{
			case 1: castValueT< int8  >() = value;
			case 2: castValueT< int16 >() = value;
			case 4: castValueT< int32 >() = value;
			}
		}
		return true;
	case PROP_UINT:
		{
			int value = ::atoi( str );
			switch( mDataSize )
			{
			case 1: castValueT< uint8  >() = value;
			case 2: castValueT< uint16 >() = value;
			case 4: castValueT< uint32 >() = value;
			}
			
		}
		return true;
	case PROP_FLOAT: 
		castValueT< float >() = ::atof( str ); 
		return true;
	case PROP_BOOL:  
		castValueT< bool >() = strcmp( str , "true" ) ? true : false; 
		return true;
	case PROP_STRING:
		castValueT< String >() = str;
		return true;
	case PROP_CTRL:  
		return static_cast< IPropCtrl* >( mData )->output( str );
	}

	return false;
}

TextPropBase::Prop* TextPropBase::findProp( char const* name )
{
	for( PropVec::iterator iter = mProps.begin() , itEnd = mProps.end();
		iter != itEnd ; ++iter )
	{
		Prop& prop = *iter;
		if ( strcmp( prop.name , name ) == 0 )
			return &prop;
	}
	return NULL;
}
