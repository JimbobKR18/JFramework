#ifndef __JFramework_Level_h_
#define __JFramework_Level_h_

class Level
{
private:
	std::string mName;
public:
	Level();
	Level(std::string const &aFilename);
	~Level();

	std::string GetName();
};

#endif
