AUX_OBJ=auxiliary/Time.o auxiliary/Filesystem.o auxiliary/Filehandle.o auxiliary/fpscounter.o auxiliary/Network.o auxiliary/Config.o
render_OBJ=render/Model.o render/OBJModel.o render/Texture.o render/Font.o render/Shader.o render/Program.o render/Camera.o render/Renderer.o
AUDIO_OBJ=audio/AudioObject.o audio/AudioSystem.o audio/Buffer.o audio/Effect.o audio/Source.o audio/StreamParser.o audio/StreamParserWav.o
GAME_OBJ=game/base/GameName.o game/base/GameServer.o game/base/GameClient.o game/base/entity/Player.o game/base/entity/Monster.o game/base/entity/Skull.o game/base/entity/Projectile.o game/base/entity/projectiles/Spoon.o game/base/Arena.o game/base/CollisionModel.o
INPUT_OBJ=input/InputHandler.o
CLIENT_OBJ=Client.o

CLIENT_TARGET=Client
LDFLAGS=`sdl2-config --libs` -lSDL2_net -lpng -lopengl32 -lglew32 -lfreetype -lphysfs -lopenal -static-libstdc++
CXX=g++
LD=g++
CXXFLAGS=-std=c++11 -Wall -Wno-comment `pkg-config --cflags freetype2 physfs openal` `sdl2-config --cflags`


all: $(CLIENT_TARGET)

Client: $(AUX_OBJ) $(render_OBJ) $(AUDIO_OBJ) $(INPUT_OBJ) $(GAME_OBJ) $(CLIENT_OBJ)
	$(LD) $(AUX_OBJ) $(render_OBJ) $(AUDIO_OBJ) $(INPUT_OBJ) $(GAME_OBJ) $(CLIENT_OBJ) -o ../$(CLIENT_TARGET) $(LDFLAGS)
	
debug: all
	CFLAGS="$(CFLAGS) -g -DDEBUG -Wall -Werror"

%.o: %.cpp
	$(CXX) $< -o $@ -c $(CXXFLAGS)

clean:
	rm -f $(AUX_OBJ) $(render_OBJ) $(AUDIO_OBJ) $(CLIENT_OBJ) $(INPUT_OBJ) $(GAME_OBJ) $(CLIENT_TARGET)

dist-clean: clean
	rm -f *~