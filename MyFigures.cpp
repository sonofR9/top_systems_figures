#include "MyFigures.h"

QPainter* MyFigures::painter;

MyPolygon::MyPolygon(std::vector<double>& sides, std::vector<double>& angles, std::pair<double,double> bottom_left):
    MyFigures()
{
    vertices_.push_back(QPointF(bottom_left.first, bottom_left.second));
    for (size_t i{0}; i<sides.size(); ++i)
        sides_.push_back(std::pair(sides[i],angles[i]));
    CalculateVertices(sides.size());
}

MyPolygon::MyPolygon(std::vector<std::pair<double, double>>& vertices):
    MyFigures()
{
    for (auto itt: vertices)
        vertices_.push_back(QPointF(itt.first, itt.second));
    CalculateSides(vertices.size());
}

MyPolygon::MyPolygon(std::pair<double,double> bottom_left): 
    MyFigures(), vertices_{QPointF(bottom_left.first, bottom_left.second)}{}

void MyPolygon::AddSides(const double side, const double angle)
{
    sides_.push_back(std::pair(side,angle));
    CalculateVertices(1);
}

void MyPolygon::AddSides(std::vector<double>& sides, std::vector<double>& angles)
{
    for (size_t i{0}; i<sides.size(); ++i)
        sides_.push_back(std::pair(sides[i],angles[i]));
    CalculateVertices(sides.size());
}

MyPolygon MyPolygon::operator+=(const std::pair<double,double> vertice)
{
    AddVertices(vertice);
    return *this;
}


void MyPolygon::AddVertices(const double x, const double y)
{
    AddVertices(std::pair(x,y));
}

void MyPolygon::AddVertices(std::pair<double, double> vertice)
{
    vertices_.push_back(QPointF(vertice.first, vertice.second));
    CalculateSides(1);
}

void MyPolygon::AddVertices(std::vector<std::pair<double, double>>& vertices)
{
    for (auto itt: vertices)
        vertices_.push_back(QPointF(itt.first, itt.second));
    CalculateSides(vertices.size());
}

void MyPolygon::Translate(const double disp_x, const double disp_y)
{
    for (int i{0};i<vertices_.size(); ++i)
    {
        vertices_[i].setX(vertices_[i].x()+disp_x);
        vertices_[i].setY(vertices_[i].y()+disp_y);
    }
}

void MyPolygon::Rotate(const double c_x, const double c_y, const double angle)
{
    sides_[0].second+=angle;
    double delta_x=c_x-vertices_[0].x();
    double delta_y=c_y-vertices_[0].y();
    vertices_[0].setX(c_x+delta_x*std::cos(angle)-delta_y*std::sin(angle));
    vertices_[0].setY(c_y+delta_x*std::sin(angle)+delta_y*std::cos(angle));
    QPointF ver_0 = vertices_[0];
    vertices_.clear();
    vertices_.push_back(ver_0);
    CalculateVertices(sides_.size());
}

void MyPolygon::Rotate(const double angle)
{
    Rotate(vertices_[0].x(),vertices_[0].y(),angle);
}

void MyPolygon::Draw()
{
    size_t v_size=vertices_.size();
    for (size_t i{0}; i<v_size; ++i)
        DrawLine(vertices_[i], vertices_[(i+1) % v_size]);
}

void MyPolygon::CalculateSides(size_t num_new_sides)
{
    for (size_t i{vertices_.size()-num_new_sides};i<vertices_.size(); ++i)
    {
        double x1=vertices_[i].x(), x2=vertices_[i-1].x();
        double y1=vertices_[i].y(), y2=vertices_[i-1].y();
        double len=std::hypot(x1-x2,y1-y2);
        double angle;
        if (i>2)
            angle = CalculateAngleBetweenVectors(x1-x2, y1-y2, x2-vertices_[i-2].x(), y2-vertices_[i-2].y());
        else
            angle = CalculateAngleBetweenVectors(x1-x2, y1-y2);
        sides_.push_back(std::pair(len,angle));
    }
}

void MyPolygon::CalculateVertices(size_t num_new_vertices)
{
    //vertice[i] is end of side[i-1], because vertice[0] is starting point
    for (size_t i{sides_.size()-num_new_vertices};i<sides_.size(); ++i)
    {
        double x,y;
        double angle{MY_PI-sides_[i].second};
        if (i>0)
        {
            double x1=vertices_[i].x(), x2=vertices_[i-1].x();
            double y1=vertices_[i].y(), y2=vertices_[i-1].y();
            angle+=CalculateAngleBetweenVectors(x1-x2,y1-y2);
        }
        x = vertices_[i].x()+sides_[i].first*std::cos(angle);
        y = vertices_[i].y()+sides_[i].first*std::sin(angle);
        vertices_.push_back(QPointF(x,y));
    }
}

double MyPolygon::CalculateAngleBetweenVectors(double x2, double y2, double x1, double y1)
{
    double dot = x1*x2+y1*y2;
    double det = x1*y2-x2*y1;
    return std::atan2(det,dot);
}

void MyPolygon::DrawLine(QPointF start, QPointF end)
{
    double x_now = start.x(), y_now = start.y();
    double x_end = end.x(), y_end = end.y();
    double delta = 0.5;
    double angle = CalculateAngleBetweenVectors(x_end-x_now, y_end-y_now, 1, 0);
    double ang_cos = std::cos(angle);
    double ang_sin = std::sin(angle);
    QPointF point_now;
    double num_itt{std::hypot(x_end-x_now, y_end-y_now)/delta};
    double i{0};
    while (i<num_itt)
    {
        ++i;
        point_now.setX(x_now);
        point_now.setY(y_now);
        MyFigures::painter->drawPoint(point_now);
        x_now+=delta*ang_cos;
        y_now+=delta*ang_sin;
    }
}

//--------------------------------------------MyRectangle-----------------------------------------------

MyRectangle::MyRectangle(double height, double width, double angle, std::pair<double,double> bottom_left):
    MyPolygon(bottom_left)
{
    AddSides(height, angle);
    AddSides(width, -MY_PI/2);
    AddSides(height, -MY_PI/2);
}

MyRectangle::MyRectangle(std::pair<double,double> bottom_left, std::pair<double,double> top_right):
    MyRectangle(top_right.second-bottom_left.second, top_right.first-bottom_left.first, MY_PI/2, bottom_left)
{}

void MyRectangle::Rotate(const double angle)
{
    auto top_right = vertices_[2];
    auto bottom_left = vertices_[0];
    MyPolygon::Rotate((top_right.x()+bottom_left.x())/2,(top_right.y()+bottom_left.y())/2,angle);
}

void MyRectangle::ChangeWidth(const double new_width)
{
    auto height = sides_[0].first; 
    auto angle = sides_[0].second;
    auto bottom_left = vertices_[0];
    vertices_.clear();
    sides_.clear();
    vertices_.push_back(bottom_left);
    AddSides(height, angle);
    AddSides(new_width, -MY_PI/2);
    AddSides(height, -MY_PI/2);
}

void MyRectangle::ChangeHeight(const double new_height)
{
    auto width = sides_[1].first;
    auto angle = sides_[0].second;
    auto bottom_left = vertices_[0];
    vertices_.clear();
    sides_.clear();
    vertices_.push_back(bottom_left);
    AddSides(new_height, angle);
    AddSides(width, -MY_PI/2);
    AddSides(new_height, -MY_PI/2);
}

//----------------------------------MySquare-------------------------------

MySquare::MySquare(double side, double angle, std::pair<double,double> bottom_left):
MyRectangle(side, side, angle, bottom_left) {}

void MySquare::ChangeSidesLen(const double new_len)
{
    ChangeHeight(new_len);
    ChangeWidth(new_len);
}

//---------------------------------MyTriangle-------------------------------

MyTriangle::MyTriangle(double len_AB, double len_BC, double len_CA, double angle, std::pair<double,double> bottom_left):
MyPolygon(bottom_left)
{
    AddSides(len_AB, angle);
    AddSides(len_BC, CalculateAngle(len_AB, len_BC, len_CA));
}

MyTriangle::MyTriangle(double len_AB, double len_BC, double angle_ABC, 
            double angle, bool with_angle, std::pair<double,double> bottom_left):
MyPolygon(bottom_left)
{
    AddSides(len_AB, angle);
    AddSides(len_BC, angle_ABC);
}

void MyTriangle::Rotate(const double angle)
{
    QPointF incenter = CalculateIncenter();
    MyPolygon::Rotate(incenter.x(), incenter.y(), angle);
}

double MyTriangle::CalculateAngle(double AB, double BC, double CA)
{
    double ABC;
    ABC=(AB*AB+BC*BC-CA*CA)/(2*AB*BC);
    ABC=std::acos(ABC);
    return ABC;
}

QPointF MyTriangle::CalculateIncenter()
{
    QPointF result(0,0);
    std::vector<double> sides;
    sides.push_back(sides_[0].first);
    sides.push_back(sides_[1].first);
    sides.push_back(std::hypot(vertices_[0].x()-vertices_[2].x(), vertices_[0].y()-vertices_[2].y()));
    for (int i{0}; i<3; ++i)
    {
        result.setX(result.x()+sides[(i+1)%3]*vertices_[i].x());
        result.setY(result.y()+sides[(i+1)%3]*vertices_[i].y());
    }
    return result;
}

//-------------------------------MyEllipse---------------------------------

MyEllipse::MyEllipse(double semi_major_axis, double semi_minor_axis, double angle, std::pair<double,double> center):
    MyFigures(), axis_(std::pair(semi_minor_axis, semi_major_axis)), center_(center), angle_(angle)
{}

void MyEllipse::Translate(const double disp_x, const double disp_y)
{
    center_.first+=disp_x;
    center_.second+=disp_y;
}

void MyEllipse::Rotate(const double c_x, const double c_y, const double angle)
{
    angle_+=angle;
    double delta_x=c_x-center_.first;
    double delta_y=c_y-center_.second;
    center_.first=c_x+delta_x*std::cos(angle)-delta_y*std::sin(angle);
    center_.second=c_y+delta_x*std::sin(angle)+delta_y*std::cos(angle);

}

void MyEllipse::Rotate(const double angle)
{
    Rotate(center_.first, center_.second, angle);
}

void MyEllipse::Draw()
{
    double c_x = center_.first, c_y = center_.second;
    double a_axis = axis_.first, b_axis = axis_.second;
    double ang_cos = std::cos(angle_), ang_sin = std::sin(angle_);
    QPointF point_now;
    for (double phi{0}; phi<2*MY_PI; phi+=0.01)
    {
        point_now.setX(c_x+b_axis*std::sin(phi)*ang_cos-a_axis*std::cos(phi)*ang_sin);
        point_now.setY(c_y+b_axis*std::sin(phi)*ang_sin+a_axis*std::cos(phi)*ang_cos);
        MyFigures::painter->drawPoint(point_now);
    }
}

//-------------------------------MyCircle---------------------------------

MyCircle::MyCircle(double radius, std::pair<double,double> center):
    MyEllipse(radius, radius, 0, center)
{}
