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

enum class ShapeType {
	Square = 0, Rectangle, Triangle, Circle 
};

float totalAreaVTBL(uint32_t shapeCount, ShapeBase** shapes, ShapeType* shapeType) {
	float accum = 0.0f;
	for(uint32_t shapeIndex = 0; shapeIndex < shapeCount; ++shapeIndex) {
		switch(shapeType[shapeIndex]) {
			case ShapeType::Square:
				accum += ((Square*)shapes[shapeIndex])->area();
				break;
			case ShapeType::Rectangle:
				accum += ((Rectangle*)shapes[shapeIndex])->area();
				break;
			case ShapeType::Triangle:
				accum += ((Triangle*)shapes[shapeIndex])->area();
				break;
			case ShapeType::Circle:
				accum += ((Circle*)shapes[shapeIndex])->area();
				break;
		}
		
	}
	
	return accum;
}

//TODO: MEMORY ERROR
float totalAreaVTBL4(uint32_t ShapeCount, ShapeBase** shapes, ShapeType* shapeType) {
    float accum0 = 0.0f;
    float accum1 = 0.0f;
    float accum2 = 0.0f;
    float accum3 = 0.0f;
    
    uint32_t count = ShapeCount/4;
    while(count--) {
		switch(shapeType[count]) {
			case ShapeType::Square:
				accum0 += ((Square*)shapes[0])->area();
				accum1 += ((Square*)shapes[1])->area();
				accum2 += ((Square*)shapes[2])->area();
				accum3 += ((Square*)shapes[3])->area();
				break;
			case ShapeType::Rectangle:
				accum0 += ((Rectangle*)shapes[0])->area();
				accum1 += ((Rectangle*)shapes[1])->area();
				accum2 += ((Rectangle*)shapes[2])->area();
				accum3 += ((Rectangle*)shapes[3])->area();
				break;
			case ShapeType::Triangle:
				accum0 += ((Triangle*)shapes[0])->area();
				accum1 += ((Triangle*)shapes[1])->area();
				accum2 += ((Triangle*)shapes[2])->area();
				accum3 += ((Triangle*)shapes[3])->area();
				break;
			case ShapeType::Circle:
				accum0 += ((Circle*)shapes[0])->area();
				accum1 += ((Circle*)shapes[1])->area();
				accum2 += ((Circle*)shapes[2])->area();
				accum3 += ((Circle*)shapes[3])->area();
				break;
		}
        
        shapes += 4;
		shapeType += 4;
    }
    
    float Result = (accum0 + accum1 + accum2 + accum3);
    return Result;
}

void createRandomFigures(ShapeBase** shapes, ShapeType* shapeTypes, uint32_t count) {
	for(uint32_t i = 0; i < count; i++) {
		switch(i%4) {
			case 0:
				shapes[i] = new Square(2);
				shapeTypes[i] = ShapeType::Square;
				break;
			case 1:
				shapes[i] = new Rectangle(1,2);
				shapeTypes[i] = ShapeType::Rectangle;
				break;
			case 2:
				shapes[i] = new Triangle(10, 1);
				shapeTypes[i] = ShapeType::Triangle;
				break;
			case 3:
				shapes[i] = new Circle(20);
				shapeTypes[i] = ShapeType::Circle;
				break;
		}
	}
}



int main() {
	uint32_t count = 50000000;

	// Create a lot of shapes
	ShapeBase** shapes = new ShapeBase*[count];
	ShapeType* shapeType = new ShapeType[count];

	createRandomFigures(shapes, shapeType, count);

	// Test
	uint64_t timeBefore = get_time_nano_since_epoch();
	float testTotalAreaVTBL = totalAreaVTBL(count, shapes, shapeType);
	uint64_t timeAfter = get_time_nano_since_epoch();
	double time_taken_seconds = (double)(timeAfter - timeBefore) / 1e9;
	printf("Time taken to get total area for %d uniform dist. shapes: %lf\n", count, time_taken_seconds);
	printf("Result: %f\n", testTotalAreaVTBL);

	printf("\n\n\n");

	timeBefore = get_time_nano_since_epoch();
	float testTotalAreaVTBL4 = totalAreaVTBL4(count, shapes, shapeType);
	timeAfter = get_time_nano_since_epoch();
	time_taken_seconds = (double)(timeAfter - timeBefore) / 1e9;
	printf("Time taken to get total area for %d uniform dist. shapes unroll version: %lf\n", count, time_taken_seconds);
	printf("Result: %f\n", testTotalAreaVTBL4);
}