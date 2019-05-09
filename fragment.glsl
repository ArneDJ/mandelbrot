#version 460 core

out vec4 fcolor;

uniform dvec2 screen;
uniform dvec2 center;
uniform double zoom;
uniform int itr;

void main(void)
{
	dvec2 z, c;
	double ratio = screen.x / screen.y;
	c.x = ratio * (gl_FragCoord.x / screen.x - 0.5);
	c.y = (gl_FragCoord.y / screen.y - 0.5);

	c.x /= zoom;
	c.y /= zoom;

	c.x += center.x;
	c.y += center.y;

	int n = 0;
	double abs = 0.0;
	while(abs < 2.0 && n < itr) {
		double x = (z.x * z.x - z.y * z.y) + c.x;
  		double y = (z.y * z.x + z.x * z.y) + c.y;
		
		z.x = x;
		z.y = y;

		abs = sqrt(x*x + y*y);
		n++;
	}
	
	float t = float(n) / float(itr);
	float r = 15.0 * (1.0 - t) * t * t * t;
	float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
	float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;
	
	fcolor = vec4(r, g, b, 1.0);
}
