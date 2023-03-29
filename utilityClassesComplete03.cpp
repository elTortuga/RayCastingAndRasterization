#include <iostream>
#include <vector>
#include <cmath>

#define INFINITY (std::numeric_limits<float>::infinity())

class Vector2 { public: float x, y;};
class Vector3 { public: float x, y, z;};
typedef Vector2 Point2;
typedef Vector3 Point3;
class Color3 { public: float r, g, b;};
typedef Color3 Radiance3;
typedef Color3 Power3;

class Ray
{
private:
    Point3 m_origin;
    Vector3 m_direction;
public:
    Ray(const Point3& org, const Vector3& dir) :
        m_origin(org), m_direction(dir) {}
    
    const Point3& origin() const { return m_origin;}

    const Vector3& direction() const { return m_direction;}
};

class Image
{
private:
    int                    m_width;
    int                    m_height;
    std::vector<Radiance3> m_data;
    int PPMGammaEncode(float radiance, float displayConstant) const {
        return int(pow(std::min(1.0f, std::max(0.0f, radiance * displayConstant)),
            1.0f / 2.2f) * 255.0f);
    };

public:
    Image(int width, int height) :
        m_width(width), m_height(height), m_data(width*height) {}

    int width() const { return m_width;}

    int height() const { return m_height;}

    void set(int x, int y, const Radiance3& value) {
        m_data[x + y * m_width] = value;
    }

    const Radiance3& get(int x, int y) const {
        return m_data[x + y * m_width];
    }

    void save(const std::string& filename, float displayConstant=15.0f) const {
        std::cout << filename;
        FILE* file = fopen(filename.c_str(), "wt");
        fprintf(file, "P3 %d %d 255\n", m_width, m_height);
        for (int y = 0; y < m_height; y++) {
            fprintf(file, "\n# y = %d\n", y);
            for (int x = 0; x < m_width; x++) {
                const Radiance3& c(get(x,y));
                fprintf(file, "%d %d %d\n",
                        PPMGammaEncode(c.r, displayConstant),
                        PPMGammaEncode(c.g, displayConstant),
                        PPMGammaEncode(c.b, displayConstant));
            }            
        }
        fclose(file);
    }
};


int main(int argc, char const *argv[])
{
    Image img(400, 400);
    for (int i = 0; i < 400; i++) {
        for (int j = 0; j < 400; j++) {
            Radiance3 r = Radiance3();
            r.r = ((i+j) % 2) * (0.05f * j/400.0f);
            r.g = ((i+j) % 2) * (0.05f * j/400.0f);
            r.b =               (0.05f * j/400.0f);
            img.set(i,j, r);
        }        
    }

    img.save("myPPM.ppm");
    return 0;
};
