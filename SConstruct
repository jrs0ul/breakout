import os
import sys

OBJ_DIR = os.path.join(os.getcwd(), 'obj/');
env = Environment(ENV=os.environ,
                  CCFLAGS=['-O3','-Wall'],
                  OBJPREFIX = OBJ_DIR)


code = [
        "src/Ball.cpp",
        "src/BBullets.cpp",
        "src/gui/OnScreenBoard.cpp",
        "src/audio/OggStream.cpp",
        "src/audio/SoundSystem.cpp",
        "src/HighScore.cpp",
        "src/map.cpp",
        "src/Xml.cpp",
        "src/Padd.cpp",
        "src/Particles2D.cpp",
        "src/Image.cpp",
        "src/TextureLoader.cpp",
        "src/SDLVideo.cpp",
        "src/SysConfig.cpp",
        "src/Utils.cpp",
        "src/Matrix.cpp",
        "src/Usefull.cpp",
        "src/Vectors.cpp",
        "src/gui/Text.cpp",
        "src/CppSingleton.cpp",
        "src/mainer.cpp"
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

