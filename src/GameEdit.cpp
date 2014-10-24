#include "GameEdit.h"

#include <cassert>
#include <sstream>

char* cutFloatZero( char* str )
{
	int len = strlen( str );
	if ( len == 0 )
		return str;

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

	return c;
}



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
			case 1: value = cast< int8  >(); break;
			case 2: value = cast< int16 >(); break;
			case 4: value = cast< int32 >(); break;
			}
			str.format( "%d" , value );
		}
		return true;
	case PROP_UINT:
		{
			uint32 value;
			switch( mDataSize )
			{
			case 1: value = cast< uint8  >(); break;
			case 2: value = cast< uint16 >(); break;
			case 4: value = cast< uint32 >(); break;
			}
			str.format( "%u" , value );
		}
		return true;
	case PROP_BOOL:  
		str = cast< bool >() ? "true" : "false"; setValue( str ); 
		return true;
	case PROP_FLOAT: 
		{
			str.format( "%f" , cast< float >() );
			cutFloatZero( str );
		}
		return true;
	case PROP_VEC3F:
		{
			FString temp;
			temp.format( "%f" , cast< Vec3f >().x );
			cutFloatZero( temp );
			temp += temp;
			str.format( " %f" , cast< Vec3f >().y );
			cutFloatZero( temp );
			str += temp;
			temp.format( " %f" , cast< Vec3f >().z );
			cutFloatZero( temp );
			str += temp;
		}
		return true;
	case PROP_STRING: 
		str = cast< String >().c_str();
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
			case 1: cast< int8  >() = value;
			case 2: cast< int16 >() = value;
			case 4: cast< int32 >() = value;
			}
		}
		return true;
	case PROP_UINT:
		{
			int value = ::atoi( str );
			switch( mDataSize )
			{
			case 1: cast< uint8  >() = value;
			case 2: cast< uint16 >() = value;
			case 4: cast< uint32 >() = value;
			}	
		}
		return true;
	case PROP_FLOAT: 
		cast< float >() = ::atof( str ); 
		return true;
	case PROP_DOUBLE: 
		cast< double >() = ::atof( str ); 
		return true;
	case PROP_VEC3F:
		{
			Vec3f value;
			int num = ::scanf( str , "%f %f %f" , value.x , value.y , value.z );
			if ( num != 3 )
				return false;
			cast< Vec3f >() = value;
		}
		return true;
	case PROP_BOOL:  
		cast< bool >() = strcmp( str , "true" ) ? true : false; 
		return true;
	case PROP_STRING:
		cast< String >() = str;
		return true;
	case PROP_CTRL:  
		return static_cast< IPropCtrl* >( mData )->output( str );
	}

	return false;
}

TextPropEditor::Prop* TextPropEditor::findProp( char const* name )
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


void TextPropEditor::addPropData(char const* name , PropData const& data , unsigned flag)
{
	Prop prop;
	prop.name = name;
	prop.data = data;
	mProps.push_back( prop );
}


void TextPropEditor::addProp(char const* name , void* value , int sizeValue , int numSet , int const valueSet[] , char const* strSet[] , unsigned flag)
{
	Prop prop;
	prop.name = name;
	prop.data.setEnumData( value , sizeValue );
	mProps.push_back( prop );
}

void TextPropEditor::exportString( String& out )
{
	FString str;
	for( PropVec::iterator iter = mProps.begin() , itEnd = mProps.end(); 
		 iter != itEnd ; ++iter )
	{
		Prop& prop = *iter;
		if ( !prop.data.getString( str ) )
			continue;
		out += prop.name;
		out += " = \"";
		out += str;
		out += "\" ";
	}
}

void TextPropEditor::importString( char const* str )
{
	char buf[ 512 ];
	std::stringstream ss( str );

	while( ss.good() )
	{
		ss >> buf;
		Prop* prop = findProp( buf );
		ss >> buf[0];
		assert( buf[0] == '=' );
		ss >> buf[0];
		assert( buf[0] == '\"' );
		ss.getline( buf , ARRAY_SIZE( buf ) , '\"' );
		if ( prop )
		{
			prop->data.setValue( buf );
		}
	}
}

void TextPropEditor::setupPorp(IEditable& editable)
{
	mProps.clear();
	editable.enumProp( *this );
}
