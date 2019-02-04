/*
 * CS 106B Trailblazer
 * This file declares functions to perform drawing in the graphical user
 * interface (GUI).
 * See trailblazergui.cpp for implementation of each function.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp, based on past code by Keith Schwarz
 * @version 2014/11/19 (initial version for 14au)
 */

#ifndef _trailblazergui_h
#define _trailblazergui_h

#include "gevents.h"
#include "ginteractors.h"
#include "gwindow.h"
#include "observable.h"
#include "WorldAbstract.h"

class TrailblazerGUI : public Observer {
public:
    /*
     * Settings for min, max, and initial animation delay in ms.
     */
    static const int ANIMATION_DELAY_MIN;
    static const int ANIMATION_DELAY_MAX;
    static const int ANIMATION_DELAY_DEFAULT;
    
    /*
     * Constructor: Initializes state of the GUI subsystem and shows it on screen.
     */
    TrailblazerGUI(std::string windowTitle);
    
    /*
     * Destructor: Frees up memory used by the GUI.
     */
    ~TrailblazerGUI();
    
    /*
     * Returns access to the GUI's graphical window.
     */
    GWindow* getGWindow() const;
    
    /*
     * Loads whatever world is currently selected in the drop-down menu.
     */
    void loadCurrentlySelectedWorld();
    
    /*
     * Reacts to an action event in the window.
     */
    void processActionEvent(GActionEvent e);
    
    /*
     * Reacts to a mouse event in the window.
     */
    void processMouseEvent(GMouseEvent e);
    
    /*
     * Reacts to a mouse event in the window.
     */
    void processWindowEvent(GWindowEvent e);
    
    /*
     * Closes the console and GUI, saves current world state, and exits
     * the program.
     */
    void shutdown();
    
    /*
     * Sets the console to be aligned directly to the right of the GUI.
     */
    void snapConsoleLocation();
    
    /*
     * Implementation of Observer interface.
     * Called when world is ready to do a path search.
     */
    virtual void update(Observable* obs, void* arg = NULL);

private:
    // member variables to store various graphical interactors in the GUI
    GWindow* gWindow;
    GChooser* gcAlgorithm;
    GChooser* gcWorld;
    GSlider* gsDelay;
    GTextField* gtfPosition;
    GLabel* glDelay;
    GLabel* glWorld;
    GButton* gbClear;
    GButton* gbLoad;
    GButton* gbRun;
    WorldAbstract* world;   // current world being displayed on screen
    int animationDelay;   // current animation delay in MS between redraws
    std::string gtfPositionText;   // text to display in gtfPosition (cached)
    
    /*
     * Checks whether the user has selected a start and end vertex.
     * If not, prints an error message and returns false.
     */
    bool ensureStartEndVerticesSelected();
    
    /*
     * Called when the user wants to create a randomly generated maze.
     */
    void generateRandomMaze(WorldSize size);
    
    /*
     * Returns all files in the current directory that start with the given
     * substring prefix.  Used to find all maze and/or terrain files to display.
     */
    Set<std::string> getFiles(std::string substr);
    
    /*
     * Given the contents of the world size selector, returns a WorldSize encoding
     * the desired world size.
     */
    WorldSize getWorldSize(std::string worldText);
    
    /*
     * Loads a world graph from the given input file.
     * Returns true if it was successful and false if not.
     */
    bool loadWorld(std::string worldFile);
    
    /*
     * Given a path, returns the cost of that path.
     * Assumes path is valid and found in graph.
     */
    double pathComputeCost(Vector<Vertex*>& path);
    
    /*
     * Displays information about the length, cost, etc. of a given path.
     */
    void pathDisplayInfo(Vector<Vertex*>& path);
    
    /*
     * Checks to make sure that a given path is valid on the current world graph.
     * That is, that all vertices are found in the graph, that they are connected
     * by edges, that it starts/ends with the expected vertices, and so on.
     */
    bool pathVerify(Vector<Vertex*>& path);
    
    /*
     * Runs the currently selected path search algorithm and returns the
     * resulting path.
     */
    Vector<Vertex*> runPathSearch();
    
    /*
     * Sets the delay between animation frames to the given value.
     */
    void setAnimationDelay(int delayMS);
    
    /*
     * Restores the previously saved GUI state, including which algorithm is
     * currently selected, the animation delay, and the world file chosen.
     * If the saved state does not exist or is corrupt, returns false and
     * uses a default initial state.
     */
    bool stateLoad();
    
    /*
     * Saves the GUI's current state, including which algorithm is
     * currently selected, the animation delay, and the world file chosen.
     * If the saved state does not exist or is corrupt, returns false and
     * uses a default initial state.
     */
    bool stateSave();
    
    /*
     * Reads the delay slider and uses its value to decide on an animation delay.
     */
    void updateAnimationDelayFromSlider(bool forbidZero = false);
};

#endif
