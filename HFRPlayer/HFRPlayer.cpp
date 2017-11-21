// MAIN ENTRY HERE 

#include <sys/stat.h>
#include "Renderer.h"
#include "ConfigReader.h"
#include <exception>

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
	if (argc < 2) // no cmd line args provided
	{
		std::cout << "NO ARGS PROVIDED... READING CONFIG: ";
		if (!ConfigReader::ReadConfig("c:/Zapas/test.txt", cinf))
		{
			std::cout << " FAILED" << std::endl;
			std::cout << " HFR PLAYER WILL QUIT NOW." << std::endl;
			return 0;
		}
		else { std::cout << " OK" << std::endl;}
	}

	else // Args provided
	{
		std::string name = "";
		float fps = -1;
		std::cout << "ARGS PROVIDED..." << std::endl;
		if (argc > 1) {	name = std::string(args[1]); }
		if (argc > 2)
		{
			try
			{
				fps = std::stof(std::string(args[2]));
				std::cout << fps << std::endl;	
			}
			catch(std::exception& e)
			{
				fps = -1;
				std::cout << "Incorrect FPS argument --> NO FPS LIMIT WILL BE APPLIED!" << std::endl;
			}	
		}
		cinf = new ConfigInfo(std::string(args[1]), fps);
	}

	std::cout << "NAME BASE: " << cinf->NameBase << ", FPS REQUEST: " << cinf->FPS << std::endl;
	
   // Init OpenGL
	Renderer::Init(1920,1080,"FPS",true);
   
   std::vector<GLuint> v;
   if (!ReadInTextures(v, cinf->NameBase))
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

