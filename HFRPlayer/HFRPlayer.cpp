// MAIN ENTRY HERE 

#include <sys/stat.h>
#include "Renderer.h"
#include "ConfigReader.h"

bool FileExists(const std::string& name) 
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

bool ReadInTextures(std::vector<GLuint>& textures,const std::string& formant)
{
	unsigned counter = 0;
	std::string fileName = formant + std::to_string(counter) + ".png";
	std::cout << fileName << std::endl;

	//check if there is at least one file
	if(!FileExists(fileName)) return false;
	while (FileExists(fileName))
	{
		textures.push_back(GLTextureLoader::LoadTexture(fileName));
		counter++;
		fileName = formant + std::to_string(counter) + ".png";
	}

	return true;
}

int main(int argc, char** args)
{
	ConfigInfo* cinf = 0;
	//hhg
	bool status = ConfigReader::ReadConfig("c:/Zapas/test.txt", cinf);
	std::cout << "STATUS: " << status << std::endl;
	if (status)
	{
		std::cout << "NAME: " << cinf->NameBase << std::endl;
		std::cout << "FPS: " << cinf->FPS << std::endl;
	}
	int linger;
	std::cin >> linger;
	return 0;
	
   Renderer::Init(1920,1080,"test",true);


   /*GLuint texID = GLTextureLoader::LoadTexture("c:/Zapas/text.png");
   GLuint texID2 = GLTextureLoader::LoadTexture("c:/Zapas/text2.png");
   GLuint texID3 = GLTextureLoader::LoadTexture("c:/Zapas/text3.png");*/
   std::vector<GLuint> v;
  // v = { texID,texID2 ,texID3};
   if (!ReadInTextures(v, "text"))
   {
	   Renderer::Cleanup();
	   std::cout << "No file(s) found!" << std::endl;
	   return 0;
   }

   Renderer::SetPictures(v);
   Renderer::Run();

 
   glDeleteTextures(v.size(), &v[0]);
   
   return 0;
}

