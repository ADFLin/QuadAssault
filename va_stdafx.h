class sampler1D;
class sampler2D;
class sampler3D;


class samplerCUBE;
class texture;
class sampler_state;

class SamplerState;

class Texture1D;
class Texture2D;
class Texture3D;

struct float2
{
	float x,y;
	float r,g;
	float s,t;
};
struct float3 : float2
{
	float2 xy,st;
	float  z,b,p;
};

struct float4 : float3
{
	float3 xyz,rgb,stp;
	float  w,a,q;

	float2 zw;
	float3 rga;
};

class float3x3
{
	float3 operator[]( int );
};

class float4x3
{
	float3 operator[]( int );
};

class float3x4
{
	float4 operator[]( int );
};

struct float4x4
{
	float4 operator[]( int );
};


template< class T >
class Buffer
{
};

float4   tex2D( sampler2D , float2 );
float4   texCUBE( samplerCUBE , float3 );


float    dot( float4 , float4 );
vec3     cross( vec3 , vec3 );
float    length( float4 );

float4x4 mul( float4x4 , float4x4 );
float4   mul( float4   , float4x4 );
float3   mul( float3   , float3x3 );

float    saturate( float );
float3   reflect( float3 , float3 );
float3   normalize( float3 );

float4   lerp( float4 , float4 , float );
float4x4 transpose( float4x4 );


typedef float  half;
typedef float2 half2;
typedef float3 half3;
typedef float4 half4;
typedef float3x3 half3x3;
typedef float4x4 half4x4;
typedef float4x3 half4x3;

#define uniform
#define attribute
#define varying
#define discard

typedef float2 vec2;
typedef float3 vec3;
typedef float4 vec4;
typedef float3x3 mat3;
typedef float4x4 mat4;


//BUILT-IN CONSTANTS (7.4 p44) 
const int gl_MaxVertexUniformComponents; 
const int gl_MaxFragmentUniformComponents; 
const int gl_MaxVertexAttribs; 
const int gl_MaxVaryingFloats; 
const int gl_MaxDrawBuffers; 
const int gl_MaxTextureCoords; 
const int gl_MaxTextureUnits; 
const int gl_MaxTextureImageUnits; 
const int gl_MaxVertexTextureImageUnits; 
const int gl_MaxCombinedTextureImageUnits; 
const int gl_MaxLights; 
const int gl_MaxClipPlanes;

//VERTEX SHADER VARIABLES 

float4 ftransform();
float4 texture1D( sampler1D , float  );
float4 texture2D( sampler2D , float2 );
float4 texture3D( sampler3D , float3 );

//Attribute Inputs (7.3 p44) access=RO
attribute vec4 gl_Vertex;
attribute vec3 gl_Noraml;
attribute vec4 gl_Color;
attribute vec4 gl_SecondaryColor;
attribute vec4 gl_MultiTexCoord0;
attribute vec4 gl_MultiTexCoord1;
attribute vec4 gl_MultiTexCoord2;
attribute vec4 gl_MultiTexCoord3;
attribute vec4 gl_MultiTexCoord4;
attribute vec4 gl_MultiTexCoord5;
attribute vec4 gl_MultiTexCoord6;
attribute vec4 gl_MultiTexCoord7;
attribute vec4 gl_FogCoord;
//Special Output Variables (7.1 p42) access=RW 
vec4  gl_Position;  //shader must write 
float gl_PointSize;  //enable GL_VERTEX_PROGRAM_POINT_SIZE 
vec4  gl_ClipVertex; 
//Varying Outputs (7.6 p48) access=RW
varying vec4  gl_FrontColor;
varying vec4  gl_BackColor; //enable GL_VERTEX_PROGRAM_TWO_SIDE 
varying vec4  gl_FrontSecondaryColor; 
varying vec4  gl_BackSecondaryColor; 
varying vec4  gl_TexCoord[ gl_MaxTextureCoords ]; 
varying float gl_FogFragCoord;

//FRAGMENT SHADER VARIABLES 

//Varying Inputs (7.6 p48) access=RO 
varying vec4  gl_Color; 
varying vec4  gl_SecondaryColor; 
varying vec4  gl_TexCoord[ gl_MaxTextureCoords ];
varying float gl_FogFragCoord;
//Special Input Variables (7.2 p43) access=RO 
vec4 gl_FragCoord; //pixel coordinates 
bool gl_FrontFacing;
//Special Output Variables (7.2 p43) access=RW 
vec4  gl_FragColor; 
vec4  gl_FragData[ gl_MaxDrawBuffers ];
float gl_FragDepth; //DEFAULT=glFragCoord.z


//BUILT-IN UNIFORMs (7.5 p45) access=RO 
uniform mat4 gl_ModelViewMatrix; 
uniform mat4 gl_ModelViewProjectionMatrix; 
uniform mat4 gl_ProjectionMatrix; 
uniform mat4 gl_TextureMatrix[ gl_MaxTextureCoords ]; 

uniform mat4 gl_ModelViewMatrixInverse; 
uniform mat4 gl_ModelViewProjectionMatrixInverse; 
uniform mat4 gl_ProjectionMatrixInverse; 
uniform mat4 gl_TextureMatrixInverse[gl_MaxTextureCoords]; 

uniform mat4 gl_ModelViewMatrixTranspose; 
uniform mat4 gl_ModelViewProjectionMatrixTranspose; 
uniform mat4 gl_ProjectionMatrixTranspose; 
uniform mat4 gl_TextureMatrixTranspose[gl_MaxTextureCoords]; 

uniform mat4 gl_ModelViewMatrixInverseTranspose; 
uniform mat4 gl_ModelViewProjectionMatrixInverseTranspose; 
uniform mat4 gl_ProjectionMatrixInverseTranspose; 
uniform mat4 gl_TextureMatrixInverseTranspose[gl_MaxTextureCoords]; 

uniform mat3  gl_NormalMatrix; 
uniform float gl_NormalScale; 

struct gl_DepthRangeParameters { 
	float near; 
	float far; 
	float diff; 
}; 
uniform gl_DepthRangeParameters gl_DepthRange; 

struct gl_FogParameters { 
	vec4 color; 
	float density; 
	float start; 
	float end; 
	float scale; 
}; 
uniform gl_FogParameters gl_Fog; 

struct gl_LightSourceParameters { 
	vec4 ambient; 
	vec4 diffuse; 
	vec4 specular; 
	vec4 position; 
	vec4 halfVector; 
	vec3 spotDirection; 
	float spotExponent; 
	float spotCutoff; 
	float spotCosCutoff; 
	float constantAttenuation; 
	float linearAttenuation; 
	float quadraticAttenuation; 
}; 
uniform gl_LightSourceParameters gl_LightSource[ gl_MaxLights ];


struct gl_LightModelProductParams  
{    
	vec4 sceneColor; // Derived. Ecm + Acm * Acs  
};  


uniform gl_LightModelProductParams gl_FrontLightModelProduct;  
uniform gl_LightModelProductParams gl_BackLightModelProduct;      

struct gl_LightProductParams
{   
	vec4 ambient;    // Acm * Acli    
	vec4 diffuse;    // Dcm * Dcli   
	vec4 specular;   // Scm * Scli  
};  


uniform gl_LightProductParams gl_FrontLightProduct[gl_MaxLights];  
uniform gl_LightProductParams gl_BackLightProduct[gl_MaxLights];