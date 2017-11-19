// HFRPlayer.cpp : Defines the entry point for the console application.
//


#include "Renderer.h"

int main()
{
   Renderer::Init(1920,1080,"test",true);
   Renderer::Run();
   return 0;
}

