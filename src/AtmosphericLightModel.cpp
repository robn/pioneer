// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "AtmosphericLightModel.h"

// setLighting: set renderer lights according to current position and sun
// positions. Original lighting is passed back in oldLights, oldAmbient, and
// should be reset after rendering with AtmosphericLightModel::ResetLighting.
void AtmosphericLightModel::SetLighting(Graphics::Renderer *r, const Camera *camera, std::vector<Graphics::Light> &oldLights, Color &oldAmbient) {
	std::vector<Graphics::Light> newLights;
	double ambient, direct;
	CalcLighting(ambient, direct, camera);
	const std::vector<Camera::LightSource> &lightSources = camera->GetLightSources();
	newLights.reserve(lightSources.size());
	oldLights.reserve(lightSources.size());
	for(size_t i = 0; i < lightSources.size(); i++) {
		Graphics::Light light(lightSources[i].GetLight());

		oldLights.push_back(light);

		const float intensity = direct * camera->ShadowedIntensity(i, this);

		Color c = light.GetDiffuse();
		Color cs = light.GetSpecular();
		c.r*=float(intensity);
		c.g*=float(intensity);
		c.b*=float(intensity);
		cs.r*=float(intensity);
		cs.g*=float(intensity);
		cs.b*=float(intensity);
		light.SetDiffuse(c);
		light.SetSpecular(cs);

		newLights.push_back(light);
	}

	oldAmbient = r->GetAmbientColor();
	r->SetAmbientColor(Color(ambient*255));
	r->SetLights(newLights.size(), &newLights[0]);
}

void AtmosphericLightModel::ResetLighting(Graphics::Renderer *r, const std::vector<Graphics::Light> &oldLights, const Color &oldAmbient) {
	// restore old lights
	if (!oldLights.empty())
		r->SetLights(oldLights.size(), &oldLights[0]);
	r->SetAmbientColor(oldAmbient);
}

// Calculates the ambiently and directly lit portions of the lighting model taking into account the atmosphere and sun positions at a given location
// 1. Calculates the amount of direct illumination available taking into account
//    * multiple suns
//    * sun positions relative to up direction i.e. light is dimmed as suns set
//    * Thickness of the atmosphere overhead i.e. as atmospheres get thicker light starts dimming earlier as sun sets, without atmosphere the light switches off at point of sunset
// 2. Calculates the split between ambient and directly lit portions taking into account
//    * Atmosphere density (optical thickness) of the sky dome overhead
//        as optical thickness increases the fraction of ambient light increases
//        this takes altitude into account automatically
//    * As suns set the split is biased towards ambient
void AtmosphericLightModel::CalcLighting(double &ambient, double &direct, const Camera *camera)
{
	const double minAmbient = 0.05;
	ambient = minAmbient;
	direct = 1.0;
	Body *astro = GetFrame()->GetBody();
	if ( ! (astro && astro->IsType(Object::PLANET)) )
		return;

	Planet *planet = static_cast<Planet*>(astro);

	// position relative to the rotating frame of the planet
	vector3d upDir = GetInterpPositionRelTo(planet->GetFrame());
	const double planetRadius = planet->GetSystemBody()->GetRadius();
	const double dist = std::max(planetRadius, upDir.Length());
	upDir = upDir.Normalized();

	double pressure, density;
	planet->GetAtmosphericState(dist, &pressure, &density);
	double surfaceDensity;
	Color cl;
	planet->GetSystemBody()->GetAtmosphereFlavor(&cl, &surfaceDensity);

	// approximate optical thickness fraction as fraction of density remaining relative to earths
	double opticalThicknessFraction = density/EARTH_ATMOSPHERE_SURFACE_DENSITY;

	// tweak optical thickness curve - lower exponent ==> higher altitude before ambient level drops
	// Commenting this out, since it leads to a sharp transition at
	// atmosphereRadius, where density is suddenly 0
	//opticalThicknessFraction = pow(std::max(0.00001,opticalThicknessFraction),0.15); //max needed to avoid 0^power

	if (opticalThicknessFraction < 0.0001)
		return;

	//step through all the lights and calculate contributions taking into account sun position
	double light = 0.0;
	double light_clamped = 0.0;

	const std::vector<Camera::LightSource> &lightSources = camera->GetLightSources();
	for(std::vector<Camera::LightSource>::const_iterator l = lightSources.begin();
			l != lightSources.end(); ++l) {
		double sunAngle;
		// calculate the extent the sun is towards zenith
		if (l->GetBody()){
			// relative to the rotating frame of the planet
			const vector3d lightDir = (l->GetBody()->GetInterpPositionRelTo(planet->GetFrame()).Normalized());
			sunAngle = lightDir.Dot(upDir);
		} else {
			// light is the default light for systems without lights
			sunAngle = 1.0;
		}

		const double critAngle = -sqrt(dist*dist-planetRadius*planetRadius)/dist;

		//0 to 1 as sunangle goes from critAngle to 1.0
		double sunAngle2 = (Clamp(sunAngle, critAngle, 1.0)-critAngle)/(1.0-critAngle);

		// angle at which light begins to fade on Earth
		const double surfaceStartAngle = 0.3;
		// angle at which sun set completes, which should be after sun has dipped below the horizon on Earth
		const double surfaceEndAngle = -0.18;

		const double start = std::min((surfaceStartAngle*opticalThicknessFraction),1.0);
		const double end = std::max((surfaceEndAngle*opticalThicknessFraction),-0.2);

		sunAngle = (Clamp(sunAngle-critAngle, end, start)-end)/(start-end);

		light += sunAngle;
		light_clamped += sunAngle2;
	}

	light_clamped /= lightSources.size();
	light /= lightSources.size();

	// brightness depends on optical depth and intensity of light from all the stars
	direct = 1.0 -  Clamp((1.0 - light),0.0,1.0) * Clamp(opticalThicknessFraction,0.0,1.0);

	// ambient light fraction
	// alter ratio between directly and ambiently lit portions towards ambiently lit as sun sets
	const double fraction = ( 0.2 + 0.8 * (1.0-light_clamped) ) * Clamp(opticalThicknessFraction,0.0,1.0);

	// fraction of light left over to be lit directly
	direct = (1.0-fraction)*direct;

	// scale ambient by amount of light
	ambient = fraction*(Clamp((light),0.0,1.0))*0.25;

	ambient = std::max(minAmbient, ambient);
}
