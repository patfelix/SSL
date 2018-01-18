#include <assert.h>
#include "curve.h"

DifferentiableVelocityConsign::DifferentiableVelocityConsign(
    double distance, 
    double max_velocity,
    double max_acceleration 
):
    distance(distance), max_velocity(max_velocity),
    max_acceleration(max_acceleration)
{ 
    assert(max_acceleration > 0.0); //Acceleration should be Greater than 0.
    if( not ( distance >= (2*max_velocity*max_velocity/max_acceleration) ) ){
        std::cerr << "Warning : Distance curve is too short for given maximal velocity and acceleration." << std::endl;
        std::cerr << "Warning :    we should have : distance > 2*max_velocity*max_velocity/max_acceleration" << std::endl;
        double old_velocity = max_velocity;
        max_velocity = std::sqrt( distance*max_acceleration/2.0 );
        std::cerr << "Warning :    we reduce the velocity from " << old_velocity << " to " << max_velocity << std::endl;
    }
}

double DifferentiableVelocityConsign::operator()(double t){
    double x = time_of_acceleration();
    double a = max_acceleration;
    double tm = time_of_deplacement(); 
    if( t<=0 ) return 0;
    if( t<= x/2.0 ) return a*t*t/x;
    if( t<= x ) return a*x/2 - a*(t-x)*(t-x)/x;
    if( t<= tm-x ) return a*x/2;
    if( t<= tm-x/2.0 ) return a*x/2 - a*(t-tm+x)*(t-tm+x)/x;
    if( t<= tm ) return a*(t-tm)*(t-tm)/x;
    return 0.0;
}
double DifferentiableVelocityConsign::time_of_deplacement(){
    double x = time_of_acceleration();
    double a = max_acceleration;
    return 2*distance/(a*x)+x;
}
double DifferentiableVelocityConsign::time_of_acceleration(){
    return 2*max_velocity/max_acceleration;
}





ContinuousVelocityConsign::ContinuousVelocityConsign(
    double distance, 
    double max_velocity,
    double max_acceleration 
):
    distance(distance), max_velocity(max_velocity),
    max_acceleration(max_acceleration)
{ 
    assert(max_acceleration > 0.0); //Acceleration should be Greater than 0.
}

double ContinuousVelocityConsign::operator()(double t){
    double x = time_of_acceleration();
    double a = max_acceleration;
    double tm = time_of_deplacement();
    assert( a > 0 );
    assert( tm > 0 );
    assert( x > 0 );
    if( t <= 0 ) return 0;
    if( t <= x ) return a*t;
    if( t <= tm-x ) return a*x;
    if( t <= tm ) return -a*(t-tm);
    return 0.0;
}
double ContinuousVelocityConsign::time_of_deplacement(){
    double x = time_of_acceleration();
    double a = max_acceleration;
    assert( a > 0 );
    assert( x > 0 );
    return distance/(a*x) + x;
}
double ContinuousVelocityConsign::time_of_acceleration(){
    return max_velocity/max_acceleration;
}



















void Curve2d::init(){
    this->curve_length = arc_length( 1.0 );
}

Curve2d::Curve2d(
    const std::function<Eigen::Vector2d (double u)> & curve,
    double step_time
):curve(curve), step_time(step_time){
    init();
};

Curve2d::Curve2d( const Curve2d & curve ):
    curve(curve.curve), step_time(curve.step_time)
{
    init();
};

Eigen::Vector2d Curve2d::operator()(double u) const {
    return this->curve(u);
}

double Curve2d::size() const {
    return this->curve_length;
}

double Curve2d::arc_length( double u ) const {
    if( u<=0 ) return 0;
    if( u>1.0 ) return this->curve_length;

    double res = 0;
    Eigen::Vector2d old = curve( 0.0 );
    for( double v = 0.0; v <= u; v+=this->step_time ){
        Eigen::Vector2d current = curve( v );
        res += ( current - old ).norm();
        old = current;
    }
    Eigen::Vector2d current = curve( u );
    res += ( current - old ).norm();
    return res;
}

double Curve2d::inverse_of_arc_length( double l ) const {
    if( l<= 0 ) return 0.0;
    if( l>= this->curve_length ) return 1.0;
    double res = 0;
    Eigen::Vector2d old = curve( 0.0 );
    double v;
    for( v = 0.0; res < l; v+=this->step_time ){
        Eigen::Vector2d current = curve( v );
        res += ( current - old ).norm();
        old = current;
    }
    return v;
}




void RenormalizedCurve::init(){
    this->time_max = time(this->curve_length);
}

RenormalizedCurve::RenormalizedCurve(
    const std::function<Eigen::Vector2d (double u)> & curve,
    const std::function<double (double t)> & velocity_consign,
    double step_time
):Curve2d(curve, step_time), velocity_consign(velocity_consign){
    init();
};

RenormalizedCurve::RenormalizedCurve(
    const Curve2d & curve,
    const std::function<double (double t)> & velocity_consign
):Curve2d(curve), velocity_consign(velocity_consign){
    init();
};

double RenormalizedCurve::max_time() const {
    return this->time_max;
}

Eigen::Vector2d RenormalizedCurve::original_curve( double u ) const {
    return curve(u);
}

void RenormalizedCurve::set_step_time( double dt ){
    assert( dt>0.0 );
    this->step_time = dt;
    init();
}

double RenormalizedCurve::get_step_time( ) const {
    return this->step_time;
}

double RenormalizedCurve::position_consign( double t ) const {
    double res = 0.0;
    for(double u=0; u<t; u+=this->step_time){
        res += this->step_time * velocity_consign(u);
    } 
    return res;
}

double RenormalizedCurve::error_position_consign() const {
    double max_velocity = 0;
    for( double t = 0; t<max_time(); t+= this->step_time ){
        max_velocity = std::max( max_velocity, velocity_consign(t) );
    }
    return this->step_time * max_velocity;
}

double RenormalizedCurve::time( double length ) const {
    assert( 0<= length and length <= this->curve_length );
    double res = 0.0;
    double t;
    
    for( t=0.0; res < length-0.001; t+=this->step_time ){
        assert( velocity_consign(t) >= 0.0 );
//        DEBUG( "res : " << res );
//        DEBUG( "length : " << length );
//        DEBUG( "t : " << t << ", step_time" << this->step_time );
//        DEBUG( "velocity_consign(t) : " << velocity_consign(t) << " -  " << (velocity_consign(t)==0.0) );
        res += this->step_time * velocity_consign(t);
    } 
    return t;
}

Eigen::Vector2d RenormalizedCurve::operator()(double t) const {
    return original_curve( this->inverse_of_arc_length( position_consign(t) ) );
}