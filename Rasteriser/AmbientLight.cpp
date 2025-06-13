#include "AmbientLight.h"

AmbientLight::AmbientLight()
{
	_redI = 0;
	_greenI = 0;
	_blueI = 0;
}

AmbientLight::AmbientLight(int red, int green, int blue)
{
	_redI = red;
	_greenI = green;
	_blueI = blue;
}

AmbientLight::AmbientLight(const AmbientLight& al)
{
	_redI = al.GetRed();
	_greenI = al.GetGreen();
	_blueI = al.GetBlue();
}

//Destructor
AmbientLight::~AmbientLight()
{

}

//Accessors and Mutators
int AmbientLight::GetRed() const
{
	return _redI;
}

void AmbientLight::SetRed(const int red)
{
	_redI = red;
}

int AmbientLight::GetGreen() const
{
	return _greenI;
}

void AmbientLight::SetGreen(const int green)
{
	_greenI = green;
}

int AmbientLight::GetBlue() const
{
	return _blueI;
}

void AmbientLight::SetBlue(const int blue)
{
	_blueI = blue;
}

void AmbientLight::CalculateLightingAmmbient(AmbientLight& AmbientLightIntensity)
{

}
