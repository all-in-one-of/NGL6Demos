#ifndef NGLSCENE_H__
#define NGLSCENE_H__
#include "OpenGLWindow.h"
#include "SpaceShip.h"
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/TransformStack.h>
#include <ngl/Text.h>
#include "SpaceShip.h"
#include "KeyRecorder.h"

#include <QSet>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public OpenGLWindow
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene(QWindow *_parent=0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initialize();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void render();

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the x rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinXFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the y rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinYFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_rotate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the Right mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_translate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origXPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origYPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Camera *m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief transformation stack for the gl transformations etc
    //----------------------------------------------------------------------------------------------------------------------
    ngl::TransformStack m_transformStack;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the keys being pressed
    //----------------------------------------------------------------------------------------------------------------------
    uint8_t m_keysPressed;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if we need to record the keystrokes
    //----------------------------------------------------------------------------------------------------------------------
    bool m_recording;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our key recorder
    //----------------------------------------------------------------------------------------------------------------------
    KeyRecorder m_keyRecorder;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a flag to indicate if were in playback mode
    //----------------------------------------------------------------------------------------------------------------------
    bool m_playbackActive;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief index into the current frame for playback
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_currentPlaybackFrame;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief text class for drawing
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Text *m_text;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a timer triggered by the startTimer call in the ctor this is called
    /// to update the ship position based on the key presses
    //----------------------------------------------------------------------------------------------------------------------
    int m_updateShipTimer;

    int m_redrawTimer;
    /// @brief a pointer to our spaceship
    SpaceShip *m_ship;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to load transform matrices to the shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when the window is re-sized
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void resizeEvent(QResizeEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief processKeyDown passed from the main window class
    /// @param *_event a pointer to the QKeyEvent passed from main window
    /// class
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief processKeyUp passed from the main window class
    /// @param *_event a pointer to the QKeyEvent passed from main window
    /// class
    //----------------------------------------------------------------------------------------------------------------------
    void keyReleaseEvent(QKeyEvent *_event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief called when the timer triggers used to update and draw
    /// @param *_event a pointer to the timer event id
    //----------------------------------------------------------------------------------------------------------------------
    void timerEvent( QTimerEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief update ship based on stored key movements
    //----------------------------------------------------------------------------------------------------------------------

    void moveShip();

};



#endif
