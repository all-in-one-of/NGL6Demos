#include "NGLDraw.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <SDL.h>
const static float INCREMENT=0.01;
const static float ZOOM=0.05;
NGLDraw::NGLDraw()
{
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
   // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();
  // the shader will use the currently active material and light0 so set them
  ngl::Material m(ngl::GOLD);
  // load our material values to the shader into the structure material (see Vertex shader)
  m.loadToShader("material");
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,1,1);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)720.0/576.0,0.05,350);
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  m_light = new ngl::Light(ngl::Vec3(-2,5,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT );
  m_light->setTransform(iv);
  // load these values to the shader as well
  m_light->loadToShader("light");
  m_text = new Text("font/arial.ttf",40);
  m_text->setColour(1.0,0.0,0.0);
}

NGLDraw::~NGLDraw()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
  delete m_cam;
  Init->NGLQuit();
}

void NGLDraw::resize(int _w, int _h)
{
  glViewport(0,0,_w,_h);
  // now set the camera size values as the screen size has changed
  m_cam->setShape(45,(float)_w/_h,0.05,350);
  std::cout<<"resize\n";
  m_text->setScreenSize(_w,_h);
  // Ok so this is still a bit of a hack we need to scale our text based
  // on max screen size so first get the size of the screen
  SDL_Rect s;
  SDL_GetDisplayBounds(0,&s);
  float x,y;
  // now get a scale transform for the text shader


  x=1.0-float(s.w-_w)/s.w;
  y=1.0-float(s.h-_h)/s.h;
  std::cout<<s.w-_w<<" "<<s.h-_h<<"\n";
  std::cout<<x<<" "<<y<<"\n";
  // now set the new transform element for this shader
  m_text->setTransform(x,y);
  draw();

}

void NGLDraw::draw()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

  // Rotation based on the mouse position for our global transform
  ngl::Transformation trans;
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  // draw
  loadMatricesToShader();
  prim->draw("teapot");
  m_text->setColour(1,1,1);
  m_text->renderText(10,40,"This is a drop in repleacement for ngl::Text");
  m_text->renderText(10,80,"ngl::Text uses Qt and if your using SDL this will crash");
  m_text->setColour(1,1,1);
  m_text->renderText(10,120,"Instead use these Text.cpp Text.h files in your class");
  m_text->renderText(10,180,"Check the Source code and .pro file as you will need to use SDL_ttf");


  m_text->renderText(1000,240,"Check Right Border");
  m_text->renderText(10,840,"Check bottom");


}


void NGLDraw::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transformStack.getCurrentTransform().getMatrix()*m_mouseGlobalTX;
  MV=  M*m_cam->getViewMatrix();
  MVP= M*m_cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mouseMoveEvent (const SDL_MouseMotionEvent &_event)
{
  if(m_rotate && _event.state &SDL_BUTTON_LMASK)
  {
    int diffx=_event.x-m_origX;
    int diffy=_event.y-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event.x;
    m_origY = _event.y;
    this->draw();

  }
  // right mouse translate code
  else if(m_translate && _event.state &SDL_BUTTON_RMASK)
  {
    int diffX = (int)(_event.x - m_origXPos);
    int diffY = (int)(_event.y - m_origYPos);
    m_origXPos=_event.x;
    m_origYPos=_event.y;
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    this->draw();
  }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mousePressEvent (const SDL_MouseButtonEvent &_event)
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event.button == SDL_BUTTON_LEFT)
  {
    m_origX = _event.x;
    m_origY = _event.y;
    m_rotate =true;
  }
  // right mouse translate mode
  else if(_event.button == SDL_BUTTON_RIGHT)
  {
    m_origXPos = _event.x;
    m_origYPos = _event.y;
    m_translate=true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mouseReleaseEvent (const SDL_MouseButtonEvent &_event)
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event.button == SDL_BUTTON_LEFT)
  {
    m_rotate=false;
  }
  // right mouse translate mode
  if (_event.button == SDL_BUTTON_RIGHT)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::wheelEvent(const SDL_MouseWheelEvent &_event)
{

  // check the diff of the wheel position (0 means no change)
  if(_event.y > 0)
  {
    m_modelPos.m_z+=ZOOM;
    this->draw();
  }
  else if(_event.y <0 )
  {
    m_modelPos.m_z-=ZOOM;
    this->draw();
  }

  // check the diff of the wheel position (0 means no change)
  if(_event.x > 0)
  {
    m_modelPos.m_x-=ZOOM;
    this->draw();
  }
  else if(_event.x <0 )
  {
    m_modelPos.m_x+=ZOOM;
    this->draw();
  }
}
//----------------------------------------------------------------------------------------------------------------------
