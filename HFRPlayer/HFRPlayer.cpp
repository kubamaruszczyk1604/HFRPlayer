// MAIN ENTRY HERE 


#include "Renderer.h"

int main()
{
   Renderer::Init(1920,1080,"test",true);

   GLuint texID = GLTextureLoader::LoadTexture("c:/Zapas/text.png");
   GLuint texID2 = GLTextureLoader::LoadTexture("c:/Zapas/text2.png");
   GLuint texID3 = GLTextureLoader::LoadTexture("c:/Zapas/text3.png");
   std::vector<GLuint> v;
   v = { texID,texID2 ,texID3};
   Renderer::SetPictures(v);
   Renderer::Run();

 
   glDeleteTextures(v.size(), &v[0]);
   
   return 0;
}

