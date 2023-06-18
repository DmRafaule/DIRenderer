#pragma once 

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DI{

  class PsevdoOrthographicCamera;
  class PerspectiveCamera;
  class OrthographicCamera;
  class CameraHandler;
  struct ViewportData;
  // Base class for cameras
  class Camera {
    friend class CameraHandler;
    protected:
      glm::mat4 projection;
      glm::mat4 eye;  // View matrix
      glm::vec3 position;  // View position
      glm::vec3 front;// View focus
      glm::vec3 up;   // View up axis
    private:
      virtual void Set()    = 0;
      virtual void UnSet()  = 0;
      virtual void Use()    = 0;
      virtual void Use(ViewportData& viewport) = 0;
    public:
      inline glm::mat4& GetProjection(){ return projection; }
      inline glm::mat4& GetEye(){ return eye; }
  };

  class SphericalCamera : public Camera{
    friend class CameraHandler;
    private:
      float fovy;
      float ratio; 
      float radius;
      float azimut;
      float zenit;
    private:
      void Set() override final;   
      void UnSet() override final; 
      void Use() override final;   
      void Use(ViewportData& viewport) override final;
    public:
      SphericalCamera(float& zenit, float& azimut, float& radius);
      ~SphericalCamera();
      // Set position in sphere coordinat system
      void SetPosition(float& zenit, float& azimut, float& radius);
      // Set focus (view.front) of view via mouse pointer
      void SetZoom(const float& zoom);
  };

  class PsevdoOrthographicCamera : public Camera{
    friend class CameraHandler;
    private:
      float fovy;
      float ratio; 
    private:
      void Set() override final;   
      void UnSet() override final; 
      void Use() override final;   
      void Use(ViewportData& viewport) override final;
    public:
      PsevdoOrthographicCamera();
      ~PsevdoOrthographicCamera();
  };

  class OrthographicCamera : public Camera{
    friend class CameraHandler;
    private:
      float left, right, bottom, top;
    private:
      void Set() override final;   
      void UnSet() override final; 
      void Use() override final;   
      void Use(ViewportData& viewport) override final;
    public:
      inline float GetLeft() { return left; }
      inline float GetRight() { return right; }
      inline float GetBottom() { return bottom; }
      inline float GetTop() { return top; }
      OrthographicCamera();
      ~OrthographicCamera();
  };

  class CameraHandler {
    private:
      CameraHandler();
      ~CameraHandler();
    public:
      static void Set(Camera& camera);
      static void UnSet(Camera& camera);
      static void Use(Camera& camera);
      static void Use(Camera& camera, ViewportData& viewport);
  };

}
