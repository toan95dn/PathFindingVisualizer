#pragma once
#include "Node.h"
#include <SDL.h>
class Grid{
public:
	Grid();
	~Grid();
	Node& NodeAtIndex(int row, int column);

	// Before start solving
	void InputProcessing();
	void Update();

	//Drawing the graph
	void DrawGraph();
	//
	void ResetGraph();
	//
	inline Node* getStartNode() { return startNode; };
	inline Node* getEndNode() { return endNode; };

	//
	void SetStartNodeAtIndex(int row, int col);
	void SetEndNodeAtIndex(int row, int col);
	void SetWallAtIndex(int row, int col);
	void DeleteWallAtIndex(int row, int col);

	//Enable and disable moving diagnoal
	void enableDiagonal();
	void enableDiagonalandCrossCorner();
	void disableDiagnoal();
	//
	void GenerateMazeBackTracking();
	void GenerateMazeDivision();
	void Devide(int row,int col,int width,int height);
	//
	void resetGrid();	//to reset all nodes to unvisited while keep all the walls 
	void clearGrid();   //to clear the whole grid, reset all the node
private:
	Node* nodes;
	Node* startNode;
	Node* endNode;
	bool hasStartNode = false;//Did we pick the startNode yet? 
	bool hasEndNode = false;
	bool hasDiagnoalPath = false;//Can we travel diagnoally?
	bool canCrossCorner = false;
	bool WillBeDevidedVertically(int width, int height);

};

