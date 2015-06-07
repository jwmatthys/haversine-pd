#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#define R 6372.8
#define TO_RAD (3.1415926536 / 180)

/* code for "haversine" pd class.  This takes two messages: floating-point
numbers, and "rats", and just prints something out for each message. */

#include "m_pd.h"

    /* the data structure for each copy of "haversine".  In this case we
    on;y need pd's obligatory header (of type t_object). */
typedef struct haversine
{
  t_object x_obj;
  t_float th1, ph1, th2, ph2;
  t_outlet *f_out;
} t_haversine;

    /* this is called back when haversine gets a "float" message (i.e., a
    number.) */
void haversine_float(t_haversine *x, t_floatarg f)
{
    double th1, ph1, th2, ph2;
    th1 = f;
    ph1 = x->ph1;
    th2 = x->th2;
    ph2 = x->ph2;
    double dx, dy, dz;
	ph1 -= ph2;
	ph1 *= TO_RAD, th1 *= TO_RAD, th2 *= TO_RAD;
 
	dz = sin(th1) - sin(th2);
	dx = cos(ph1) * cos(th1) - cos(th2);
	dy = sin(ph1) * cos(th1);
	outlet_float(x->f_out, asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R);
}

    /* this is a pointer to the class for "haversine", which is created in the
    "setup" routine below and used to create new ones in the "new" routine. */
t_class *haversine_class;

    /* this is called when a new "haversine" object is created. */
void *haversine_new(void)
{
    t_haversine *x = (t_haversine *)pd_new(haversine_class);
    post("Haversine Formula, with code from http://rosettacode.org/wiki/Haversine_formula");
    x->th1 = x->th2 = x->ph1 = x->ph2 = 0;
    floatinlet_new(&x->x_obj, &x->ph1);
    floatinlet_new(&x->x_obj, &x->th2);
    floatinlet_new(&x->x_obj, &x->ph2);
    x->f_out = outlet_new(&x->x_obj, &s_float);
    return (void *)x;
}

    /* this is called once at setup time, when this code is loaded into Pd. */
void haversine_setup(void)
{
    haversine_class = class_new(gensym("haversine"), (t_newmethod)haversine_new, 0,
    	sizeof(t_haversine), 0, 0);
    class_addfloat(haversine_class, haversine_float);
}

