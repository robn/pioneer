#include "Gui.h"
#include "GuiPlasma.h"
#include "mtrand.h"

namespace Gui {

#define NUMCONSTS 18
#define MAXTEXSIZE 1024

#define PIx2 (M_PI*2.0)

MTRand rand;

// Globals
float aspectRatio;
float wide;
float high;
float c[NUMCONSTS];		// constant
float ct[NUMCONSTS];		// temporary value of constant
float cv[NUMCONSTS];		// velocity of constant
float ***position;
float ***plasma;
unsigned int tex;
int texsize = 256;
int plasmaWidth, plasmaHeight;
float texright, textop;
float *plasmamap;

// Parameters edited in the dialog box
int dZoom = 10;
int dFocus = 30;
int dSpeed = 20;
int dResolution = 25;

// Find absolute value and truncate to 1.0
#define fabstrunc(f) (f >= 0.0f ? (f <= 1.0f ? f : 1.0f) : (f >= -1.0f ? -f : 1.0f))

Plasma::Plasma(unsigned int width, unsigned int height) :
	Widget(),
	m_width(width),
	m_height(height)
{
	int i, j;

	aspectRatio = float (Gui::Screen::GetWidth()) / float (Gui::Screen::GetHeight());

	if (aspectRatio >= 1.0f) {
		wide = 30.0f / float (dZoom);

		high = wide / aspectRatio;
	} else {
		high = 30.0f / float (dZoom);

		wide = high * aspectRatio;
	}

	// Set resolution of plasma
	if (aspectRatio >= 1.0f)
		plasmaHeight = (dResolution * MAXTEXSIZE) / 100;
	else
		plasmaHeight = int (float (dResolution * MAXTEXSIZE) * aspectRatio * 0.01f);

	plasmaWidth = int (float (plasmaHeight) / aspectRatio);

	// Set resolution of texture
	texsize = 16;
	if (aspectRatio >= 1.0f)
		while (plasmaHeight > texsize)
			texsize *= 2;
	else
		while (plasmaWidth > texsize)
			texsize *= 2;

	// The "- 1" cuts off right and top edges to get rid of blending to black
	texright = float (plasmaHeight - 1) / float (texsize);
	textop = texright / aspectRatio;

	// Initialize memory and positions
	plasmamap = new float[texsize * texsize * 3];

	for (i = 0; i < texsize * texsize * 3; i++)
		plasmamap[i] = 0.0f;
	plasma = new float **[plasmaHeight];
	position = new float **[plasmaHeight];

	for (i = 0; i < plasmaHeight; i++) {
		plasma[i] = new float *[plasmaWidth];
		position[i] = new float *[plasmaWidth];

		for (j = 0; j < plasmaWidth; j++) {
			plasma[i][j] = new float[3];
			position[i][j] = new float[2];

			plasma[i][j][0] = 0.0f;
			plasma[i][j][1] = 0.0f;
			plasma[i][j][2] = 0.0f;
			position[i][j][0] = float (i * wide) / float (plasmaHeight - 1) - (wide * 0.5f);
			position[i][j][1] = float (j * high) / (float(plasmaHeight) / aspectRatio - 1.0f) - (high * 0.5f);
		}
	}
	// Initialize constants
	for (i = 0; i < NUMCONSTS; i++) {
		ct[i] = rand.Double (PIx2);
		cv[i] = rand.Double (0.005f * dSpeed) + 0.0001f;
	}

	// Make texture
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D (GL_TEXTURE_2D, 0, 3, texsize, texsize, 0, GL_RGB, GL_FLOAT, plasmamap);
	glEnable (GL_TEXTURE_2D);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	printf("plasma: %d\n", tex);
}

Plasma::~Plasma()
{
}

void Plasma::Draw()
{
	int i, j;
	float rgb[3];
	float temp;
	static float focus = float (dFocus) / 50.0f + 0.3f;
	static float maxdiff = 0.004f * float (dSpeed);
	static int index;

	// Update constants
	for (i = 0; i < NUMCONSTS; i++) {
		ct[i] += cv[i];
		if (ct[i] > PIx2)
			ct[i] -= PIx2;
		c[i] = sin (ct[i]) * focus;
	}

	// Update colors
	for (i = 0; i < plasmaHeight; i++) {
		for (j = 0; j < plasmaWidth; j++) {
			// Calculate vertex colors
			rgb[0] = plasma[i][j][0];
			rgb[1] = plasma[i][j][1];
			rgb[2] = plasma[i][j][2];
			plasma[i][j][0] = 0.7f * (c[0] * position[i][j][0] + c[1] * position[i][j][1]
						  + c[2] * (position[i][j][0] * position[i][j][0] + 1.0f)
						  + c[3] * position[i][j][0] * position[i][j][1]
						  + c[4] * rgb[1] + c[5] * rgb[2]);
			plasma[i][j][1] = 0.7f * (c[6] * position[i][j][0] + c[7] * position[i][j][1]
						  + c[8] * position[i][j][0] * position[i][j][0]
						  + c[9] * (position[i][j][1] * position[i][j][1] - 1.0f)
						  + c[10] * rgb[0] + c[11] * rgb[2]);
			plasma[i][j][2] = 0.7f * (c[12] * position[i][j][0] + c[13] * position[i][j][1]
						  + c[14] * (1.0f - position[i][j][0] * position[i][j][1])
						  + c[15] * position[i][j][1] * position[i][j][1]
						  + c[16] * rgb[0] + c[17] * rgb[1]);

			// Don't let the colors change too much
			temp = plasma[i][j][0] - rgb[0];
			if (temp > maxdiff)
				plasma[i][j][0] = rgb[0] + maxdiff;
			if (temp < -maxdiff)
				plasma[i][j][0] = rgb[0] - maxdiff;
			temp = plasma[i][j][1] - rgb[1];
			if (temp > maxdiff)
				plasma[i][j][1] = rgb[1] + maxdiff;
			if (temp < -maxdiff)
				plasma[i][j][1] = rgb[1] - maxdiff;
			temp = plasma[i][j][2] - rgb[2];
			if (temp > maxdiff)
				plasma[i][j][2] = rgb[2] + maxdiff;
			if (temp < -maxdiff)
				plasma[i][j][2] = rgb[2] - maxdiff;

			// Put colors into texture
			index = (i * texsize + j) * 3;
			plasmamap[index] = fabstrunc (plasma[i][j][0]);
			plasmamap[index + 1] = fabstrunc (plasma[i][j][1]);
			plasmamap[index + 2] = fabstrunc (plasma[i][j][2]);
		}
	}

	// Update texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, texsize);
	glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, plasmaWidth, plasmaHeight, GL_RGB, GL_FLOAT, plasmamap);

	float size[2];
	GetSize(size);

	glBegin (GL_QUADS);
	glTexCoord2f (0.0f, 0.0f);
		glVertex2f(0,size[1]);
	glTexCoord2f (0.0f, texright);
		glVertex2f(size[0],size[1]);
	glTexCoord2f (textop, 0.0f);
		glVertex2f(size[0],0);
	glTexCoord2f (textop, texright);
		glVertex2f(0,0);
	glEnd ();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void Plasma::GetSizeRequested(float size[2])
{
	size[0] = float(m_width);
	size[1] = float(m_height);
}

}
