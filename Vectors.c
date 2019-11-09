#define SIND sinDegrees
#define COSD cosDegrees
typedef struct _Vector{
	float angle;
	float distance;
}Vector;
typedef struct _Vector2{
	float x;
	float y;
}Vector2;
void initVector(float an, float dist, struct Vector *out){
out->angle=an;
out->distance=dist;
}
void initVector2(float x, float y, struct Vector2 *out){
out->x=x;
out->y=y;
}
float dist(float x, float y){
	return (sqrt(pow(x,2)+pow(y,2)));
}
float tan(float x){
	return (SIND(x)/COSD(x));
}
void Vec2xy(struct Vector v, struct Vector2 *out){
	v.angle+=90;
	v.angle=v.angle%360;
	float yval = tan(v.angle);
	float xval = 1.0;
	float mult = (v.distance/dist(yval,xval));
	xval*=mult;
	yval*=mult;
	out->x = xval;
	out->y = yval;
}
