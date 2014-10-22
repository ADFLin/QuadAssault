#ifndef Renderer_h__
#define Renderer_h__

class IRenderer
{
public:
	IRenderer();
	virtual void init() = 0;

	static void initialize();
	static void cleanup();
	
private:
	IRenderer* mLink;
};

#endif // Renderer_h__