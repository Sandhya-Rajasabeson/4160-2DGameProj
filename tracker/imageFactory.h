#include <string>
#include <vector>
#include <map>
#include "image.h"
#include "gameData.h"
#include "spriteSheet.h"

class ImageFactory {
public:

  //static ImageFactory* getInstance(); //Meyers singleton step. pointer to reference.
  static ImageFactory& getInstance();
  ~ImageFactory();

  Image* getImage(const std::string&);
  std::vector<Image*> getImages(const std::string&);

private:
  //static ImageFactory* instance; //instead of classwide, it's going to be in a function
  const Gamedata& gdata;

  std::map<std::string, SDL_Surface*> surfaces;
  std::map<std::string, SDL_Texture*> textures;
  std::map<std::string, Image*> images;

  std::map<std::string, std::vector<SDL_Surface*> > multiSurfaces;
  std::map<std::string, std::vector<SDL_Texture*> > multiTextures;
  std::map<std::string, std::vector<Image*> > multiImages;

  ImageFactory() :
    gdata( Gamedata::getInstance() ),
    surfaces(),
    textures(),
    images(),
    multiSurfaces(),
    multiTextures(),
    multiImages()
  {}
  ImageFactory(const ImageFactory&);
  ImageFactory& operator=(const ImageFactory&);
};
