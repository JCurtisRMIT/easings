/**
 easefunc: MaxMSP External
 
 J.Curtis RMIT University 2017
 Industrial Design (Honours)
 
 Open source under the BSD License.
 
 Copyright © 2001 Robert Penner
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 Neither the name of the author nor the names of contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 */

#include "ext.h"
#include "ext_obex.h"


//---------------------------------------------------------------------------

typedef struct _easings
{
    t_object    e_obj;
    double		e_val0;
    long		e_val1;
    long        e_val2;
    double      g_aval;
    double      g_bval;
    long        g_nval;
    double      g_cval;
    double      g_dval;
    t_object    *s_ui;
    void		*outlet;
    void        *outlet1;
} t_easings;


//---------------------------------------------------------------------------

void *easings_new(t_symbol *s, long argc, t_atom *argv);
void easings_assist(t_easings *x, void *b, long m, long a, char *s);
void easings_bang(t_easings *x);
void easings_ft1(t_easings *x, double ga);
void easings_ft2(t_easings *x, double gb);
void easings_ft3(t_easings *x, double gc);
void easings_ft4(t_easings *x, double gd);
void easings_in5(t_easings *x, long gn);
void easings_float(t_easings *x, double f);
//void easings_free(t_easings) // frees up proxy inlet


t_max_err easings_setattr_expr(t_easings *x, void *attr, long ac, t_atom *av);
t_max_err easings_getattr_expr(t_easings *x, void *attr, long ac, t_atom *av);
t_max_err easings_notify(t_easings *x, t_symbol *s, t_symbol *msg, void *sender, void *data, long ac, t_atom *av);


t_class *easings_class;


//---------------------------------------------------------------------------

void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("easings", (method)easings_new, (method)NULL, sizeof(t_easings), 0L, 0);
    class_addmethod(c, (method)easings_bang,    "bang",   0);
    //class_addmethod(c, (method)easings_int,     "int",    A_LONG, 0);
    class_addmethod(c, (method)easings_float,   "float",  A_FLOAT, 0);
    class_addmethod(c, (method)easings_float,   "float",  A_FLOAT, 0);
    
    class_addmethod(c, (method)easings_assist,  "assist", A_CANT, 0);
    class_addmethod(c, (method)easings_notify,  "notify", A_CANT, 0);
    
    class_addmethod(c, (method)easings_ft1, "ft1", A_FLOAT, 0); // Levin 'a'
    class_addmethod(c, (method)easings_ft2, "ft2", A_FLOAT, 0); // Levin 'b'
    class_addmethod(c, (method)easings_ft3, "ft3", A_FLOAT, 0); // Levin 'c'
    class_addmethod(c, (method)easings_ft4, "ft4", A_FLOAT, 0); // Levin 'd'
    class_addmethod(c, (method)easings_in5, "in5", A_LONG, 0); // Levin 'n'
    
    CLASS_ATTR_LONG(c, "easing", 0, t_easings, e_val1);
    CLASS_ATTR_CATEGORY(c, "easing", 0, "Easings");
    
    CLASS_ATTR_ORDER(c, "easing", 0, "1");
    CLASS_ATTR_LABEL(c, "easing", 0, "Easing Function");
    
    CLASS_ATTR_ENUMINDEX(c, "easing", 0, "\"Linear(No Easing)\" Quad(EaseIn) Quad(EaseOut) Quad(EaseInOut) Quad(EaseOutIn) Cubic(EaseIn) Cubic(EaseOut) Cubic(EaseInOut) Cubic(EaseOutIn) Quart(EaseIn) Quart(EaseOut) Quart(EaseInOut) Quart(EaseOutIn) Quint(EaseIn) Quint(EaseOut) Quint(EaseInOut) Quint(EaseOutIn) Sine(EaseIn) Sine(EaseOut) Sine(EaseInOut) Sine(EaseOutIn) Circ(EaseIn) Circ(EaseOut) Circ(EaseInOut) Circ(EaseOutIn) Expo(EaseIn) Expo(EaseOut) Expo(EaseInOut) Expo(EaseOutIn) Back(EaseIn) Back(EaseOut) Back(EaseInOut) Back(EaseOutIn) Bounce(EaseIn) Bounce(EaseOut) Bounce(EaseInOut) Bounce(EaseOutIn) Elastic(EaseIn) Elastic(EaseOut) Elastic(EaseInOut) Elastic(EaseOutIn) \"Blinn-Wyvill Cosine Approx\" \"Double-Cubic Seat\" \"Double-Cubic Seat with Linear Bend\" \"Double-Odd-Polynomial Seat\" \"Symmetric Double-Polynomial Sigmoids\" \"Quadratic Through a Given Point\" \"Exponential Emphasis\" \"Double-Exponential Seat\" \"Double-Exponential Sigmoid\" \"Modified Logistic Sigmoid\" \"Circular Ease In\" \"Circular Ease Out\" \"Double-Circular Seat\" \"Double-Circular Sigmoid\" \"Double-Elliptic Seat\" \"Double-Elliptic Sigmoid\" \"Double-Linear with Circular Fillet\" \"Circular Arc Through A Given Point\" \"Quadtratic Bezier\" \"Cubic Bezier\" \"Cubic Bezier (Nearly) Through Two Given Points\"  ");
    CLASS_ATTR_SAVE(c, "easing", 0);
    CLASS_ATTR_SELFSAVE(c, "easing", 0);
    //CLASS_ATTR_ACCESSORS(c, "easing", (method)easings_getattr_expr, (method)easings_setattr_expr);
    
    
    class_register(CLASS_BOX, c);
    easings_class = c;
    
    post("easings object loaded successfully",0);
}


//---------------------------------------------------------------------------

void *easings_new(t_symbol *s, long argc, t_atom *argv)
{
    t_easings *x;   // local variable (pointer to an easings data structure)
    
    x = (t_easings *)object_alloc(easings_class); // create a new instance of this object
    
    intin(x, 5); // new inlet for levin 'n'
    floatin(x, 4); // new inlet for levin 'd'
    floatin(x, 3); // new inlet for levin 'c'
    floatin(x, 2); // new inlet for levin 'b'
    floatin(x, 1); // new inlet for levin 'a'
    
    x->outlet = floatout((t_easings *)x); // create a float outlet
    //x->outlet1 = floatout((t_easings *)x); // create second float outlet
    
    x->e_val0 = 0; // set initial left inlet value to zero
    x->e_val1 = 0; // set initial left inlet value to (not: variable set by arguments edit: do this)
    x->g_aval = 0.0; // set initial 'a' inlet value to zero
    x->g_bval = 0.0; // set initial 'b' inlet value to zero
    x->g_nval = 0; // set initial 'n' inlet value to zero
    x->g_cval = 0.0; // set initial 'c' inlet value to zero
    x->g_dval = 0.0; // set initial 'd' inlet value to zero
    
    attr_args_process(x, argc, argv);
    object_attach_byptr_register(x, x, CLASS_BOX);
    
    post(" new easings object instance added to patch..",0);
    
    return(x);     // return reference to the object instance
    
}


//---------------------------------------------------------------------------

void easings_assist(t_easings *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_OUTLET)
        sprintf(s, "eased output (float)");
    else {
        switch (a) {
            case 0:
                sprintf(s, "inlet %ld: normalised float 0.0 - 1.0", a);
                break;
            case 1:
                sprintf(s, "inlet %ld: 'a' (levin algos only) normalised float 0.0 - 1.0", a);
                break;
            case 2:
                sprintf(s, "inlet %ld: 'b' (levin algos only) normalised float 0.0 - 1.0", a);
                break;
            case 3:
                sprintf(s, "inlet %ld: 'c' (levin algos only) normalised float 0.0 - 1.0", a);
                break;
            case 4:
                sprintf(s, "inlet %ld: 'd' (levin algos only) normalised float 0.0 - 1.0", a);
                break;
            case 5:
                sprintf(s, "inlet %ld: 'n' (levin algos only) integer 1-20", a);
                break;

        }
    }
}

t_max_err easings_notify(t_easings *x, t_symbol *s, t_symbol *msg, void *sender, void *data, long ac, t_atom *av)
{
    t_symbol *attrname;
    if (msg == gensym("attr_modified")) {       // check notification type
        switch(x->e_val1) {
            case 0: {
                post("Linear(No Easing)");
                break;
            }
            case 1: {
                post("Quad(EaseIn)");
                break;
            }
            case 2: {
                post("Quad(EaseOut)");
                break;
            }
            case 3: {
                post("Quad(EaseInOut)");
                break;
            }
            case 4: {
                post("Quad(EaseOutIn)");
                break;
            }
            case 5: {
                post(" Cubic(EaseIn)");
                break;
            }
            case 6: {
                post("Cubic(EaseOut)");
                break;
            }
            case 7: {
                post("Cubic(EaseInOut)");
                break;
            }
            case 8: {
                post("Cubic(EaseOutIn)");
                break;
            }
            case 9: {
                post("Quart(EaseIn)");
                break;
            }
            case 10: {
                post("Quart(EaseOut)");
                break;
            }
            case 11: {
                post("Quart(EaseInOut)");
                break;
            }
            case 12: {
                post("Quart(EaseOutIn)");
                break;
            }
            case 13: {
                post("Quint(EaseIn)");
                break;
            }
            case 14: {
                post("Quint(EaseOut)");
                break;
            }
            case 15: {
                post("Quint(EaseInOut)");
                break;
            }
            case 16: {
                post("Quint(EaseOutIn)");
                break;
            }
            case 17: {
                post("Sine(EaseIn)");
                break;
            }
            case 18: {
                post("Sine(EaseOut)");
                break;
            }
            case 19: {
                post("Sine(EaseInOut)");
                break;
            }
            case 20: {
                post("Sine(EaseOutIn)");
                break;
            }
            case 21: {
                post("Circ(EaseIn)");
                break;
            }
            case 22: {
                post("Circ(EaseOut)");
                break;
            }
            case 23: {
                post("Circ(EaseInOut)");
                break;
            }
            case 24: {
                post("Circ(EaseOutIn)");
                break;
            }
            case 25: {
                post("Expo(EaseIn)");
                break;
            }
            case 26: {
                post("Expo(EaseOut)");
                break;
            }
            case 27: {
                post("Expo(EaseInOut)");
                break;
            }
            case 28: {
                post("Expo(EaseOutIn)");
                break;
            }
            case 29: {
                post("Back(EaseIn)");
                break;
            }
            case 30: {
                post("Back(EaseOut)");
                break;
            }
            case 31: {
                post("Back(EaseInOut)");
                break;
            }
            case 32: {
                post("Back(EaseOutIn)");
                break;
            }
            case 33: {
                post("Bounce(EaseIn)");
                break;
            }
            case 34: {
                post("Bounce(EaseOut)");
                break;
            }
            case 35: {
                post("Bounce(EaseInOut)");
                break;
            }
            case 36: {
                post("Bounce(EaseOutIn)");
                break;
            }
            case 37: {
                post("Elastic(EaseIn)");
                break;
            }
            case 38: {
                post("Elastic(EaseOut)");
                break;
            }
            case 39: {
                post("Elastic(EaseInOut)");
                break;
            }
            case 40: {
                post("Elastic(EaseOutIn)");
                break;
            }
            case 41: {
                post("Blinn-Wyvill Cosine Approximation");
                break;
            }
            case 42: {
                post("Double-Cubic Seat");
                post("use variables 'a' & 'b' to shape");
                break;
            }
            case 43: {
                post("Double-Cubic Seat with Linear Bend");
                post("use variables 'a' & 'b' to shape");
                break;
            }
            case 44: {
                post("Double-Odd-Polynomial Seat");
                post("use variables 'a', 'b' & 'n' to shape");
                break;
            }
            case 45: {
                post("Symmetric Double-Polynomial Sigmoids");
                post("use variables 'a', 'b' & 'n' to shape");
                break;
            }
            case 46: {
                post("Quadratic Through a Given Point");
                post("use variables 'a' & 'b' to shape");
                break;
            }
            case 47: {
                post("Exponential Emphasis");
                post("use variable 'a' to shape");
                break;
            }
            case 48: {
                post("Double-Exponential Seat");
                post("use variable 'a' to shape");
                break;
            }
            case 49: {
                post("Double-Exponential Sigmoid");
                post("use variable 'a' to shape");
                break;
            }
            case 50: {
                post("Modified Logistic Sigmoid");
                post("use variable 'a' to shape");
                break;
            }
            case 51: {
                post("Circular Ease In");
                break;
            }
            case 52: {
                post("Circular Ease Out");
                break;
            }
            case 53: {
                post("Double-Circular Seat");
                post("use variable 'a' to shape");
                break;
            }
            case 54: {
                post("Double-Circular Sigmoid");
                post("use variable 'a' to shape");
                break;
            }
            case 55: {
                post("Double-Elliptic Seat");
                post("use variables 'a' & 'b' to shape");
                break;
            }
            case 56: {
                post("Double-Elliptic Sigmoid");
                post("use variables 'a' & 'b' to shape");
                break;
            }
            case 57: {
                post("Double-Linear with Circular Fillet");
                post("use variables 'a', 'b' & 'd' to shape");
                break;
            }
            case 58: {
                post("Circular Arc Through A Given Point");
                post("use variables 'a' & 'b' to shape");
                post("NOT FINISHED!");
                break;
            }
            case 59: {
                post("Quadtratic Bezier");
                post("use variables 'a' & 'b' to shape");
                break;
            }
            case 60: {
                post("Cubic Bezier");
                post("use variables 'a', 'b', 'c' & 'd' to shape");
                break;
            }
            case 61: {
                post("Cubic Bezier (Nearly) Through Two Given Points");
                post("use variables 'a', 'b', 'c' & 'd' to shape");
                break;
            }
        }
        //attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));      // "getname" = ask attribute object for name
        //object_post((t_object *)x, "changed attr name is %s",attrname->s_name);
        //object_post((t_object *)x, "it has %ld arguments", ac);
    }
    return 0;
}

//t_max_err easings_setattr_expr(t_easings *x, void *attr, long ac, t_atom *av)
//{
//    if(ac && av)
//    {
//        post("%ld SETTED",av);
//        //object_attr_setvalueof(x->s_ui, gensym("easing"), ac, av);
//    }
//    return MAX_ERR_NONE;
//}
//
//t_max_err easings_getattr_expr(t_easings *x, void *attr, long ac, t_atom *av)
//{
//    if(ac && av)
//    {
//        post("%d GETTED",av);
//        object_attr_getvalueof(x->s_ui, gensym("easing"), ac, &av);
//    }
//    return MAX_ERR_NONE;
//}


//---------------------------------------------------------------------------

void easings_bang(t_easings *x)
{

}


void easings_float(t_easings *x, double f)
{
    x->e_val0 = f; // pass float into global variable
    double t = 0.0; // initialise t as 0.0
    t = x->e_val0; // current time from float
    double myval = 0.0; // initialise myval as 0.0
    
    double ga = x->g_aval; // levin 'a'
    double gb = x->g_bval; // levin 'b'
    long   gn = x->g_nval; // levin 'n'
    double gc = x->g_cval; // levin 'c'
    double gd = x->g_dval; // levin 'd'
    
    double b = 0.0; // start value (0.0 for norm)
    double c = 1.0; // change in value (1.0 for norm)
    double d = 1.0; // duration
    double ss = 1.70158; // for "back" algos only
    
    double epsilon = 0.00001;
    double min_param_a = 0.0 + epsilon;
    double max_param_a = 1.0 - epsilon;
    double min_param_b = 0.0;
    double max_param_b = 1.0;
    ga = fmin(max_param_a, fmax(min_param_a, ga));
    gb = fmin(max_param_b, fmax(min_param_b, gb));
    
    
    
    
    
    switch(x->e_val1){
        case(0):{
            //Linear (No Easing)
            myval = c * ( t / d ) + b;
             
            break;
        }
        case(1):{
            //Quadratic(EaseIn)
            myval = c * ( t /= d ) * t + b;
             
            break;
        }
        case(2):{
            //Quadratic(EaseOut)
            myval = -c * ( t/=d ) * ( t - 2 ) + b;
             
            break;
        }
        case(3):{
            //Quadratic(EaseInOut)
            if ( ( t /= d / 2 ) < 1 ) {
                myval = c/2 * t * t + b;
                 
                break;
            } else {
                myval = -c/2 * (( --t ) * ( t - 2 ) - 1) + b;
                 
                break;
            }
        }
        case(4):{
            //Quadratic(EaseOutIn)
            if (t < d/2) {
                double tt = t * 2;
                double cc = c / 2;
                myval = -cc * ( tt /= d ) * ( tt - 2 ) + b;
                 
                break;
            } else {
                double tt = ( t * 2 ) - d;
                double bb = b + ( c/2 );
                double cc = c / 2;
                myval = cc * ( tt /= d ) * tt + bb;
                 
                break;
            }
        }
        case(5):{
            //Cubic(EaseIn)
            myval = c * ( t /= d ) * t * t + b;
             
            break;
        }
        case(6):{
            //Cubic(EaseOut)
            myval = c * (( t = t/d - 1) * t * t + 1) + b;
             
            break;
        }
        case(7):{
            //Cubic(EaseInOut)
            if (( t /= d / 2) < 1) {
                myval = c / 2 * t * t * t + b;
                 
                break;
            } else {
                myval = c/2 * (( t -= 2) * t * t + 2) + b;
                 
                break;
            }
        }
        case(8):{
            //Cubic(EaseOutIn)
            if (t < d / 2) {
                double tt = t * 2;
                double cc = c / 2;
                myval = cc * ( ( tt = tt / d - 1 ) * tt * tt + 1) + b;
                 
                break;
            } else {
                double tt = ( t * 2 ) - d;
                double bb = b + ( c / 2 );
                double cc = c / 2;
                myval = cc * ( tt /= d) * tt * tt + bb;
                 
                break;
            }
        }
        case(9):{
            //Quartic(EaseIn)
            myval = c * ( t /= d ) * t * t * t + b;
             
            break;
        }
        case(10):{
            //Quartic(EaseOut)
            myval = -c * ( ( t = t / d - 1) * t * t * t - 1) + b;
             
            break;
        }
        case(11):{
            //Quartic(EaseInOut)
            if ( ( t /= d / 2 ) < 1) {
                myval = c / 2 * t * t * t * t + b;
                 
                break;
            } else {
                myval = -c / 2 * ( ( t -= 2) * t * t * t - 2) + b;
                 
                break;
            }
        }
        case(12):{
            //Quartic(EaseOutIn)
            if (t < d/2) {
                double tt = t * 2;
                double cc = c / 2;
                myval = -cc * (( tt = tt / d - 1) * tt * tt * tt - 1) + b;
                 
                break;
            } else {
                double tt = ( t * 2 ) - d;
                double bb = b + c / 2;
                double cc = c / 2;
                myval = cc * ( tt /= d ) * tt * tt * tt + bb;
                 
                break;
            }
        }
        case(13):{
            //Quintic(EaseIn)
            myval = c * ( t /= d) * t * t * t * t + b;
             
            break;
        }
        case(14):{
            //Quintic(EaseOut)
            myval = c * ( ( t = t / d - 1 ) * t * t * t * t + 1) + b;
             
            break;
        }
        case(15):{
            //Quintic(EaseInOut)
            if ( ( t /= d / 2) < 1) {
                myval = c / 2 * t * t * t * t * t + b;
                 
                break;
            } else {
                myval = c / 2 * ( ( t -= 2 ) * t * t * t * t + 2) + b;
                 
                break;
            }
        }
        case(16):{
            //Quintic(EaseOutIn)
            if (t < d/2) {
                double tt = t * 2;
                double cc = c / 2;
                myval = cc * ( ( tt = tt / d - 1) * tt * tt * tt * tt + 1) + b;
                 
                break;
            } else {
                double tt = ( t * 2 )-d;
                double bb = b + c / 2;
                double cc = c / 2;
                myval = cc * ( tt /= d ) * tt * tt * tt * tt + bb;
                 
                break;
            }
        }
        case(17):{
            //Sine(EaseIn)
            myval = -c * cos( t / d * ( M_PI / 2 )) + c + b;
             
            break;
        }
        case(18):{
            //Sine(EaseOut)
            myval = c * sin( t / d * ( M_PI / 2 )) + b;
             
            break;
        }
        case(19):{
            //Sine(EaseInOut)
            myval = -c / 2 * ( cos( M_PI * t / d ) - 1) + b;
             
            break;
        }
        case(20):{
            //Sine(EaseOutIn)
            if(t < d/2){
                double tt = t * 2;
                double cc = c / 2;
                myval = cc * sin( tt / d * ( M_PI / 2 )) + b;
                 
                break;
            } else {
                double tt = ( t * 2 ) - d;
                double bb = b + c / 2;
                double cc = c / 2;
                myval = -cc * cos( tt / d * ( M_PI / 2 )) + cc + bb;
                 
                break;
            }
        }
        case(21):{
            //Circ(EaseIn)
            myval = -c * ( sqrt( 1 - ( t /= d ) * t ) - 1) + b;
             
            break;
        }
        case(22):{
            //Circ(EaseOut)
            myval = c * sqrt(1 - ( t = t/d - 1 ) * t) + b;
             
            break;
        }
        case(23):{
            //Circ(EaseInOut)
            if ((t/=d/2) < 1) {
                myval = -c/2 * (sqrt( 1 - t * t ) - 1) + b;
                 
                break;
            } else {
                myval = c/2 * (sqrt(1 - ( t -= 2 ) * t) + 1) + b;
                 
                break;
            }
        }
        case(24):{
            //Circ(EaseOutIn)
            if (t < d/2) {
                double tt = t * 2;
                double cc = c / 2;
                myval = cc * sqrt(1 - ( tt = tt/d - 1) * tt) + b;
                 
                break;
            } else {
                double tt = ( t * 2 ) - d;
                double bb = b + c / 2;
                double cc = c / 2;
                myval = -cc * (sqrt(1 - ( tt /= d ) * tt) - 1) + bb;
                 
                break;
            }
        }
        case(25):{
            //Expo(EaseIn)
            if (t==0) {
                myval = b;
                 
                break;
            } else {
                myval = c * pow( 2, 10 * ( t/d - 1 )) + b;
                 
                break;
            }
        }
        case(26):{
            //Expo(EaseOut)
            if (t==d) {
                myval = c+d;
                 
                break;
            } else {
                myval = c * (-pow(2, -10 * t/d) + 1) + b;
                 
                break;
            }
        }
        case(27):{
            //Expo(EaseInOut)
            if (t==0){
                myval = b;
                 
                break;
            } else if (t==d){
                myval = b+c;
                 
                break;
            } else if (( t /= d/2 ) < 1) {
                myval = c/2 * pow( 2, 10 * (t - 1) ) + b;
                 
                break;
            } else {
                myval = c/2 * (-pow( 2, -10 * --t ) + 2) + b;
                 
                break;
            }
        }
        case(28):{
            //Expo(EaseOutIn)
            if (t < d/2) {
                double tt = t * 2;
                double cc = c / 2;
                myval = cc * ( -pow( 2, -10 * tt/d ) + 1) + b;
                 
                break;
            } else {
                double tt = ( t * 2 )-d;
                double bb = b + ( c / 2 );
                double cc = c / 2;
                myval = cc * pow(2, 10 * ( tt/d - 1 )) + bb;
                 
                break;
            }
        }
        case(29):{
            //Back(EaseIn)
            myval = c * ( t /= d ) * t * (( ss + 1 ) * t - ss) + b;
             
            break;
        }
        case(30):{
            //Back(EaseOut)
            myval = c * (( t = t / d - 1 ) * t * (( ss + 1 ) * t + ss) + 1) + b;
             
            break;
        }
        case(31):{
            //Back(EaseInOut)
            if ( (t/=(d/2)) < 1 ){
                double s = ss; // < need to adjust for custom in future
                myval = c / 2 * ( t * t * ((( s *= 1.525 ) + 1 ) * t - s)) + b;
                 
                break;
            } else {
                double s = ss; // < need to adjust for custom in future
                myval = c / 2 * (( t -= 2 ) * t * ((( s *= 1.525 ) + 1 ) * t + s) + 2) + b;
                 
                break;
            }
        }
        case(32):{
            //Back(EaseOutIn)
            if (t < d/2) {
                double tt=t*2;
                double cc=c/2;
                myval = cc * (( tt = tt/d - 1 ) * tt * (( ss + 1 ) * tt + ss) + 1) + b;
                 
                break;
            } else {
                double tt = ( t * 2 ) - d;
                double bb = b + c / 2;
                double cc = c / 2;
                myval = cc * ( tt /= d ) * tt * (( ss + 1 ) * tt - ss) + bb;
                 
                break;
            }
        }
        case(33):{
            //Bounce(EaseIn)
            t = d-t;
            b = 0;
            if (( t /= d ) < ( 1/2.75 )){
                double e = c * ( 7.5625 * t * t ) + b;
                myval = c - e + b;
                 
                break;
            } else if (t < (2/2.75)) {
                double e = c * ( 7.5625 * ( t -= ( 1.5/2.75 )) * t + 0.75) + b;
                myval = c - e + b;
                 
                break;
            } else if (t < (2.5/2.75)) {
                double e = c * ( 7.5625 * ( t -= (2.25/2.75) ) * t + 0.9375) + b;
                myval = c - e + b;
                 
                break;
            } else {
                double e = c * (7.5625 * (t -= (2.625 / 2.75)) * t + 0.984375) + b;
                myval = c - e + b;
                 
                break;
            }
        }
        case(34):{
            //Bounce(EaseOut)
            if (( t /= d ) < (1/2.75)) {
                myval = c * (7.5625 * t * t) + b;
                 
                break;
            } else if (t < (2/2.75)) {
                myval = c * (7.5625 * ( t -= (1.5/2.75)) * t + 0.75) + b;
                 
                break;
            } else if (t < (2.5/2.75)) {
                myval = c * (7.5625 * ( t -= (2.25/2.75)) * t + 0.9375) + b;
                 
                break;
            } else {
                myval = c*(7.5625*( t -= (2.625/2.75)) * t + 0.984375) + b;
                 
                break;
            }
        }
        case(35):{
            //Bounce(EaseInOut)
            if (t < d/2) {
                t = t * 2;
                t = d - t;
                b = 0;
                if ((t/=d) < (1/2.75)){
                    double e = c * (7.5625 * t * t) + b;
                    myval = (c - e + b)*0.5 + b;
                     
                    break;
                } else if (t < (2/2.75)) {
                    double e = c * (7.5625 * ( t -= (1.5/2.75)) * t + 0.75) + b;
                    myval = (c - e + b)*0.5 + b;
                     
                    break;
                } else if (t < (2.5/2.75)) {
                    double e = c * (7.5625 * ( t -= (2.25/2.75)) * t + 0.9375) + b;
                    myval = (c - e + b)*0.5 + b;
                     
                    break;
                } else {
                    double e = c * (7.5625 * ( t -= (2.625/2.75)) * t + 0.984375) + b;
                    myval = (c - e + b) * 0.5 + b;
                     
                    break;
                }
            } else {
                t = t * 2 - d;
                if ((t/=d) < (1/2.75)) {
                    myval = ( c * (7.5625 * t * t) + b) * 0.5 + (c * 0.5) + b;
                     
                    break;
                } else if (t < (2/2.75)) {
                    myval = ( c * (7.5625 * ( t -= (1.5/2.75)) * t + 0.75) + b) * 0.5 + (c * 0.5) + b;
                     
                    break;
                } else if (t < (2.5/2.75)) {
                    myval = (c * (7.5625 * (t -= (2.25/2.75)) * t + 0.9375) + b) * 0.5 + (c * 0.5) + b;
                     
                    break;
                } else {
                    myval = (c * (7.5625 * (t -= (2.625/2.75)) * t + 0.984375) + b) * 0.5 + (c * 0.5) +b;
                     
                    break;
                }
            }
        }
        case(36):{
            //Bounce(EaseOutIn)
            if(t <d/2){
                double tt = t * 2;
                double cc=c/2;
                if ((tt/=d) < (1/2.75)) {
                    myval = cc * (7.5625 * tt * tt) + b;
                     
                    break;
                } else if (tt < (2/2.75)) {
                    myval = cc * (7.5625 * (tt -= (1.5/2.75)) * tt + 0.75) + b;
                     
                    break;
                } else if (tt < (2.5/2.75)) {
                    myval = cc * (7.5625 * (tt -= (2.25/2.75)) * tt + 0.9375) + b;
                     
                    break;
                } else {
                    myval = cc * (7.5625*(tt -= (2.625/2.75)) * tt + 0.984375) + b;
                     
                    break;
                }
            } else {
                double tt = ( t * 2 )-d;
                double bb = b + c / 2;
                double cc = c / 2;
                double ttt = d - tt;
                double bbb = 0;
                if ((ttt/=d) < (1/2.75)){
                    double e = cc * (7.5625 * ttt * ttt) + bbb;
                    myval = cc - e + bb;
                     
                    break;
                } else if (ttt < (2/2.75)) {
                    double e = cc * ( 7.5625 * (ttt -= (1.5/2.75)) * ttt + 0.75) + bbb;
                    myval = cc - e + bb;
                     
                    break;
                } else if (ttt < (2.5/2.75)) {
                    double e = cc * (7.5625 * (ttt -= (2.25/2.75)) * ttt + 0.9375) + bbb;
                    myval = cc - e + bb;
                     
                    break;
                } else {
                    double e = cc * (7.5625 * (ttt -= (2.625/2.75)) * ttt + 0.984375) + bbb;
                    myval = cc - e + bb;
                     
                    break;
                }
            }	
        }
        case(37):{
            //Elastic(EaseIn)
            if(t==0) {
                myval = b;
                 
                break;
            } else if((t/=d)==1) {
                myval = b+c;
                 
                break;
            } else {
                double p = d * 0.3;
                double a = c;
                double z = p/4;
                double zz = a * pow(2,10 *( t -= 1 )) * sin((t * d - z) * (2 * M_PI) / p);
                myval = -zz + b;
                 
                break;
            }
        }
        case(38):{
            //Elastic(EaseOut)
            if(t==0) {
                myval = b;
                 
                break;
            } else if((t/=d)==1) {
                myval = b+c;
                 
                break;
            } else {
                double p = d * 0.3;
                double a = c;
                double z = p/4;
                double zz = a * pow(2,-10 * t) * sin(( t * d - z ) * ( 2 * M_PI)/p);
                myval = zz + c + b;
                 
                break;
            }
        }
        case(39):{
            //Elastic(EaseInOut)
            if(t==0) {
                myval = b;
                 
                break;
            } else if((t/=d/2)==2) {
                myval = b + c;
                 
                break;
            } else {
                double p = d * (0.3 * 1.5);
                double a = c;
                double z = p / 4;
                if(t < 1){
                    myval = -0.5 * (a * pow(2,10 * ( t -= 1 )) * sin( ( t * d - z ) * ( 2 * M_PI) / p )) + b;
                     
                    break;
                } else {
                    myval = a * pow(2,-10 * ( t -= 1 )) * sin( ( t * d - z ) * ( 2 * M_PI) / p ) * 0.5 + c + b;
                     
                    break;
                }
            }
        }
        case(40):{
            //Elastic(EaseOutIn)
            if(t < d/2){
                double tt = t * 2;
                double cc = c / 2;
                if(tt==0) {
                    myval = b;
                     
                    break;
                } else if((tt/=d)==1) {
                    myval = b+cc;
                     
                    break;
                } else {
                    double p = d * 0.3;
                    double a = cc;
                    double z = p/4;
                    double zz = a * pow( 2,-10 * tt ) * sin(( tt * d - z ) * ( 2 * M_PI )/p);
                    myval = zz + cc + b;
                     
                    break;
                }
            } else {
                double tt = ( t * 2 ) - d;
                double bb = b + c / 2;
                double cc = c / 2;
                if(t==0) {
                    myval = bb;
                     
                    break;
                } else if((tt/=d)==1) {
                    myval = bb+cc;
                     
                    break;
                } else {
                    double p = d * 0.3;
                    double a = cc;
                    double z = p/4;
                    double zz = a * pow( 2,10 * ( tt -= 1 )) * sin(( tt * d - z ) * ( 2 * M_PI ) / p );
                    myval = -zz + bb;
                     
                    break;
                }
            }	
        }
        case(41):{
            //Blinn-Wyvill Approximation to the Raised Inverted Cosine
            double t2 = t * t;
            double t4 = t2 * t2;
            double t6 = t4 * t2;
            
            double fa = ( 4.0/9.0);
            double fb = (17.0/9.0);
            double fc = (22.0/9.0);
            
            myval = fa * t6 - fb * t4 + fc * t2;
             
            break;
        }
        case(42):{
            //Double-Cubic Seat
            if (t <= ga){
                myval = gb - gb*pow(1-t/ga, 3.0);
                 
                break;
            } else {
                myval = gb + (1-gb)*pow((t-ga)/(1-ga), 3.0);
                 
                break;
            }
        }
        case(43):{
            //Double-Cubic Seat with Linear Blend
            gb = 1.0 - gb; // reverse for intelligibility
            if (t <= ga){
                myval = gb * t + (1-gb) * ga * (1 - pow(1 - t / ga, 3.0));
                 
                break;
            } else {
                myval = gb * t + (1-gb) * (ga + (1 - ga) * pow(( t - ga )/( 1 - ga), 3.0));
                 
                break;
            }
        }
        case(44):{
            //Double-Odd-Polynomial Seat
            int p = 2 * gn + 1;
            if (t <= ga){
                myval = gb - gb * pow(1-t/ga, p);
                 
                break;
            } else {
                myval = gb + (1 - gb) * pow((t - ga) / (1 - ga), p);
                 
                break;
            }
        }
        case(45):{
            //Symmetric Double-Polynomial Sigmoids
            if (gn%2 == 0){
                // even polynomial
                if (t <= 0.5){
                    myval = pow(2.0 * t, gn) / 2.0;
                     
                    break;
                } else {
                    myval = 1.0 - pow(2 * (t - 1), gn) / 2.0;
                     
                    break;
                }
            }
            else {
                // odd polynomial
                if (t <= 0.5){
                    myval = pow(2.0 * t, gn) / 2.0;
                     
                    break;
                } else {
                    myval = 1.0 + pow(2.0 * (t - 1), gn) / 2.0;
                     
                    break;
                }
            }
        }
        case(46):{
            //Quadratic Through a Given Point
            double GA = (1 - gb) / (1 - ga) - (gb / ga);
            double GB = (GA * (ga * ga) - gb) / ga;
            double y = GA * (t * t) - GB * (t);
            myval = fmin(1, fmax(0,y));
             
            break;
        }
        case(47):{
            //Exponential Emphasis
            if (ga < 0.5){
                // emphasis
                double gga = 2.0 * (ga);
                myval = pow(t, gga);
                 
                break;
            } else {
                // de-emphasis
                double gga = 2.0 * (ga - 0.5);
                myval = pow(t, 1.0 / (1 - gga));
                 
                break;
            }
        }
        case(48):{
            //Double-Exponential Seat
            if (t <= 0.5){
                myval = (pow(2.0 * t, 1 - ga)) / 2.0;
                 
                break;
            } else {
                myval = 1.0 - (pow(2.0 * (1.0 - t), 1 - ga)) / 2.0;
                 
                break;
            }
        }
        case(49):{
            //Double-Exponential Sigmoid
            ga = 1.0 - ga; // reversed for sensible results
            if ( t <= 0.5){
                myval = (pow(2.0 * t, 1.0 / ga)) / 2.0;
                 
                break;
            } else {
                myval = 1.0 - (pow(2.0 * (1.0 - t), 1.0 / ga)) / 2.0;
                 
                break;
            }
        }
        case(50):{
            //Modified Logistic Sigmoid
            ga = (1 / (1 - ga) - 1);
            double GA = 1.0 / (1.0 + exp(0 -((t - 0.5) * ga * 2.0)));
            double GB = 1.0 / (1.0 + exp(ga));
            double GC = 1.0 / (1.0 + exp(0 - ga));
            myval = (GA - GB) / (GC - GB);
             
            break;
        }
        case(51):{
            //Circular Ease In
            myval = 1 - sqrt(1 - t * t);
             
            break;
        }
        case(52):{
            //Circular Ease Out
            myval = sqrt(1 - pow((1 - t), 2.0));
             
            break;
        }
        case(53):{
            //Double-Circular Seat
            if (t <= ga){
                myval = sqrt(pow(ga,2.0) - pow((t-ga),2.0));
                 
                break;
            } else {
                myval = 1 - sqrt(pow((1-ga),2.0) - pow((t-ga),2.0));
                 
                break;
            }
        }
        case(54):{
            //Double-Circular Sigmoid
            if (t <= ga){
                myval = ga - sqrt(ga * ga - t * t);
                 
                break;
            } else {
                myval = ga + sqrt(pow((1 - ga),2.0) - pow((t - 1), 2.0));
                 
                break;
            }
        }
        case(55):{
            //Double-Elliptic Seat
            if (t <= ga){
                myval = (gb / ga) * sqrt(pow(ga, 2.0) - pow((t - ga), 2.0));
                 
                break;
            } else {
                myval = 1- ((1 - gb) / (1 - ga)) * sqrt(pow((1 - ga), 2.0) - pow((t - ga),2.0));
                 
                break;
            }
        }
        case(56):{
            //Double-Elliptical Sigmoid
            if (t <= ga){
                myval = gb * (1 - (sqrt(pow(ga, 2.0) - pow(t, 2.0)) / ga));
                 
                break;
            } else {
                myval = gb + ((1 - gb) / (1 - ga)) * sqrt(pow((1 - ga),2.0) - pow((t - 1),2.0));
                 
                break;
            }
        }
        case(57):{
            // Joining Two Lines with a Circular Arc Fillet
            // Adapted from Robert D. Miller / Graphics Gems III.
            
            double arcStartAngle;
            double arcEndAngle;
            double arcStartX,  arcStartY;
            double arcEndX,    arcEndY;
            double arcCenterX, arcCenterY;
            double arcRadius;
            
            //computeFilletParameters (0,0, a,b, a,b, 1,1,  R);
                double p1x = 0; double p1y = 0;
                double p2x = ga; double p2y = gb;
                double p3x = ga; double p3y = gb;
                double p4x = 1; double p4y = 1;
                double r = gd;
            
                double c1   = p2x * p1y - p1x * p2y;
                double a1   = p2y - p1y;
                double b1   = p1x - p2x;
                double c2   = p4x * p3y - p3x * p4y;
                double a2   = p4y - p3y;
                double b2   = p3x - p4x;
                if ((a1 * b2) == (a2 * b1)){  /* Parallel or coincident lines */
                    return;
                }
                
                double d1, d2;
                double mPx, mPy;
                mPx = (p3x + p4x)/2.0;
                mPy = (p3y + p4y)/2.0;
            
                    //d1 = linetopoint(a1,b1,c1,mPx,mPy);  /* Find distance p1p2 to p3 */\
                
                    // Return signed distance from line Ax + By + C = 0 to point P.
                    // float linetopoint (float a, float b, float c, float ptx, float pty){
                    //      float lp = 0.0;
                    //      float d = sqrt((a*a)+(b*b));
                    //      if (d != 0.0){
                    //          lp = (a*ptx + b*pty + c)/d;
                    //      }
                    //      return lp;
                    // }
            
                    d1 = 0.0;
                    double dd1 = sqrt((a1*a1)+(b1*b1));
                    if (dd1 != 0.0){
                        d1 = (a1*mPx + b1*mPy + c1)/dd1;
                    }
            
                if (d1 == 0.0) {
                    break;
                }
            
                mPx = (p1x + p2x)/2.0;
                mPy = (p1y + p2y)/2.0;
            
                    //d2 = linetopoint(a2,b2,c2,mPx,mPy);  /* Find distance p3p4 to p2 */
            
                    d2 = 0.0;
                    double dd2 = sqrt((a2*a2)+(b2*b2));
                    if (dd2 != 0.0){
                        d2 = (a2*mPx + b2*mPy + c2)/dd2;
                    }
            
                if (d2 == 0.0) {
                    break;
                }
                
                double c1p, c2p, d;
                double rr = r;
                if (d1 <= 0.0) {
                    rr= -rr;
                }
                c1p = c1 - rr*sqrt((a1*a1)+(b1*b1));  /* Line parallel l1 at d */
                rr = r;
                if (d2 <= 0.0){
                    rr = -rr;
                }
                c2p = c2 - rr*sqrt((a2*a2)+(b2*b2));  /* Line parallel l2 at d */
                d = (a1*b2)-(a2*b1);
                
                double pCx = (c2p*b1-c1p*b2)/d; /* Intersect constructed lines */
                double pCy = (c1p*a2-c2p*a1)/d; /* to find center of arc */
                double pAx = 0;
                double pAy = 0;
                double pBx = 0;
                double pBy = 0;
                double dP,cP;
                
                dP = (a1*a1) + (b1*b1);        /* Clip or extend lines as required */
                if (dP != 0.0){
                    cP = a1*pCy - b1*pCx;
                    pAx = (-a1*c1 - b1*cP)/dP;
                    pAy = ( a1*cP - b1*c1)/dP;
                }
                dP = (a2*a2) + (b2*b2);
                if (dP != 0.0){
                    cP = a2*pCy - b2*pCx;
                    pBx = (-a2*c2 - b2*cP)/dP;
                    pBy = ( a2*cP - b2*c2)/dP;
                }
                
                double gv1x = pAx-pCx;
                double gv1y = pAy-pCy;
                double gv2x = pBx-pCx; 
                double gv2y = pBy-pCy;
                
                double arcStart = (double) atan2(gv1y,gv1x); 
                double arcAngle = 0.0;
                double dd = (double) sqrt(((gv1x*gv1x)+(gv1y*gv1y)) * ((gv2x*gv2x)+(gv2y*gv2y)));
                if (dd != (double) 0.0){
                    arcAngle = (acos((gv1x*gv2x + gv1y*gv2y)/dd));
                } 
                double crossProduct = (gv1x*gv2y - gv2x*gv1y);
                if (crossProduct < 0.0){ 
                    arcStart -= arcAngle;
                }
                
                double arc1 = arcStart;
                double arc2 = arcStart + arcAngle;
                if (crossProduct < 0.0){
                    arc1 = arcStart + arcAngle;
                    arc2 = arcStart;
                }
                
                arcCenterX    = pCx;
                arcCenterY    = pCy;
                arcStartAngle = arc1;
                arcEndAngle   = arc2;
                arcRadius     = r;
                arcStartX     = arcCenterX + arcRadius*cos(arcStartAngle);
                arcStartY     = arcCenterY + arcRadius*sin(arcStartAngle);
                arcEndX       = arcCenterX + arcRadius*cos(arcEndAngle);
                arcEndY       = arcCenterY + arcRadius*sin(arcEndAngle);
            //End compute parameters
            
            double tt = 0;
            t = fmax(0, fmin(1, t));
            
            if (t <= arcStartX){
                tt = t / arcStartX;
                myval = tt * arcStartY;
                 
                break;
                
            } else if (t >= arcEndX){
                tt = (t - arcEndX)/(1 - arcEndX);
                myval = arcEndY + tt * (1 - arcEndY);
                 
                break;
                
            } else {
                if (t >= arcCenterX){
                    myval = arcCenterY - sqrt(pow(arcRadius,2.0) - pow((t-arcCenterX),2.0));
                     
                    break;
                } else{
                    myval = arcCenterY + sqrt(pow(arcRadius, 2.0) - pow((t-arcCenterX),2.0));
                     
                    break;
                }
            }
            break;
        }
        case(58):{
            //Circular Arc Through a Given Point
            //Adapted from Paul Bourke
            double m_Centerx;
            double m_Centery;
            double m_dRadius;
            
            t = fmin(1.0-epsilon, fmax(0.0+epsilon, t));
            double pt1x = 0;
            double pt1y = 0;
            double pt2x = ga;
            double pt2y = gb;
            double pt3x = 1;
            double pt3y = 1;
            
            
            
            break;
        }
        case(59):{
            //Quadratic Bezier
            // adapted from BEZMATH.PS (1993)
            // by Don Lancaster, SYNERGETICS Inc.
            // http://www.tinaja.com/text/bezmath.html
            
            double epsilon = 0.00001;
            ga = fmax(0, fmin(1, ga));
            gb = fmax(0, fmin(1, gb));
            if (ga == 0.5){
                ga += epsilon;
            }
            // solve t from x (an inverse operation)
            double om2a = 1 - 2 * ga;
            double gt = (sqrt(ga * ga + om2a * t) - ga) / om2a;
            myval = (1 - 2 * gb) * (gt * gt) + (2 * gb) * gt;
             
            break;
        }
        case(60):{
            //Cubic Bezier
            //The implementation here is adapted from the Bezmath Postscript library by Don Lancaster.
            //https://www.tinaja.com/text/bezmath.html
            
            double y0a = 0.00; // initial y
            double x0a = 0.00; // initial x
            double y1a = gb;    // 1st influence y
            double x1a = ga;    // 1st influence x
            double y2a = gd;    // 2nd influence y
            double x2a = gc;    // 2nd influence x
            double y3a = 1.00; // final y
            double x3a = 1.00; // final x
            
            double A =   x3a - 3 * x2a + 3 * x1a - x0a;
            double B =   3 * x2a - 6 * x1a + 3 * x0a;
            double C =   3 * x1a - 3 * x0a;
            double D =   x0a;
            
            double E =   y3a - 3 * y2a + 3 * y1a - y0a;
            double F =   3 * y2a - 6 * y1a + 3 * y0a;
            double G =   3 * y1a - 3 * y0a;
            double H =   y0a;
            
            // Solve for t given x (using Newton-Raphelson), then solve for y given t.
            // Assume for the first guess that t = x.
            double currentt = t;
            int nRefinementIterations = 5;
            for (int i=0; i < nRefinementIterations; i++)
            {
                //float currentx = xFromT (currentt, A,B,C,D);
                double currentx = A * (currentt * currentt * currentt) + B * (currentt * currentt) + C * currentt + D;
                
                //float currentslope = slopeFromT (currentt, A,B,C);
                double currentslope = 1.0 / (3.0 * A * currentt * currentt + 2.0 * B * currentt + C);
                
                currentt -= (currentx - t) * (currentslope);
                currentt = CLAMP(currentt, 0,1);
            }
            //float y = yFromT (currentt,  E,F,G,H);
            myval = E * (currentt * currentt * currentt) + F * (currentt * currentt) + G * currentt + H;
             
            break;
        }
        case(61):{
            //Cubic Bezier (Nearly) Through Two Given Points
            //The method is adapted from Don Lancaster
            //https://www.tinaja.com/text/bezmath.html

            double x0 = 0;
            double y0 = 0;
            double x4 = ga;
            double y4 = gb;
            double x5 = gc;
            double y5 = gd;
            double x3 = 1;
            double y3 = 1;
            double x1,y1,x2,y2; // to be solved.
            
            // arbitrary but reasonable
            // t-values for interior control points
            double t1 = 0.3;
            double t2 = 0.7;
            
            double B0t1 = (1 - t1) * (1 - t1) * (1 - t1);
            double B1t1 = 3 * t1 * (1 - t1) * (1 - t1);
            double B2t1 = 3 * t1 * t1 * (1-t1);
            double B3t1 = t1 * t1 * t1;
            double B0t2 = (1-t2) * (1 - t2) * (1 - t2);
            double B1t2 = 3 * t2 * (1 - t2) * (1 - t2);
            double B2t2 = 3 * t2 * t2 * (1 - t2);
            double B3t2 = t2 * t2 * t2;
            
            double ccx = x4 - x0*B0t1 - x3*B3t1;
            double ccy = y4 - y0*B0t1 - y3*B3t1;
            double ffx = x5 - x0*B0t2 - x3*B3t2;
            double ffy = y5 - y0*B0t2 - y3*B3t2;
            
            x2 = (ccx - (ffx*B1t1)/B1t2) / (B2t1 - (B1t1*B2t2)/B1t2);
            y2 = (ccy - (ffy*B1t1)/B1t2) / (B2t1 - (B1t1*B2t2)/B1t2);
            x1 = (ccx - x2*B2t1) / B1t1;
            y1 = (ccy - y2*B2t1) / B1t1;
            
            x1 = fmax(0+epsilon, fmin(1-epsilon, x1));
            x2 = fmax(0+epsilon, fmin(1-epsilon, x2));
            
            //Cubic Bezier function:
        
            double y0a = 0.00; // initial y
            double x0a = 0.00; // initial x
            double y1a = y1;    // 1st influence y
            double x1a = x1;    // 1st influence x
            double y2a = y2;    // 2nd influence y
            double x2a = x2;    // 2nd influence x
            double y3a = 1.00; // final y
            double x3a = 1.00; // final x
            
            double A =   x3a - 3 * x2a + 3 * x1a - x0a;
            double B =   3 * x2a - 6 * x1a + 3 * x0a;
            double C =   3 * x1a - 3 * x0a;
            double D =   x0a;
            
            double E =   y3a - 3 * y2a + 3 * y1a - y0a;
            double F =   3 * y2a - 6 * y1a + 3 * y0a;
            double G =   3 * y1a - 3 * y0a;
            double H =   y0a;
            
            // Solve for t given x (using Newton-Raphelson), then solve for y given t.
            // Assume for the first guess that t = x.
            double currentt = t;
            int nRefinementIterations = 5;
            for (int i=0; i < nRefinementIterations; i++){
                //double currentx = xFromT (currentt, A,B,C,D);
                double currentx = A * (currentt * currentt * currentt) + B * (currentt * currentt) + C * currentt + D;
                //double currentslope = slopeFromT (currentt, A,B,C);
                double currentslope = 1.0/(3.0 * A * currentt * currentt + 2.0 * B * currentt + C);
                currentt -= (currentx - t)*(currentslope);
                currentt = CLAMP(currentt, 0,1);
            }
            
            //float y = yFromT (currentt,  E,F,G,H);
            myval = E * (currentt * currentt * currentt) + F * (currentt * currentt) + G * currentt + H;
            myval = fmax(0, fmin(1, myval));
             
            break;
        }
            
    } // closed paren: (switch)
    
    
    outlet_float(x->outlet, myval);
}

void easings_ft1(t_easings *x, double ga)
{
    x->g_aval = ga;
    //post("a=%.6f", ga);
}

void easings_ft2(t_easings *x, double gb)
{
    x->g_bval = gb;
    //post("b=%.6f", gb);
}


void easings_ft3(t_easings *x, double gc)
{
    x->g_cval = gc;
    //post("c=%.6f", gc);
}

void easings_ft4(t_easings *x, double gd)
{
    x->g_dval = gd;
    //post("d=%.6f", gd);
}

void easings_in5(t_easings *x, long gn)
{
    x->g_nval = gn;
    //post("easing: %ld", gn);
}







