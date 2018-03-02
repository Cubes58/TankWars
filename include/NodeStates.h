#pragma once

enum NodeState {
	PATH,
	WALL,
	GOAL,
	START,
	OPEN, 
	CLOSED,
	CURRENT
};