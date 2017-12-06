
Preffered build config: Release x64 (x86 also available)

Command line args: HFRPlayer <Name Base> <FPS> <View Time> <Offset>
All arguments are optional.
If no arguments provided, application will try to read config file(config.txt).

Name Base: string defining the sequence of images to load. E.g. "C:\foo\a" will load "C:\foo\a<Offset>", "C:\foo\a<Offset+1>" etc.
FPS: desired FPS to display (using Gsync)
View Time: how many seconds should the video play (on loop) before the screen is dimmed. negative values result in no dimming
Offset: loading offset. By default, loading will start with <Name Base>0.png.