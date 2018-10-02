
/* shape.cpp
 *  - Example code for the 3rd year undergraduate OOP programming course I teach on
 *  - This assignment asks students to use class hierarchy and polymorphism to
 *    build a code that finds the area and volume of different shapes.
 *  - In this code I use abstract base classes for shapes of different numbers
 *    of dimensions, then derive classes for individual shapes.
 *  - The prism class uses a base class pointer to access the area function
 *    of any 2D shape class, from which arbitrary prisms may be constructed.
 *
 * Max Potter 2018
 */

#define _USE_MATH_DEFINES // for M_PI

#include<iostream>
#include<string>
#include<vector>
#include<cmath>

using namespace std;

const int err_novolume(10); // Throw error when calculating volume of flat shape

// Abstract base class - all shapes derive from here
class shape {
  public:    
    // All shapes have functions to calculate area, volume and return a string containing their name
    virtual double area() = 0;
    virtual double volume() = 0;
    virtual string tag() = 0;

    virtual ~shape(){}
};

// Abstract derived class for all 2D shapes
class shape2d : public shape {
  public:
    // 2D shapes are flat and have zero volume
    double volume() {throw err_novolume;}

    virtual ~shape2d(){}
};

class rectangle : public shape2d {
  private:
    // private member variables for each dimension of the rectangle
    double x, y; 

  public:
    rectangle(): x(0), y(0) {}
    rectangle(double xin, double yin): x(xin), y(yin) {}

    double area(){return x * y;}
    virtual string tag(){return "rectangle";} // virtual: overridden in derived square class

    ~rectangle(){cout << "Destroying rectangle object" << endl;}
};

// Squares are just a special case of rectangles so can inherit from rectangle
class square : public rectangle {
  public:
    square():rectangle(){}
    square(double xin):rectangle(xin, xin){}

    string tag(){return "square";}

    ~square(){cout << "Destroying square object" << endl;}
};


class ellipse : public shape2d {
  protected:
    double rx, ry;

  public:
    ellipse():rx(0), ry(0){}
    ellipse(double rxin, double ryin):rx(rxin), ry(ryin){}

    double area(){ return M_PI * rx * ry;}
    virtual string tag(){return "ellipse";} // virtual: overridden in derived circle class

    ~ellipse(){cout << "Destroying ellipse" << endl;}
};

// Similarly to squares, circles are just ellipses with extra symmetry 
class circle : public ellipse {
  public:
    circle():ellipse(){}
    circle(double rin):ellipse(rin, rin){}

    string tag(){return "circle";}

    ~circle(){cout << "Destroying circle" << endl;}
};

// Abstract derived class for all 3D shapes
class shape3d : public shape2d {
  public:
    virtual ~shape3d(){}
};

class cuboid : public shape3d {
  private:
    double x, y, z;

  public:
    cuboid():x(0), y(0), z(0){};
    cuboid(double xin, double yin, double zin):x(xin), y(yin), z(zin){};

    double volume(){return x * y * z;}
    double area(){return 2 * (x * y + x * z + y * z);}
    virtual string tag(){return "cuboid";} // virtual: overridden in derived cube class

    ~cuboid(){cout << "Destroying cuboid" << endl;}
};

// Cubes are cuboids in the most simple case
class cube : public cuboid {
  public:
    cube():cuboid(){}
    cube(double xin): cuboid(xin, xin, xin) {}

    string tag(){return "cube";}

    ~cube(){cout << "Destroying cube" << endl;}
};

class ellipsoid : public shape3d {
  private:
    double rx, ry, rz;

  public:
    ellipsoid(): rx(0), ry(0), rz(0){}
    ellipsoid(double rxin, double ryin, double rzin): rx(rxin), ry(ryin), rz(rzin){}

    double volume(){return 4./3. * M_PI * rx * ry * rz;}
    double area(){
      // Approximation: Knud Thomsen formula (error < 1% for p = 1.6075)
      const double p = 1.6075; 
      return 4 * M_PI * pow((pow(rx * ry, p) + pow(rx * ry, p) + pow(rx * ry, p)) / 3., 1./p);
    }
    virtual string tag(){return "ellipsoid";} // virtual: overridden in derived class sphere

    ~ellipsoid(){cout << "Destroying ellipsoid" << endl;}
};

// Spheres are ellipsoids in the most simple case
class sphere : public ellipsoid {
  public:
    sphere() : ellipsoid() {}
    sphere(double r) : ellipsoid(r, r, r) {}

    string tag(){return "sphere";}

    ~sphere(){cout << "Destroying sphere" << endl;}
};

// A prism is any 2D shape extruded along the out-of-plane axis
class prism : public shape2d {
  private:
    double z; // height/depth of prism
    shape2d *ptr_shape2d; // base class pointer to any 2D shape, which when extruded forms a prism

  public:
    prism(double zin, shape2d *shape2din):z(zin), ptr_shape2d(shape2din){};

    double area(){return ptr_shape2d->area();}
    double volume(){return ptr_shape2d->area() * z;}
    string tag(){return ptr_shape2d->tag() + " prism";}

    ~prism(){cout << "Destroying prism" << endl;}
};

int main(void) {
  vector<shape*> myshapes; // vector of base class pointers to store instances of derived shapes
  vector<shape*>::iterator myshapeit;

  // Add shapes
  myshapes.push_back(new square(4));
  cout << "Area of " << myshapes[0]->tag() << " = " << myshapes[0]->area() << endl;

  myshapes.push_back(new rectangle(4, 5));
  cout << "Area of " << myshapes[1]->tag() << " = " << myshapes[1]->area() << endl;

  myshapes.push_back(new circle(3));
  cout << "Area of " << myshapes[2]->tag() << " = " << myshapes[2]->area() << endl;

  myshapes.push_back(new cube(4));
  cout << "Area of " << myshapes[3]->tag() << " = " << myshapes[3]->area() << endl;
  cout << "Volume of " << myshapes[3]->tag() << " = " << myshapes[3]->volume() << endl;

  myshapes.push_back(new cuboid(4, 5, 6));
  cout << "Area of " << myshapes[4]->tag() << " = " << myshapes[4]->area() << endl;
  cout << "Volume of " << myshapes[4]->tag() << " = " << myshapes[4]->volume() << endl;

  myshapes.push_back(new sphere(3));
  cout << "Area of " << myshapes[5]->tag() << " = " << myshapes[5]->area() << endl;
  cout << "Volume of " << myshapes[5]->tag() << " = " << myshapes[5]->volume() << endl;

  myshapes.push_back(new prism(10, (shape2d*)myshapes[0]));
  cout << "Area of " << myshapes[6]->tag() << " = " << myshapes[6]->area() << endl;
  cout << "Volume of " << myshapes[6]->tag() << " = " << myshapes[6]->volume() << endl;


  myshapes.push_back(new prism(15, (shape2d*)myshapes[2]));
  cout << "Area of " << myshapes[7]->tag() << " = " << myshapes[7]->area() << endl;
  cout << "Volume of " << myshapes[7]->tag() << " = " << myshapes[7]->volume() << endl;

  // Clean up
  for(myshapeit = myshapes.begin(); myshapeit < myshapes.end(); ++myshapeit)
    delete *myshapeit;
  myshapes.clear();

  return 0;
}