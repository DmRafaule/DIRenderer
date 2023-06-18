#include "camera.hpp"
#include "DIWindow/win.hpp"
#include "DIWindow/viewport.hpp"

namespace DI{

  extern WinData* eg_winData;

  SphericalCamera::SphericalCamera(float& zenit, float& azimut, float& radius) : 
    zenit(zenit), azimut(azimut), radius(radius){
     
  }

  SphericalCamera::~SphericalCamera(){
    
  }

  void SphericalCamera::Set() {
    fovy    = 45.0f;
    ratio   = eg_winData->size.x/eg_winData->size.y;
    position = glm::vec3(3.0f,3.0f,3.0f);
    front   = glm::vec3(0,0,0);
    up      = glm::vec3(0.0f,1.0f,0.0f);

    eye = glm::lookAt(position,position + front,up);
    projection = glm::perspective(glm::radians(fovy),ratio,0.1f,100.0f);
  }

  void SphericalCamera::SetPosition(float& zenit, float& azimut, float& radius){
    this->zenit = zenit;
    this->azimut = azimut;
    this->radius = radius;
    // Zenit(or angle bettwen fundamental plane and OY) are not define in 180 and 0 angle so ..
    if (zenit >= 180)
      zenit--;
    else if (zenit <= 0 )
      zenit++;
    // OX and OZ make fundamental plane
    position.z = sin(glm::radians(zenit)) * cos(glm::radians(azimut)) * radius;
    position.x = sin(glm::radians(zenit)) * sin(glm::radians(azimut)) * radius;
    position.y = cos(glm::radians(zenit)) * radius;
  }

  void SphericalCamera::SetZoom(const float& zoom){
    radius -= zoom;
    SetPosition(zenit,azimut,radius);
  }
  
  void SphericalCamera::UnSet() {
    delete this; 
  }
  
  void SphericalCamera::Use() {
    //TODO: remove this to old one 
    ratio = eg_winData->viewports[ViewportsSupportedID::MAIN_VIEWPORT]->w / eg_winData->viewports[ViewportsSupportedID::MAIN_VIEWPORT]->h; 
    eye = glm::lookAt(position,front,up);
    projection = glm::perspective(glm::radians(fovy),ratio,0.1f,100.0f);
  }
  void SphericalCamera::Use(ViewportData& viewport){
    ratio = viewport.w / viewport.h; 
    eye = glm::lookAt(position,front,up);
    projection = glm::perspective(glm::radians(fovy),ratio,0.1f,100.0f);
  }


  PsevdoOrthographicCamera::PsevdoOrthographicCamera(){
     
  }

  PsevdoOrthographicCamera::~PsevdoOrthographicCamera(){
    
  }
  void PsevdoOrthographicCamera::Set(){
    fovy = 60.0f;
    ratio = 1.0f;
    // Set view position
    position = glm::vec3(0.0f,0.0f,sqrt(3.0f)); 
    // Set view direction to see 
    front = glm::vec3(0.0f,0.0f,0.0f);
    // Set right axis
    up = glm::vec3(0.f,1.f,0.f);

    // Set view matrix
    eye = glm::lookAt(position,front,up);
    // Set projection matrix
    projection = glm::perspective(glm::radians(fovy),ratio,0.1f,100.0f);
  }
  void PsevdoOrthographicCamera::UnSet() {
    delete this; 
  }
  void PsevdoOrthographicCamera::Use() {
    eye = glm::lookAt(position,front,up);
    projection = glm::perspective(glm::radians(fovy),ratio,0.1f,100.0f);
  }
  void PsevdoOrthographicCamera::Use(ViewportData& viewport){

  }



  OrthographicCamera::OrthographicCamera(){
    eye = glm::mat4(1.0f); 
    projection = glm::mat4(1.0f);
    left = 0;
    right = 0;
    bottom = 0;
    top = 0;
  }

  OrthographicCamera::~OrthographicCamera(){
    
  }

  void OrthographicCamera::Set() {
    left = 0;
    right = eg_winData->size.x;
    bottom = 0;
    top = eg_winData->size.y;

    projection = glm::ortho(left,right,bottom,top);
  }
  
  void OrthographicCamera::UnSet() {
    delete this; 
  }
  
  void OrthographicCamera::Use() {
    left = 0;
    right = eg_winData->size.x;
    bottom = 0;
    top = eg_winData->size.y;

    projection = glm::ortho(left,right,bottom,top);
  }
  void OrthographicCamera::Use(ViewportData& viewport){
    left = viewport.x;
    right = viewport.w;
    bottom = viewport.y;
    top = viewport.h;

    projection = glm::ortho(0.f,right,0.f,top);
  }


  void CameraHandler::Set(Camera& camera){
    camera.Set();  
  }
  void CameraHandler::UnSet(Camera& camera){
    camera.UnSet(); 
  }
  void CameraHandler::Use(Camera& camera){
    camera.Use(); 
  }
  void CameraHandler::Use(Camera& camera, ViewportData& viewport){
    camera.Use(viewport);
  }


}
