#include <cstdio>
#include <cstdint>
#include <cmath>
#include <chrono>

inline uint64_t get_time_nano_since_epoch() {
	auto time_point_now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


class ShapeBase {
public:
    ShapeBase() {}
    virtual float area() = 0;
};

class Square : public ShapeBase {
public:
    Square(float sideInit) : side(sideInit) {}
    virtual float area() {return side*side;}
    
private:
    float side;
};

class Rectangle : public ShapeBase {
public:
    Rectangle(float widthInit, float heightInit) : width(widthInit), height(heightInit) {}
    virtual float area() {return width*height;}
    
private:
    float width, height;
};

class Triangle : public ShapeBase {
public:
    Triangle(float BaseInit, float heightInit) : base(BaseInit), height(heightInit) {}
    virtual float area() {return 0.5f*base*height;}
    
private:
    float base, height;
};

class Circle : public ShapeBase {
public:
    Circle(float radiusInit) : radius(radiusInit) {}
    virtual float area() {return M_PI*radius*radius;}
    
private:
    float radius;
};

float totalAreaVTBL(uint32_t shapeCount, ShapeBase** shapes) {
	float accum = 0.0f;
	for(uint32_t shapeIndex = 0; shapeIndex < shapeCount; ++shapeIndex) {
		accum += shapes[shapeIndex]->area();
	}
	
	return accum;
}

float totalAreaVTBL4(uint32_t ShapeCount, ShapeBase** shapes) {
    float accum0 = 0.0f;
    float accum1 = 0.0f;
    float accum2 = 0.0f;
    float accum3 = 0.0f;
    
    uint32_t Count = ShapeCount/4;
    while(Count--) {
        accum0 += shapes[0]->area();
        accum1 += shapes[1]->area();
        accum2 += shapes[2]->area();
        accum3 += shapes[3]->area();
        
        shapes += 4;
    }
    
    float Result = (accum0 + accum1 + accum2 + accum3);
    return Result;
}

void createRandomFigures(ShapeBase** shapes, uint32_t count) {
	for(uint32_t i = 0; i < count; i++) {
		switch(i%4) {
			case 0:
				shapes[i] = new Square(2);
				break;
			case 1:
				shapes[i] = new Rectangle(1,2);
				break;
			case 2:
				shapes[i] = new Triangle(10, 1);
				break;
			case 3:
				shapes[i] = new Circle(20);
				break;
		}
	}
}

int main() {
	uint32_t count = 50000000;

	// Create a lot of shapes
	ShapeBase** shapes = new ShapeBase*[count];
	createRandomFigures(shapes, count);

	// Test
	uint64_t timeBefore = get_time_nano_since_epoch();
	float testTotalAreaVTBL = totalAreaVTBL(count, shapes);
	uint64_t timeAfter = get_time_nano_since_epoch();
	double time_taken_seconds = (double)(timeAfter - timeBefore) / 1e9;
	printf("Time taken to get total area for %d uniform dist. shapes: %lf\n", count, time_taken_seconds);
	printf("Result: %f\n", testTotalAreaVTBL);

	printf("\n\n\n");

	timeBefore = get_time_nano_since_epoch();
	float testTotalAreaVTBL4 = totalAreaVTBL4(count, shapes);
	timeAfter = get_time_nano_since_epoch();
	time_taken_seconds = (double)(timeAfter - timeBefore) / 1e9;
	printf("Time taken to get total area for %d uniform dist. shapes unroll version: %lf\n", count, time_taken_seconds);
	printf("Result: %f\n", testTotalAreaVTBL4);
}