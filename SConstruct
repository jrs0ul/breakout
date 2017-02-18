import os
import sys

OBJ_DIR = os.path.join(os.getcwd(), 'obj/');
env = Environment(ENV=os.environ,
                  CCFLAGS=['-O3','-Wall'],
                  OBJPREFIX = OBJ_DIR)


code = [
        "app/src/main/cpp/Ball.cpp",
        "app/src/main/cpp/BBullets.cpp",
        "app/src/main/cpp/gui/OnScreenBoard.cpp",
        "app/src/main/cpp/audio/OggStream.cpp",
        "app/src/main/cpp/audio/SoundSystem.cpp",
        "app/src/main/cpp/HighScore.cpp",
        "app/src/main/cpp/BreakOutMap.cpp",
        "app/src/main/cpp/Xml.cpp",
        "app/src/main/cpp/Padle.cpp",
        "app/src/main/cpp/Particles2D.cpp",
        "app/src/main/cpp/Image.cpp",
        "app/src/main/cpp/TextureLoader.cpp",
        "app/src/main/cpp/SDLVideo.cpp",
        "app/src/main/cpp/SysConfig.cpp",
        "app/src/main/cpp/Utils.cpp",
        "app/src/main/cpp/Matrix.cpp",
        "app/src/main/cpp/Usefull.cpp",
        "app/src/main/cpp/Vectors.cpp",
        "app/src/main/cpp/gui/Text.cpp",
        "app/src/main/cpp/CppSingleton.cpp",
        "app/src/main/cpp/mainer.cpp"
        ]

#Windblows
if sys.platform == 'win32':
    Tool('mingw')(env)
    env.Append(LIBS = ['mingw32','SDLmain', 'SDL', 'OpenGL32',
                       'openal32', 'vorbisfile', 'ogg'])
else: #Mac OS X
    if sys.platform == 'darwin':
        env['FRAMEWORKS'] = ['Cocoa', 'SDL', 'OPENGL', 'OpenAL','vorbis', 'Ogg']
        env.Append(LIBS = ['iconv', 'pthread'])
        env.Append(CCFLAGS = ['-arch', 'i386'])
        env.Object('src/osx/SDLMain.m', OBJPREFIX = OBJ_DIR,
                   CCFLAGS = ['-arch', 'i386', '-I/Library/Frameworks/SDL.framework/Headers/'])
        code.append('obj/SDLMain.o')
        env.Append(LINKFLAGS = ['-arch', 'i386'])
    else: #Linux
        env.Append(LIBS = ['SDL','GL', 'openal', 'vorbisfile',
                           'ogg', 'pthread'])


env.Program(target = "breakout", 
            source = code
            )

