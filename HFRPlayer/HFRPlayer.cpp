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

//
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
	ConfigInfo* conf{ nullptr };
	if (argc < 2) // no cmd line args provided - READ CONFIG
	{
		std::cout << "NO ARGS PROVIDED... READING CONFIG: ";
		if (!ConfigReader::ReadConfig("c:/Zapas/config.txt", conf))
		{
			std::cout << " FAILED" << std::endl;
			std::cout << " HFR PLAYER WILL QUIT NOW." << std::endl;
			return 0;
		}
		else { std::cout << " OK" << std::endl;}
	}

	else // Args provided - USE PROVIDED ARGS
	{
		std::string name = "";
		float fps = -1;
		std::cout << "ARGS PROVIDED..." << std::endl;
		if (argc > 1) {	name = std::string(args[1]); } // name base (formant) provided
		if (argc > 2) // fps provided..
		{
			try // check if floating point number.
			{
				fps = std::stof(std::string(args[2]));
				std::cout << fps << std::endl;	
			}
			catch(std::exception& e) // not a float
			{
				fps = -1;
				std::cout << "Incorrect FPS argument --> NO FPS LIMIT WILL BE APPLIED!" << std::endl;
			}	
		}
		conf = new ConfigInfo(std::string(args[1]), fps);
	}

	std::cout << "NAME BASE: " << conf->NameBase << ", FPS REQUEST: " << conf->FPS << std::endl;
	
	///////////////  RENDERING STARTS HERE ///////////////////////


   // Init OpenGL
	Renderer::Init(1920,1080,"FPS",true); // arguments (resX, resY, Title, windowed or  fullscreen)
   
   //Read in images
   std::vector<GLuint> v;
   if (!ReadInTextures(v, conf->NameBase)) // if there is not a single image file - quit.
   {
	   Renderer::Cleanup();
	   std::cout << "No file(s) found!" << std::endl;
	   return 0;
   }

   // Pass images to the renderer
   Renderer::SetPictures(v);

   // Renderer loop(blocking)
   Renderer::Run();

   // free all textures
   glDeleteTextures(v.size(), &v[0]);
   
   return 0;
}

