#pragma once


class AmbientLight
{
public:
	AmbientLight();
	AmbientLight(int red, int green, int blue);
	AmbientLight(const AmbientLight& a);
	~AmbientLight();

	//Accessors and Mutators
	int GetRed() const;
	void SetRed(const int red);
	int GetGreen() const;
	void SetGreen(const int green);
	int GetBlue() const;
	void SetBlue(const int blue);

	void CalculateLightingAmmbient(AmbientLight& AmbientLightIntensity);

private:
	int _redI;
	int _greenI;
	int _blueI;

};