#ifndef MYFIGURES_H
#define MYFIGURES_H

//C++17 minimum required


#include "QPainter"
#include "QPointF"
#include <vector>
#include <cmath>
#include <utility>

#define MY_PI 3.141592653589793

class MyFigures
{
public:
    MyFigures()=default;

    virtual void Translate(const double disp_x, const double disp_y)=0;
    virtual void Rotate(const double c_x, const double c_y, const double angle)=0;
    virtual void Rotate(const double angle)=0;
    virtual void Draw()=0;

    virtual ~MyFigures()=default;

    static QPainter* painter;

protected:

private:

};


class MyPolygon: public MyFigures
{
public:
    MyPolygon(std::vector<double>& sides, std::vector<double>& angles, std::pair<double,double> bottom_left = std::pair (0,0));
    //angle between previous and current side
    //last side will be created automatically
    //(for the first side angle will be angle between side and x axe)
    MyPolygon(std::vector<std::pair<double, double>>& vertices);
    MyPolygon(std::pair<double,double> bottom_left = std::pair (0,0));
    
    void AddSides(const double side, const double angle);
    void AddSides(std::vector<double>& sides, std::vector<double>& angles);
    MyPolygon operator+=(const std::pair<double,double> vertice);
    void AddVertices(const double x, const double y);
    void AddVertices(std::pair<double, double> vertice);
    void AddVertices(std::vector<std::pair<double, double>>& vertices);

    void Translate(const double disp_x, const double disp_y) final;
    void Rotate(const double c_x, const double c_y, const double angle) final;
    void Rotate(const double angle) override;
    void Draw() override;

    virtual ~MyPolygon()=default;

protected:

    void CalculateVertices(size_t num_new_sides);
    void CalculateSides(size_t num_new_vertices);
    std::vector<QPointF> vertices_;
    std::vector<std::pair<double, double>> sides_;
    //holds side_len=sides_.first, side_angle=sides_.second

    //sides needed for more precise rotation 
    //(in case of rotating using vertices precision will descent)
    //for displaying may be not critical, but for CAD, for example, will be

private:

    void DrawLine(QPointF start, QPointF end);
    double CalculateAngleBetweenVectors(double x2, double y2, double x1=1, double y1=0);

};

class MyRectangle: public MyPolygon
{
public:
    MyRectangle(double height, double width, double angle=MY_PI/2, std::pair<double,double> bottom_left = std::pair (0,0));
    //height along y-axis, width along x-axis
    //angle between x-axis and width
    MyRectangle(std::pair<double,double> bottom_left, std::pair<double,double> top_right);

    void Rotate(const double angle) override;
    void ChangeWidth(const double new_width);
    void ChangeHeight(const double new_height);
    virtual ~MyRectangle()=default;

protected:

private:

    using MyPolygon::AddSides;
    using MyPolygon::operator+=;
    using MyPolygon::AddVertices;
};

class MySquare: public MyRectangle
{
public:
    MySquare(double side, double angle=MY_PI/2, std::pair<double,double> bottom_left = std::pair (0,0));

    void ChangeSidesLen(const double new_len);

    virtual ~MySquare()=default;

protected:

private:

};

class MyTriangle: public MyPolygon
{
public:
    MyTriangle(double len_AB, double len_BC, double len_CA, double angle=0, std::pair<double,double> bottom_left = std::pair (0,0));
    //angle between x-axis and AB
    MyTriangle(double len_AB, double len_BC, double angle_ABC, 
                double angle=0, bool with_angle=true, std::pair<double,double> bottom_left = std::pair (0,0));

    void Rotate(const double angle);

    virtual ~MyTriangle()=default;

protected:

private:

    double CalculateAngle(double AB, double BC, double CA);
    //calculates ABC
    QPointF CalculateIncenter();
    using MyPolygon::AddSides;
    using MyPolygon::operator+=;
    using MyPolygon::AddVertices;
};

class MyEllipse: public MyFigures
{
public:
    MyEllipse(double semi_major_axis, double semi_minor_axis, double angle=0, std::pair<double,double> center = std::pair (0,0));

    void Translate(const double disp_x, const double disp_y);
    void Rotate(const double c_x, const double c_y, const double angle);
    void Rotate(const double angle);
    void Draw();

    virtual ~MyEllipse() = default;

protected:

private:

    std::pair<double, double> axis_;
    std::pair<double, double> center_;
    double angle_;
};

class MyCircle: public MyEllipse
{
public:
    MyCircle(double radius, std::pair<double,double> center = std::pair (0,0));

    virtual ~MyCircle() = default;

protected:

private:

};

#endif // MYFIGURES_H
